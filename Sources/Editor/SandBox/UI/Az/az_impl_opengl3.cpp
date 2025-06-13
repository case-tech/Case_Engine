#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Az/Az.hpp>
#ifndef AZ_DISABLE
#include "az_impl_opengl3.hpp"

#include <stdint.h>
#include <stdio.h>
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-macros"
#pragma clang diagnostic ignored "-Wnonportable-system-include-path"
#pragma clang diagnostic ignored "-Wcast-function-type"
#endif
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif

#if defined(AZ_IMPL_OPENGL_ES2)
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV))
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif
#if defined(__EMSCRIPTEN__)
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GLES2/gl2ext.h>
#endif
#elif defined(AZ_IMPL_OPENGL_ES3)
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV))
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#endif
#elif !defined(AZ_IMPL_OPENGL_LOADER_CUSTOM)

#define AZGL3W_IMPL
#define AZ_IMPL_OPENGL_LOADER_AZGL3W
#include "az_impl_opengl3_loader.hpp"
#endif

#ifndef AZ_IMPL_OPENGL_ES2
#define AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
#elif defined(__EMSCRIPTEN__)
#define AZ_AZPL_OPENGL_USE_VERTEX_ARRAY
#define glBindVertexArray glBindVertexArrayOES
#define glGenVertexArrays glGenVertexArraysOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define GL_VERTEX_ARRAY_BINDING GL_VERTEX_ARRAY_BINDING_OES
#endif

#if !defined(AZ_IMPL_OPENGL_ES2) && !defined(AZ_IMPL_OPENGL_ES3)
#define AZ_IMPL_OPENGL_HAS_EXTENSIONS
#define AZ_IMPL_OPENGL_MAY_HAVE_POLYGON_MODE
#endif

#if !defined(AZ_IMPL_OPENGL_ES2)
#define AZ_IMPL_OPENGL_MAY_HAVE_BIND_BUFFER_PIXEL_UNPACK
#endif

#if !defined(AZ_IMPL_OPENGL_ES2) && !defined(AZ_IMPL_OPENGL_ES3)               \
        && defined(GL_VERSION_3_1)
#define AZ_IMPL_OPENGL_MAY_HAVE_PRIMITIVE_RESTART
#endif

#if !defined(AZ_IMPL_OPENGL_ES2) && !defined(AZ_IMPL_OPENGL_ES3)               \
        && defined(GL_VERSION_3_2)
#define AZ_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
#endif

#if !defined(AZ_IMPL_OPENGL_ES2)                                               \
        && (defined(AZ_IMPL_OPENGL_ES3) || defined(GL_VERSION_3_3))
#define AZ_IMPL_OPENGL_MAY_HAVE_BIND_SAMPLER
#endif

#ifdef AZ_IMPL_OPENGL_DEBUG
#include <stdio.h>
#define GL_CALL(_CALL)                                                         \
    do                                                                         \
    {                                                                          \
        _CALL;                                                                 \
        GLenum gl_err = glGetError();                                          \
        if (gl_err != 0)                                                       \
            fprintf(stderr,                                                    \
                    "GL error 0x%x returned from '%s'.\n",                     \
                    gl_err,                                                    \
                    #_CALL);                                                   \
    } while (0)
#else
#define GL_CALL(_CALL) _CALL
#endif

struct Az_ImplOpenGL3_Data
{
    GLuint GlVersion;
    char GlslVersionString[32];
    bool GlProfileIsES2;
    bool GlProfileIsES3;
    bool GlProfileIsCompat;
    GLint GlProfileMask;
    GLint MaxTextureSize;
    GLuint ShaderHandle;
    GLint AttribLocationTex;
    GLint AttribLocationProjMtx;
    GLuint AttribLocationVtxPos;
    GLuint AttribLocationVtxUV;
    GLuint AttribLocationVtxColor;
    unsigned int VboHandle, ElementsHandle;
    GLsizeiptr VertexBufferSize;
    GLsizeiptr IndexBufferSize;
    bool HasPolygonMode;
    bool HasClipOrigin;
    bool UseBufferSubData;
    AzVector<char> TempBuffer;

    Az_ImplOpenGL3_Data()
    {
        memset((void*)this, 0, sizeof(*this));
    }
};

static Az_ImplOpenGL3_Data* Az_ImplOpenGL3_GetBackendData()
{
    return Az::GetCurrentContext()
                   ? (Az_ImplOpenGL3_Data*)Az::GetIO().BackendRendererUserData
                   : nullptr;
}

static void Az_ImplOpenGL3_InitMultiViewportSupport();
static void Az_ImplOpenGL3_ShutdownMultiViewportSupport();

#ifndef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
struct Az_ImplOpenGL3_VtxAttribState
{
    GLint Enabled, Size, Type, Normalized, Stride;
    GLvoid* Ptr;

    void GetState(GLint index)
    {
        glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &Enabled);
        glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &Size);
        glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &Type);
        glGetVertexAttribiv(index,
                            GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                            &Normalized);
        glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &Stride);
        glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &Ptr);
    }
    void SetState(GLint index)
    {
        glVertexAttribPointer(index,
                              Size,
                              Type,
                              (GLboolean)Normalized,
                              Stride,
                              Ptr);
        if (Enabled)
            glEnableVertexAttribArray(index);
        else
            glDisableVertexAttribArray(index);
    }
};
#endif

