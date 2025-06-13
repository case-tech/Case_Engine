#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include "az_impl_vulkan.hpp"

#include <stdio.h>
#ifndef AZ_MAX
#define AZ_MAX(A, B) (((A) >= (B)) ? (A) : (B))
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4127)
#endif

struct Az_AzplVulkan_FrameRenderBuffers;
struct Az_AzplVulkan_WindowRenderBuffers;
bool Az_AzplVulkan_CreateDeviceObjects();
void Az_AzplVulkan_DestroyDeviceObjects();
void Az_AzplVulkan_DestroyFrameRenderBuffers(
        VkDevice device,
        Az_AzplVulkan_FrameRenderBuffers* buffers,
        const VkAllocationCallbacks* allocator);
void Az_AzplVulkan_DestroyWindowRenderBuffers(
        VkDevice device,
        Az_AzplVulkan_WindowRenderBuffers* buffers,
        const VkAllocationCallbacks* allocator);
void Az_AzplVulkanH_DestroyFrame(VkDevice device,
                                 Az_AzplVulkanH_Frame* fd,
                                 const VkAllocationCallbacks* allocator);
void Az_AzplVulkanH_DestroyFrameSemaphores(
        VkDevice device,
        Az_AzplVulkanH_FrameSemaphores* fsd,
        const VkAllocationCallbacks* allocator);
void Az_AzplVulkanH_DestroyAllViewportsRenderBuffers(
        VkDevice device,
        const VkAllocationCallbacks* allocator);
void Az_AzplVulkanH_CreateWindowSwapChain(
        VkPhysicalDevice physical_device,
        VkDevice device,
        Az_AzplVulkanH_Window* wd,
        const VkAllocationCallbacks* allocator,
        int w,
        int h,
        uint32_t min_image_count);
void Az_AzplVulkanH_CreateWindowCommandBuffers(
        VkPhysicalDevice physical_device,
        VkDevice device,
        Az_AzplVulkanH_Window* wd,
        uint32_t queue_family,
        const VkAllocationCallbacks* allocator);

#if defined(VK_NO_PROTOTYPES) && !defined(VOLK_H_)
#define AZ_AZPL_VULKAN_USE_LOADER
static bool g_FunctionsLoaded = false;
#else
static bool g_FunctionsLoaded = true;
#endif
#ifdef AZ_AZPL_VULKAN_USE_LOADER
#define AZ_VULKAN_FUNC_MAP(AZ_VULKAN_FUNC_MAP_MACRO)                           \
    AZ_VULKAN_FUNC_MAP_MACRO(vkAllocateCommandBuffers)                         \
    AZ_VULKAN_FUNC_MAP_MACRO(vkAllocateDescriptorSets)                         \
    AZ_VULKAN_FUNC_MAP_MACRO(vkAllocateMemory)                                 \
    AZ_VULKAN_FUNC_MAP_MACRO(vkAcquireNextAzageKHR)                            \
    AZ_VULKAN_FUNC_MAP_MACRO(vkBeginCommandBuffer)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkBindBufferMemory)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkBindAzageMemory)                                \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdBeginRenderPass)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdBindDescriptorSets)                          \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdBindIndexBuffer)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdBindPipeline)                                \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdBindVertexBuffers)                           \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdCopyBufferToAzage)                           \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdDrawIndexed)                                 \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdEndRenderPass)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdPipelineBarrier)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdPushConstants)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdSetScissor)                                  \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCmdSetViewport)                                 \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateBuffer)                                   \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateCommandPool)                              \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateDescriptorPool)                           \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateDescriptorSetLayout)                      \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateFence)                                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateFramebuffer)                              \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateGraphicsPipelines)                        \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateAzage)                                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateAzageView)                                \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreatePipelineLayout)                           \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateRenderPass)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateSampler)                                  \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateSemaphore)                                \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateShaderModule)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkCreateSwapchainKHR)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyBuffer)                                  \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyCommandPool)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyDescriptorPool)                          \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyDescriptorSetLayout)                     \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyFence)                                   \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyFramebuffer)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyAzage)                                   \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyAzageView)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyPipeline)                                \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyPipelineLayout)                          \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyRenderPass)                              \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroySampler)                                 \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroySemaphore)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroyShaderModule)                            \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroySurfaceKHR)                              \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDestroySwapchainKHR)                            \
    AZ_VULKAN_FUNC_MAP_MACRO(vkDeviceWaitIdle)                                 \
    AZ_VULKAN_FUNC_MAP_MACRO(vkEnumeratePhysicalDevices)                       \
    AZ_VULKAN_FUNC_MAP_MACRO(vkEndCommandBuffer)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkFlushMappedMemoryRanges)                        \
    AZ_VULKAN_FUNC_MAP_MACRO(vkFreeCommandBuffers)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkFreeDescriptorSets)                             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkFreeMemory)                                     \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetBufferMemoryRequirements)                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetDeviceQueue)                                 \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetAzageMemoryRequirements)                     \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceProperties)                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceMemoryProperties)              \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceQueueFamilyProperties)         \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)        \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceSurfaceFormatsKHR)             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceSurfacePresentModesKHR)        \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetPhysicalDeviceSurfaceSupportKHR)             \
    AZ_VULKAN_FUNC_MAP_MACRO(vkGetSwapchainAzagesKHR)                          \
    AZ_VULKAN_FUNC_MAP_MACRO(vkMapMemory)                                      \
    AZ_VULKAN_FUNC_MAP_MACRO(vkQueuePresentKHR)                                \
    AZ_VULKAN_FUNC_MAP_MACRO(vkQueueSubmit)                                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkQueueWaitIdle)                                  \
    AZ_VULKAN_FUNC_MAP_MACRO(vkResetCommandPool)                               \
    AZ_VULKAN_FUNC_MAP_MACRO(vkResetFences)                                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkUnmapMemory)                                    \
    AZ_VULKAN_FUNC_MAP_MACRO(vkUpdateDescriptorSets)                           \
    AZ_VULKAN_FUNC_MAP_MACRO(vkWaitForFences)

#define AZ_VULKAN_FUNC_DEF(func) static PFN_##func func;
AZ_VULKAN_FUNC_MAP(AZ_VULKAN_FUNC_DEF)
#undef AZ_VULKAN_FUNC_DEF
#endif

#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
static PFN_vkCmdBeginRenderingKHR AzAzplVulkanFuncs_vkCmdBeginRenderingKHR;
static PFN_vkCmdEndRenderingKHR AzAzplVulkanFuncs_vkCmdEndRenderingKHR;
#endif

struct Az_AzplVulkan_FrameRenderBuffers
{
    VkDeviceMemory VertexBufferMemory;
    VkDeviceMemory IndexBufferMemory;
    VkDeviceSize VertexBufferSize;
    VkDeviceSize IndexBufferSize;
    VkBuffer VertexBuffer;
    VkBuffer IndexBuffer;
};

struct Az_AzplVulkan_WindowRenderBuffers
{
    uint32_t Index;
    uint32_t Count;
    AzVector<Az_AzplVulkan_FrameRenderBuffers> FrameRenderBuffers;
};

struct Az_AzplVulkan_Texture
{
    VkDeviceMemory Memory;
    VkAzage Azage;
    VkAzageView AzageView;
    VkDescriptorSet DescriptorSet;

    Az_AzplVulkan_Texture()
    {
        memset((void*)this, 0, sizeof(*this));
    }
};

struct Az_AzplVulkan_ViewportData
{
    Az_AzplVulkanH_Window Window;
    Az_AzplVulkan_WindowRenderBuffers RenderBuffers;
    bool WindowOwned;
    bool SwapChainNeedRebuild;
    bool SwapChainSuboptimal;

    Az_AzplVulkan_ViewportData()
    {
        WindowOwned = SwapChainNeedRebuild = SwapChainSuboptimal = false;
        memset((void*)&RenderBuffers, 0, sizeof(RenderBuffers));
    }
    ~Az_AzplVulkan_ViewportData()
    {}
};

struct Az_AzplVulkan_Data
{
    Az_AzplVulkan_InitInfo VulkanInitInfo;
    VkDeviceSize BufferMemoryAlignment;
    VkPipelineCreateFlags PipelineCreateFlags;
    VkDescriptorSetLayout DescriptorSetLayout;
    VkPipelineLayout PipelineLayout;
    VkPipeline Pipeline;
    VkPipeline PipelineForViewports;
    VkShaderModule ShaderModuleVert;
    VkShaderModule ShaderModuleFrag;
    VkDescriptorPool DescriptorPool;

    VkSampler TexSampler;
    VkCommandPool TexCommandPool;
    VkCommandBuffer TexCommandBuffer;

    Az_AzplVulkan_WindowRenderBuffers MainWindowRenderBuffers;

    Az_AzplVulkan_Data()
    {
        memset((void*)this, 0, sizeof(*this));
        BufferMemoryAlignment = 256;
    }
};

static void Az_AzplVulkan_InitMultiViewportSupport();
static void Az_AzplVulkan_ShutdownMultiViewportSupport();

