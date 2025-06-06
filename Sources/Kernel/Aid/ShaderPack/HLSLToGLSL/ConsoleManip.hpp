// Copyright (c) 2025 Case Technologies

#pragma once
#include "Export.hpp"

#include <iostream>
#include <ostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            namespace ConsoleManip
            {
                struct ColorFlags
                {
                    enum
                    {
                        Red = (1 << 0),
                        Green = (1 << 1),
                        Blue = (1 << 2),

                        Intens = (1 << 3),

                        Black = 0,
                        Gray = (Red | Green | Blue),
                        White = (Gray | Intens),

                        Yellow = (Red | Green | Intens),
                        Pink = (Red | Blue | Intens),
                        Cyan = (Green | Blue | Intens),
                    };
                };

                void HTG_EXPORT Enable(bool enable_a);
                bool HTG_EXPORT IsEnabled();
                void HTG_EXPORT PushColor(long front_a,
                                          std::ostream& stream_a = std::cout);
                
                void HTG_EXPORT PushColor(long front_a,
                                          long back_a,
                                          std::ostream& stream_a = std::cout);
                
                void HTG_EXPORT PopColor(std::ostream& stream_a = std::cout);

                class ScopedColor
                {
                public:
                    inline ScopedColor(long front_a,
                                       std::ostream& stream_a = std::cout)
                        : stream_ {stream_a}
                    {
                        PushColor(front_a, stream_);
                    }

                    inline ScopedColor(long front_a,
                                       long back_a,
                                       std::ostream& stream_a = std::cout)
                        : stream_ {stream_a}
                    {
                        PushColor(front_a, back_a, stream_);
                    }

                    inline ~ScopedColor()
                    {
                        PopColor(stream_);
                    }

                private:
                    std::ostream& stream_;
                };
            } // namespace ConsoleManip
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
