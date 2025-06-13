// Copyright (c) 2025 Case Technologies

#pragma once
#include <Az/Az.hpp>
#ifndef AZ_DISABLE

AZ_IMPL_API bool Az_ImplOpenGL3_Init(const char* glsl_version = nullptr);
AZ_IMPL_API void Az_ImplOpenGL3_Shutdown();
AZ_IMPL_API void Az_ImplOpenGL3_NewFrame();
AZ_IMPL_API void Az_ImplOpenGL3_RenderDrawData(AzDrawData* draw_data);

AZ_IMPL_API bool Az_ImplOpenGL3_CreateDeviceObjects();
AZ_IMPL_API void Az_ImplOpenGL3_DestroyDeviceObjects();

AZ_IMPL_API void Az_ImplOpenGL3_UpdateTexture(AzTextureData* tex);

#if !defined(AZ_AZPL_OPENGL_ES2) && !defined(AZ_AZPL_OPENGL_ES3)

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV))                    \
        || (defined(__ANDROID__))
#define AZ_AZPL_OPENGL_ES3
#elif defined(__EMSCRIPTEN__) || defined(__amigaos4__)
#define AZ_AZPL_OPENGL_ES2
#else

#endif

#endif

#endif