bool Az_ImplOpenGL3_InitLoader();
bool Az_ImplOpenGL3_InitLoader()
{
#ifdef AZ_IMPL_OPENGL_LOADER_AZGL3W
    if (glGetIntegerv == nullptr && imgl3wInit() != 0)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return false;
    }
#endif
    return true;
}

bool Az_ImplOpenGL3_Init(const char* glsl_version)
{
    AzIO& io = Az::GetIO();
    AZ_CHECKVERSION();
    AZ_ASSERT(io.BackendRendererUserData == nullptr
              && "Already initialized a renderer backend!");

    if (!Az_ImplOpenGL3_InitLoader())
        return false;

    Az_ImplOpenGL3_Data* bd = AZ_NEW(Az_ImplOpenGL3_Data)();
    io.BackendRendererUserData = (void*)bd;
    io.BackendRendererName = "imgui_impl_opengl3";

    const char* gl_version_str = (const char*)glGetString(GL_VERSION);
#if defined(AZ_IMPL_OPENGL_ES2)

    bd->GlVersion = 200;
    bd->GlProfileIsES2 = true;
    AZ_UNUSED(gl_version_str);
#else

    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major == 0 && minor == 0)
        sscanf(gl_version_str, "%d.%d", &major, &minor);
    bd->GlVersion = (GLuint)(major * 100 + minor * 10);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &bd->MaxTextureSize);

#if defined(AZ_IMPL_OPENGL_ES3)
    bd->GlProfileIsES3 = true;
#else
    if (strncmp(gl_version_str, "OpenGL ES 3", 11) == 0)
        bd->GlProfileIsES3 = true;
#endif

#if defined(GL_CONTEXT_PROFILE_MASK)
    if (!bd->GlProfileIsES3 && bd->GlVersion >= 320)
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &bd->GlProfileMask);
    bd->GlProfileIsCompat = (bd->GlProfileMask
                             & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
                            != 0;
#endif

    bd->UseBufferSubData = false;

#endif

#ifdef AZ_IMPL_OPENGL_DEBUG
    printf("GlVersion = %d, \"%s\"\nGlProfileIsCompat = %d\nGlProfileMask = "
           "0x%X\nGlProfileIsES2/IsEs3 = %d/%d\nGL_VENDOR = '%s'\nGL_RENDERER "
           "= '%s'\n",
           bd->GlVersion,
           gl_version_str,
           bd->GlProfileIsCompat,
           bd->GlProfileMask,
           bd->GlProfileIsES2,
           bd->GlProfileIsES3,
           (const char*)glGetString(GL_VENDOR),
           (const char*)glGetString(GL_RENDERER));
#endif

#ifdef AZ_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
    if (bd->GlVersion >= 320)
        io.BackendFlags |= AzBackendFlags_RendererHasVtxOffset;
#endif
    io.BackendFlags |= AzBackendFlags_RendererHasTextures;
    io.BackendFlags |= AzBackendFlags_RendererHasViewports;

    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Renderer_TextureMaxWidth =
            platform_io.Renderer_TextureMaxHeight = (int)bd->MaxTextureSize;

    if (glsl_version == nullptr)
    {
#if defined(AZ_IMPL_OPENGL_ES2)
        glsl_version = "#version 100";
#elif defined(AZ_IMPL_OPENGL_ES3)
        glsl_version = "#version 300 es";
#elif defined(__APPLE__)
        glsl_version = "#version 150";
#else
        glsl_version = "#version 130";
#endif
    }

    AZ_ASSERT((int)strlen(glsl_version) + 2
              < AZ_ARRAYSIZE(bd->GlslVersionString));
    strcpy(bd->GlslVersionString, glsl_version);
    strcat(bd->GlslVersionString, "\n");

    GLint current_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);

