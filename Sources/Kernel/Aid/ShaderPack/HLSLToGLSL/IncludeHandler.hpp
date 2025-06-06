// Copyright (c) 2025 Case Technologies

#pragma once
#include "Export.hpp"

#include <istream>
#include <memory>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class HTG_EXPORT IncludeHandler
            {
            public:
                IncludeHandler();
                virtual ~IncludeHandler();
                virtual std::unique_ptr<std::istream> Include(
                        const std::string& filename_a,
                        bool use_search_paths_first_a);

                std::vector<std::string>& GetSearchPaths();
                const std::vector<std::string>& GetSearchPaths() const;

            private:
                struct OpaqueData;
                OpaqueData* data_ = nullptr;
            };
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
