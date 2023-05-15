#include "WarpEngine/Engine/Device.hpp"

#include <set>
#include <unordered_set>

namespace WarpEngine
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << "\n\n";

        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT pDebugMessenger,
                                       const VkAllocationCallbacks *pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pDebugMessenger, pAllocator);
        }
    }

    Device::Device(Window &window) : window{window}
    {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createCommandPool();
        getVulkanVersion();
    }

    Device::~Device()
    {
        vkDestroyCommandPool(_device,commandPool,nullptr);
        vkDestroyDevice(_device,nullptr);

        if(enableValidationLayers){
            DestroyDebugUtilsMessengerEXT(instance,debugMessenger,nullptr);
        }

        vkDestroySurfaceKHR(instance,surface,nullptr);
        vkDestroyInstance(instance,nullptr);
    }

    void Device::createInstance()
    {
        if(enableValidationLayers && !checkValidationLayerSupport()){
            throw std::runtime_error("ERROR 1: Validation layers requested, but not available!"); 
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "";
        appInfo.applicationVersion = VK_MAKE_VERSION(0,0,0);
        appInfo.pEngineName = "Warp Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if(enableValidationLayers){
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            debugCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else{
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        if(vkCreateInstance(&instanceCreateInfo,nullptr,&instance) != VK_SUCCESS){
            throw std::runtime_error("ERROR 2: Couldn't create instance!");
        }

        hasGLFWrequiredExtensions();
    }

    void Device::setupDebugMessenger()
    {
        if(!enableValidationLayers)
            return;
        
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);
        if(CreateDebugUtilsMessengerEXT(instance,&createInfo,nullptr,&debugMessenger)){
            throw std::runtime_error("ERROR2 3: Couldn't create debug messenger!");
        }
    }

    void Device::createSurface()
    {
        window.createWindowSurface(instance,&surface);
    }

    void Device::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
    }

    void Device::createLogicalDevice()
    {

    }

    void Device::createCommandPool()
    {

    }

    void Device::getVulkanVersion()
    {
        
    }
}