#ifdef AZ_IMPL_OPENGL_MAY_HAVE_POLYGON_MODE
    bd->HasPolygonMode = (!bd->GlProfileIsES2 && !bd->GlProfileIsES3);
#endif
    bd->HasClipOrigin = (bd->GlVersion >= 450);
#ifdef AZ_IMPL_OPENGL_HAS_EXTENSIONS
    GLint num_extensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    for (GLint i = 0; i < num_extensions; i++)
    {
        const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
        if (extension != nullptr
            && strcmp(extension, "GL_ARB_clip_control") == 0)
            bd->HasClipOrigin = true;
    }
#endif

    Az_ImplOpenGL3_InitMultiViewportSupport();

    return true;
}

void Az_ImplOpenGL3_Shutdown()
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();
    AZ_ASSERT(bd != nullptr
              && "No renderer backend to shutdown, or already shutdown?");
    AzIO& io = Az::GetIO();

    Az_ImplOpenGL3_ShutdownMultiViewportSupport();
    Az_ImplOpenGL3_DestroyDeviceObjects();
    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
    io.BackendFlags &= ~(AzBackendFlags_RendererHasVtxOffset
                         | AzBackendFlags_RendererHasTextures
                         | AzBackendFlags_RendererHasViewports);
    AZ_DELETE(bd);
}

void Az_ImplOpenGL3_NewFrame()
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();
    AZ_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call Az_AzplOpenGL3_Init()?");

    Az_ImplOpenGL3_InitLoader();

    if (!bd->ShaderHandle)
        Az_ImplOpenGL3_CreateDeviceObjects();
}

static void Az_ImplOpenGL3_SetupRenderState(AzDrawData* draw_data,
                                            int fb_width,
                                            int fb_height,
                                            GLuint vertex_array_object)
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA,
                        GL_ONE_MINUS_SRC_ALPHA,
                        GL_ONE,
                        GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_SCISSOR_TEST);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_PRIMITIVE_RESTART
    if (!bd->GlProfileIsES3 && bd->GlVersion >= 310)
        glDisable(GL_PRIMITIVE_RESTART);
#endif
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_POLYGON_MODE
    if (bd->HasPolygonMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

#if defined(GL_CLIP_ORIGIN)
    bool clip_origin_lower_left = true;
    if (bd->HasClipOrigin)
    {
        GLenum current_clip_origin = 0;
        glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&current_clip_origin);
        if (current_clip_origin == GL_UPPER_LEFT)
            clip_origin_lower_left = false;
    }
#endif

    GL_CALL(glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height));
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
#if defined(GL_CLIP_ORIGIN)
    if (!clip_origin_lower_left)
    {
        float tmp = T;
        T = B;
        B = tmp;
    }