static uint32_t __glsl_shader_vert_spv[] = {
        0x07230203, 0x00010000, 0x00080001, 0x0000002e, 0x00000000, 0x00020011,
        0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
        0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x000a000f, 0x00000000,
        0x00000004, 0x6e69616d, 0x00000000, 0x0000000b, 0x0000000f, 0x00000015,
        0x0000001b, 0x0000001c, 0x00030003, 0x00000002, 0x000001c2, 0x00040005,
        0x00000004, 0x6e69616d, 0x00000000, 0x00030005, 0x00000009, 0x00000000,
        0x00050006, 0x00000009, 0x00000000, 0x6f6c6f43, 0x00000072, 0x00040006,
        0x00000009, 0x00000001, 0x00005655, 0x00030005, 0x0000000b, 0x0074754f,
        0x00040005, 0x0000000f, 0x6c6f4361, 0x0000726f, 0x00030005, 0x00000015,
        0x00565561, 0x00060005, 0x00000019, 0x505f6c67, 0x65567265, 0x78657472,
        0x00000000, 0x00060006, 0x00000019, 0x00000000, 0x505f6c67, 0x7469736f,
        0x006e6f69, 0x00030005, 0x0000001b, 0x00000000, 0x00040005, 0x0000001c,
        0x736f5061, 0x00000000, 0x00060005, 0x0000001e, 0x73755075, 0x6e6f4368,
        0x6e617473, 0x00000074, 0x00050006, 0x0000001e, 0x00000000, 0x61635375,
        0x0000656c, 0x00060006, 0x0000001e, 0x00000001, 0x61725475, 0x616c736e,
        0x00006574, 0x00030005, 0x00000020, 0x00006370, 0x00040047, 0x0000000b,
        0x0000001e, 0x00000000, 0x00040047, 0x0000000f, 0x0000001e, 0x00000002,
        0x00040047, 0x00000015, 0x0000001e, 0x00000001, 0x00050048, 0x00000019,
        0x00000000, 0x0000000b, 0x00000000, 0x00030047, 0x00000019, 0x00000002,
        0x00040047, 0x0000001c, 0x0000001e, 0x00000000, 0x00050048, 0x0000001e,
        0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000001e, 0x00000001,
        0x00000023, 0x00000008, 0x00030047, 0x0000001e, 0x00000002, 0x00020013,
        0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006,
        0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040017,
        0x00000008, 0x00000006, 0x00000002, 0x0004001e, 0x00000009, 0x00000007,
        0x00000008, 0x00040020, 0x0000000a, 0x00000003, 0x00000009, 0x0004003b,
        0x0000000a, 0x0000000b, 0x00000003, 0x00040015, 0x0000000c, 0x00000020,
        0x00000001, 0x0004002b, 0x0000000c, 0x0000000d, 0x00000000, 0x00040020,
        0x0000000e, 0x00000001, 0x00000007, 0x0004003b, 0x0000000e, 0x0000000f,
        0x00000001, 0x00040020, 0x00000011, 0x00000003, 0x00000007, 0x0004002b,
        0x0000000c, 0x00000013, 0x00000001, 0x00040020, 0x00000014, 0x00000001,
        0x00000008, 0x0004003b, 0x00000014, 0x00000015, 0x00000001, 0x00040020,
        0x00000017, 0x00000003, 0x00000008, 0x0003001e, 0x00000019, 0x00000007,
        0x00040020, 0x0000001a, 0x00000003, 0x00000019, 0x0004003b, 0x0000001a,
        0x0000001b, 0x00000003, 0x0004003b, 0x00000014, 0x0000001c, 0x00000001,
        0x0004001e, 0x0000001e, 0x00000008, 0x00000008, 0x00040020, 0x0000001f,
        0x00000009, 0x0000001e, 0x0004003b, 0x0000001f, 0x00000020, 0x00000009,
        0x00040020, 0x00000021, 0x00000009, 0x00000008, 0x0004002b, 0x00000006,
        0x00000028, 0x00000000, 0x0004002b, 0x00000006, 0x00000029, 0x3f800000,
        0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8,
        0x00000005, 0x0004003d, 0x00000007, 0x00000010, 0x0000000f, 0x00050041,
        0x00000011, 0x00000012, 0x0000000b, 0x0000000d, 0x0003003e, 0x00000012,
        0x00000010, 0x0004003d, 0x00000008, 0x00000016, 0x00000015, 0x00050041,
        0x00000017, 0x00000018, 0x0000000b, 0x00000013, 0x0003003e, 0x00000018,
        0x00000016, 0x0004003d, 0x00000008, 0x0000001d, 0x0000001c, 0x00050041,
        0x00000021, 0x00000022, 0x00000020, 0x0000000d, 0x0004003d, 0x00000008,
        0x00000023, 0x00000022, 0x00050085, 0x00000008, 0x00000024, 0x0000001d,
        0x00000023, 0x00050041, 0x00000021, 0x00000025, 0x00000020, 0x00000013,
        0x0004003d, 0x00000008, 0x00000026, 0x00000025, 0x00050081, 0x00000008,
        0x00000027, 0x00000024, 0x00000026, 0x00050051, 0x00000006, 0x0000002a,
        0x00000027, 0x00000000, 0x00050051, 0x00000006, 0x0000002b, 0x00000027,
        0x00000001, 0x00070050, 0x00000007, 0x0000002c, 0x0000002a, 0x0000002b,
        0x00000028, 0x00000029, 0x00050041, 0x00000011, 0x0000002d, 0x0000001b,
        0x0000000d, 0x0003003e, 0x0000002d, 0x0000002c, 0x000100fd, 0x00010038};

static uint32_t __glsl_shader_frag_spv[] = {
        0x07230203, 0x00010000, 0x00080001, 0x0000001e, 0x00000000, 0x00020011,
        0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
        0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0007000f, 0x00000004,
        0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x0000000d, 0x00030010,
        0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x00040005,
        0x00000004, 0x6e69616d, 0x00000000, 0x00040005, 0x00000009, 0x6c6f4366,
        0x0000726f, 0x00030005, 0x0000000b, 0x00000000, 0x00050006, 0x0000000b,
        0x00000000, 0x6f6c6f43, 0x00000072, 0x00040006, 0x0000000b, 0x00000001,
        0x00005655, 0x00030005, 0x0000000d, 0x00006e49, 0x00050005, 0x00000016,
        0x78655473, 0x65727574, 0x00000000, 0x00040047, 0x00000009, 0x0000001e,
        0x00000000, 0x00040047, 0x0000000d, 0x0000001e, 0x00000000, 0x00040047,
        0x00000016, 0x00000022, 0x00000000, 0x00040047, 0x00000016, 0x00000021,
        0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002,
        0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006,
        0x00000004, 0x00040020, 0x00000008, 0x00000003, 0x00000007, 0x0004003b,
        0x00000008, 0x00000009, 0x00000003, 0x00040017, 0x0000000a, 0x00000006,
        0x00000002, 0x0004001e, 0x0000000b, 0x00000007, 0x0000000a, 0x00040020,
        0x0000000c, 0x00000001, 0x0000000b, 0x0004003b, 0x0000000c, 0x0000000d,
        0x00000001, 0x00040015, 0x0000000e, 0x00000020, 0x00000001, 0x0004002b,
        0x0000000e, 0x0000000f, 0x00000000, 0x00040020, 0x00000010, 0x00000001,
        0x00000007, 0x00090019, 0x00000013, 0x00000006, 0x00000001, 0x00000000,
        0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001b, 0x00000014,
        0x00000013, 0x00040020, 0x00000015, 0x00000000, 0x00000014, 0x0004003b,
        0x00000015, 0x00000016, 0x00000000, 0x0004002b, 0x0000000e, 0x00000018,
        0x00000001, 0x00040020, 0x00000019, 0x00000001, 0x0000000a, 0x00050036,
        0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005,
        0x00050041, 0x00000010, 0x00000011, 0x0000000d, 0x0000000f, 0x0004003d,
        0x00000007, 0x00000012, 0x00000011, 0x0004003d, 0x00000014, 0x00000017,
        0x00000016, 0x00050041, 0x00000019, 0x0000001a, 0x0000000d, 0x00000018,
        0x0004003d, 0x0000000a, 0x0000001b, 0x0000001a, 0x00050057, 0x00000007,
        0x0000001c, 0x00000017, 0x0000001b, 0x00050085, 0x00000007, 0x0000001d,
        0x00000012, 0x0000001c, 0x0003003e, 0x00000009, 0x0000001d, 0x000100fd,
        0x00010038};

static Az_AzplVulkan_Data* Az_AzplVulkan_GetBackendData()
{
    return Az::GetCurrentContext()
                   ? (Az_AzplVulkan_Data*)Az::GetIO().BackendRendererUserData
                   : nullptr;
}

static uint32_t Az_AzplVulkan_MemoryType(VkMemoryPropertyFlags properties,
                                         uint32_t type_bits)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkPhysicalDeviceMemoryProperties prop;
    vkGetPhysicalDeviceMemoryProperties(v->PhysicalDevice, &prop);
    for (uint32_t i = 0; i < prop.memoryTypeCount; i++)
        if ((prop.memoryTypes[i].propertyFlags & properties) == properties
            && type_bits & (1 << i))
            return i;
    return 0xFFFFFFFF;
}

static void check_vk_result(VkResult err)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    if (!bd)
        return;
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    if (v->CheckVkResultFn)
        v->CheckVkResultFn(err);
}

static inline VkDeviceSize AlignBufferSize(VkDeviceSize size,
                                           VkDeviceSize alignment)
{
    return (size + alignment - 1) & ~(alignment - 1);
}

static void CreateOrResizeBuffer(VkBuffer& buffer,
                                 VkDeviceMemory& buffer_memory,
                                 VkDeviceSize& buffer_size,
                                 VkDeviceSize new_size,
                                 VkBufferUsageFlagBits usage)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkResult err;
    if (buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(v->Device, buffer, v->Allocator);
    if (buffer_memory != VK_NULL_HANDLE)
        vkFreeMemory(v->Device, buffer_memory, v->Allocator);

    VkDeviceSize buffer_size_aligned =
            AlignBufferSize(AZ_MAX(v->MinAllocationSize, new_size),
                            bd->BufferMemoryAlignment);
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = buffer_size_aligned;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    err = vkCreateBuffer(v->Device, &buffer_info, v->Allocator, &buffer);
    check_vk_result(err);

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(v->Device, buffer, &req);
    bd->BufferMemoryAlignment = (bd->BufferMemoryAlignment > req.alignment)
                                        ? bd->BufferMemoryAlignment
                                        : req.alignment;
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = req.size;
    alloc_info.memoryTypeIndex =
            Az_AzplVulkan_MemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                                     req.memoryTypeBits);
    err = vkAllocateMemory(v->Device,
                           &alloc_info,
                           v->Allocator,
                           &buffer_memory);
    check_vk_result(err);

    err = vkBindBufferMemory(v->Device, buffer, buffer_memory, 0);
    check_vk_result(err);
    buffer_size = buffer_size_aligned;
}

static void Az_AzplVulkan_SetupRenderState(AzDrawData* draw_data,
                                           VkPipeline pipeline,
                                           VkCommandBuffer command_buffer,
                                           Az_AzplVulkan_FrameRenderBuffers* rb,
                                           int fb_width,
                                           int fb_height)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();

    {
        vkCmdBindPipeline(command_buffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline);
    }

    if (draw_data->TotalVtxCount > 0)
    {
        VkBuffer vertex_buffers[1] = {rb->VertexBuffer};
        VkDeviceSize vertex_offset[1] = {0};
        vkCmdBindVertexBuffers(command_buffer,
                               0,
                               1,
                               vertex_buffers,
                               vertex_offset);
        vkCmdBindIndexBuffer(command_buffer,
                             rb->IndexBuffer,
                             0,
                             sizeof(AzDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16
                                                    : VK_INDEX_TYPE_UINT32);
    }

    {
        VkViewport viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = (float)fb_width;
        viewport.height = (float)fb_height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    }

    {
        float scale[2];
        scale[0] = 2.0f / draw_data->DisplaySize.x;
        scale[1] = 2.0f / draw_data->DisplaySize.y;
        float translate[2];
        translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
        translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];
        vkCmdPushConstants(command_buffer,
                           bd->PipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           sizeof(float) * 0,
                           sizeof(float) * 2,
                           scale);
        vkCmdPushConstants(command_buffer,
                           bd->PipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT,
                           sizeof(float) * 2,
                           sizeof(float) * 2,
                           translate);
    }
}

