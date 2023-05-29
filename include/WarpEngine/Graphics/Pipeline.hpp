#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Device.hpp"

#include <string>
#include <vector>

namespace WarpEngine
{

        struct PipelineConfigInfo{
            PipelineConfigInfo(const PipelineConfigInfo&) = delete;
            PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

            VkPipelineViewportStateCreateInfo viewportStateInfo;
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
            VkPipelineRasterizationStateCreateInfo rasterizerInfo;
            VkPipelineMultisampleStateCreateInfo multisampleInfo;
            VkPipelineColorBlendAttachmentState colorblendAttachment;
            VkPipelineColorBlendStateCreateInfo colorblendInfo;
            VkPipelineDepthStencilStateCreateInfo depthstencilInfo;
            std::vector<VkDynamicState> vkDynamicStates;
            VkPipelineDynamicStateCreateInfo dynamicStateInfo;
            VkPipelineLayout pipelineLayout = nullptr;
            VkRenderPass renderpass = nullptr;
            uint32_t subpass = 0;
        };

    class Pipeline
    {
    public:
        
    };
}

#endif