#endif
    const float ortho_projection[4][4] = {
            {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
            {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f, 0.0f},
            {(R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f},
    };
    glUseProgram(bd->ShaderHandle);
    glUniform1i(bd->AttribLocationTex, 0);
    glUniformMatrix4fv(bd->AttribLocationProjMtx,
                       1,
                       GL_FALSE,
                       &ortho_projection[0][0]);

#ifdef AZ_IMPL_OPENGL_MAY_HAVE_BIND_SAMPLER
    if (bd->GlVersion >= 330 || bd->GlProfileIsES3)
        glBindSampler(0, 0);
#endif

    (void)vertex_array_object;
#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    glBindVertexArray(vertex_array_object);
#endif

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, bd->VboHandle));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd->ElementsHandle));
    GL_CALL(glEnableVertexAttribArray(bd->AttribLocationVtxPos));
    GL_CALL(glEnableVertexAttribArray(bd->AttribLocationVtxUV));
    GL_CALL(glEnableVertexAttribArray(bd->AttribLocationVtxColor));
    GL_CALL(glVertexAttribPointer(bd->AttribLocationVtxPos,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(AzDrawVert),
                                  (GLvoid*)offsetof(AzDrawVert, pos)));
    GL_CALL(glVertexAttribPointer(bd->AttribLocationVtxUV,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(AzDrawVert),
                                  (GLvoid*)offsetof(AzDrawVert, uv)));
    GL_CALL(glVertexAttribPointer(bd->AttribLocationVtxColor,
                                  4,
                                  GL_UNSIGNED_BYTE,
                                  GL_TRUE,
                                  sizeof(AzDrawVert),
                                  (GLvoid*)offsetof(AzDrawVert, col)));
}

void Az_ImplOpenGL3_RenderDrawData(AzDrawData* draw_data)
{
    int fb_width = (int)(draw_data->DisplaySize.x
                         * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y
                          * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    Az_ImplOpenGL3_InitLoader();

    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();

    if (draw_data->Textures != nullptr)
        for (AzTextureData* tex : *draw_data->Textures)
            if (tex->Status != AzTextureStatus_OK)
                Az_ImplOpenGL3_UpdateTexture(tex);

    GLenum last_active_texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLuint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
    GLuint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_BIND_SAMPLER
    GLuint last_sampler;
    if (bd->GlVersion >= 330 || bd->GlProfileIsES3)
    {
        glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&last_sampler);
    } else
    {
        last_sampler = 0;
    }
#endif
    GLuint last_array_buffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);
#ifndef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY

    GLint last_element_array_buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    Az_AzplOpenGL3_VtxAttribState last_vtx_attrib_state_pos;
    last_vtx_attrib_state_pos.GetState(bd->AttribLocationVtxPos);
    Az_AzplOpenGL3_VtxAttribState last_vtx_attrib_state_uv;
    last_vtx_attrib_state_uv.GetState(bd->AttribLocationVtxUV);
    Az_AzplOpenGL3_VtxAttribState last_vtx_attrib_state_color;
    last_vtx_attrib_state_color.GetState(bd->AttribLocationVtxColor);
#endif
#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    GLuint last_vertex_array_object;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);
#endif
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_POLYGON_MODE
    GLint last_polygon_mode[2];
    if (bd->HasPolygonMode)
    {
        glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
    }
#endif
    GLint last_viewport[4];
    glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4];
    glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb;
    glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb;
    glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb;
    glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha;
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_PRIMITIVE_RESTART
    GLboolean last_enable_primitive_restart =
            (!bd->GlProfileIsES3 && bd->GlVersion >= 310)
                    ? glIsEnabled(GL_PRIMITIVE_RESTART)
                    : GL_FALSE;
#endif

    GLuint vertex_array_object = 0;