void Az_AzplVulkan_RenderDrawData(AzDrawData* draw_data,
                                  VkCommandBuffer command_buffer,
                                  VkPipeline pipeline)
{
    int fb_width = (int)(draw_data->DisplaySize.x
                         * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y
                          * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    if (draw_data->Textures != nullptr)
        for (AzTextureData* tex : *draw_data->Textures)
            if (tex->Status != AzTextureStatus_OK)
                Az_AzplVulkan_UpdateTexture(tex);

    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    if (pipeline == VK_NULL_HANDLE)
        pipeline = bd->Pipeline;

    Az_AzplVulkan_ViewportData* viewport_renderer_data =
            (Az_AzplVulkan_ViewportData*)
                    draw_data->OwnerViewport->RendererUserData;
    AZ_ASSERT(viewport_renderer_data != nullptr);
    Az_AzplVulkan_WindowRenderBuffers* wrb =
            &viewport_renderer_data->RenderBuffers;
    if (wrb->FrameRenderBuffers.Size == 0)
    {
        wrb->Index = 0;
        wrb->Count = v->AzageCount;
        wrb->FrameRenderBuffers.resize(wrb->Count);
        memset((void*)wrb->FrameRenderBuffers.Data,
               0,
               wrb->FrameRenderBuffers.size_in_bytes());
    }
    AZ_ASSERT(wrb->Count == v->AzageCount);
    wrb->Index = (wrb->Index + 1) % wrb->Count;
    Az_AzplVulkan_FrameRenderBuffers* rb = &wrb->FrameRenderBuffers[wrb->Index];

    if (draw_data->TotalVtxCount > 0)
    {
        VkDeviceSize vertex_size = AlignBufferSize(draw_data->TotalVtxCount
                                                           * sizeof(AzDrawVert),
                                                   bd->BufferMemoryAlignment);
        VkDeviceSize index_size = AlignBufferSize(draw_data->TotalIdxCount
                                                          * sizeof(AzDrawIdx),
                                                  bd->BufferMemoryAlignment);
        if (rb->VertexBuffer == VK_NULL_HANDLE
            || rb->VertexBufferSize < vertex_size)
            CreateOrResizeBuffer(rb->VertexBuffer,
                                 rb->VertexBufferMemory,
                                 rb->VertexBufferSize,
                                 vertex_size,
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        if (rb->IndexBuffer == VK_NULL_HANDLE
            || rb->IndexBufferSize < index_size)
            CreateOrResizeBuffer(rb->IndexBuffer,
                                 rb->IndexBufferMemory,
                                 rb->IndexBufferSize,
                                 index_size,
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        AzDrawVert* vtx_dst = nullptr;
        AzDrawIdx* idx_dst = nullptr;
        VkResult err = vkMapMemory(v->Device,
                                   rb->VertexBufferMemory,
                                   0,
                                   vertex_size,
                                   0,
                                   (void**)&vtx_dst);
        check_vk_result(err);
        err = vkMapMemory(v->Device,
                          rb->IndexBufferMemory,
                          0,
                          index_size,
                          0,
                          (void**)&idx_dst);
        check_vk_result(err);
        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const AzDrawList* draw_list = draw_data->CmdLists[n];
            memcpy(vtx_dst,
                   draw_list->VtxBuffer.Data,
                   draw_list->VtxBuffer.Size * sizeof(AzDrawVert));
            memcpy(idx_dst,
                   draw_list->IdxBuffer.Data,
                   draw_list->IdxBuffer.Size * sizeof(AzDrawIdx));
            vtx_dst += draw_list->VtxBuffer.Size;
            idx_dst += draw_list->IdxBuffer.Size;
        }
        VkMappedMemoryRange range[2] = {};
        range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range[0].memory = rb->VertexBufferMemory;
        range[0].size = VK_WHOLE_SIZE;
        range[1].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range[1].memory = rb->IndexBufferMemory;
        range[1].size = VK_WHOLE_SIZE;
        err = vkFlushMappedMemoryRanges(v->Device, 2, range);
        check_vk_result(err);
        vkUnmapMemory(v->Device, rb->VertexBufferMemory);
        vkUnmapMemory(v->Device, rb->IndexBufferMemory);
    }

    Az_AzplVulkan_SetupRenderState(draw_data,
                                   pipeline,
                                   command_buffer,
                                   rb,
                                   fb_width,
                                   fb_height);

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    Az_AzplVulkan_RenderState render_state;
    render_state.CommandBuffer = command_buffer;
    render_state.Pipeline = pipeline;
    render_state.PipelineLayout = bd->PipelineLayout;
    platform_io.Renderer_RenderState = &render_state;

    AzVec2 clip_off = draw_data->DisplayPos;
    AzVec2 clip_scale = draw_data->FramebufferScale;

    int global_vtx_offset = 0;
    int global_idx_offset = 0;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const AzDrawList* draw_list = draw_data->CmdLists[n];
        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const AzDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr)
            {
                if (pcmd->UserCallback == AzDrawCallback_ResetRenderState)
                    Az_AzplVulkan_SetupRenderState(draw_data,
                                                   pipeline,
                                                   command_buffer,
                                                   rb,
                                                   fb_width,
                                                   fb_height);
                else
                    pcmd->UserCallback(draw_list, pcmd);
            } else
            {
                AzVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                                (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                AzVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                                (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                if (clip_min.x < 0.0f)
                {
                    clip_min.x = 0.0f;
                }
                if (clip_min.y < 0.0f)
                {
                    clip_min.y = 0.0f;
                }
                if (clip_max.x > fb_width)
                {
                    clip_max.x = (float)fb_width;
                }
                if (clip_max.y > fb_height)
                {
                    clip_max.y = (float)fb_height;
                }
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                VkRect2D scissor;
                scissor.offset.x = (int32_t)(clip_min.x);
                scissor.offset.y = (int32_t)(clip_min.y);
                scissor.extent.width = (uint32_t)(clip_max.x - clip_min.x);
                scissor.extent.height = (uint32_t)(clip_max.y - clip_min.y);
                vkCmdSetScissor(command_buffer, 0, 1, &scissor);

                VkDescriptorSet desc_set = (VkDescriptorSet)pcmd->GetTexID();
                vkCmdBindDescriptorSets(command_buffer,
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        bd->PipelineLayout,
                                        0,
                                        1,
                                        &desc_set,
                                        0,
                                        nullptr);

                vkCmdDrawIndexed(command_buffer,
                                 pcmd->ElemCount,
                                 1,
                                 pcmd->IdxOffset + global_idx_offset,
                                 pcmd->VtxOffset + global_vtx_offset,
                                 0);
            }
        }
        global_idx_offset += draw_list->IdxBuffer.Size;
        global_vtx_offset += draw_list->VtxBuffer.Size;
    }
    platform_io.Renderer_RenderState = nullptr;

    VkRect2D scissor = {{0, 0}, {(uint32_t)fb_width, (uint32_t)fb_height}};
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
}

static void Az_AzplVulkan_DestroyTexture(AzTextureData* tex)
{
    Az_AzplVulkan_Texture* backend_tex = (Az_AzplVulkan_Texture*)
                                                 tex->BackendUserData;
    if (backend_tex == nullptr)
        return;
    AZ_ASSERT(backend_tex->DescriptorSet == (VkDescriptorSet)tex->TexID);
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    Az_AzplVulkan_RemoveTexture(backend_tex->DescriptorSet);
    vkDestroyAzageView(v->Device, backend_tex->AzageView, v->Allocator);
    vkDestroyAzage(v->Device, backend_tex->Azage, v->Allocator);
    vkFreeMemory(v->Device, backend_tex->Memory, v->Allocator);
    AZ_DELETE(backend_tex);

    tex->SetTexID(AzTextureID_Invalid);
    tex->SetStatus(AzTextureStatus_Destroyed);
    tex->BackendUserData = nullptr;
}

void Az_AzplVulkan_UpdateTexture(AzTextureData* tex)
{
    if (tex->Status == AzTextureStatus_OK)
        return;
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkResult err;

    if (tex->Status == AzTextureStatus_WantCreate)
    {
        AZ_ASSERT(tex->TexID == AzTextureID_Invalid
                  && tex->BackendUserData == nullptr);
        AZ_ASSERT(tex->Format == AzTextureFormat_RGBA32);
        Az_AzplVulkan_Texture* backend_tex = AZ_NEW(Az_AzplVulkan_Texture)();

        {
            VkAzageCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_AZAGE_CREATE_INFO;
            info.imageType = VK_AZAGE_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.extent.width = tex->Width;
            info.extent.height = tex->Height;
            info.extent.depth = 1;
            info.mipLevels = 1;
            info.arrayLayers = 1;
            info.samples = VK_SAMPLE_COUNT_1_BIT;
            info.tiling = VK_AZAGE_TILING_OPTAZAL;
            info.usage = VK_AZAGE_USAGE_SAMPLED_BIT
                         | VK_AZAGE_USAGE_TRANSFER_DST_BIT;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            info.initialLayout = VK_AZAGE_LAYOUT_UNDEFINED;
            err = vkCreateAzage(v->Device,
                                &info,
                                v->Allocator,
                                &backend_tex->Azage);
            check_vk_result(err);
            VkMemoryRequirements req;
            vkGetAzageMemoryRequirements(v->Device, backend_tex->Azage, &req);
            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = AZ_MAX(v->MinAllocationSize, req.size);
            alloc_info.memoryTypeIndex = Az_AzplVulkan_MemoryType(
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    req.memoryTypeBits);
            err = vkAllocateMemory(v->Device,
                                   &alloc_info,
                                   v->Allocator,
                                   &backend_tex->Memory);
            check_vk_result(err);
            err = vkBindAzageMemory(v->Device,
                                    backend_tex->Azage,
                                    backend_tex->Memory,
                                    0);
            check_vk_result(err);
        }

        {
            VkAzageViewCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_AZAGE_VIEW_CREATE_INFO;
            info.image = backend_tex->Azage;
            info.viewType = VK_AZAGE_VIEW_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.subresourceRange.aspectMask = VK_AZAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.layerCount = 1;
            err = vkCreateAzageView(v->Device,
                                    &info,
                                    v->Allocator,
                                    &backend_tex->AzageView);
            check_vk_result(err);
        }

        backend_tex->DescriptorSet = Az_AzplVulkan_AddTexture(
                bd->TexSampler,
                backend_tex->AzageView,
                VK_AZAGE_LAYOUT_SHADER_READ_ONLY_OPTAZAL);

        tex->SetTexID((AzTextureID)backend_tex->DescriptorSet);
        tex->BackendUserData = backend_tex;
    }

    if (tex->Status == AzTextureStatus_WantCreate
        || tex->Status == AzTextureStatus_WantUpdates)
    {
        Az_AzplVulkan_Texture* backend_tex = (Az_AzplVulkan_Texture*)
                                                     tex->BackendUserData;

        const int upload_x = (tex->Status == AzTextureStatus_WantCreate)
                                     ? 0
                                     : tex->UpdateRect.x;
        const int upload_y = (tex->Status == AzTextureStatus_WantCreate)
                                     ? 0
                                     : tex->UpdateRect.y;
        const int upload_w = (tex->Status == AzTextureStatus_WantCreate)
                                     ? tex->Width
                                     : tex->UpdateRect.w;
        const int upload_h = (tex->Status == AzTextureStatus_WantCreate)
                                     ? tex->Height
                                     : tex->UpdateRect.h;

        VkDeviceMemory upload_buffer_memory;

        VkBuffer upload_buffer;
        VkDeviceSize upload_pitch = upload_w * tex->BytesPerPixel;
        VkDeviceSize upload_size = upload_h * upload_pitch;
        {
            VkBufferCreateInfo buffer_info = {};
            buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_info.size = upload_size;
            buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            err = vkCreateBuffer(v->Device,
                                 &buffer_info,
                                 v->Allocator,
                                 &upload_buffer);
            check_vk_result(err);
            VkMemoryRequirements req;
            vkGetBufferMemoryRequirements(v->Device, upload_buffer, &req);
            bd->BufferMemoryAlignment = (bd->BufferMemoryAlignment
                                         > req.alignment)
                                                ? bd->BufferMemoryAlignment
                                                : req.alignment;
            VkMemoryAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            alloc_info.allocationSize = AZ_MAX(v->MinAllocationSize, req.size);
            alloc_info.memoryTypeIndex = Az_AzplVulkan_MemoryType(
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                    req.memoryTypeBits);
            err = vkAllocateMemory(v->Device,
                                   &alloc_info,
                                   v->Allocator,
                                   &upload_buffer_memory);
            check_vk_result(err);
            err = vkBindBufferMemory(v->Device,
                                     upload_buffer,
                                     upload_buffer_memory,
                                     0);
            check_vk_result(err);
        }

        {
            char* map = nullptr;
            err = vkMapMemory(v->Device,
                              upload_buffer_memory,
                              0,
                              upload_size,
                              0,
                              (void**)(&map));
            check_vk_result(err);
            for (int y = 0; y < upload_h; y++)
                memcpy(map + upload_pitch * y,
                       tex->GetPixelsAt(upload_x, upload_y + y),
                       (size_t)upload_pitch);
            VkMappedMemoryRange range[1] = {};
            range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            range[0].memory = upload_buffer_memory;
            range[0].size = upload_size;
            err = vkFlushMappedMemoryRanges(v->Device, 1, range);
            check_vk_result(err);
            vkUnmapMemory(v->Device, upload_buffer_memory);
        }

        {
            err = vkResetCommandPool(v->Device, bd->TexCommandPool, 0);
            check_vk_result(err);
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TAZE_SUBMIT_BIT;
            err = vkBeginCommandBuffer(bd->TexCommandBuffer, &begin_info);
            check_vk_result(err);
        }

        {
            VkAzageMemoryBarrier copy_barrier[1] = {};
            copy_barrier[0].sType = VK_STRUCTURE_TYPE_AZAGE_MEMORY_BARRIER;
            copy_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            copy_barrier[0].oldLayout = VK_AZAGE_LAYOUT_UNDEFINED;
            copy_barrier[0].newLayout = VK_AZAGE_LAYOUT_TRANSFER_DST_OPTAZAL;
            copy_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copy_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            copy_barrier[0].image = backend_tex->Azage;
            copy_barrier[0].subresourceRange.aspectMask =
                    VK_AZAGE_ASPECT_COLOR_BIT;
            copy_barrier[0].subresourceRange.levelCount = 1;
            copy_barrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(bd->TexCommandBuffer,
                                 VK_PIPELINE_STAGE_HOST_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 copy_barrier);

            VkBufferAzageCopy region = {};
            region.imageSubresource.aspectMask = VK_AZAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent.width = upload_w;
            region.imageExtent.height = upload_h;
            region.imageExtent.depth = 1;
            region.imageOffset.x = upload_x;
            region.imageOffset.y = upload_y;
            vkCmdCopyBufferToAzage(bd->TexCommandBuffer,
                                   upload_buffer,
                                   backend_tex->Azage,
                                   VK_AZAGE_LAYOUT_TRANSFER_DST_OPTAZAL,
                                   1,
                                   &region);

            VkAzageMemoryBarrier use_barrier[1] = {};
            use_barrier[0].sType = VK_STRUCTURE_TYPE_AZAGE_MEMORY_BARRIER;
            use_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            use_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            use_barrier[0].oldLayout = VK_AZAGE_LAYOUT_TRANSFER_DST_OPTAZAL;
            use_barrier[0].newLayout = VK_AZAGE_LAYOUT_SHADER_READ_ONLY_OPTAZAL;
            use_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            use_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            use_barrier[0].image = backend_tex->Azage;
            use_barrier[0].subresourceRange.aspectMask =
                    VK_AZAGE_ASPECT_COLOR_BIT;
            use_barrier[0].subresourceRange.levelCount = 1;
            use_barrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(bd->TexCommandBuffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 use_barrier);
        }

        {
            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &bd->TexCommandBuffer;
            err = vkEndCommandBuffer(bd->TexCommandBuffer);
            check_vk_result(err);
            err = vkQueueSubmit(v->Queue, 1, &end_info, VK_NULL_HANDLE);
            check_vk_result(err);
        }

        err = vkQueueWaitIdle(v->Queue);
        check_vk_result(err);
        vkDestroyBuffer(v->Device, upload_buffer, v->Allocator);
        vkFreeMemory(v->Device, upload_buffer_memory, v->Allocator);

        tex->SetStatus(AzTextureStatus_OK);
    }

    if (tex->Status == AzTextureStatus_WantDestroy
        && tex->UnusedFrames >= (int)bd->VulkanInitInfo.AzageCount)
        Az_AzplVulkan_DestroyTexture(tex);
}

static void Az_AzplVulkan_CreateShaderModules(
        VkDevice device,
        const VkAllocationCallbacks* allocator)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    if (bd->ShaderModuleVert == VK_NULL_HANDLE)
    {
        VkShaderModuleCreateInfo vert_info = {};
        vert_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vert_info.codeSize = sizeof(__glsl_shader_vert_spv);
        vert_info.pCode = (uint32_t*)__glsl_shader_vert_spv;
        VkResult err = vkCreateShaderModule(device,
                                            &vert_info,
                                            allocator,
                                            &bd->ShaderModuleVert);
        check_vk_result(err);
    }
    if (bd->ShaderModuleFrag == VK_NULL_HANDLE)
    {
        VkShaderModuleCreateInfo frag_info = {};
        frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        frag_info.codeSize = sizeof(__glsl_shader_frag_spv);
        frag_info.pCode = (uint32_t*)__glsl_shader_frag_spv;
        VkResult err = vkCreateShaderModule(device,
                                            &frag_info,
                                            allocator,
                                            &bd->ShaderModuleFrag);
        check_vk_result(err);
    }
}

static void Az_AzplVulkan_CreatePipeline(VkDevice device,
                                         const VkAllocationCallbacks* allocator,
                                         VkPipelineCache pipelineCache,
                                         VkRenderPass renderPass,
                                         VkSampleCountFlagBits MSAASamples,
                                         VkPipeline* pipeline,
                                         uint32_t subpass)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_CreateShaderModules(device, allocator);

    VkPipelineShaderStageCreateInfo stage[2] = {};
    stage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stage[0].module = bd->ShaderModuleVert;
    stage[0].pName = "main";
    stage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stage[1].module = bd->ShaderModuleFrag;
    stage[1].pName = "main";

    VkVertexInputBindingDescription binding_desc[1] = {};
    binding_desc[0].stride = sizeof(AzDrawVert);
    binding_desc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attribute_desc[3] = {};
    attribute_desc[0].location = 0;
    attribute_desc[0].binding = binding_desc[0].binding;
    attribute_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_desc[0].offset = offsetof(AzDrawVert, pos);
    attribute_desc[1].location = 1;
    attribute_desc[1].binding = binding_desc[0].binding;
    attribute_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_desc[1].offset = offsetof(AzDrawVert, uv);
    attribute_desc[2].location = 2;
    attribute_desc[2].binding = binding_desc[0].binding;
    attribute_desc[2].format = VK_FORMAT_R8G8B8A8_UNORM;
    attribute_desc[2].offset = offsetof(AzDrawVert, col);

    VkPipelineVertexInputStateCreateInfo vertex_info = {};
    vertex_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_info.vertexBindingDescriptionCount = 1;
    vertex_info.pVertexBindingDescriptions = binding_desc;
    vertex_info.vertexAttributeDescriptionCount = 3;
    vertex_info.pVertexAttributeDescriptions = attribute_desc;

    VkPipelineInputAssemblyStateCreateInfo ia_info = {};
    ia_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia_info.topology = VK_PRAZITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewport_info = {};
    viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_info.viewportCount = 1;
    viewport_info.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo raster_info = {};
    raster_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_info.polygonMode = VK_POLYGON_MODE_FILL;
    raster_info.cullMode = VK_CULL_MODE_NONE;
    raster_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    raster_info.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo ms_info = {};
    ms_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms_info.rasterizationSamples = (MSAASamples != 0) ? MSAASamples
                                                      : VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState color_attachment[1] = {};
    color_attachment[0].blendEnable = VK_TRUE;
    color_attachment[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_attachment[0].dstColorBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_attachment[0].colorBlendOp = VK_BLEND_OP_ADD;
    color_attachment[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_attachment[0].dstAlphaBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_attachment[0].alphaBlendOp = VK_BLEND_OP_ADD;
    color_attachment[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                         | VK_COLOR_COMPONENT_G_BIT
                                         | VK_COLOR_COMPONENT_B_BIT
                                         | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineDepthStencilStateCreateInfo depth_info = {};
    depth_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    VkPipelineColorBlendStateCreateInfo blend_info = {};
    blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blend_info.attachmentCount = 1;
    blend_info.pAttachments = color_attachment;

    VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT,
                                        VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamic_state = {};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = (uint32_t)AZ_ARRAYSIZE(dynamic_states);
    dynamic_state.pDynamicStates = dynamic_states;

    VkGraphicsPipelineCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.flags = bd->PipelineCreateFlags;
    info.stageCount = 2;
    info.pStages = stage;
    info.pVertexInputState = &vertex_info;
    info.pInputAssemblyState = &ia_info;
    info.pViewportState = &viewport_info;
    info.pRasterizationState = &raster_info;
    info.pMultisampleState = &ms_info;
    info.pDepthStencilState = &depth_info;
    info.pColorBlendState = &blend_info;
    info.pDynamicState = &dynamic_state;
    info.layout = bd->PipelineLayout;
    info.renderPass = renderPass;
    info.subpass = subpass;

#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
    if (bd->VulkanInitInfo.UseDynamicRendering)
    {
        AZ_ASSERT(bd->VulkanInitInfo.PipelineRenderingCreateInfo.sType == VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR && "PipelineRenderingCreateInfo sType must be VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR");
        AZ_ASSERT(bd->VulkanInitInfo.PipelineRenderingCreateInfo.pNext
                          == nullptr
                  && "PipelineRenderingCreateInfo pNext must be nullptr");
        info.pNext = &bd->VulkanInitInfo.PipelineRenderingCreateInfo;
        info.renderPass = VK_NULL_HANDLE;
    }
#endif

    VkResult err = vkCreateGraphicsPipelines(device,
                                             pipelineCache,
                                             1,
                                             &info,
                                             allocator,
                                             pipeline);
    check_vk_result(err);
}

bool Az_AzplVulkan_CreateDeviceObjects()
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkResult err;

    if (!bd->TexSampler)
    {
        VkSamplerCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        info.magFilter = VK_FILTER_LINEAR;
        info.minFilter = VK_FILTER_LINEAR;
        info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        info.minLod = -1000;
        info.maxLod = 1000;
        info.maxAnisotropy = 1.0f;
        err = vkCreateSampler(v->Device, &info, v->Allocator, &bd->TexSampler);
        check_vk_result(err);
    }

    if (!bd->DescriptorSetLayout)
    {
        VkDescriptorSetLayoutBinding binding[1] = {};
        binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_AZAGE_SAMPLER;
        binding[0].descriptorCount = 1;
        binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        VkDescriptorSetLayoutCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = 1;
        info.pBindings = binding;
        err = vkCreateDescriptorSetLayout(v->Device,
                                          &info,
                                          v->Allocator,
                                          &bd->DescriptorSetLayout);
        check_vk_result(err);
    }

    if (v->DescriptorPoolSize != 0)
    {
        AZ_ASSERT(v->DescriptorPoolSize
                  > AZ_AZPL_VULKAN_MINAZUM_AZAGE_SAMPLER_POOL_SIZE);
        VkDescriptorPoolSize pool_size = {
                VK_DESCRIPTOR_TYPE_COMBINED_AZAGE_SAMPLER,
                v->DescriptorPoolSize};
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = v->DescriptorPoolSize;
        pool_info.poolSizeCount = 1;
        pool_info.pPoolSizes = &pool_size;

        err = vkCreateDescriptorPool(v->Device,
                                     &pool_info,
                                     v->Allocator,
                                     &bd->DescriptorPool);
        check_vk_result(err);
    }

    if (!bd->PipelineLayout)
    {
        VkPushConstantRange push_constants[1] = {};
        push_constants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        push_constants[0].offset = sizeof(float) * 0;
        push_constants[0].size = sizeof(float) * 4;
        VkDescriptorSetLayout set_layout[1] = {bd->DescriptorSetLayout};
        VkPipelineLayoutCreateInfo layout_info = {};
        layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layout_info.setLayoutCount = 1;
        layout_info.pSetLayouts = set_layout;
        layout_info.pushConstantRangeCount = 1;
        layout_info.pPushConstantRanges = push_constants;
        err = vkCreatePipelineLayout(v->Device,
                                     &layout_info,
                                     v->Allocator,
                                     &bd->PipelineLayout);
        check_vk_result(err);
    }

    Az_AzplVulkan_CreatePipeline(v->Device,
                                 v->Allocator,
                                 v->PipelineCache,
                                 v->RenderPass,
                                 v->MSAASamples,
                                 &bd->Pipeline,
                                 v->Subpass);

    if (!bd->TexCommandPool)
    {
        VkCommandPoolCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.flags = 0;
        info.queueFamilyIndex = v->QueueFamily;
        err = vkCreateCommandPool(v->Device,
                                  &info,
                                  v->Allocator,
                                  &bd->TexCommandPool);
        check_vk_result(err);
    }
    if (!bd->TexCommandBuffer)
    {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool = bd->TexCommandPool;
        info.commandBufferCount = 1;
        err = vkAllocateCommandBuffers(v->Device, &info, &bd->TexCommandBuffer);
        check_vk_result(err);
    }

    return true;
}

void Az_AzplVulkan_DestroyDeviceObjects()
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    Az_AzplVulkanH_DestroyAllViewportsRenderBuffers(v->Device, v->Allocator);

    for (AzTextureData* tex : Az::GetPlatformIO().Textures)
        if (tex->RefCount == 1)
            Az_AzplVulkan_DestroyTexture(tex);

    if (bd->TexCommandBuffer)
    {
        vkFreeCommandBuffers(v->Device,
                             bd->TexCommandPool,
                             1,
                             &bd->TexCommandBuffer);
        bd->TexCommandBuffer = VK_NULL_HANDLE;
    }
    if (bd->TexCommandPool)
    {
        vkDestroyCommandPool(v->Device, bd->TexCommandPool, v->Allocator);
        bd->TexCommandPool = VK_NULL_HANDLE;
    }
    if (bd->TexSampler)
    {
        vkDestroySampler(v->Device, bd->TexSampler, v->Allocator);
        bd->TexSampler = VK_NULL_HANDLE;
    }
    if (bd->ShaderModuleVert)
    {
        vkDestroyShaderModule(v->Device, bd->ShaderModuleVert, v->Allocator);
        bd->ShaderModuleVert = VK_NULL_HANDLE;
    }
    if (bd->ShaderModuleFrag)
    {
        vkDestroyShaderModule(v->Device, bd->ShaderModuleFrag, v->Allocator);
        bd->ShaderModuleFrag = VK_NULL_HANDLE;
    }
    if (bd->DescriptorSetLayout)
    {
        vkDestroyDescriptorSetLayout(v->Device,
                                     bd->DescriptorSetLayout,
                                     v->Allocator);
        bd->DescriptorSetLayout = VK_NULL_HANDLE;
    }
    if (bd->PipelineLayout)
    {
        vkDestroyPipelineLayout(v->Device, bd->PipelineLayout, v->Allocator);
        bd->PipelineLayout = VK_NULL_HANDLE;
    }
    if (bd->Pipeline)
    {
        vkDestroyPipeline(v->Device, bd->Pipeline, v->Allocator);
        bd->Pipeline = VK_NULL_HANDLE;
    }
    if (bd->PipelineForViewports)
    {
        vkDestroyPipeline(v->Device, bd->PipelineForViewports, v->Allocator);
        bd->PipelineForViewports = VK_NULL_HANDLE;
    }
    if (bd->DescriptorPool)
    {
        vkDestroyDescriptorPool(v->Device, bd->DescriptorPool, v->Allocator);
        bd->DescriptorPool = VK_NULL_HANDLE;
    }
}

