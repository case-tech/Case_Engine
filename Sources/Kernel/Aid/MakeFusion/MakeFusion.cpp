#include "MakeFusion.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace MakeFusion
        {
            MakeFusion::MakeFusion()
                : compiler_("g++"), output_name_("a.out"), output_dir_("."),
                  build_type_("Release"), cpp_std_("c++17"), c_std_("c11"),
                  opt_level_("-O2"), warn_level_("-Wall"), debug_symbols_(false),
                  warnings_as_errors_(false), parallel_jobs_(1),
                  install_prefix_("/usr/local")
            {}

            void MakeFusion::AddSource(const std::string& src_a)
            {
                sources_.push_back(src_a);
            }

            void MakeFusion::AddIncludeDir(const std::string& dir_a)
            {
                includes_.push_back(dir_a);
            }
            
            void MakeFusion::AddLibrary(const std::string& lib_a)
            {
                libs_.push_back(lib_a);
            }
            
            void MakeFusion::SetOutputName(const std::string& name_a)
            {
                output_name_ = name_a;
            }

            void MakeFusion::SetCompiler(const std::string& compiler_a)
            {
                compiler_ = compiler_a;
            }

            void MakeFusion::AddCompilerFlag(const std::string& flag_a)
            {
                cxx_flags_.push_back(flag_a);
            }
            
            void MakeFusion::AddLinkerFlag(const std::string& flag_a)
            {
                ld_flags_.push_back(flag_a);
            }

            void MakeFusion::SetCppStandard(const std::string& std_a)
            {
                cpp_std_ = std_a;
            }
            
            void MakeFusion::SetCStandard(const std::string& std_a)
            {
                c_std_ = std_a;
            }
            
            void MakeFusion::SetOptAzizationLevel(const std::string& lvl_a)
            {
                opt_level_ = lvl_a;
            }
            
            void MakeFusion::EnableDebugSymbols(bool on_a)
            {
                debug_symbols_ = on_a;
            }
            
            void MakeFusion::SetBuildType(const std::string& type_a)
            {
                build_type_ = type_a;
            }

            void MakeFusion::EnableWarningsAsErrors(bool on_a)
            {
                warnings_as_errors_ = on_a;
            }
            
            void MakeFusion::SetWarningLevel(const std::string& lvl_a)
            {
                warn_level_ = lvl_a;
            }
            
            void MakeFusion::AddDefinition(const std::string& def_a)
            {
                definitions_.push_back(def_a);
            }

            void MakeFusion::SetOutputDirectory(const std::string& dir_a)
            {
                output_dir_ = dir_a;
            }
            
            void MakeFusion::AddResourceFile(const std::string& file_a)
            {
                resources_.push_back(file_a);
            }

            void MakeFusion::SetParallelJobs(int jobs_a)
            {
                parallel_jobs_ = jobs_a;
            }
            
            void MakeFusion::AddPreBuildCommand(const std::string& cmd_a)
            {
                pre_build_cmds_.push_back(cmd_a);
            }
            
            void MakeFusion::AddPostBuildCommand(const std::string& cmd_a)
            {
                post_build_cmds_.push_back(cmd_a);
            }

            void MakeFusion::AddCustomRule(const std::string& tgt_a,
                                           const std::vector<std::string>& deps_a,
                                           const std::string& cmd_a)
            {
                custom_rules_.push_back({tgt_a, deps_a, cmd_a});
            }

            void MakeFusion::SetInstallPrefix(const std::string& prefix_a)
            {
                install_prefix_ = prefix_a;
            }
            
            void MakeFusion::AddInstallTarget(const std::string& tgt_a,
                                              const std::string& dest_a)
            {
                install_rules_.push_back({tgt_a, dest_a});
            }

            void MakeFusion::AddTest(const std::string& test_name_a,
                                     const std::string& command_a)
            {
                tests_.push_back({test_name_a, command_a});
            }

            bool MakeFusion::Generate()
            {
                std::ofstream mf_("Makefile");
                if (!mf_)
                {
                    std::cerr << "Couldn't open Makefile\n";
                    return false;
                }

                mf_ << "CXX := " << compiler_ << "\n";
                mf_ << "BUILD_TYPE := " << build_type_ << "\n";
                mf_ << "CPP_STD := -std=" << cpp_std_ << "\n";
                mf_ << "OPT := " << opt_level_ << "\n";
                mf_ << "WARN := " << warn_level_
                   << (warnings_as_errors_ ? " -Werror" : "") << "\n";
                mf_ << "DBG := " << (debug_symbols_ ? "-g" : "") << "\n";

                mf_ << "CXXFLAGS := $(CPP_STD) $(OPT) $(WARN) $(DBG)";
                for (auto& f_ : cxx_flags_)
                    mf_ << " " << f_;
                mf_ << "\n";

                mf_ << "LDFLAGS :=";
                for (auto& f_ : ld_flags_)
                    mf_ << " " << f_;
                mf_ << "\n";

                mf_ << "DEFS :=";
                for (auto& d_ : definitions_)
                    mf_ << " -D" << d_;
                mf_ << "\n";

                mf_ << "INCLUDES :=";
                for (auto& d_ : includes_)
                    mf_ << " -I" << d_;
                mf_ << "\nLIBS :=";
                for (auto& l_ : libs_)
                    mf_ << " -l" << l_;
                mf_ << "\n";

                mf_ << "SRCS :=";
                for (auto& s_ : sources_)
                    mf_ << " " << s_;
                mf_ << "\nOBJS := $(SRCS:.cpp=.o)\n";

                mf_ << "TARGET := " << output_dir_ << "/" << output_name_
                   << "\n\n";

                mf_ << "all: prebuild $(TARGET) postbuild\n\n";

                mf_ << "prebuild:\n";
                for (auto& cmd_ : pre_build_cmds_)
                    mf_ << "\t" << cmd_ << "\n";
                mf_ << "\n";

                mf_ << "$(TARGET): $(OBJS)\n"
                    << "\t$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)\n\n";

                mf_ << "%.o: %.cpp\n"
                    << "\t$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDES) "
                       "$(DEFS)\n\n";

                mf_ << "postbuild:\n";
                for (auto& cmd_ : post_build_cmds_)
                    mf_ << "\t" << cmd_ << "\n";
                mf_ << "\n";
                
                mf_ << "clean:\n\trm -f $(TARGET) $(OBJS)\n\n";

                if (!resources_.empty())
                {
                    mf_ << "# Resources\n";
                    for (auto& r_ : resources_)
                        mf_ << "# resource: " << r_ << "\n";
                    mf_ << "\n";
                }

                for (auto& rule_ : custom_rules_)
                {
                    mf_ << rule_.target_ << ":";
                    for (auto& d_ : rule_.deps_)
                        mf_ << " " << d_;
                    mf_ << "\n\t" << rule_.command_ << "\n\n";
                }

                mf_ << "install:\n";
                for (auto& ir_ : install_rules_)
                {
                    mf_ << "\tcp $(TARGET) " << install_prefix_ << "/"
                       << ir_.destination_ << "\n";
                }

                mf_ << "\n";

                if (!tests_.empty())
                {
                    mf_ << "test: ";
                    for (auto& t_ : tests_)
                        mf_ << "run_" << t_.name_ << " ";
                    mf_ << "\n\n";
                    for (auto& t_ : tests_)
                    {
                        mf_ << "run_" << t_.name_ << ":\n"
                            << "\t" << t_.command_ << "\n\n";
                    }
                }

                mf_.close();
                return true;
            }

            bool MakeFusion::Build()
            {
                std::cout << "Start of assembly...\n";
                std::string cmd_ = "make -j" + std::to_string(parallel_jobs_);
                int res_ = std::system(cmd_.c_str());
                if (res_ != 0)
                {
                    std::cerr << "The build failed with an error.\n";
                    return false;
                }
                std::cout << "The build has been completed successfully.\n";
                return true;
            }
        } // namespace MakeFusion
    } // namespace Aid
} // namespace CE_Kernel