#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    GL_CALL(glGenVertexArrays(1, &vertex_array_object));
#endif
    Az_ImplOpenGL3_SetupRenderState(draw_data,
                                    fb_width,
                                    fb_height,
                                    vertex_array_object);

    AzVec2 clip_off = draw_data->DisplayPos;
    AzVec2 clip_scale = draw_data->FramebufferScale;

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const AzDrawList* draw_list = draw_data->CmdLists[n];

        const GLsizeiptr vtx_buffer_size = (GLsizeiptr)draw_list->VtxBuffer.Size
                                           * (int)sizeof(AzDrawVert);
        const GLsizeiptr idx_buffer_size = (GLsizeiptr)draw_list->IdxBuffer.Size
                                           * (int)sizeof(AzDrawIdx);
        if (bd->UseBufferSubData)
        {
            if (bd->VertexBufferSize < vtx_buffer_size)
            {
                bd->VertexBufferSize = vtx_buffer_size;
                GL_CALL(glBufferData(GL_ARRAY_BUFFER,
                                     bd->VertexBufferSize,
                                     nullptr,
                                     GL_STREAM_DRAW));
            }
            if (bd->IndexBufferSize < idx_buffer_size)
            {
                bd->IndexBufferSize = idx_buffer_size;
                GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                     bd->IndexBufferSize,
                                     nullptr,
                                     GL_STREAM_DRAW));
            }
            GL_CALL(glBufferSubData(GL_ARRAY_BUFFER,
                                    0,
                                    vtx_buffer_size,
                                    (const GLvoid*)draw_list->VtxBuffer.Data));
            GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                                    0,
                                    idx_buffer_size,
                                    (const GLvoid*)draw_list->IdxBuffer.Data));
        } else
        {
            GL_CALL(glBufferData(GL_ARRAY_BUFFER,
                                 vtx_buffer_size,
                                 (const GLvoid*)draw_list->VtxBuffer.Data,
                                 GL_STREAM_DRAW));
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                 idx_buffer_size,
                                 (const GLvoid*)draw_list->IdxBuffer.Data,
                                 GL_STREAM_DRAW));
        }

        for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
        {
            const AzDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr)
            {
                if (pcmd->UserCallback == AzDrawCallback_ResetRenderState)
                    Az_ImplOpenGL3_SetupRenderState(draw_data,
                                                    fb_width,
                                                    fb_height,
                                                    vertex_array_object);
                else
                    pcmd->UserCallback(draw_list, pcmd);
            } else
            {
                AzVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                                (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                AzVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                                (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                GL_CALL(glScissor((int)clip_min.x,
                                  (int)((float)fb_height - clip_max.y),
                                  (int)(clip_max.x - clip_min.x),
                                  (int)(clip_max.y - clip_min.y)));

                GL_CALL(glBindTexture(GL_TEXTURE_2D,
                                      (GLuint)(intptr_t)pcmd->GetTexID()));
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
                if (bd->GlVersion >= 320)
                    GL_CALL(glDrawElementsBaseVertex(
                            GL_TRIANGLES,
                            (GLsizei)pcmd->ElemCount,
                            sizeof(AzDrawIdx) == 2 ? GL_UNSIGNED_SHORT
                                                   : GL_UNSIGNED_INT,
                            (void*)(intptr_t)(pcmd->IdxOffset
                                              * sizeof(AzDrawIdx)),
                            (GLint)pcmd->VtxOffset));
                else
#endif
                    GL_CALL(glDrawElements(
                            GL_TRIANGLES,
                            (GLsizei)pcmd->ElemCount,
                            sizeof(AzDrawIdx) == 2 ? GL_UNSIGNED_SHORT
                                                   : GL_UNSIGNED_INT,
                            (void*)(intptr_t)(pcmd->IdxOffset
                                              * sizeof(AzDrawIdx))));
            }
        }
    }

#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    GL_CALL(glDeleteVertexArrays(1, &vertex_array_object));
#endif

    if (last_program == 0 || glIsProgram(last_program))
        glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_BIND_SAMPLER
    if (bd->GlVersion >= 330 || bd->GlProfileIsES3)
        glBindSampler(0, last_sampler);
#endif
    glActiveTexture(last_active_texture);
#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    glBindVertexArray(last_vertex_array_object);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifndef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    last_vtx_attrib_state_pos.SetState(bd->AttribLocationVtxPos);
    last_vtx_attrib_state_uv.SetState(bd->AttribLocationVtxUV);
    last_vtx_attrib_state_color.SetState(bd->AttribLocationVtxColor);