#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
static void Az_AzplVulkan_LoadDynamicRenderingFunctions(
        uint32_t api_version,
        PFN_vkVoidFunction (*loader_func)(const char* function_name,
                                          void* user_data),
        void* user_data)
{
    AZ_UNUSED(api_version);

    AzAzplVulkanFuncs_vkCmdBeginRenderingKHR =
            reinterpret_cast<PFN_vkCmdBeginRenderingKHR>(
                    loader_func("vkCmdBeginRendering", user_data));
    AzAzplVulkanFuncs_vkCmdEndRenderingKHR =
            reinterpret_cast<PFN_vkCmdEndRenderingKHR>(
                    loader_func("vkCmdEndRendering", user_data));

    if (AzAzplVulkanFuncs_vkCmdBeginRenderingKHR == nullptr
        || AzAzplVulkanFuncs_vkCmdEndRenderingKHR == nullptr)
    {
        AzAzplVulkanFuncs_vkCmdBeginRenderingKHR =
                reinterpret_cast<PFN_vkCmdBeginRenderingKHR>(
                        loader_func("vkCmdBeginRenderingKHR", user_data));
        AzAzplVulkanFuncs_vkCmdEndRenderingKHR =
                reinterpret_cast<PFN_vkCmdEndRenderingKHR>(
                        loader_func("vkCmdEndRenderingKHR", user_data));
    }
}
#endif

static uint32_t Az_AzplVulkan_GetDefaultApiVersion()
{
#ifdef VK_HEADER_VERSION_COMPLETE
    return VK_HEADER_VERSION_COMPLETE;
#else
    return VK_API_VERSION_1_0;
#endif
}

bool Az_AzplVulkan_LoadFunctions(
        uint32_t api_version,
        PFN_vkVoidFunction (*loader_func)(const char* function_name,
                                          void* user_data),
        void* user_data)
{
    if (api_version == 0)
        api_version = Az_AzplVulkan_GetDefaultApiVersion();

#ifdef AZ_AZPL_VULKAN_USE_LOADER
#define AZ_VULKAN_FUNC_LOAD(func)                                              \
    func = reinterpret_cast<decltype(func)>(loader_func(#func, user_data));    \
    if (func == nullptr)                                                       \
        return false;
    AZ_VULKAN_FUNC_MAP(AZ_VULKAN_FUNC_LOAD)
#undef AZ_VULKAN_FUNC_LOAD

#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
    Az_AzplVulkan_LoadDynamicRenderingFunctions(api_version,
                                                loader_func,
                                                user_data);
#endif
#else
    AZ_UNUSED(loader_func);
    AZ_UNUSED(user_data);
#endif

    g_FunctionsLoaded = true;
    return true;
}

bool Az_AzplVulkan_Init(Az_AzplVulkan_InitInfo* info)
{
    AZ_ASSERT(g_FunctionsLoaded && "Need to call Az_AzplVulkan_LoadFunctions() if AZ_AZPL_VULKAN_NO_PROTOTYPES or VK_NO_PROTOTYPES are set!");

    if (info->ApiVersion == 0)
        info->ApiVersion = Az_AzplVulkan_GetDefaultApiVersion();

    if (info->UseDynamicRendering)
    {
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
#ifndef AZ_AZPL_VULKAN_USE_LOADER
        Az_AzplVulkan_LoadDynamicRenderingFunctions(
                info->ApiVersion,
                [](const char* function_name, void* user_data) {
                    return vkGetDeviceProcAddr((VkDevice)user_data,
                                               function_name);
                },
                (void*)info->Device);
#endif
        AZ_ASSERT(AzAzplVulkanFuncs_vkCmdBeginRenderingKHR != nullptr);
        AZ_ASSERT(AzAzplVulkanFuncs_vkCmdEndRenderingKHR != nullptr);
#else
        AZ_ASSERT(0 && "Can't use dynamic rendering when neither VK_VERSION_1_3 or VK_KHR_dynamic_rendering is defined.");
#endif
    }

    AzIO& io = Az::GetIO();
    AZ_CHECKVERSION();
    AZ_ASSERT(io.BackendRendererUserData == nullptr
              && "Already initialized a renderer backend!");

    Az_AzplVulkan_Data* bd = AZ_NEW(Az_AzplVulkan_Data)();
    io.BackendRendererUserData = (void*)bd;
    io.BackendRendererName = "imgui_impl_vulkan";
    io.BackendFlags |= AzBackendFlags_RendererHasVtxOffset;
    io.BackendFlags |= AzBackendFlags_RendererHasTextures;
    io.BackendFlags |= AzBackendFlags_RendererHasViewports;

    AZ_ASSERT(info->Instance != VK_NULL_HANDLE);
    AZ_ASSERT(info->PhysicalDevice != VK_NULL_HANDLE);
    AZ_ASSERT(info->Device != VK_NULL_HANDLE);
    AZ_ASSERT(info->Queue != VK_NULL_HANDLE);
    if (info->DescriptorPool != VK_NULL_HANDLE)
        AZ_ASSERT(info->DescriptorPoolSize == 0);
    else
        AZ_ASSERT(info->DescriptorPoolSize > 0);
    AZ_ASSERT(info->MinAzageCount >= 2);
    AZ_ASSERT(info->AzageCount >= info->MinAzageCount);
    if (info->UseDynamicRendering == false)
        AZ_ASSERT(info->RenderPass != VK_NULL_HANDLE);

    bd->VulkanInitInfo = *info;
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    if (v->PipelineRenderingCreateInfo.pColorAttachmentFormats != NULL)
    {
        VkFormat* formats_copy = (VkFormat*)AZ_ALLOC(
                sizeof(VkFormat)
                * v->PipelineRenderingCreateInfo.colorAttachmentCount);
        memcpy(formats_copy,
               v->PipelineRenderingCreateInfo.pColorAttachmentFormats,
               sizeof(VkFormat)
                       * v->PipelineRenderingCreateInfo.colorAttachmentCount);
        v->PipelineRenderingCreateInfo.pColorAttachmentFormats = formats_copy;
    }
#endif

    Az_AzplVulkan_CreateDeviceObjects();

    AzViewport* main_viewport = Az::GetMainViewport();
    main_viewport->RendererUserData = AZ_NEW(Az_AzplVulkan_ViewportData)();

    Az_AzplVulkan_InitMultiViewportSupport();

    return true;
}

void Az_AzplVulkan_Shutdown()
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    AZ_ASSERT(bd != nullptr
              && "No renderer backend to shutdown, or already shutdown?");
    AzIO& io = Az::GetIO();

    Az_AzplVulkan_DestroyDeviceObjects();
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
    AZ_FREE((void*)bd->VulkanInitInfo.PipelineRenderingCreateInfo
                    .pColorAttachmentFormats);
#endif

    AzViewport* main_viewport = Az::GetMainViewport();
    if (Az_AzplVulkan_ViewportData* vd =
                (Az_AzplVulkan_ViewportData*)main_viewport->RendererUserData)
        AZ_DELETE(vd);
    main_viewport->RendererUserData = nullptr;

    Az_AzplVulkan_ShutdownMultiViewportSupport();

    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendFlags &= ~(AzBackendFlags_RendererHasVtxOffset
                         | AzBackendFlags_RendererHasTextures
                         | AzBackendFlags_RendererHasViewports);
    AZ_DELETE(bd);
}

void Az_AzplVulkan_NewFrame()
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    AZ_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call Az_AzplVulkan_Init()?");
    AZ_UNUSED(bd);
}

