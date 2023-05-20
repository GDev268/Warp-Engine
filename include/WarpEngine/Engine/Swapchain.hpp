#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "WarpEngine/Common.hpp"
#include "WarpEngine/Engine/Device.hpp"

namespace WarpEngine
{

    class SwapChain
    {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        SwapChain(Device &device, VkExtent2D viewportExtent);
        SwapChain(Device &device, VkExtent2D viewportExtent, SwapChain *older);
        ~SwapChain();

        SwapChain(const SwapChain &) = delete;
        SwapChain &operator=(const SwapChain &) = delete;

        VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return renderPass; }
        VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        size_t imageCount() { return swapChainImages.size(); }
        VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
        VkExtent2D getSwapChainExtent() { return swapChainExtent; }
        uint32_t width() { return swapChainExtent.width; }
        uint32_t height() { return swapChainExtent.height; }

        float getAspectRatio() {
            return static_cast<float>(swapChainExtent.width,swapChainExtent.height);
        }

        VkFormat getDepthFormat();
        VkResult acquireNextImage(uint32_t *imageIndex);
        VkResult submitCommandBuffer(const VkCommandBuffer* buffer,uint32_t *imageIndex);

        bool compareSwapFormats(const SwapChain &swapChain) const {
            return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
            swapChain.swapChainImageFormat == swapChainImageFormat;
        }
        

        private:
        void init();
        void createSwapChain();
        void createImageView();
        void createDepthResource();
        void createRenderPass();
        void createMainFramebuffer();
        void createSyncObjects();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemories;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkImage> swapChainImages;
        
        VkFormat swapChainImageFormat;
        VkFormat swapChainDepthFormat;
        VkExtent2D swapChainExtent;

        VkRenderPass renderPass;

        Device &device;
        VkExtent2D windowExtent;

        VkSwapchainKHR swapchain;
        SwapChain* oldSwapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        uint32_t currentFrame = 0;
    };
}
#endif