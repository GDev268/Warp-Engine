#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "WarpEngine/Data/Utils.hpp"

namespace WarpEngine{
    struct ImageConfig{
        VkImageType imageType = VK_IMAGE_TYPE_2D;
        VkExtent3D extent;
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

    class Image{
        Image(ImageConfig config);
    };

    class ImageView{

    };
}

#endif