void Az_AzplVulkan_SetMinAzageCount(uint32_t min_image_count)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    AZ_ASSERT(min_image_count >= 2);
    if (bd->VulkanInitInfo.MinAzageCount == min_image_count)
        return;

    AZ_ASSERT(0);
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkResult err = vkDeviceWaitIdle(v->Device);
    check_vk_result(err);
    Az_AzplVulkanH_DestroyAllViewportsRenderBuffers(v->Device, v->Allocator);

    bd->VulkanInitInfo.MinAzageCount = min_image_count;
}

VkDescriptorSet Az_AzplVulkan_AddTexture(VkSampler sampler,
                                         VkAzageView image_view,
                                         VkAzageLayout image_layout)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkDescriptorPool pool = bd->DescriptorPool ? bd->DescriptorPool
                                               : v->DescriptorPool;

    VkDescriptorSet descriptor_set;
    {
        VkDescriptorSetAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = pool;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &bd->DescriptorSetLayout;
        VkResult err = vkAllocateDescriptorSets(v->Device,
                                                &alloc_info,
                                                &descriptor_set);
        check_vk_result(err);
    }

    {
        VkDescriptorAzageInfo desc_image[1] = {};
        desc_image[0].sampler = sampler;
        desc_image[0].imageView = image_view;
        desc_image[0].imageLayout = image_layout;
        VkWriteDescriptorSet write_desc[1] = {};
        write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_desc[0].dstSet = descriptor_set;
        write_desc[0].descriptorCount = 1;
        write_desc[0].descriptorType =
                VK_DESCRIPTOR_TYPE_COMBINED_AZAGE_SAMPLER;
        write_desc[0].pAzageInfo = desc_image;
        vkUpdateDescriptorSets(v->Device, 1, write_desc, 0, nullptr);
    }
    return descriptor_set;
}

void Az_AzplVulkan_RemoveTexture(VkDescriptorSet descriptor_set)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkDescriptorPool pool = bd->DescriptorPool ? bd->DescriptorPool
                                               : v->DescriptorPool;
    vkFreeDescriptorSets(v->Device, pool, 1, &descriptor_set);
}

void Az_AzplVulkan_DestroyFrameRenderBuffers(
        VkDevice device,
        Az_AzplVulkan_FrameRenderBuffers* buffers,
        const VkAllocationCallbacks* allocator)
{
    if (buffers->VertexBuffer)
    {
        vkDestroyBuffer(device, buffers->VertexBuffer, allocator);
        buffers->VertexBuffer = VK_NULL_HANDLE;
    }
    if (buffers->VertexBufferMemory)
    {
        vkFreeMemory(device, buffers->VertexBufferMemory, allocator);
        buffers->VertexBufferMemory = VK_NULL_HANDLE;
    }
    if (buffers->IndexBuffer)
    {
        vkDestroyBuffer(device, buffers->IndexBuffer, allocator);
        buffers->IndexBuffer = VK_NULL_HANDLE;
    }
    if (buffers->IndexBufferMemory)
    {
        vkFreeMemory(device, buffers->IndexBufferMemory, allocator);
        buffers->IndexBufferMemory = VK_NULL_HANDLE;
    }
    buffers->VertexBufferSize = 0;
    buffers->IndexBufferSize = 0;
}

void Az_AzplVulkan_DestroyWindowRenderBuffers(
        VkDevice device,
        Az_AzplVulkan_WindowRenderBuffers* buffers,
        const VkAllocationCallbacks* allocator)
{
    for (uint32_t n = 0; n < buffers->Count; n++)
        Az_AzplVulkan_DestroyFrameRenderBuffers(device,
                                                &buffers->FrameRenderBuffers[n],
                                                allocator);
    buffers->FrameRenderBuffers.clear();
    buffers->Index = 0;
    buffers->Count = 0;
}

VkSurfaceFormatKHR Az_AzplVulkanH_SelectSurfaceFormat(
        VkPhysicalDevice physical_device,
        VkSurfaceKHR surface,
        const VkFormat* request_formats,
        int request_formats_count,
        VkColorSpaceKHR request_color_space)
{
    AZ_ASSERT(g_FunctionsLoaded && "Need to call Az_AzplVulkan_LoadFunctions() if AZ_AZPL_VULKAN_NO_PROTOTYPES or VK_NO_PROTOTYPES are set!");
    AZ_ASSERT(request_formats != nullptr);
    AZ_ASSERT(request_formats_count > 0);

    uint32_t avail_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                         surface,
                                         &avail_count,
                                         nullptr);
    AzVector<VkSurfaceFormatKHR> avail_format;
    avail_format.resize((int)avail_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                         surface,
                                         &avail_count,
                                         avail_format.Data);

    if (avail_count == 1)
    {
        if (avail_format[0].format == VK_FORMAT_UNDEFINED)
        {
            VkSurfaceFormatKHR ret;
            ret.format = request_formats[0];
            ret.colorSpace = request_color_space;
            return ret;
        } else
        {
            return avail_format[0];
        }
    } else
    {
        for (int request_i = 0; request_i < request_formats_count; request_i++)
            for (uint32_t avail_i = 0; avail_i < avail_count; avail_i++)
                if (avail_format[avail_i].format == request_formats[request_i]
                    && avail_format[avail_i].colorSpace == request_color_space)
                    return avail_format[avail_i];

        return avail_format[0];
    }
}

VkPresentModeKHR Az_AzplVulkanH_SelectPresentMode(
        VkPhysicalDevice physical_device,
        VkSurfaceKHR surface,
        const VkPresentModeKHR* request_modes,
        int request_modes_count)
{
    AZ_ASSERT(g_FunctionsLoaded && "Need to call Az_AzplVulkan_LoadFunctions() if AZ_AZPL_VULKAN_NO_PROTOTYPES or VK_NO_PROTOTYPES are set!");
    AZ_ASSERT(request_modes != nullptr);
    AZ_ASSERT(request_modes_count > 0);

    uint32_t avail_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                              surface,
                                              &avail_count,
                                              nullptr);
    AzVector<VkPresentModeKHR> avail_modes;
    avail_modes.resize((int)avail_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                              surface,
                                              &avail_count,
                                              avail_modes.Data);

    for (int request_i = 0; request_i < request_modes_count; request_i++)
        for (uint32_t avail_i = 0; avail_i < avail_count; avail_i++)
            if (request_modes[request_i] == avail_modes[avail_i])
                return request_modes[request_i];

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkPhysicalDevice Az_AzplVulkanH_SelectPhysicalDevice(VkInstance instance)
{
    uint32_t gpu_count;
    VkResult err = vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr);
    check_vk_result(err);
    AZ_ASSERT(gpu_count > 0);

    AzVector<VkPhysicalDevice> gpus;
    gpus.resize(gpu_count);
    err = vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.Data);
    check_vk_result(err);

    for (VkPhysicalDevice& device : gpus)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            return device;
    }

    if (gpu_count > 0)
        return gpus[0];
    return VK_NULL_HANDLE;
}

uint32_t Az_AzplVulkanH_SelectQueueFamilyIndex(VkPhysicalDevice physical_device)
{
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);
    AzVector<VkQueueFamilyProperties> queues_properties;
    queues_properties.resize((int)count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
                                             &count,
                                             queues_properties.Data);
    for (uint32_t i = 0; i < count; i++)
        if (queues_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            return i;
    return (uint32_t)-1;
}

void Az_AzplVulkanH_CreateWindowCommandBuffers(
        VkPhysicalDevice physical_device,
        VkDevice device,
        Az_AzplVulkanH_Window* wd,
        uint32_t queue_family,
        const VkAllocationCallbacks* allocator)
{
    AZ_ASSERT(physical_device != VK_NULL_HANDLE && device != VK_NULL_HANDLE);
    AZ_UNUSED(physical_device);

    VkResult err;
    for (uint32_t i = 0; i < wd->AzageCount; i++)
    {
        Az_AzplVulkanH_Frame* fd = &wd->Frames[i];
        {
            VkCommandPoolCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            info.flags = 0;
            info.queueFamilyIndex = queue_family;
            err = vkCreateCommandPool(device,
                                      &info,
                                      allocator,
                                      &fd->CommandPool);
            check_vk_result(err);
        }
        {
            VkCommandBufferAllocateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            info.commandPool = fd->CommandPool;
            info.level = VK_COMMAND_BUFFER_LEVEL_PRAZARY;
            info.commandBufferCount = 1;
            err = vkAllocateCommandBuffers(device, &info, &fd->CommandBuffer);
            check_vk_result(err);
        }
        {
            VkFenceCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            err = vkCreateFence(device, &info, allocator, &fd->Fence);
            check_vk_result(err);
        }
    }

    for (uint32_t i = 0; i < wd->SemaphoreCount; i++)
    {
        Az_AzplVulkanH_FrameSemaphores* fsd = &wd->FrameSemaphores[i];
        {
            VkSemaphoreCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            err = vkCreateSemaphore(device,
                                    &info,
                                    allocator,
                                    &fsd->AzageAcquiredSemaphore);
            check_vk_result(err);
            err = vkCreateSemaphore(device,
                                    &info,
                                    allocator,
                                    &fsd->RenderCompleteSemaphore);
            check_vk_result(err);
        }
    }
}

