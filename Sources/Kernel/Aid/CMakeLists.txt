# Copyright (c) 2025 Case Technologies
# Aid Case Engine Core Module

# ------------------------------------------------------
# -----------------------SOURCES------------------------
# ------------------------------------------------------

set(Algorithms
    Algorithms/Algorithms.cpp
    Algorithms/Algorithms.hpp)
	
set(FPLParser
    FPLParser/FPLParser.cpp
    FPLParser/FPLParser.hpp)
	
set(INIParser
    INIParser/INIParser.cpp
    INIParser/INIParser.hpp)
	
set(JSONParser
    JSONParser/JSONParser.cpp
    JSONParser/JSONParser.hpp)
	
set(Lua
    Lua/Lua.hpp
    Lua/LuaContext.cpp
    Lua/LuaContext.hpp
    Lua/LuaCpp.hpp
    Lua/LuaMetaObject.cpp
    Lua/LuaMetaObject.hpp
    Lua/LuaVersion.cpp
    Lua/LuaVersion.hpp)
	
set(Lua_Registry
    Lua/Registry/LuaCFunction.cpp
    Lua/Registry/LuaCFunction.hpp
    Lua/Registry/LuaCodeSnippet.cpp
    Lua/Registry/LuaCodeSnippet.hpp
    Lua/Registry/LuaCompiler.cpp
    Lua/Registry/LuaCompiler.hpp
    Lua/Registry/LuaLibrary.cpp
    Lua/Registry/LuaLibrary.hpp
    Lua/Registry/LuaRegistry.cpp
    Lua/Registry/LuaRegistry.hpp)
	
set(Lua_Types
    Lua/Types/LuaState.cpp
    Lua/Types/LuaState.hpp
    Lua/Types/LuaTBoolean.cpp
    Lua/Types/LuaTBoolean.hpp
    Lua/Types/LuaTNil.cpp
    Lua/Types/LuaTNil.hpp
    Lua/Types/LuaTNumber.cpp
    Lua/Types/LuaTNumber.hpp
    Lua/Types/LuaTString.cpp
    Lua/Types/LuaTString.hpp
    Lua/Types/LuaTTable.cpp
    Lua/Types/LuaTTable.hpp
    Lua/Types/LuaTUserData.cpp
    Lua/Types/LuaTUserData.hpp
    Lua/Types/LuaType.cpp
    Lua/Types/LuaType.hpp)
	
set(MakeFusion
    MakeFusion/MakeFusion.cpp
    MakeFusion/MakeFusion.hpp)
	
set(Math_Matrix
    Math/Matrix/Matrix2.cpp
    Math/Matrix/Matrix2.hpp
    Math/Matrix/Matrix3.cpp
    Math/Matrix/Matrix3.hpp
    Math/Matrix/Matrix4.cpp
    Math/Matrix/Matrix4.hpp)

set(Math_Quaternion
    Math/Quaternion/Quaternion.cpp
    Math/Quaternion/Quaternion.hpp)

set(Math_Vector
    Math/Vector/Vector2.cpp
    Math/Vector/Vector2.hpp
    Math/Vector/Vector3.cpp
    Math/Vector/Vector3.hpp
    Math/Vector/Vector4.cpp
    Math/Vector/Vector4.hpp)
	
set(Plugins
    Plugins/Plugins.cpp
    Plugins/Plugins.hpp)
	
set(PNodes  
    PNodes/PNodes.cpp
    PNodes/PNodes.hpp)
	
set(Python
    Python/PythonBuilders.cpp
    Python/PythonBuilders.hpp
    Python/PythonClass.cpp
    Python/PythonClass.hpp
    Python/PythonCpp.hpp
    Python/PythonCppCommon.hpp
    Python/PythonDefs.hpp
    Python/PythonDict.cpp
    Python/PythonDict.hpp
    Python/PythonException.cpp
    Python/PythonException.hpp
    Python/PythonFunction.cpp
    Python/PythonFunction.hpp
    Python/PythonInspect.cpp
    Python/PythonInspect.hpp
    Python/PythonInstance.cpp
    Python/PythonInstance.hpp
    Python/PythonList.cpp
    Python/PythonList.hpp
    Python/PythonMain.cpp
    Python/PythonMain.hpp
    Python/PythonModule.cpp
    Python/PythonModule.hpp
    Python/PythonObject.cpp
    Python/PythonObject.hpp
    Python/PythonTuple.cpp
    Python/PythonTuple.hpp
    Python/PythonType.cpp
    Python/PythonType.hpp)
	
