// Copyright (c) 2025 Case Technologies

#pragma once
#include <Az/Az.hpp>
#ifndef AZ_DISABLE

AZ_IMPL_API bool Az_AzplWin32_Init(void* hwnd);
AZ_IMPL_API bool Az_AzplWin32_InitForOpenGL(void* hwnd);
AZ_IMPL_API void Az_AzplWin32_Shutdown();
AZ_IMPL_API void Az_AzplWin32_NewFrame();

AZ_IMPL_API void Az_AzplWin32_EnableDpiAwareness();
AZ_IMPL_API float Az_AzplWin32_GetDpiScaleForHwnd(void* hwnd);
AZ_IMPL_API float Az_AzplWin32_GetDpiScaleForMonitor(void* monitor);

AZ_IMPL_API void Az_AzplWin32_EnableAlphaCompositing(void* hwnd);

#endif