int Az_AzplVulkanH_GetMinAzageCountFromPresentMode(
        VkPresentModeKHR present_mode)
{
    if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        return 3;
    if (present_mode == VK_PRESENT_MODE_FIFO_KHR
        || present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
        return 2;
    if (present_mode == VK_PRESENT_MODE_AZMEDIATE_KHR)
        return 1;
    AZ_ASSERT(0);
    return 1;
}

void Az_AzplVulkanH_CreateWindowSwapChain(
        VkPhysicalDevice physical_device,
        VkDevice device,
        Az_AzplVulkanH_Window* wd,
        const VkAllocationCallbacks* allocator,
        int w,
        int h,
        uint32_t min_image_count)
{
    VkResult err;
    VkSwapchainKHR old_swapchain = wd->Swapchain;
    wd->Swapchain = VK_NULL_HANDLE;
    err = vkDeviceWaitIdle(device);
    check_vk_result(err);

    for (uint32_t i = 0; i < wd->AzageCount; i++)
        Az_AzplVulkanH_DestroyFrame(device, &wd->Frames[i], allocator);
    for (uint32_t i = 0; i < wd->SemaphoreCount; i++)
        Az_AzplVulkanH_DestroyFrameSemaphores(device,
                                              &wd->FrameSemaphores[i],
                                              allocator);
    wd->Frames.clear();
    wd->FrameSemaphores.clear();
    wd->AzageCount = 0;
    if (wd->RenderPass)
        vkDestroyRenderPass(device, wd->RenderPass, allocator);

    if (min_image_count == 0)
        min_image_count = Az_AzplVulkanH_GetMinAzageCountFromPresentMode(
                wd->PresentMode);

    {
        VkSurfaceCapabilitiesKHR cap;
        err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device,
                                                        wd->Surface,
                                                        &cap);
        check_vk_result(err);

        VkSwapchainCreateInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        info.surface = wd->Surface;
        info.minAzageCount = min_image_count;
        info.imageFormat = wd->SurfaceFormat.format;
        info.imageColorSpace = wd->SurfaceFormat.colorSpace;
        info.imageArrayLayers = 1;
        info.imageUsage = VK_AZAGE_USAGE_COLOR_ATTACHMENT_BIT;
        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        info.preTransform = (cap.supportedTransforms
                             & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
                                    ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
                                    : cap.currentTransform;
        info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        info.presentMode = wd->PresentMode;
        info.clipped = VK_TRUE;
        info.oldSwapchain = old_swapchain;
        if (info.minAzageCount < cap.minAzageCount)
            info.minAzageCount = cap.minAzageCount;
        else if (cap.maxAzageCount != 0
                 && info.minAzageCount > cap.maxAzageCount)
            info.minAzageCount = cap.maxAzageCount;
        if (cap.currentExtent.width == 0xffffffff)
        {
            info.imageExtent.width = wd->Width = w;
            info.imageExtent.height = wd->Height = h;
        } else
        {
            info.imageExtent.width = wd->Width = cap.currentExtent.width;
            info.imageExtent.height = wd->Height = cap.currentExtent.height;
        }
        err = vkCreateSwapchainKHR(device, &info, allocator, &wd->Swapchain);
        check_vk_result(err);
        err = vkGetSwapchainAzagesKHR(device,
                                      wd->Swapchain,
                                      &wd->AzageCount,
                                      nullptr);
        check_vk_result(err);
        VkAzage backbuffers[16] = {};
        AZ_ASSERT(wd->AzageCount >= min_image_count);
        AZ_ASSERT(wd->AzageCount < AZ_ARRAYSIZE(backbuffers));
        err = vkGetSwapchainAzagesKHR(device,
                                      wd->Swapchain,
                                      &wd->AzageCount,
                                      backbuffers);
        check_vk_result(err);

        wd->SemaphoreCount = wd->AzageCount + 1;
        wd->Frames.resize(wd->AzageCount);
        wd->FrameSemaphores.resize(wd->SemaphoreCount);
        memset(wd->Frames.Data, 0, wd->Frames.size_in_bytes());
        memset(wd->FrameSemaphores.Data,
               0,
               wd->FrameSemaphores.size_in_bytes());
        for (uint32_t i = 0; i < wd->AzageCount; i++)
            wd->Frames[i].Backbuffer = backbuffers[i];
    }
    if (old_swapchain)
        vkDestroySwapchainKHR(device, old_swapchain, allocator);

    if (wd->UseDynamicRendering == false)
    {
        VkAttachmentDescription attachment = {};
        attachment.format = wd->SurfaceFormat.format;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = wd->ClearEnable ? VK_ATTACHMENT_LOAD_OP_CLEAR
                                            : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_AZAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_AZAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_AZAGE_LAYOUT_COLOR_ATTACHMENT_OPTAZAL;
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;
        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;
        info.dependencyCount = 1;
        info.pDependencies = &dependency;
        err = vkCreateRenderPass(device, &info, allocator, &wd->RenderPass);
        check_vk_result(err);
    }

    {
        VkAzageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_AZAGE_VIEW_CREATE_INFO;
        info.viewType = VK_AZAGE_VIEW_TYPE_2D;
        info.format = wd->SurfaceFormat.format;
        info.components.r = VK_COMPONENT_SWIZZLE_R;
        info.components.g = VK_COMPONENT_SWIZZLE_G;
        info.components.b = VK_COMPONENT_SWIZZLE_B;
        info.components.a = VK_COMPONENT_SWIZZLE_A;
        VkAzageSubresourceRange image_range = {VK_AZAGE_ASPECT_COLOR_BIT,
                                               0,
                                               1,
                                               0,
                                               1};
        info.subresourceRange = image_range;
        for (uint32_t i = 0; i < wd->AzageCount; i++)
        {
            Az_AzplVulkanH_Frame* fd = &wd->Frames[i];
            info.image = fd->Backbuffer;
            err = vkCreateAzageView(device,
                                    &info,
                                    allocator,
                                    &fd->BackbufferView);
            check_vk_result(err);
        }
    }

    if (wd->UseDynamicRendering == false)
    {
        VkAzageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = wd->RenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = wd->Width;
        info.height = wd->Height;
        info.layers = 1;
        for (uint32_t i = 0; i < wd->AzageCount; i++)
        {
            Az_AzplVulkanH_Frame* fd = &wd->Frames[i];
            attachment[0] = fd->BackbufferView;
            err = vkCreateFramebuffer(device,
                                      &info,
                                      allocator,
                                      &fd->Framebuffer);
            check_vk_result(err);
        }
    }
}

void Az_AzplVulkanH_CreateOrResizeWindow(VkInstance instance,
                                         VkPhysicalDevice physical_device,
                                         VkDevice device,
                                         Az_AzplVulkanH_Window* wd,
                                         uint32_t queue_family,
                                         const VkAllocationCallbacks* allocator,
                                         int width,
                                         int height,
                                         uint32_t min_image_count)
{
    AZ_ASSERT(g_FunctionsLoaded && "Need to call Az_AzplVulkan_LoadFunctions() if AZ_AZPL_VULKAN_NO_PROTOTYPES or VK_NO_PROTOTYPES are set!");
    (void)instance;
    Az_AzplVulkanH_CreateWindowSwapChain(physical_device,
                                         device,
                                         wd,
                                         allocator,
                                         width,
                                         height,
                                         min_image_count);

    Az_AzplVulkanH_CreateWindowCommandBuffers(physical_device,
                                              device,
                                              wd,
                                              queue_family,
                                              allocator);

    VkCommandPool command_pool;
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = queue_family;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkResult err =
            vkCreateCommandPool(device, &pool_info, allocator, &command_pool);
    check_vk_result(err);

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    VkFence fence;
    err = vkCreateFence(device, &fence_info, allocator, &fence);
    check_vk_result(err);

    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRAZARY;
    alloc_info.commandBufferCount = 1;
    VkCommandBuffer command_buffer;
    err = vkAllocateCommandBuffers(device, &alloc_info, &command_buffer);
    check_vk_result(err);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TAZE_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    check_vk_result(err);

    for (uint32_t i = 0; i < wd->AzageCount; i++)
    {
        VkAzageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_AZAGE_MEMORY_BARRIER;
        barrier.image = wd->Frames[i].Backbuffer;
        barrier.oldLayout = VK_AZAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_AZAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_AZAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.layerCount = 1;
        vkCmdPipelineBarrier(command_buffer,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);
    }

    err = vkEndCommandBuffer(command_buffer);
    check_vk_result(err);
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    VkQueue queue;
    vkGetDeviceQueue(device, queue_family, 0, &queue);
    err = vkQueueSubmit(queue, 1, &submit_info, fence);
    check_vk_result(err);
    err = vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
    check_vk_result(err);
    err = vkResetFences(device, 1, &fence);
    check_vk_result(err);

    err = vkResetCommandPool(device, command_pool, 0);
    check_vk_result(err);

    vkFreeCommandBuffers(device, command_pool, 1, &command_buffer);
    vkDestroyCommandPool(device, command_pool, allocator);
    vkDestroyFence(device, fence, allocator);
    command_pool = VK_NULL_HANDLE;
    command_buffer = VK_NULL_HANDLE;
    fence = VK_NULL_HANDLE;
    queue = VK_NULL_HANDLE;
}

void Az_AzplVulkanH_DestroyWindow(VkInstance instance,
                                  VkDevice device,
                                  Az_AzplVulkanH_Window* wd,
                                  const VkAllocationCallbacks* allocator)
{
    vkDeviceWaitIdle(device);

    for (uint32_t i = 0; i < wd->AzageCount; i++)
        Az_AzplVulkanH_DestroyFrame(device, &wd->Frames[i], allocator);
    for (uint32_t i = 0; i < wd->SemaphoreCount; i++)
        Az_AzplVulkanH_DestroyFrameSemaphores(device,
                                              &wd->FrameSemaphores[i],
                                              allocator);
    wd->Frames.clear();
    wd->FrameSemaphores.clear();
    vkDestroyRenderPass(device, wd->RenderPass, allocator);
    vkDestroySwapchainKHR(device, wd->Swapchain, allocator);
    vkDestroySurfaceKHR(instance, wd->Surface, allocator);

    *wd = Az_AzplVulkanH_Window();
}

void Az_AzplVulkanH_DestroyFrame(VkDevice device,
                                 Az_AzplVulkanH_Frame* fd,
                                 const VkAllocationCallbacks* allocator)
{
    vkDestroyFence(device, fd->Fence, allocator);
    vkFreeCommandBuffers(device, fd->CommandPool, 1, &fd->CommandBuffer);
    vkDestroyCommandPool(device, fd->CommandPool, allocator);
    fd->Fence = VK_NULL_HANDLE;
    fd->CommandBuffer = VK_NULL_HANDLE;
    fd->CommandPool = VK_NULL_HANDLE;

    vkDestroyAzageView(device, fd->BackbufferView, allocator);
    vkDestroyFramebuffer(device, fd->Framebuffer, allocator);
}

void Az_AzplVulkanH_DestroyFrameSemaphores(
        VkDevice device,
        Az_AzplVulkanH_FrameSemaphores* fsd,
        const VkAllocationCallbacks* allocator)
{
    vkDestroySemaphore(device, fsd->AzageAcquiredSemaphore, allocator);
    vkDestroySemaphore(device, fsd->RenderCompleteSemaphore, allocator);
    fsd->AzageAcquiredSemaphore = fsd->RenderCompleteSemaphore = VK_NULL_HANDLE;
}

void Az_AzplVulkanH_DestroyAllViewportsRenderBuffers(
        VkDevice device,
        const VkAllocationCallbacks* allocator)
{
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    for (int n = 0; n < platform_io.Viewports.Size; n++)
        if (Az_AzplVulkan_ViewportData* vd = (Az_AzplVulkan_ViewportData*)
                                                     platform_io.Viewports[n]
                                                             ->RendererUserData)
            Az_AzplVulkan_DestroyWindowRenderBuffers(device,
                                                     &vd->RenderBuffers,
                                                     allocator);
}

static void Az_AzplVulkan_CreateWindow(AzViewport* viewport)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_ViewportData* vd = AZ_NEW(Az_AzplVulkan_ViewportData)();
    viewport->RendererUserData = vd;
    Az_AzplVulkanH_Window* wd = &vd->Window;
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    VkResult err = (VkResult)platform_io.Platform_CreateVkSurface(
            viewport,
            (AzU64)v->Instance,
            (const void*)v->Allocator,
            (AzU64*)&wd->Surface);
    check_vk_result(err);

    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(v->PhysicalDevice,
                                         v->QueueFamily,
                                         wd->Surface,
                                         &res);
    if (res != VK_TRUE)
    {
        AZ_ASSERT(0);
        return;
    }

    AzVector<VkFormat> requestSurfaceAzageFormats;
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
    for (uint32_t n = 0;
         n < v->PipelineRenderingCreateInfo.colorAttachmentCount;
         n++)
        requestSurfaceAzageFormats.push_back(
                v->PipelineRenderingCreateInfo.pColorAttachmentFormats[n]);
#endif
    const VkFormat defaultFormats[] = {VK_FORMAT_B8G8R8A8_UNORM,
                                       VK_FORMAT_R8G8B8A8_UNORM,
                                       VK_FORMAT_B8G8R8_UNORM,
                                       VK_FORMAT_R8G8B8_UNORM};
    for (VkFormat format : defaultFormats)
        requestSurfaceAzageFormats.push_back(format);

    const VkColorSpaceKHR requestSurfaceColorSpace =
            VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat = Az_AzplVulkanH_SelectSurfaceFormat(
            v->PhysicalDevice,
            wd->Surface,
            requestSurfaceAzageFormats.Data,
            (size_t)requestSurfaceAzageFormats.Size,
            requestSurfaceColorSpace);

    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR,
                                        VK_PRESENT_MODE_AZMEDIATE_KHR,
                                        VK_PRESENT_MODE_FIFO_KHR};
    wd->PresentMode = Az_AzplVulkanH_SelectPresentMode(v->PhysicalDevice,
                                                       wd->Surface,
                                                       &present_modes[0],
                                                       AZ_ARRAYSIZE(
                                                               present_modes));

    wd->ClearEnable = (viewport->Flags & AzViewportFlags_NoRendererClear)
                              ? false
                              : true;
    wd->UseDynamicRendering = v->UseDynamicRendering;
    Az_AzplVulkanH_CreateOrResizeWindow(v->Instance,
                                        v->PhysicalDevice,
                                        v->Device,
                                        wd,
                                        v->QueueFamily,
                                        v->Allocator,
                                        (int)viewport->Size.x,
                                        (int)viewport->Size.y,
                                        v->MinAzageCount);
    vd->WindowOwned = true;

    if (bd->PipelineForViewports == VK_NULL_HANDLE)
        Az_AzplVulkan_CreatePipeline(v->Device,
                                     v->Allocator,
                                     VK_NULL_HANDLE,
                                     wd->RenderPass,
                                     VK_SAMPLE_COUNT_1_BIT,
                                     &bd->PipelineForViewports,
                                     0);
}

