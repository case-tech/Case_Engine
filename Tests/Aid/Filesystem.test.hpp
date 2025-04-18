#pragma once
#include "Aid/System/Filesystem/Filesystem.hpp"
#include <CUtils/UnitTests.hpp>
#include <fstream>
#include <filesystem>

using namespace CUtils;

UTEST(FilesystemTest, CreateAndRemoveDirectory)
{
    CE_Kernel::Aid::System::Filesystem fs_;
    std::string dir_ = "test_dir";
    
    ASSERT_TRUE(fs_.CreateDirectory(dir_));
    ASSERT_TRUE(fs_.Exists(dir_));
    ASSERT_TRUE(fs_.IsDirectory(dir_));
    
    ASSERT_TRUE(fs_.Remove(dir_));
    ASSERT_FALSE(fs_.Exists(dir_));
}

UTEST(FilesystemTest, FileOperations)
{
    CE_Kernel::Aid::System::Filesystem fs_;
    std::string file_ = "test_file.txt";
  
    std::ofstream(file_) << "test";
    ASSERT_TRUE(fs_.Exists(file_));
    ASSERT_TRUE(fs_.IsFile(file_));
    
    std::string copy_ = "test_copy.txt";
    ASSERT_TRUE(fs_.Copy(file_, copy_));
    ASSERT_TRUE(fs_.Exists(copy_));
    
    ASSERT_TRUE(fs_.Remove(file_));
    ASSERT_TRUE(fs_.Remove(copy_));
}
