#include "GLSLPreProcessor.hpp"

#include "../../Report/ReportIdents.hpp"
#include "GLSLExtensions.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            GLSLPreProcessor::GLSLPreProcessor(IncludeHandler& include_handler_a,
                                               Log* log_a)
                : PreProcessor {include_handler_a, log_a}
            {}

            bool GLSLPreProcessor::OnDefineMacro(const Macro& macro_a)
            {
                if (!macro_a.std_macro_)
                {
                    const auto& ident_ = macro_a.ident_tkn_->Spell();
                    if (ident_.compare(0, 3, "GL_") == 0)
                    {
                        Error(R_MacrosBeginWithGLReserved(ident_),
                              macro_a.ident_tkn_.get(),
                              false);
                        return false;
                    }

                    const auto underscore_pos_ = ident_.find("__");
                    if (underscore_pos_ != std::string::npos)
                    {
                        auto source_area_ = macro_a.ident_tkn_->Area();
                        source_area_.Offset(
                                static_cast<unsigned int>(underscore_pos_));
                        
                        Error(R_MacrosWithTwoUnderscoresReserved(ident_),
                              source_area_,
                              false);
                        
                        return false;
                    }
                }
                return PreProcessor::OnDefineMacro(macro_a);
            }

            bool GLSLPreProcessor::OnRedefineMacro(const Macro& macro_a,
                                                   const Macro& previous_macro_a)
            {
                if (previous_macro_a.std_macro_)
                {
                    Error(R_IllegalRedefOfStdMacro(
                                  previous_macro_a.ident_tkn_->Spell()),
                          macro_a.ident_tkn_.get(),
                          false);
                    return false;
                } 
                else
                    return PreProcessor::OnRedefineMacro(macro_a, previous_macro_a);
            }

            bool GLSLPreProcessor::OnUndefineMacro(const Macro& macro_a)
            {
                if (macro_a.std_macro_)
                {
                    Error(R_IllegalUndefOfStdMacro(macro_a.ident_tkn_->Spell()),
                          macro_a.ident_tkn_.get(),
                          false);
                    return false;
                } 
                else
                    return PreProcessor::OnUndefineMacro(macro_a);
            }

            bool GLSLPreProcessor::OnSubstitueStdMacro(
                    const Token& ident_tkn_a,
                    TokenPtrString& token_string_a)
            {
                if (ident_tkn_a.Spell() == "__FILE__")
                {
                    token_string_a.PushBack(Make<Token>(Tokens::IntLiteral, "1"));
                    return true;
                }
                return PreProcessor::OnSubstitueStdMacro(ident_tkn_a, token_string_a);
            }

            void GLSLPreProcessor::ParseDirective(const std::string& directive_a,
                                                  bool ignore_unknown_a)
            {
                if (directive_a == "version")
                    ParseDirectiveVersion();
                else if (directive_a == "extension")
                    ParseDirectiveExtension();
                else
                {
                    if (version_no_ == 0)
                    {
                        version_no_ = 1;
                        Error(R_VersionMustBeFirstDirective, true, false);
                    }

                    PreProcessor::ParseDirective(directive_a, ignore_unknown_a);
                }
            }

            void GLSLPreProcessor::ParseDirectiveVersion()
            {
                if (version_defined_)
                {
                    Error(R_GLSLVersionAlreadyDefined(version_no_), true, false);
                    IgnoreDirective();
                    return;
                }

                version_defined_ = true;

                IgnoreWhiteSpaces();

                auto version_tkn_ = Accept(Tokens::IntLiteral);
                version_no_ = std::stoi(version_tkn_->Spell());

                bool is_essl_ = false;
                bool is_compatibility_profile_ = false;
                std::string profile_;

                IgnoreWhiteSpaces();
                if (Is(Tokens::Ident))
                {
                    profile_ = Accept(Tokens::Ident)->Spell();

                    if (profile_ == "es")
                    {
                        is_essl_ = true;
                    } 
                    else
                    {
                        if (profile_ == "compatibility")
                            is_compatibility_profile_ = true;
                        else if (profile_ != "core")
                            Error(R_InvalidGLSLVersionProfile(profile_),
                                  true,
                                  false);
                    }
                }

                if (is_essl_)
                {
                    static const int versions_essl_[] = {100, 300, 310, 320, 0};

                    if (!VerifyVersionNo(versions_essl_))
                        Error(R_UnknownESSLVersion(version_no_),
                              version_tkn_.get(),
                              false);
                } 
                else
                {
                    static const int versions_glsl_[] = {110,
                                                       120,
                                                       130,
                                                       140,
                                                       150,
                                                       330,
                                                       400,
                                                       410,
                                                       420,
                                                       430,
                                                       440,
                                                       450,
                                                       460,
                                                       0};

                    if (!VerifyVersionNo(versions_glsl_))
                        Error(R_UnknownGLSLVersion(version_no_),
                              version_tkn_.get(),
                              false);

                    if (!profile_.empty() && version_no_ < 150)
                        Error(R_NoProfileForGLSLVersionBefore150, true, false);
                }

                Out() << "#version " << version_no_;

                if (!profile_.empty())
                    Out() << ' ' << profile_;

                DefineStandardMacro("GL_core_profile");

                if (is_essl_)
                {
                    DefineStandardMacro("GL_es_profile");
                    DefineStandardMacro("GL_ES");
                } 
                else if (is_compatibility_profile_)
                    DefineStandardMacro("GL_compatibility_profile");

                DefineStandardMacro("__VERSION__", version_no_);
            }

            void GLSLPreProcessor::ParseDirectiveExtension()
            {
                IgnoreWhiteSpaces(false, true);
                auto extension_ = Accept(Tokens::Ident)->Spell();

                if (extension_ != "all")
                {
                    const auto& ext_map_ = GetGLSLExtensionVersionMap();
                    if (ext_map_.find(extension_) == ext_map_.end())
                        Error(R_ExtensionNotSupported(extension_), true, false);
                }

                IgnoreWhiteSpaces(false, true);
                Accept(Tokens::Colon);

                IgnoreWhiteSpaces(false, true);
                auto behavior_ = Accept(Tokens::Ident)->Spell();

                if (behavior_ != "enable" && behavior_ != "require"
                    && behavior_ != "warn" && behavior_ != "disable")
                    Error(R_InvalidGLSLExtensionBehavior(behavior_),
                          true,
                          false);

                Out() << "#extension " << extension_ << " : " << behavior_;
            }

            bool GLSLPreProcessor::VerifyVersionNo(
                    const int* valid_versions_a) const
            {
                while (*valid_versions_a)
                {
                    if (*valid_versions_a == version_no_)
                        return true;
                    if (*valid_versions_a > version_no_)
                        return false;
                    ++valid_versions_a;
                }
                return false;
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
