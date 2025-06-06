// Copyright (c) 2025 Case Technologies

#pragma once
#if defined(_MSC_VER) && defined(HTG_SHARED_LIB)
#   define HTG_EXPORT __declspec(dllexport)
#else
#   define HTG_EXPORT
#endif