set(Random
    Random/Random.cpp
    Random/Random.hpp)
	
set(Res
    Res/Res.cpp
    Res/Res.hpp)
	
set(ShaderPack
    ShaderPack/ShaderPack.cpp
    ShaderPack/ShaderPack.hpp)
	
set(ShaderPack_HLSLToGLSL
    ShaderPack/HLSLToGLSL/CiString.hpp
    ShaderPack/HLSLToGLSL/CodeWriter.cpp
    ShaderPack/HLSLToGLSL/CodeWriter.hpp
    ShaderPack/HLSLToGLSL/Compiler.cpp
    ShaderPack/HLSLToGLSL/Compiler.hpp
    ShaderPack/HLSLToGLSL/ConsoleManip.cpp
    ShaderPack/HLSLToGLSL/ConsoleManip.hpp
    ShaderPack/HLSLToGLSL/Dictionary.hpp
    ShaderPack/HLSLToGLSL/Exception.cpp
    ShaderPack/HLSLToGLSL/Exception.hpp
    ShaderPack/HLSLToGLSL/Export.hpp
    ShaderPack/HLSLToGLSL/Flags.hpp
    ShaderPack/HLSLToGLSL/Helper.hpp
    ShaderPack/HLSLToGLSL/HTG.cpp
    ShaderPack/HLSLToGLSL/HTG.hpp
    ShaderPack/HLSLToGLSL/IncludeHandler.cpp
    ShaderPack/HLSLToGLSL/IncludeHandler.hpp    
    ShaderPack/HLSLToGLSL/IndentHandler.cpp
    ShaderPack/HLSLToGLSL/IndentHandler.hpp
    ShaderPack/HLSLToGLSL/JoinString.cpp
    ShaderPack/HLSLToGLSL/JoinString.hpp
    ShaderPack/HLSLToGLSL/Log.cpp
    ShaderPack/HLSLToGLSL/Log.hpp
    ShaderPack/HLSLToGLSL/Reflection.cpp
    ShaderPack/HLSLToGLSL/Reflection.hpp
    ShaderPack/HLSLToGLSL/ReflectionPrinter.cpp
    ShaderPack/HLSLToGLSL/ReflectionPrinter.hpp
    ShaderPack/HLSLToGLSL/Report.hpp
    ShaderPack/HLSLToGLSL/ShaderVersion.cpp
    ShaderPack/HLSLToGLSL/ShaderVersion.hpp
    ShaderPack/HLSLToGLSL/SourceCode.cpp
    ShaderPack/HLSLToGLSL/SourceCode.hpp
    ShaderPack/HLSLToGLSL/SymbolTable.cpp
    ShaderPack/HLSLToGLSL/SymbolTable.hpp
    ShaderPack/HLSLToGLSL/Targets.cpp
    ShaderPack/HLSLToGLSL/Targets.hpp
    ShaderPack/HLSLToGLSL/Variant.cpp
    ShaderPack/HLSLToGLSL/Variant.hpp
    ShaderPack/HLSLToGLSL/Version.hpp
    ShaderPack/HLSLToGLSL/PlatformConsoleManip.cpp)
	