#endif
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb,
                        last_blend_dst_rgb,
                        last_blend_src_alpha,
                        last_blend_dst_alpha);
    if (last_enable_blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    if (last_enable_cull_face)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    if (last_enable_depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    if (last_enable_stencil_test)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
    if (last_enable_scissor_test)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_PRIMITIVE_RESTART
    if (!bd->GlProfileIsES3 && bd->GlVersion >= 310)
    {
        if (last_enable_primitive_restart)
            glEnable(GL_PRIMITIVE_RESTART);
        else
            glDisable(GL_PRIMITIVE_RESTART);
    }
#endif

#ifdef AZ_IMPL_OPENGL_MAY_HAVE_POLYGON_MODE

    if (bd->HasPolygonMode)
    {
        if (bd->GlVersion <= 310 || bd->GlProfileIsCompat)
        {
            glPolygonMode(GL_FRONT, (GLenum)last_polygon_mode[0]);
            glPolygonMode(GL_BACK, (GLenum)last_polygon_mode[1]);
        } else
        {
            glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
        }
    }
#endif

    glViewport(last_viewport[0],
               last_viewport[1],
               (GLsizei)last_viewport[2],
               (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0],
              last_scissor_box[1],
              (GLsizei)last_scissor_box[2],
              (GLsizei)last_scissor_box[3]);
    (void)bd;
}

static void Az_ImplOpenGL3_DestroyTexture(AzTextureData* tex)
{
    GLuint gl_tex_id = (GLuint)(intptr_t)tex->TexID;
    glDeleteTextures(1, &gl_tex_id);

    tex->SetTexID(AzTextureID_Invalid);
    tex->SetStatus(AzTextureStatus_Destroyed);
}

void Az_ImplOpenGL3_UpdateTexture(AzTextureData* tex)
{
    if (tex->Status == AzTextureStatus_WantCreate)
    {
        AZ_ASSERT(tex->TexID == 0 && tex->BackendUserData == nullptr);
        AZ_ASSERT(tex->Format == AzTextureFormat_RGBA32);
        const void* pixels = tex->GetPixels();
        GLuint gl_texture_id = 0;

        GLint last_texture;
        GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture));
        GL_CALL(glGenTextures(1, &gl_texture_id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_texture_id));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE));
#ifdef GL_UNPACK_ROW_LENGTH
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
#endif
        GL_CALL(glTexAzage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGBA,
                             tex->Width,
                             tex->Height,
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             pixels));

        tex->SetTexID((AzTextureID)(intptr_t)gl_texture_id);
        tex->SetStatus(AzTextureStatus_OK);

        GL_CALL(glBindTexture(GL_TEXTURE_2D, last_texture));
    } else if (tex->Status == AzTextureStatus_WantUpdates)
    {
        GLint last_texture;
        GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture));

        GLuint gl_tex_id = (GLuint)(intptr_t)tex->TexID;
        GL_CALL(glBindTexture(GL_TEXTURE_2D, gl_tex_id));
#if 0
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->Width));
        for (AzTextureRect& r : tex->Updates)
            GL_CALL(glTexSubAzage2D(GL_TEXTURE_2D, 0, r.x, r.y, r.w, r.h, GL_RGBA, GL_UNSIGNED_BYTE, tex->GetPixelsAt(r.x, r.y)));
        GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
#else

        Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();
        for (AzTextureRect& r : tex->Updates)
        {
            const int src_pitch = r.w * tex->BytesPerPixel;
            bd->TempBuffer.resize(r.h * src_pitch);
            char* out_p = bd->TempBuffer.Data;
            for (int y = 0; y < r.h; y++, out_p += src_pitch)
                memcpy(out_p, tex->GetPixelsAt(r.x, r.y + y), src_pitch);
            AZ_ASSERT(out_p == bd->TempBuffer.end());
            GL_CALL(glTexSubAzage2D(GL_TEXTURE_2D,
                                    0,
                                    r.x,
                                    r.y,
                                    r.w,
                                    r.h,
                                    GL_RGBA,
                                    GL_UNSIGNED_BYTE,
                                    bd->TempBuffer.Data));
        }
#endif
        tex->SetStatus(AzTextureStatus_OK);
        GL_CALL(glBindTexture(GL_TEXTURE_2D, last_texture));
    } else if (tex->Status == AzTextureStatus_WantDestroy
               && tex->UnusedFrames > 0)
        Az_ImplOpenGL3_DestroyTexture(tex);
}

static bool CheckShader(GLuint handle, const char* desc)
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();
    GLint status = 0, log_length = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr,
                "ERROR: Az_AzplOpenGL3_CreateDeviceObjects: failed to compile "
                "%s! With GLSL: %s\n",
                desc,
                bd->GlslVersionString);
    if (log_length > 1)
    {
        AzVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetShaderInfoLog(handle, log_length, nullptr, (GLchar*)buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
    }
    return (GLboolean)status == GL_TRUE;
}

