// Copyright (c) 2025 Case Technologies

#pragma once
#include <map>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace MakeFusion
        {
            struct CustomRule
            {
                std::string target_;
                std::vector<std::string> deps_;
                std::string command_;
            };

            struct InstallRule
            {
                std::string target_;
                std::string destination_;
            };

            struct TestRule
            {
                std::string name_;
                std::string command_;
            };

            class MakeFusion
            {
            public:
                MakeFusion();

                void AddSource(const std::string& src_a);
                void AddIncludeDir(const std::string& dir_a);
                void AddLibrary(const std::string& lib_a);
                void SetOutputName(const std::string& name_a);
                bool Generate();
                bool Build();

                void SetCompiler(const std::string& compiler_a);
                void AddCompilerFlag(const std::string& flag_a);
                void AddLinkerFlag(const std::string& flag_a);

                void SetCppStandard(const std::string& std_a);
                void SetCStandard(const std::string& std_a);
                void SetOptimizationLevel(const std::string& lvl_a);
                                                                 
                void EnableDebugSymbols(bool on_a);
                void SetBuildType(const std::string& type_a);

                void EnableWarningsAsErrors(bool on_a);
                void SetWarningLevel(const std::string& lvl_a);
                                                        
                void AddDefinition(const std::string& def_a);

                void SetOutputDirectory(const std::string& dir_a);
                void AddResourceFile(const std::string& file_a);

                void SetParallelJobs(int jobs_a);
                void AddPreBuildCommand(const std::string& cmd_a);
                void AddPostBuildCommand(const std::string& cmd_a);

                void AddCustomRule(const std::string& target_a,
                                   const std::vector<std::string>& deps_a,
                                   const std::string& command_a);
                
                void SetInstallPrefix(const std::string& prefix_a);
                void AddInstallTarget(const std::string& target_a,
                                      const std::string& destination_a);

                void AddTest(const std::string& test_name_a,
                             const std::string& command_a);

            private:
                std::vector<std::string> sources_, includes_, libs_;

                std::string compiler_, output_name_, output_dir_, build_type_;
                std::string cpp_std_, c_std_, opt_level_, warn_level_;
                bool debug_symbols_, warnings_as_errors_;
                int parallel_jobs_;

                std::vector<std::string> cxx_flags_, ld_flags_, definitions_;
                std::vector<std::string> pre_build_cmds_, post_build_cmds_;
                std::vector<std::string> resources_;

                std::vector<CustomRule> custom_rules_;
                std::string install_prefix_;
                std::vector<InstallRule> install_rules_;
                std::vector<TestRule> tests_;
            };
        } // namespace MakeFusion
    } // namespace Aid
} // namespace CE_Kernel
