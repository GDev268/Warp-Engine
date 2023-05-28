#include "WarpEngine/Engine/Swapchain.hpp"

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace WarpEngine
{
    SwapChain::SwapChain(Device &device, VkExtent2D viewportExtent) : device{device}, windowExtent{viewportExtent}
    {
        init();
    }

    SwapChain::SwapChain(Device &device, VkExtent2D viewportExtent, SwapChain *older) : device{device}, windowExtent{viewportExtent}, oldSwapChain{older}
    {
        init();
        oldSwapChain = nullptr;
    }

    SwapChain::~SwapChain()
    {
        for (auto imageView : swapChainImageViews)
        {
            vkDestroyImageView(device._device, imageView, nullptr);
        }
        swapChainImageViews.clear();

        if (swapchain != nullptr)
        {
            vkDestroySwapchainKHR(device._device, swapchain, nullptr);
            swapchain = nullptr;
        }

        for (int i = 0; i < depthImages.size(); i++)
        {
            vkDestroyImageView(device._device, depthImageViews[i], nullptr);
            vkDestroyImage(device._device, depthImages[i], nullptr);
            vkFreeMemory(device._device, depthImageMemories[i], nullptr);
        }

        for (auto framebuffer : swapChainFramebuffers)
        {
            vkDestroyFramebuffer(device._device, framebuffer, nullptr);
        }

        vkDestroyRenderPass(device._device, renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(device._device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device._device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device._device, inFlightFences[i], nullptr);
        }
    }

    void SwapChain::init()
    {
        createSwapChain();
        createImageView();
        createRenderPass();
        createDepthResource();
        createMainFramebuffer();
        createSyncObjects();
    }

    VkResult SwapChain::acquireNextImage(uint32_t *imageIndex)
    {
        vkWaitForFences(device._device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

        VkResult result = vkAcquireNextImageKHR(device._device, swapchain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, imageIndex);

        return result;
    }

    void SwapChain::createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = device.querySwapchainSupport(device.physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.surfaceFormats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.surfaceCapabilities);

        uint32_t imageCount = swapChainSupport.surfaceCapabilities.minImageCount + 1;
        if (swapChainSupport.surfaceCapabilities.maxImageCount > 0 && imageCount > swapChainSupport.surfaceCapabilities.maxImageCount)
        {
            imageCount = swapChainSupport.surfaceCapabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = device.surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamily indices = device.findQueueFamilies(device.physicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainSupport.surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = oldSwapChain == nullptr ? VK_NULL_HANDLE : oldSwapChain->swapchain;

        if (vkCreateSwapchainKHR(device._device, &createInfo, nullptr, &swapchain) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create the swapchain!");
        }

        vkGetSwapchainImagesKHR(device._device, swapchain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device._device, swapchain, &imageCount, swapChainImages.data());
    }

    void SwapChain::createImageView()
    {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device._device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create image view!");
            }
        }
    }

    void SwapChain::createDepthResource()
    {
        VkFormat depthFormat = getDepthFormat();
        swapChainDepthFormat = depthFormat;

        depthImages.resize(imageCount());
        depthImageMemories.resize(imageCount());
        depthImageViews.resize(imageCount());

        for(int i = 0; i < imageCount();i++){
            VkImageCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = swapChainExtent.width;
            createInfo.extent.height = swapChainExtent.height;
            createInfo.extent.depth = 1;
            createInfo.mipLevels = 1;
            createInfo.arrayLayers = 1;
            createInfo.format = depthFormat;
            createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.flags = 0;

            if(vkCreateImage(device._device,&createInfo,nullptr,&depthImages[i]) != VK_SUCCESS){
                throw std::runtime_error("Couldn't create image!");
            }

            VkMemoryRequirements memoryRequirements;
            vkGetImageMemoryRequirements(device._device,depthImages[i],&memoryRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memoryRequirements.size;
            allocInfo.memoryTypeIndex = device.findMemoryType(memoryRequirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            if(vkAllocateMemory(device._device,&allocInfo,nullptr,&depthImageMemories[i]) != VK_SUCCESS){
                throw std::runtime_error("Couldn't allocate image memory!");
            }

            if(vkBindImageMemory(device._device,depthImages[i],depthImageMemories[i],0) != VK_SUCCESS){
                throw std::runtime_error("Couldn't bind image memory!");
            }
        }
    }

    void SwapChain::createRenderPass()
    {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = getDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentReference{};
        depthAttachmentReference.attachment = 1;
        depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = getSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentReference{};
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentReference;
        subpass.pDepthStencilAttachment = &depthAttachmentReference;
    
        VkSubpassDependency subpassDependency = {};
        subpassDependency.dstSubpass = 0;
        subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency.srcAccessMask = 0;
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

        std::array<VkAttachmentDescription , 2> attachments = {colorAttachment,depthAttachment};
        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        createInfo.pAttachments = attachments.data();
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &subpassDependency;

        if(vkCreateRenderPass(device._device,&createInfo,nullptr,&renderPass) != VK_SUCCESS){
            throw std::runtime_error("Couldn't create the renderpass!");
        }
    }

    void SwapChain::createMainFramebuffer()
    {
        swapChainFramebuffers.resize(imageCount());
        for(size_t i = 0; i < imageCount();i++){
            std::array<VkImageView,2> attachments = {swapChainImageViews[i],depthImageViews[i]};

            VkExtent2D swapchainExtent = getSwapChainExtent();

            VkFramebufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass = renderPass;
            createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            createInfo.pAttachments = attachments.data();
            createInfo.width = swapChainExtent.width;
            createInfo.height = swapChainExtent.height;
            createInfo.layers = 1;

            if(vkCreateFramebuffer(device._device,&createInfo,nullptr,&swapChainFramebuffers[i]) != VK_SUCCESS){
                throw std::runtime_error("Couldn't create framebuffer");
            }
        }
    }

    void SwapChain::createSyncObjects()
    {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(imageCount(),VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
        for(size_t i = 0;i < MAX_FRAMES_IN_FLIGHT;i++){
            if(vkCreateSemaphore(device._device,&semaphoreInfo,nullptr,&imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device._device,&semaphoreInfo,nullptr,&imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device._device,&fenceInfo,nullptr,&inFlightFences[i]) != VK_SUCCESS){
                throw std::runtime_error("Couldn't create the syncronization objects for the frame!");
            }
        }
    }

    VkSurfaceFormatKHR WarpEngine::SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
       for(const auto &availableFormat : availableFormats){
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && 
            availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR ){
                return availableFormat;
            }
       }
    }
    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        for(const auto &availablePresentMode : availablePresentModes){
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR){
                std::cout << "Present Mode: Mailbox" << std::endl;
                return availablePresentMode;
            }
        }

        std::cout << "Present mode: V-Sync" << std::endl;
        return VK_PRESENT_MODE_FIFO_KHR;
    }
    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;
        else{
            VkExtent2D actualExtent = windowExtent;
            actualExtent.width = std::max(capabilities.minImageExtent.width,
            std::min(capabilities.minImageExtent.width,actualExtent.width));
            
            actualExtent.height = std::max(capabilities.minImageExtent.height,
            std::min(capabilities.minImageExtent.height,actualExtent.height));
            
            return actualExtent;
        }
    }

    VkFormat SwapChain::getDepthFormat()
    {
        return device.findSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

}