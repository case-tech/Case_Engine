#include "ConsoleManip.hpp"

#include <stack>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <Windows.h>
#endif

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            namespace ConsoleManip
            {
#if defined(_WIN32) || defined(_WIN64)
                class ScreenBufferInfo
                {
                public:
                    ~ScreenBufferInfo();

                    void Push();
                    void Pop();

                private:
                    std::stack<CONSOLE_SCREEN_BUFFER_INFO> info_stack_;
                };

                thread_local static ScreenBufferInfo g_screen_buffer_info_;

                static HANDLE StdOut()
                {
                    return GetStdHandle(STD_OUTPUT_HANDLE);
                }

                ScreenBufferInfo::~ScreenBufferInfo()
                {
                    while (!info_stack_.empty())
                        Pop();
                }

                void ScreenBufferInfo::Push()
                {
                    CONSOLE_SCREEN_BUFFER_INFO buf_info_;
                    GetConsoleScreenBufferInfo(StdOut(), &buf_info_);

                    g_screen_buffer_info_.info_stack_.push(buf_info_);
                }

                void ScreenBufferInfo::Pop()
                {
                    if (!info_stack_.empty())
                    {
                        SetConsoleTextAttribute(StdOut(),
                                                info_stack_.top().wAttributes);
                        info_stack_.pop();
                    }
                }

                void PushColor(long front_a, std::ostream&)
                {
                    if (!IsEnabled())
                        return;

                    g_screen_buffer_info_.Push();

                    CONSOLE_SCREEN_BUFFER_INFO buf_info_;
                    GetConsoleScreenBufferInfo(StdOut(), &buf_info_);

                    WORD attrib_ = (buf_info_.wAttributes & 0xFFF0);

                    if ((front_a & ColorFlags::Red) != 0)
                    {
                        attrib_ |= FOREGROUND_RED;
                    }
                    if ((front_a & ColorFlags::Green) != 0)
                    {
                        attrib_ |= FOREGROUND_GREEN;
                    }
                    if ((front_a & ColorFlags::Blue) != 0)
                    {
                        attrib_ |= FOREGROUND_BLUE;
                    }
                    if ((front_a & ColorFlags::Intens) != 0)
                    {
                        attrib_ |= FOREGROUND_INTENSITY;
                    }

                    SetConsoleTextAttribute(StdOut(), attrib_);
                }

                void PushColor(long front_a, long back_a, std::ostream&)
                {
                    if (!IsEnabled())
                        return;

                    g_screen_buffer_info_.Push();

                    WORD attrib_ = 0;

                    if ((front_a & ColorFlags::Red) != 0)
                    {
                        attrib_ |= FOREGROUND_RED;
                    }
                    if ((front_a & ColorFlags::Green) != 0)
                    {
                        attrib_ |= FOREGROUND_GREEN;
                    }
                    if ((front_a & ColorFlags::Blue) != 0)
                    {
                        attrib_ |= FOREGROUND_BLUE;
                    }
                    if ((front_a & ColorFlags::Intens) != 0)
                    {
                        attrib_ |= FOREGROUND_INTENSITY;
                    }

                    if ((back_a & ColorFlags::Red) != 0)
                    {
                        attrib_ |= BACKGROUND_RED;
                    }
                    if ((back_a & ColorFlags::Green) != 0)
                    {
                        attrib_ |= BACKGROUND_GREEN;
                    }
                    if ((back_a & ColorFlags::Blue) != 0)
                    {
                        attrib_ |= BACKGROUND_BLUE;
                    }
                    if ((back_a & ColorFlags::Intens) != 0)
                    {
                        attrib_ |= BACKGROUND_INTENSITY;
                    }

                    SetConsoleTextAttribute(StdOut(), attrib_);
                }

                void PopColor(std::ostream&)
                {
                    if (IsEnabled())
                        g_screen_buffer_info_.Pop();
                }
#else
                class IOModifier
                {
                public:
                    struct Codes
                    {
                        enum
                        {
                            Red = 1,
                            Green = 2,
                            Blue = 4,

                            Foreground = 30,
                            Background = 40,
                            Bright = 60,
                        };
                    };

                    IOModifier() = default;

                    inline IOModifier(int code_a) : code_fg_ {code_a}
                    {}

                    inline IOModifier(int code_fg_a, int code_bg_a)
                        : code_fg_ {code_fg_a}, code_bg_ {code_bg_a}
                    {}

                    inline int CodeFg() const
                    {
                        return code_fg_;
                    }

                    inline int CodeBg() const
                    {
                        return code_bg_;
                    }

                private:
                    int code_fg_ = 0;
                    int code_bg_ = 0;
                };

                static std::ostream& operator<<(std::ostream& os_a,
                                                const IOModifier& mod_a)
                {
                    os_a << "\x1b[";
                    if (mod_a.CodeFg() && mod_a.CodeBg())
                        os_a << mod_a.CodeFg() << ";" << mod_a.CodeBg();
                    else if (mod_a.CodeFg())
                        os_a << mod_a.CodeFg();
                    else if (mod_a.CodeBg())
                        os_a << mod_a.CodeBg();
                    return os_a << "m";
                }

                class IOModifierState
                {
                public:
                    ~IOModifierState();

                    void Push(std::ostream& stream_a, const IOModifier& mod_a);
                    void Pop();

                private:
                    struct StackEntry
                    {
                        std::ostream& stream_;
                        IOModifier modifier_;
                    };

                    std::stack<StackEntry> modifier_stack_;
                };

                static IOModifierState g_modifier_state_;

                static int GetModCode(long color_a, bool fg_a)
                {
                    using Cd = IOModifier::Codes;

                    int code_ = 0;

                    if ((color_a & ColorFlags::Red) != 0)
                        code_ += Cd::Red;
                    if ((color_a & ColorFlags::Green) != 0)
                        code_ += Cd::Green;
                    if ((color_a & ColorFlags::Blue) != 0)
                        code_ += Cd::Blue;
                    if ((color_a & ColorFlags::Intens) != 0)
                        code_ += Cd::Bright;

                    code_ += (fg_a ? Cd::Foreground : Cd::Background);

                    return code_;
                }

                IOModifierState::~IOModifierState()
                {
                    while (!modifier_stack_.empty())
                        Pop();
                }

                void IOModifierState::Push(std::ostream& stream_a,
                                           const IOModifier& mod_a)
                {
                    modifier_stack_.push({stream_a, mod_a});
                    stream_a << mod_a;
                }

                void IOModifierState::Pop()
                {
                    if (!modifier_stack_.empty())
                    {
                        auto& stream_ = modifier_stack_.top().stream;

                        modifier_stack_.pop();

                        if (modifier_stack_.empty())
                            stream_ << IOModifier();
                        else
                            stream_ << modifier_stack_.top().modifier;
                    }
                }

                void PushColor(long front_a, std::ostream& stream_a)
                {
                    if (IsEnabled())
                        g_modifier_state_.Push(stream_a,
                                             IOModifier(
                                                     GetModCode(front_a, true)));
                }

                void PushColor(long front_a, long back_a, std::ostream& stream_a)
                {
                    if (IsEnabled())
                        g_modifier_state_.Push(stream_a,
                                             IOModifier(GetModCode(front_a, true),
                                                        GetModCode(back_a,
                                                                   false)));
                }

                void PopColor(std::ostream& stream_a)
                {
                    (void)stream_a;
                    if (IsEnabled())
                        g_modifier_state_.Pop();
                }
#endif
            } // namespace ConsoleManip
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel