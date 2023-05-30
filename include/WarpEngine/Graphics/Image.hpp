#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "WarpEngine/Data/Utils.hpp"

namespace WarpEngine{
    struct ImageConfig{
        VkImageType imageType = VK_IMAGE_TYPE_2D;
        VkExtent2D extent;
        uint32_t depth;
        uint32_t mipLevels = 1;
        uint32_t arrayLayers = 1;
        VkFormat format;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageUsageFlags usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkImageCreateFlags flags;
    };

    struct ImageViewConfig{
        VkImageViewType viewType;
        VkFormat format;
        VkImageAspectFlags aspectFlags;
        uint32_t baseMipLevel;
        uint32_t levelCount;
        uint32_t baseArrayLayer;
        uint32_t layerCount;
    };

    class Image{
        Image();
        ~Image();

        void createTexture2D(VkExtent2D extent,VkSampleCountFlagBits samples,VkSharingMode sharingMode);
        void setIntialLayout(VkImageLayout layout);
        void setImageUsageFlags(VkImageUsageFlags usage);

        private:
            VkImageCreateInfo imageInfo;
            VkImageViewCreateInfo imageViewInfo;
    };
}

#endif