static void Az_AzplVulkan_DestroyWindow(AzViewport* viewport)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    if (Az_AzplVulkan_ViewportData* vd = (Az_AzplVulkan_ViewportData*)
                                                 viewport->RendererUserData)
    {
        Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
        if (vd->WindowOwned)
            Az_AzplVulkanH_DestroyWindow(v->Instance,
                                         v->Device,
                                         &vd->Window,
                                         v->Allocator);
        Az_AzplVulkan_DestroyWindowRenderBuffers(v->Device,
                                                 &vd->RenderBuffers,
                                                 v->Allocator);
        AZ_DELETE(vd);
    }
    viewport->RendererUserData = nullptr;
}

static void Az_AzplVulkan_SetWindowSize(AzViewport* viewport, AzVec2 size)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_ViewportData* vd = (Az_AzplVulkan_ViewportData*)
                                             viewport->RendererUserData;
    if (vd == nullptr)
        return;
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    vd->Window.ClearEnable = (viewport->Flags & AzViewportFlags_NoRendererClear)
                                     ? false
                                     : true;
    Az_AzplVulkanH_CreateOrResizeWindow(v->Instance,
                                        v->PhysicalDevice,
                                        v->Device,
                                        &vd->Window,
                                        v->QueueFamily,
                                        v->Allocator,
                                        (int)size.x,
                                        (int)size.y,
                                        v->MinAzageCount);
}

static void Az_AzplVulkan_RenderWindow(AzViewport* viewport, void*)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_ViewportData* vd = (Az_AzplVulkan_ViewportData*)
                                             viewport->RendererUserData;
    Az_AzplVulkanH_Window* wd = &vd->Window;
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;
    VkResult err;

    if (vd->SwapChainNeedRebuild || vd->SwapChainSuboptimal)
    {
        Az_AzplVulkanH_CreateOrResizeWindow(v->Instance,
                                            v->PhysicalDevice,
                                            v->Device,
                                            wd,
                                            v->QueueFamily,
                                            v->Allocator,
                                            (int)viewport->Size.x,
                                            (int)viewport->Size.y,
                                            v->MinAzageCount);
        vd->SwapChainNeedRebuild = vd->SwapChainSuboptimal = false;
    }

    Az_AzplVulkanH_Frame* fd = nullptr;
    Az_AzplVulkanH_FrameSemaphores* fsd =
            &wd->FrameSemaphores[wd->SemaphoreIndex];
    {
        {
            err = vkAcquireNextAzageKHR(v->Device,
                                        wd->Swapchain,
                                        UINT64_MAX,
                                        fsd->AzageAcquiredSemaphore,
                                        VK_NULL_HANDLE,
                                        &wd->FrameIndex);
            if (err == VK_ERROR_OUT_OF_DATE_KHR)
            {
                vd->SwapChainNeedRebuild = true;
                return;
            }
            if (err == VK_SUBOPTAZAL_KHR)
                vd->SwapChainSuboptimal = true;
            else
                check_vk_result(err);
            fd = &wd->Frames[wd->FrameIndex];
        }
        for (;;)
        {
            err = vkWaitForFences(v->Device, 1, &fd->Fence, VK_TRUE, 100);
            if (err == VK_SUCCESS)
                break;
            if (err == VK_TAZEOUT)
                continue;
            check_vk_result(err);
        }
        {
            err = vkResetCommandPool(v->Device, fd->CommandPool, 0);
            check_vk_result(err);
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TAZE_SUBMIT_BIT;
            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            check_vk_result(err);
        }
        {
            AzVec4 clear_color = AzVec4(0.0f, 0.0f, 0.0f, 1.0f);
            memcpy(&wd->ClearValue.color.float32[0],
                   &clear_color,
                   4 * sizeof(float));
        }
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
        if (v->UseDynamicRendering)
        {
            VkAzageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_AZAGE_MEMORY_BARRIER;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.oldLayout = VK_AZAGE_LAYOUT_PRESENT_SRC_KHR;
            barrier.newLayout = VK_AZAGE_LAYOUT_COLOR_ATTACHMENT_OPTAZAL;
            barrier.image = fd->Backbuffer;
            barrier.subresourceRange.aspectMask = VK_AZAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(fd->CommandBuffer,
                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                                         | VK_PIPELINE_STAGE_NONE,
                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);

            VkRenderingAttachmentInfo attachmentInfo = {};
            attachmentInfo.sType =
                    VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
            attachmentInfo.imageView = fd->BackbufferView;
            attachmentInfo.imageLayout =
                    VK_AZAGE_LAYOUT_COLOR_ATTACHMENT_OPTAZAL;
            attachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
            attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentInfo.clearValue = wd->ClearValue;

            VkRenderingInfo renderingInfo = {};
            renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
            renderingInfo.renderArea.extent.width = wd->Width;
            renderingInfo.renderArea.extent.height = wd->Height;
            renderingInfo.layerCount = 1;
            renderingInfo.viewMask = 0;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &attachmentInfo;

            AzAzplVulkanFuncs_vkCmdBeginRenderingKHR(fd->CommandBuffer,
                                                     &renderingInfo);
        } else
#endif
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = wd->RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = (viewport->Flags
                                    & AzViewportFlags_NoRendererClear)
                                           ? 0
                                           : 1;
            info.pClearValues = (viewport->Flags
                                 & AzViewportFlags_NoRendererClear)
                                        ? nullptr
                                        : &wd->ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer,
                                 &info,
                                 VK_SUBPASS_CONTENTS_INLINE);
        }
    }

    Az_AzplVulkan_RenderDrawData(viewport->DrawData,
                                 fd->CommandBuffer,
                                 bd->PipelineForViewports);

    {
#ifdef AZ_AZPL_VULKAN_HAS_DYNAMIC_RENDERING
        if (v->UseDynamicRendering)
        {
            AzAzplVulkanFuncs_vkCmdEndRenderingKHR(fd->CommandBuffer);

            VkAzageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_AZAGE_MEMORY_BARRIER;
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.oldLayout = VK_AZAGE_LAYOUT_COLOR_ATTACHMENT_OPTAZAL;
            barrier.newLayout = VK_AZAGE_LAYOUT_PRESENT_SRC_KHR;
            barrier.image = fd->Backbuffer;
            barrier.subresourceRange.aspectMask = VK_AZAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(fd->CommandBuffer,
                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &barrier);
        } else
#endif
        {
            vkCmdEndRenderPass(fd->CommandBuffer);
        }
        {
            VkPipelineStageFlags wait_stage =
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &fsd->AzageAcquiredSemaphore;
            info.pWaitDstStageMask = &wait_stage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &fsd->RenderCompleteSemaphore;

            err = vkEndCommandBuffer(fd->CommandBuffer);
            check_vk_result(err);
            err = vkResetFences(v->Device, 1, &fd->Fence);
            check_vk_result(err);
            err = vkQueueSubmit(v->Queue, 1, &info, fd->Fence);
            check_vk_result(err);
        }
    }
}

static void Az_AzplVulkan_SwapBuffers(AzViewport* viewport, void*)
{
    Az_AzplVulkan_Data* bd = Az_AzplVulkan_GetBackendData();
    Az_AzplVulkan_ViewportData* vd = (Az_AzplVulkan_ViewportData*)
                                             viewport->RendererUserData;
    Az_AzplVulkanH_Window* wd = &vd->Window;
    Az_AzplVulkan_InitInfo* v = &bd->VulkanInitInfo;

    if (vd->SwapChainNeedRebuild)
        return;

    VkResult err;
    uint32_t present_index = wd->FrameIndex;

    Az_AzplVulkanH_FrameSemaphores* fsd =
            &wd->FrameSemaphores[wd->SemaphoreIndex];
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &fsd->RenderCompleteSemaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &wd->Swapchain;
    info.pAzageIndices = &present_index;
    err = vkQueuePresentKHR(v->Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR)
    {
        vd->SwapChainNeedRebuild = true;
        return;
    }
    if (err == VK_SUBOPTAZAL_KHR)
        vd->SwapChainSuboptimal = true;
    else
        check_vk_result(err);
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount;
}

void Az_AzplVulkan_InitMultiViewportSupport()
{
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    if (Az::GetIO().ConfigFlags & AzConfigFlags_ViewportsEnable)
        AZ_ASSERT(platform_io.Platform_CreateVkSurface != nullptr
                  && "Platform needs to setup the CreateVkSurface handler.");
    platform_io.Renderer_CreateWindow = Az_AzplVulkan_CreateWindow;
    platform_io.Renderer_DestroyWindow = Az_AzplVulkan_DestroyWindow;
    platform_io.Renderer_SetWindowSize = Az_AzplVulkan_SetWindowSize;
    platform_io.Renderer_RenderWindow = Az_AzplVulkan_RenderWindow;
    platform_io.Renderer_SwapBuffers = Az_AzplVulkan_SwapBuffers;
}

void Az_AzplVulkan_ShutdownMultiViewportSupport()
{
    Az::DestroyPlatformWindows();
}

#endif
