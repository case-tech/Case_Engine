// Copyright (c) 2025 Case Technologies

#pragma once
#ifndef AZ_DISABLE
#include <Az/Az.hpp>

#if defined(AZ_AZPL_VULKAN_NO_PROTOTYPES) && !defined(VK_NO_PROTOTYPES)
#define VK_NO_PROTOTYPES
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && !defined(NOMINMAX)
#define NOMINMAX
#endif

#ifdef AZ_AZPL_VULKAN_USE_VOLK
#include <volk.h>
#else
#include <vulkan/vulkan.h>
#endif
#if defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering)
#define AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
#endif

#define AZ_AZPL_VULKAN_MINAZUM_AZAGE_SAMPLER_POOL_SIZE (8)

struct Az_AzplVulkan_InitInfo
{
    uint32_t ApiVersion;
    VkInstance Instance;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;
    uint32_t QueueFamily;
    VkQueue Queue;
    VkDescriptorPool DescriptorPool;
    VkRenderPass RenderPass;
    uint32_t MinAzageCount;
    uint32_t AzageCount;
    VkSampleCountFlagBits MSAASamples;

    VkPipelineCache PipelineCache;
    uint32_t Subpass;

    uint32_t DescriptorPoolSize;

    bool UseDynamicRendering;
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
    VkPipelineRenderingCreateInfoKHR PipelineRenderingCreateInfo;
#endif

    const VkAllocationCallbacks* Allocator;
    void (*CheckVkResultFn)(VkResult err);
    VkDeviceSize MinAllocationSize;
};

AZ_AZPL_API bool Az_AzplVulkan_Init(Az_AzplVulkan_InitInfo* info);
AZ_AZPL_API void Az_AzplVulkan_Shutdown();
AZ_AZPL_API void Az_AzplVulkan_NewFrame();
AZ_AZPL_API void Az_AzplVulkan_RenderDrawData(
        AzDrawData* draw_data,
        VkCommandBuffer command_buffer,
        VkPipeline pipeline = VK_NULL_HANDLE);
AZ_AZPL_API void Az_AzplVulkan_SetMinAzageCount(uint32_t min_image_count);

AZ_AZPL_API void Az_AzplVulkan_UpdateTexture(AzTextureData* tex);

AZ_AZPL_API VkDescriptorSet
Az_AzplVulkan_AddTexture(VkSampler sampler,
                         VkAzageView image_view,
                         VkAzageLayout image_layout);
AZ_AZPL_API void Az_AzplVulkan_RemoveTexture(VkDescriptorSet descriptor_set);

AZ_AZPL_API bool Az_AzplVulkan_LoadFunctions(
        uint32_t api_version,
        PFN_vkVoidFunction (*loader_func)(const char* function_name,
                                          void* user_data),
        void* user_data = nullptr);

struct Az_AzplVulkan_RenderState
{
    VkCommandBuffer CommandBuffer;
    VkPipeline Pipeline;
    VkPipelineLayout PipelineLayout;
};

struct Az_AzplVulkanH_Frame;
struct Az_AzplVulkanH_Window;

AZ_AZPL_API void Az_AzplVulkanH_CreateOrResizeWindow(
        VkInstance instance,
        VkPhysicalDevice physical_device,
        VkDevice device,
        Az_AzplVulkanH_Window* wd,
        uint32_t queue_family,
        const VkAllocationCallbacks* allocator,
        int w,
        int h,
        uint32_t min_image_count);
AZ_AZPL_API void Az_AzplVulkanH_DestroyWindow(
        VkInstance instance,
        VkDevice device,
        Az_AzplVulkanH_Window* wd,
        const VkAllocationCallbacks* allocator);
AZ_AZPL_API VkSurfaceFormatKHR
Az_AzplVulkanH_SelectSurfaceFormat(VkPhysicalDevice physical_device,
                                   VkSurfaceKHR surface,
                                   const VkFormat* request_formats,
                                   int request_formats_count,
                                   VkColorSpaceKHR request_color_space);
AZ_AZPL_API VkPresentModeKHR
Az_AzplVulkanH_SelectPresentMode(VkPhysicalDevice physical_device,
                                 VkSurfaceKHR surface,
                                 const VkPresentModeKHR* request_modes,
                                 int request_modes_count);
AZ_AZPL_API VkPhysicalDevice
Az_AzplVulkanH_SelectPhysicalDevice(VkInstance instance);
AZ_AZPL_API uint32_t
Az_AzplVulkanH_SelectQueueFamilyIndex(VkPhysicalDevice physical_device);
AZ_AZPL_API int Az_AzplVulkanH_GetMinAzageCountFromPresentMode(
        VkPresentModeKHR present_mode);

struct Az_AzplVulkanH_Frame
{
    VkCommandPool CommandPool;
    VkCommandBuffer CommandBuffer;
    VkFence Fence;
    VkAzage Backbuffer;
    VkAzageView BackbufferView;
    VkFramebuffer Framebuffer;
};

struct Az_AzplVulkanH_FrameSemaphores
{
    VkSemaphore AzageAcquiredSemaphore;
    VkSemaphore RenderCompleteSemaphore;
};

struct Az_AzplVulkanH_Window
{
    int Width;
    int Height;
    VkSwapchainKHR Swapchain;
    VkSurfaceKHR Surface;
    VkSurfaceFormatKHR SurfaceFormat;
    VkPresentModeKHR PresentMode;
    VkRenderPass RenderPass;
    bool UseDynamicRendering;
    bool ClearEnable;
    VkClearValue ClearValue;
    uint32_t FrameIndex;
    uint32_t AzageCount;
    uint32_t SemaphoreCount;
    uint32_t SemaphoreIndex;
    AzVector<Az_AzplVulkanH_Frame> Frames;
    AzVector<Az_AzplVulkanH_FrameSemaphores> FrameSemaphores;

    Az_AzplVulkanH_Window()
    {
        memset((void*)this, 0, sizeof(*this));
        PresentMode = (VkPresentModeKHR)~0;
        ClearEnable = true;
    }
};

#endif