set(ShaderPack_HLSLToGLSL_AST
    ShaderPack/HLSLToGLSL/AST/AST.cpp
    ShaderPack/HLSLToGLSL/AST/AST.hpp
    ShaderPack/HLSLToGLSL/AST/ASTEnums.cpp
    ShaderPack/HLSLToGLSL/AST/ASTEnums.hpp
    ShaderPack/HLSLToGLSL/AST/ASTFactory.cpp
    ShaderPack/HLSLToGLSL/AST/ASTFactory.hpp
    ShaderPack/HLSLToGLSL/AST/Identifier.cpp
    ShaderPack/HLSLToGLSL/AST/Identifier.hpp
    ShaderPack/HLSLToGLSL/AST/SourceArea.cpp
    ShaderPack/HLSLToGLSL/AST/SourceArea.hpp
    ShaderPack/HLSLToGLSL/AST/SourcePosition.cpp
    ShaderPack/HLSLToGLSL/AST/SourcePosition.hpp
    ShaderPack/HLSLToGLSL/AST/Token.cpp
    ShaderPack/HLSLToGLSL/AST/Token.hpp
    ShaderPack/HLSLToGLSL/AST/TokenString.cpp
    ShaderPack/HLSLToGLSL/AST/TokenString.hpp
    ShaderPack/HLSLToGLSL/AST/TypeDenoter.cpp
    ShaderPack/HLSLToGLSL/AST/TypeDenoter.hpp)
	
set(ShaderPack_HLSLToGLSL_AST_Visitor
    ShaderPack/HLSLToGLSL/AST/Visitor/ASTPrinter.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ASTPrinter.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ControlPathAnalyzer.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ControlPathAnalyzer.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/EndOfScopeAnalyzer.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/EndOfScopeAnalyzer.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ExprConverter.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ExprConverter.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ExprEvaluator.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ExprEvaluator.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/FuncNameConverter.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/FuncNameConverter.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/Optimizer.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/Optimizer.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ReferenceAnalyzer.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ReferenceAnalyzer.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ReflectionAnalyzer.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/ReflectionAnalyzer.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/StructParameterAnalyzer.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/StructParameterAnalyzer.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/TypeConverter.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/TypeConverter.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/UniformPacker.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/UniformPacker.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/Visitor.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/Visitor.hpp
    ShaderPack/HLSLToGLSL/AST/Visitor/VisitorTracker.cpp
    ShaderPack/HLSLToGLSL/AST/Visitor/VisitorTracker.hpp)
	
set(ShaderPack_HLSLToGLSL_Backend	
    ShaderPack/HLSLToGLSL/Backend/Converter.cpp
    ShaderPack/HLSLToGLSL/Backend/Converter.hpp
    ShaderPack/HLSLToGLSL/Backend/Generator.cpp
    ShaderPack/HLSLToGLSL/Backend/Generator.hpp)
	
set(ShaderPack_HLSLToGLSL_Backend_GLSL
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLConverter.cpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLConverter.hpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLExtensionAgent.cpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLExtensionAgent.hpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLGenerator.cpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLGenerator.hpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLIntrinsics.cpp
    ShaderPack/HLSLToGLSL/Backend/GLSL/GLSLIntrinsics.hpp)	
	
set(ShaderPack_HLSLToGLSL_Frontend
    ShaderPack/HLSLToGLSL/Frontend/Analyzer.cpp
    ShaderPack/HLSLToGLSL/Frontend/Analyzer.hpp
    ShaderPack/HLSLToGLSL/Frontend/IntrinsicAdept.cpp
    ShaderPack/HLSLToGLSL/Frontend/IntrinsicAdept.hpp
    ShaderPack/HLSLToGLSL/Frontend/Parser.cpp
    ShaderPack/HLSLToGLSL/Frontend/Parser.hpp
    ShaderPack/HLSLToGLSL/Frontend/PreProcessor.cpp
    ShaderPack/HLSLToGLSL/Frontend/PreProcessor.hpp
    ShaderPack/HLSLToGLSL/Frontend/PreProcessorScanner.cpp
    ShaderPack/HLSLToGLSL/Frontend/PreProcessorScanner.hpp
    ShaderPack/HLSLToGLSL/Frontend/Scanner.cpp
    ShaderPack/HLSLToGLSL/Frontend/Scanner.hpp
    ShaderPack/HLSLToGLSL/Frontend/SLParser.cpp
    ShaderPack/HLSLToGLSL/Frontend/SLParser.hpp
    ShaderPack/HLSLToGLSL/Frontend/SLScanner.cpp
    ShaderPack/HLSLToGLSL/Frontend/SLScanner.hpp)
	
set(ShaderPack_HLSLToGLSL_Frontend_GLSL
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLExtensions.cpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLExtensions.hpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLKeywords.cpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLKeywords.hpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLParser.cpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLParser.hpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLPreProcessor.cpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLPreProcessor.hpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLScanner.cpp
    ShaderPack/HLSLToGLSL/Frontend/GLSL/GLSLScanner.hpp)

set(ShaderPack_HLSLToGLSL_Frontend_HLSL
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLAnalyzer.cpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLAnalyzer.hpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLIntrinsics.cpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLIntrinsics.hpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLKeywords.cpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLKeywords.hpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLParser.cpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLParser.hpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLScanner.cpp
    ShaderPack/HLSLToGLSL/Frontend/HLSL/HLSLScanner.hpp)

set(ShaderPack_HLSLToGLSL_Report
    ShaderPack/HLSLToGLSL/Report/ReportHandler.cpp
    ShaderPack/HLSLToGLSL/Report/ReportHandler.hpp
    ShaderPack/HLSLToGLSL/Report/ReportIdents.hpp
    ShaderPack/HLSLToGLSL/Report/ReportIdentsEN.hpp)

set(System_DateTime
    System/DateTime/DateTime.cpp
    System/DateTime/DateTime.hpp)

set(System_Filesystem
    System/Filesystem/Filesystem.cpp
    System/Filesystem/Filesystem.hpp)

set(System_InfoSys
    System/InfoSys/InfoSys.cpp
    System/InfoSys/InfoSys.hpp)
	
set(Timer
    Timer/Timer.cpp
    Timer/Timer.hpp)
	
set(TypeToType
    TypeToType/TypeToType.cpp
    TypeToType/TypeToType.hpp)

# ------------------------------------------------------
# -----------------------GROUPS-------------------------
# ------------------------------------------------------

source_group("Algorithms"    						  FILES ${Algorithms}						  )
source_group("FPLParser"     						  FILES ${FPLParser} 						  )
source_group("INIParser"     						  FILES ${INIParser} 						  )
source_group("JSONParser"    						  FILES ${JSONParser}						  )
source_group("Lua" 		     						  FILES ${Lua}       						  )
source_group("Lua\\Registry" 						  FILES ${Lua_Registry}       				  )
source_group("Lua\\Types" 		     				  FILES ${Lua_Types}       					  )
source_group("MakeFusion"    						  FILES ${MakeFusion}						  )
source_group("Math\\Matrix" 		  			      FILES ${Math_Matrix}       				  )
source_group("Math\\Quaternion" 		  			  FILES ${Math_Quaternion}       			  )
source_group("Math\\Vector" 		  				  FILES ${Math_Vector}       				  )
source_group("Plugins"       						  FILES ${Plugins}   						  )
source_group("PNodes"        						  FILES ${PNodes}    						  )
source_group("Python"        					      FILES ${Python}    						  )
source_group("Random"        						  FILES ${Random}    						  )
source_group("Res"           					      FILES ${Res}       						  )
source_group("ShaderPack"    						  FILES ${ShaderPack}						  )
source_group("ShaderPack\\HLSLToGLSL"    		      FILES ${ShaderPack_HLSLToGLSL}			  )
source_group("ShaderPack\\HLSLToGLSL\\AST"    	      FILES ${ShaderPack_HLSLToGLSL_AST} 		  )
source_group("ShaderPack\\HLSLToGLSL\\AST\\Visitor"   FILES ${ShaderPack_HLSLToGLSL_AST_Visitor}  )
source_group("ShaderPack\\HLSLToGLSL\\Backend"    	  FILES ${ShaderPack_HLSLToGLSL_Backend}      )
source_group("ShaderPack\\HLSLToGLSL\\Backend\\GLSL"  FILES ${ShaderPack_HLSLToGLSL_Backend_GLSL} )
source_group("ShaderPack\\HLSLToGLSL\\Frontend"       FILES ${ShaderPack_HLSLToGLSL_Frontend}	  )
source_group("ShaderPack\\HLSLToGLSL\\Frontend\\GLSL" FILES ${ShaderPack_HLSLToGLSL_Frontend_GLSL})
source_group("ShaderPack\\HLSLToGLSL\\Frontend\\HLSL" FILES ${ShaderPack_HLSLToGLSL_Frontend_HLSL})
source_group("ShaderPack\\HLSLToGLSL\\Report"    	  FILES ${ShaderPack_HLSLToGLSL_Report}       )
source_group("System\\DateTime"        				  FILES ${System_DateTime}    				  )
source_group("System\\Filesystem"        			  FILES ${System_Filesystem}    			  )
source_group("System\\InfoSys"       				  FILES ${System_InfoSys}    				  )
source_group("Timer"         						  FILES ${Timer}       						  )
source_group("TypeToType"    						  FILES ${TypeToType}						  )  

# ------------------------------------------------------
# -------------------CREATE-MODULE----------------------
# ------------------------------------------------------

add_library(
    Aid
    STATIC
    ${Algorithms}
    ${FPLParser}
    ${INIParser}
    ${JSONParser}
    ${Lua}
	${Lua_Registry}
	${Lua_Types}
    ${MakeFusion}
    ${Math_Matrix}
	${Math_Quaternion}
	${Math_Vector}
    ${Plugins}
    ${PNodes}
    ${Python}
    ${Random}
    ${Res}
    ${ShaderPack}
	${ShaderPack_HLSLToGLSL}
	${ShaderPack_HLSLToGLSL_AST}
	${ShaderPack_HLSLToGLSL_AST_Visitor}
	${ShaderPack_HLSLToGLSL_Backend}
	${ShaderPack_HLSLToGLSL_Backend_GLSL}
	${ShaderPack_HLSLToGLSL_Frontend}
	${ShaderPack_HLSLToGLSL_Frontend_GLSL}
	${ShaderPack_HLSLToGLSL_Frontend_HLSL}
	${ShaderPack_HLSLToGLSL_Report}
    ${System_DateTime}
	${System_Filesystem}
	${System_InfoSys}
    ${Timer}
    ${TypeToType})

# ------------------------------------------------------
# ------------------COMPILE-OPTIONS---------------------
# ------------------------------------------------------

target_compile_options(Aid PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4309>
    $<$<CXX_COMPILER_ID:MSVC>:/wd4100>
    $<$<CXX_COMPILER_ID:MSVC>:/wd4244>
    $<$<CXX_COMPILER_ID:MSVC>:/wd4996>
    $<$<CXX_COMPILER_ID:MSVC>:/wd4702>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wno-unused-variable>)

set_target_properties(Aid PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

if(MSVC)
  # Настройка рантайм-библиотеки
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

  # Общие флаги для всех конфигураций (если нужно)
  target_compile_options(Aid PRIVATE 
    /Oy-  # Этот флаг может быть общим
  )

  # Флаги только для RELEASE
  target_compile_options(Aid PRIVATE 
    "$<$<CONFIG:Release>:/O1>"
    "$<$<CONFIG:Release>:/Ob1>"
    "$<$<CONFIG:Release>:/Os>"
    "$<$<CONFIG:Release>:/Gw>"
    "$<$<CONFIG:Release>:/GF>"
    "$<$<CONFIG:Release>:/Gy>"
  )

  # Флаги только для DEBUG
  target_compile_options(Aid PRIVATE 
    "$<$<CONFIG:Debug>:/Od>"   # Отключение оптимизации
    "$<$<CONFIG:Debug>:/RTC1>" # Проверки времени выполнения
  )

  # Линкер-флаги только для RELEASE
  target_link_options(Aid PRIVATE 
    "$<$<CONFIG:Release>:/OPT:REF>"
    "$<$<CONFIG:Release>:/OPT:ICF>"
    "$<$<CONFIG:Release>:/MERGE:.rdata=.text>"
  )
endif()

# ------------------------------------------------------
# ---------------- CONNECTING LIBRARIES -----------------
# ------------------------------------------------------

target_include_directories(Aid PRIVATE
    "${CMAKE_SOURCE_DIR}/External/CUtils/Include"
    "${CMAKE_SOURCE_DIR}/External/Lua/Include"
    "${CMAKE_SOURCE_DIR}/External/Python/Include"
    "${CMAKE_SOURCE_DIR}/External/GLEW/Include"
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
else()
    set(ARCH_DIR "x86")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_DIR)
if(NOT BUILD_TYPE_DIR MATCHES "^(debug|release)$")
    set(BUILD_TYPE_DIR "Release")
endif()

if(WIN32)
    set(OS_DIR "Win")
    set(LUA_LIB "lua54.lib")
    set(CUTILS_LIB "CUtils.lib")
    set(PYTHON_LIB1 "python3.lib")
    set(PYTHON_LIB2 "python313.lib")
    set(GLEW_LIB1 "glew32.lib")
    set(GLEW_LIB2 "glew32s.lib")
    set(OPENGL_LIB "opengl32.lib")
    
    set(LUA_PATH "${CMAKE_SOURCE_DIR}/External/Lua/Lib/${OS_DIR}/${LUA_LIB}")
    set(CUTILS_PATH "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/${OS_DIR}/${ARCH_DIR}/${BUILD_TYPE_DIR}/${CUTILS_LIB}")
    set(GLEW_PATH1 "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/${OS_DIR}/Release/${ARCH_DIR}/${GLEW_LIB1}")
    set(GLEW_PATH2 "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/${OS_DIR}/Release/${ARCH_DIR}/${GLEW_LIB2}")

elseif(LINUX)
    set(OS_DIR "Linux")
    set(LUA_LIB "liblua.a")
    set(CUTILS_LIB "libCUtils.a")
    set(PYTHON_LIB1 "libpython3.a")
    set(PYTHON_LIB2 "libpython3.11.a")
    set(OPENGL_LIB "GL")
    
    set(LUA_PATH "${CMAKE_SOURCE_DIR}/External/Lua/Lib/${OS_DIR}/${ARCH_DIR}/${LUA_LIB}")
    set(CUTILS_PATH "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/${OS_DIR}/${ARCH_DIR}/${BUILD_TYPE_DIR}/${CUTILS_LIB}")
    set(GLEW_PATH1 "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/${OS_DIR}/${ARCH_DIR}/libGLEW.a")
    set(GLEW_PATH2 "")

elseif(APPLE)
    set(OS_DIR "Mac")
    set(LUA_LIB "liblua.a")
    set(CUTILS_LIB "libCUtils.a")
    set(PYTHON_LIB1 "libpython3.a")
    set(PYTHON_LIB2 "libpython3.11.a")
    set(OPENGL_LIB "-framework OpenGL")
    
    set(LUA_PATH "${CMAKE_SOURCE_DIR}/External/Lua/Lib/${OS_DIR}/${ARCH_DIR}/${LUA_LIB}")
    set(CUTILS_PATH "${CMAKE_SOURCE_DIR}/External/CUtils/Lib/${OS_DIR}/${ARCH_DIR}/${BUILD_TYPE_DIR}/${CUTILS_LIB}")
    set(GLEW_PATH1 "${CMAKE_SOURCE_DIR}/External/GLEW/Lib/${OS_DIR}/${ARCH_DIR}/libGLEW.a")
    set(GLEW_PATH2 "")
endif()

set(PYTHON_PATH1 "${CMAKE_SOURCE_DIR}/External/Python/Lib/${PYTHON_LIB1}")
set(PYTHON_PATH2 "${CMAKE_SOURCE_DIR}/External/Python/Lib/${PYTHON_LIB2}")

target_link_libraries(Aid PRIVATE
    ${LUA_PATH}
    ${CUTILS_PATH}
    ${PYTHON_PATH1}
    ${PYTHON_PATH2}
    ${GLEW_PATH1}
    ${GLEW_PATH2}
    ${OPENGL_LIB}
)

if(LINUX)
    target_link_libraries(Aid PRIVATE
        dl      
        pthread
    )
elseif(APPLE)
    find_library(COCOA_LIB Cocoa)
    target_link_libraries(Aid PRIVATE ${COCOA_LIB})
endif()