static bool CheckProgram(GLuint handle, const char* desc)
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr,
                "ERROR: Az_AzplOpenGL3_CreateDeviceObjects: failed to link %s! "
                "With GLSL %s\n",
                desc,
                bd->GlslVersionString);
    if (log_length > 1)
    {
        AzVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetProgramInfoLog(handle, log_length, nullptr, (GLchar*)buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
    }
    return (GLboolean)status == GL_TRUE;
}

bool Az_ImplOpenGL3_CreateDeviceObjects()
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();

    GLint last_texture, last_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_BIND_BUFFER_PIXEL_UNPACK
    GLint last_pixel_unpack_buffer = 0;
    if (bd->GlVersion >= 210)
    {
        glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING,
                      &last_pixel_unpack_buffer);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
#endif
#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    GLint last_vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
#endif

    int glsl_version = 130;
    sscanf(bd->GlslVersionString, "#version %d", &glsl_version);

    const GLchar* vertex_shader_glsl_120 =
            "uniform mat4 ProjMtx;\n"
            "attribute vec2 Position;\n"
            "attribute vec2 UV;\n"
            "attribute vec4 Color;\n"
            "varying vec2 Frag_UV;\n"
            "varying vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    Frag_UV = UV;\n"
            "    Frag_Color = Color;\n"
            "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
            "}\n";

    const GLchar* vertex_shader_glsl_130 =
            "uniform mat4 ProjMtx;\n"
            "in vec2 Position;\n"
            "in vec2 UV;\n"
            "in vec4 Color;\n"
            "out vec2 Frag_UV;\n"
            "out vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    Frag_UV = UV;\n"
            "    Frag_Color = Color;\n"
            "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
            "}\n";

    const GLchar* vertex_shader_glsl_300_es =
            "precision highp float;\n"
            "layout (location = 0) in vec2 Position;\n"
            "layout (location = 1) in vec2 UV;\n"
            "layout (location = 2) in vec4 Color;\n"
            "uniform mat4 ProjMtx;\n"
            "out vec2 Frag_UV;\n"
            "out vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    Frag_UV = UV;\n"
            "    Frag_Color = Color;\n"
            "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
            "}\n";

    const GLchar* vertex_shader_glsl_410_core =
            "layout (location = 0) in vec2 Position;\n"
            "layout (location = 1) in vec2 UV;\n"
            "layout (location = 2) in vec4 Color;\n"
            "uniform mat4 ProjMtx;\n"
            "out vec2 Frag_UV;\n"
            "out vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    Frag_UV = UV;\n"
            "    Frag_Color = Color;\n"
            "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
            "}\n";

    const GLchar* fragment_shader_glsl_120 =
            "#ifdef GL_ES\n"
            "    precision mediump float;\n"
            "#endif\n"
            "uniform sampler2D Texture;\n"
            "varying vec2 Frag_UV;\n"
            "varying vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
            "}\n";

    const GLchar* fragment_shader_glsl_130 =
            "uniform sampler2D Texture;\n"
            "in vec2 Frag_UV;\n"
            "in vec4 Frag_Color;\n"
            "out vec4 Out_Color;\n"
            "void main()\n"
            "{\n"
            "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
            "}\n";

    const GLchar* fragment_shader_glsl_300_es =
            "precision mediump float;\n"
            "uniform sampler2D Texture;\n"
            "in vec2 Frag_UV;\n"
            "in vec4 Frag_Color;\n"
            "layout (location = 0) out vec4 Out_Color;\n"
            "void main()\n"
            "{\n"
            "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
            "}\n";

    const GLchar* fragment_shader_glsl_410_core =
            "in vec2 Frag_UV;\n"
            "in vec4 Frag_Color;\n"
            "uniform sampler2D Texture;\n"
            "layout (location = 0) out vec4 Out_Color;\n"
            "void main()\n"
            "{\n"
            "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
            "}\n";

    const GLchar* vertex_shader = nullptr;
    const GLchar* fragment_shader = nullptr;
    if (glsl_version < 130)
    {
        vertex_shader = vertex_shader_glsl_120;
        fragment_shader = fragment_shader_glsl_120;
    } else if (glsl_version >= 410)
    {
        vertex_shader = vertex_shader_glsl_410_core;
        fragment_shader = fragment_shader_glsl_410_core;
    } else if (glsl_version == 300)
    {
        vertex_shader = vertex_shader_glsl_300_es;
        fragment_shader = fragment_shader_glsl_300_es;
    } else
    {
        vertex_shader = vertex_shader_glsl_130;
        fragment_shader = fragment_shader_glsl_130;
    }

    const GLchar* vertex_shader_with_version[2] = {bd->GlslVersionString,
                                                   vertex_shader};
    GLuint vert_handle;
    GL_CALL(vert_handle = glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(vert_handle, 2, vertex_shader_with_version, nullptr);
    glCompileShader(vert_handle);
    CheckShader(vert_handle, "vertex shader");

    const GLchar* fragment_shader_with_version[2] = {bd->GlslVersionString,
                                                     fragment_shader};
    GLuint frag_handle;
    GL_CALL(frag_handle = glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(frag_handle, 2, fragment_shader_with_version, nullptr);
    glCompileShader(frag_handle);
    CheckShader(frag_handle, "fragment shader");

    bd->ShaderHandle = glCreateProgram();
    glAttachShader(bd->ShaderHandle, vert_handle);
    glAttachShader(bd->ShaderHandle, frag_handle);
    glLinkProgram(bd->ShaderHandle);
    CheckProgram(bd->ShaderHandle, "shader program");

    glDetachShader(bd->ShaderHandle, vert_handle);
    glDetachShader(bd->ShaderHandle, frag_handle);
    glDeleteShader(vert_handle);
    glDeleteShader(frag_handle);

    bd->AttribLocationTex = glGetUniformLocation(bd->ShaderHandle, "Texture");
    bd->AttribLocationProjMtx = glGetUniformLocation(bd->ShaderHandle,
                                                     "ProjMtx");
    bd->AttribLocationVtxPos = (GLuint)glGetAttribLocation(bd->ShaderHandle,
                                                           "Position");
    bd->AttribLocationVtxUV = (GLuint)glGetAttribLocation(bd->ShaderHandle,
                                                          "UV");
    bd->AttribLocationVtxColor = (GLuint)glGetAttribLocation(bd->ShaderHandle,
                                                             "Color");

    glGenBuffers(1, &bd->VboHandle);
    glGenBuffers(1, &bd->ElementsHandle);

    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifdef AZ_IMPL_OPENGL_MAY_HAVE_BIND_BUFFER_PIXEL_UNPACK
    if (bd->GlVersion >= 210)
    {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, last_pixel_unpack_buffer);
    }
#endif
#ifdef AZ_IMPL_OPENGL_USE_VERTEX_ARRAY
    glBindVertexArray(last_vertex_array);
#endif

    return true;
}

