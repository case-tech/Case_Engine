// Copyright (c) 2025 Case Technologies

#pragma once
#include <Az/Az.hpp> 
#ifndef AZ_DISABLE

#ifdef __OBJC__

@class NSEvent;
@class NSView;

AZ_AZPL_API bool     Az_AzplOSX_Init(NSView* _Nonnull view);
AZ_AZPL_API void     Az_AzplOSX_Shutdown();
AZ_AZPL_API void     Az_AzplOSX_NewFrame(NSView* _Nullable view);

#endif

#ifdef AZ_AZPL_METAL_CPP_EXTENSIONS

#ifndef __OBJC__

AZ_AZPL_API bool     Az_AzplOSX_Init(void* _Nonnull view);
AZ_AZPL_API void     Az_AzplOSX_Shutdown();
AZ_AZPL_API void     Az_AzplOSX_NewFrame(void* _Nullable view);

#endif
#endif

#endif 
