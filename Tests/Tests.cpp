// Copyright (c) 2025 Case Technologies

/*This is the main file for managing 
  the Unit tests of the project.*/

#include <CUtils/UnitTests.hpp>

// Tests for Aid module
#include "Aid/Algorithms.test.hpp"
#include "Aid/DateTime.test.hpp"
#include "Aid/Filesystem.test.hpp"
#include "Aid/FPLParser.test.hpp"
#include "Aid/InfoSys.test.hpp"
#include "Aid/INIParser.test.hpp"
#include "Aid/JSONParser.test.hpp"
#include "Aid/Matrix2.test.hpp"
#include "Aid/Matrix3.test.hpp"
#include "Aid/Matrix4.test.hpp"
#include "Aid/Quaternion.test.hpp"
#include "Aid/Random.test.hpp"
#include "Aid/Timer.test.hpp"
#include "Aid/Vector2.test.hpp"
#include "Aid/Vector3.test.hpp"
#include "Aid/Vector4.test.hpp"

int main()
{
    return CUtils::RUN_ALL_TESTS();
}