void Az_ImplOpenGL3_DestroyDeviceObjects()
{
    Az_ImplOpenGL3_Data* bd = Az_ImplOpenGL3_GetBackendData();
    if (bd->VboHandle)
    {
        glDeleteBuffers(1, &bd->VboHandle);
        bd->VboHandle = 0;
    }
    if (bd->ElementsHandle)
    {
        glDeleteBuffers(1, &bd->ElementsHandle);
        bd->ElementsHandle = 0;
    }
    if (bd->ShaderHandle)
    {
        glDeleteProgram(bd->ShaderHandle);
        bd->ShaderHandle = 0;
    }

    for (AzTextureData* tex : Az::GetPlatformIO().Textures)
        if (tex->RefCount == 1)
            Az_ImplOpenGL3_DestroyTexture(tex);
}

static void Az_ImplOpenGL3_RenderWindow(AzViewport* viewport, void*)
{
    if (!(viewport->Flags & AzViewportFlags_NoRendererClear))
    {
        AzVec4 clear_color = AzVec4(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(clear_color.x,
                     clear_color.y,
                     clear_color.z,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    Az_ImplOpenGL3_RenderDrawData(viewport->DrawData);
}

static void Az_ImplOpenGL3_InitMultiViewportSupport()
{
    AzPlatformIO& platform_io = Az::GetPlatformIO();
    platform_io.Renderer_RenderWindow = Az_ImplOpenGL3_RenderWindow;
}

static void Az_ImplOpenGL3_ShutdownMultiViewportSupport()
{
    Az::DestroyPlatformWindows();
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif
