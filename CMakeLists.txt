cmake_minimum_required (VERSION 3.6)
project (ImageEffects)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "Configuration types" FORCE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/Source/*.h
    ${CMAKE_SOURCE_DIR}/Source/*.cpp
)

file(GLOB EXT_FILES
    ${CMAKE_SOURCE_DIR}/thirdparty/*.h
	${CMAKE_SOURCE_DIR}/thirdparty/imgui/*.h
	${CMAKE_SOURCE_DIR}/thirdparty/imgui/*.cpp
)

file(GLOB FRAMEWORK_FILES
    ${CMAKE_SOURCE_DIR}/framework/*.h
	${CMAKE_SOURCE_DIR}/framework/*.cpp
	${CMAKE_SOURCE_DIR}/framework/dx11/*.h
	${CMAKE_SOURCE_DIR}/framework/dx11/*.cpp
)

include_directories(
  ${CMAKE_SOURCE_DIR}/Source/
  ${CMAKE_SOURCE_DIR}/thirdparty/
  ${CMAKE_SOURCE_DIR}/thirdparty/imgui/
  ${CMAKE_SOURCE_DIR}/framework/
  ${CMAKE_SOURCE_DIR}/framework/dx11
)

## Output include directory for debug
#######################################################################################
get_property(includes DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${includes})
    message(STATUS "include dir='${dir}'")
endforeach()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
find_package(DirectX)

#######################################################################################

#set (USE_MSVC_RUNTIME_LIBRARY_DLL true)
#IF (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
#    #We statically link to reduce dependancies
#    FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RETAIL)
#        IF(${flag_var} MATCHES "/MD")
#            STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#        ENDIF(${flag_var} MATCHES "/MD")
#        IF(${flag_var} MATCHES "/MDd")
#            STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
#        ENDIF(${flag_var} MATCHES "/MDd")
#    ENDFOREACH(flag_var)
#ENDIF (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)

#######################################################################################

foreach(f ${SRC_FILES})
    # Get the path of the file relative to ${DIRECTORY},
    # then alter it (not compulsory)
    file(RELATIVE_PATH SRCGR ${CMAKE_SOURCE_DIR} ${f})
    set(SRCGR "Sources/${SRCGR}")

    # Extract the folder, ie remove the filename part
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

    # Source_group expects \\ (double antislash), not / (slash)
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

foreach(f ${EXT_FILES})
    # Get the path of the file relative to ${DIRECTORY},
    # then alter it (not compulsory)
    file(RELATIVE_PATH SRCGR ${CMAKE_SOURCE_DIR} ${f})
    set(SRCGR "External/${SRCGR}")

    # Extract the folder, ie remove the filename part
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

    # Source_group expects \\ (double antislash), not / (slash)
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

foreach(f ${FRAMEWORK_FILES})
    # Get the path of the file relative to ${DIRECTORY},
    # then alter it (not compulsory)
    file(RELATIVE_PATH SRCGR ${CMAKE_SOURCE_DIR} ${f})
    set(SRCGR "Framework/${SRCGR}")

    # Extract the folder, ie remove the filename part
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

    # Source_group expects \\ (double antislash), not / (slash)
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

#--------------------------------------------------------------------
# preproc
#--------------------------------------------------------------------
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-DBX_CONFIG_ENABLE_MSVC_LEVEL4_WARNINGS=1)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
add_definitions(-DUSE_DL_PREFIX)

if(MSVC)
add_definitions(-DWIN32)
add_definitions(-D_WIN32)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
#add_definitions(-DNOMINMAX)
endif()

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_17_flag_supported)
    if (_cpp_17_flag_supported)
		message(STATUS "C++ 17 is suppoted in VS")
        add_compile_options("/std:c++17")
    endif()
endif()

if(WINDOWS)
  set(GUI_TYPE WIN32)
elseif(MACOSX)
  set(GUI_TYPE MACOSX_BUNDLE)
endif()

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

SET(LINK_OPTIONS " ")

SET(EXE_NAME "ImageEffects")

ADD_EXECUTABLE(${EXE_NAME} ${SRC_FILES} ${FRAMEWORK_FILES} ${EXT_FILES})
TARGET_LINK_LIBRARIES(${EXE_NAME} ${DirectX_D3D11_LIBRARY} ${DirectX_D3D11_COMPILER})


#--------------------------------------------------------------------
# output dirs
#--------------------------------------------------------------------
# https://cmake.org/cmake/help/v3.0/manual/cmake-properties.7.html 
#--------------------------------------------------------------------
set_target_properties(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(${EXE_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${EXE_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX "RelWithDebInfo")
set_target_properties(${EXE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")


#--------------------------------------------------------------------
# Hide the console window in visual studio projects
#--------------------------------------------------------------------
set(ENABLE_HIDECONSOLE_BUILD TRUE CACHE BOOL "TRUE to hide console for Windows sys")
if(ENABLE_HIDECONSOLE_BUILD)
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()
endif()

if(WINDOWS)
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif()

if(ENABLE_HIDECONSOLE_BUILD)
MESSAGE(STATUS "Console is hidden")
else()
MESSAGE(STATUS "Console is visible")
endif()

