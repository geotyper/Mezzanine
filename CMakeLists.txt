# © Copyright 2010 - 2016 BlackTopp Studios Inc.
# This file is part of The Mezzanine Engine.
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

###############################################################################
# This is the root cmake file for the Mezzanine engine. It includes the example files
# from the engine demo, Catch! and a series of UnitTests. These may be freely copied
# and used to build your projects as long as you release any changes you make to the
# build scripts back to the public in a usable form.
#
# This file primarily gathers information from the developer about the build environment
# and desired game engine options. It makes this information available to the rest of
# the include cmake files to minimize duplicated effort.
#
# Here is a listing of some of the values that this files sets up for consumption by other
# build scripts, that can be set as cmake options:
#
# That affect/add/remove behavior of the Mezzanine engine
#   Mezz_BuildTypeDebug     - False - The Engine will be compiled with debug symbols by setting CMAKE_BUILD_TYPE. MEZZDEBUG will be passed as a preprocessor macro causing more logging messages to be included. All libraries will also include debug symbols.
#   Mezz_BuildTypeRelease   - False - The Engine will be compiled with optimizations by setting CMAKE_BUILD_TYPE. MEZZRELEASE will be passed into the source via a preprocessor macro.
#   Mezz_BuildTypeSmall     - False - The Engine will be optimizing for minimum size by setting CMAKE_BUILD_TYPE, MEZZSMALL is passed into the source when compiling as a preprocessor macro.
#   Mezz_Lua51              - True if Lua is to be included and link against. False if Lua was not selected to be compiled in.
#   Mezz_Ruby               - True if Ruby is to be included and link against. False if Ruby was not selected to be compiled in.
#   Mezz_SWIG               - Rebuild Scripting interfaces with SWIG each build
#   Mezz_Network            - Enable or disable networking support
#   Mezz_AllWarnings        - Enable all compiler warnings
#
# Build Extra Features alongside the Mezzanine
#        Mezz_Doc                           - Check for Doxygen on the system and force a doxygen build as part of a full build when it is present.
#
# Advanced Mezzanine build options
#        Mezz_Profile                       - If Set to True, then a number of engine level and compiler tools will be enabled that track performance in the engine. This should be disabled when not debugging and benchmarking. Not supported in visual studio currently
#        Mezz_ThreadLog                     - Enables threadlogging, If disabled macros will insert no code at all.
#        Mezz_ThrowOnSingletonFetch         - When retrieving an invalid Singleton, should an exception be thrown, defaults to false.
#        Mezz_DecacheWorkUnits              - Different CPUs have different Cache sizes and different ratios of actual work to IO bandwith. This tries to increase the amount of work done at the expense of forcing work units to be reloaded into CPU from RAM each frame. Use this with your favorite benchmark to maximize performance.
#        Mezz_FramesToTrack                 - How many frame should the runtime performance metrics be tracked for
#        Mezz_Lua51IncludeDirectory         - Location of headers for lua 5.1
#
# Deprecated options
#        Mezz_XML                           - Previously disabled/enabled XML options, now XML is used everywhere. Thsi option has been removed.
#
# That affect only build time values:
#        OgreLinkName                       - This is the name of the ogre library that the compiler expects. Examples: OgreMain, OgreMain_d
#        OgresD                             - This can be used to help pick/select ogre libraries that may or may not have an "_d" depending on how they are compiled. If debug libraries are in use this is set to "_d" otherwise "".
#        OgreLibFileName                    - The filename of the actual Ogre library. On some platforms this file may just be a symlink. Examples: OgreMain.dll, libOgreMain.so
#        OgreLinuxSoName                    - Linux's versioned library system expects specific version of so files. This is the more specific file, that must copied along with the regular so. Examples libOgreMain.so.1.7.1, libOgreMain.so.1.9.3
#        OgrePluginCGPM                     - The File name of the Ogre CG manager plugin
#        OgrePluginRenderGL                 - The File name of the Ogre OpenGL rendersystem plugin
#        OgrePluginParticle                 - The File name of the Ogre ParticleFX plugin
#        OgreConfigHeaderIncludeDir         - The Folder where Ogre puts its configured Headers. This is only needed by subprojects if they link directly against Ogre.
#        OgreHeaderIncludeDir               - The Folder where all the non-configured headers go. This is only needed by subprojects if they link directly against Ogre.
#
# Here are some more values that can be used when creating cmake scripts that are indirectly
# set and should not be directly passed as cmake values
#        BuildPlatform                      - A string that Contains either "windows", "linux", or "macosx" at the time of this writing. This directly corresponds to the folders names in the Mezzanine engine repository.
#        CommonBuildFlags                   - A listing of all the currently set build flags to pass to the compiler at compile time. This is the mechanism that enforces Mezz_BuildTypeDebug's and Mezz_Profile's settings.
#        DynamicLibPrefix                   - This is a string that is required by a given platform to begin the filename of shared libraries. One windows it is "" on Linux/Unix it is "lib"
#        DynamicLibSuffix                   - This is a string that is required by a given platform to end the filename of shared libraries. One windows it is ".dll" on Linux/Unix it is ".so"
#        LuaLinkName                        - This is the name that of the actual compiled lua library, it will be blank if lua is not to be linked
#        LuaProject                         - The Name of the lua cmake project
#        MezzanineLinkName                  - What should be entered when somethings needs to link against Mezzanine usually "Mezzanine" sometime it specifies and import library, like
#        MezzanineLibFileName               - The actual name of the file the Mezzanine is called.

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in source tree build. Please create a build directory outside of the Mezzanine source code and have cmake build from there.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

###############################################################################
# The name of our project is "MezzanineAndCatch".
cmake_minimum_required (VERSION 2.8)
project (MezzanineAndCatch)
message ("${PROJECT_NAME} - Starting Configuration.")

#######################################
# Mezzanine helper variables
set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR}/)
Message(STATUS "Using Source From: ${ProjectRootDir}")

set(ProjectBinaryDir ${${PROJECT_NAME}_BINARY_DIR}/)
Message(STATUS "Putting all major output in: ${ProjectBinaryDir}")

#######################################
# PostFix configuration
SET(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Adds a postfix for debug-built libraries." FORCE)
SET(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "Adds a postfix for MinsizeRelease-built libraries." FORCE)
SET(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "Adds a postfix for ReleaseWithDebug-built libraries." FORCE)
mark_as_advanced(CMAKE_DEBUG_POSTFIX CMAKE_MINSIZEREL_POSTFIX CMAKE_RELWITHDEBINFO_POSTFIX)

###############################################################################
# Set Platform Specific Variables
set(DynamicLibPrefix "")
set(DynamicLibSuffix "")
set(StaticLibPrefix "")
set(StaticLibSuffix ".a")
set(LibPostfix "")

if(${WIN32})
    set(BuildPlatform windows)
    set(DynamicLibSuffix .dll)
    add_definitions( -DMEZZ_WINDOWS )
    if(MSVC)
        add_definitions( -DMEZZ_WIN_MSVC )
    endif(MSVC)
    if(MINGW)
        add_definitions( -DMEZZ_WIN_MINGW )
    endif(MINGW)
endif(${WIN32})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(BuildPlatform linux)
    set(DynamicLibPrefix lib)
    set(DynamicLibSuffix .so)
    add_definitions( -DMEZZ_LINUX )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(BuildPlatform macosx)
    set(DynamicLibPrefix lib)
    set(DynamicLibSuffix .dylib)
    add_definitions( -DMEZZ_MACOSX )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${WIN32})
    set(FPIC "")
else(${WIN32})
    set(FPIC "-fPIC")
endif(${WIN32})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FPIC}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FPIC}")

if(MSVC)
    set(D "/D")
else(MSVC)
    set(D "-D")
endif(MSVC)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=athlon64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=athlon64")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=i686")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i686")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

###############################################################################
# Check For the Mezzanine Build Type options and set values accordingly
option (Mezz_BuildTypeDebug "Compile with debug symbols, Cannot be used with Mezz_BuildTypeSmall" OFF)
option (Mezz_BuildTypeRelease "Compile Optimized for PC release, Cannot be used with Mezz_BuildTypeSmall" OFF)
option (Mezz_BuildTypeSmall "Compile Optimized for Minimal size, Cannot be used with Mezz_BuildTypeDebug or Mezz_BuildTypeRelease" OFF)

if(Mezz_BuildTypeDebug)
    if(Mezz_BuildTypeSmall)
        message(FATAL_ERROR "Mezz_BuildTypeDebug and Mezz_BuildTypeSmall cannot be used together")
    endif(Mezz_BuildTypeSmall)
    if(Mezz_BuildTypeRelease)
        message( STATUS "${PROJECT_NAME} - Enabling Mezz_BuildTypeDebug and Mezz_BuildTypeRelease; Disabling Mezz_BuildTypeSmall.")
        set(Mezz_BuildTypeDebug_Arguments "${D}MEZZDEBUG")
        set(Mezz_BuildTypeRelease_Arguments "${D}MEZZRELEASE")
        set(Mezz_BuildTypeSmall_Arguments "${D}MEZZSMALL_undefined")
        set(Mezz_BuildTypeDebug_Defined "MEZZDEBUG = 1")
        set(Mezz_BuildTypeRelease_Defined "MEZZRELEASE = 1")
        set(Mezz_BuildTypeSmall_Defined "MEZZSMALL_undefined")
        set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Set by Mezz_BuildTypeDebug and Mezz_BuildTypeRelease" FORCE)
    else(Mezz_BuildTypeRelease)
        message( STATUS "${PROJECT_NAME} - Enabling Mezz_BuildTypeDebug; Disabling Mezz_BuildTypeRelease and Mezz_BuildTypeSmall")
        set(Mezz_BuildTypeDebug_Arguments "${D}MEZZDEBUG")
        set(Mezz_BuildTypeRelease_Arguments "${D}MEZZRELEASE_undefined")
        set(Mezz_BuildTypeSmall_Arguments "${D}MEZZSMALL_undefined")
        set(Mezz_BuildTypeDebug_Defined "MEZZDEBUG = 1")
        set(Mezz_BuildTypeRelease_Defined "MEZZRELEASE_undefined")
        set(Mezz_BuildTypeSmall_Defined "MEZZSMALL_undefined")
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Set by Mezz_BuildTypeDebug" FORCE)
    endif(Mezz_BuildTypeRelease)
else(Mezz_BuildTypeDebug)
    if(Mezz_BuildTypeRelease)
        if(Mezz_BuildTypeSmall)
            message(FATAL_ERROR "Mezz_BuildTypeRelease and Mezz_BuildTypeSmall cannot be used together")
        endif(Mezz_BuildTypeSmall)
        message( STATUS "${PROJECT_NAME} - Enabling Mezz_BuildTypeRelease; Disabling Mezz_BuildTypeDebug and Mezz_BuildTypeSmall.")
        set(Mezz_BuildTypeDebug_Arguments "${D}MEZZDEBUG_undefined")
        set(Mezz_BuildTypeRelease_Arguments "${D}MEZZRELEASE")
        set(Mezz_BuildTypeSmall_Arguments "${D}MEZZSMALL_undefined")
        set(Mezz_BuildTypeDebug_Defined "MEZZDEBUG_undefined")
        set(Mezz_BuildTypeRelease_Defined "MEZZRELEASE = 1")
        set(Mezz_BuildTypeSmall_Defined "MEZZSMALL_undefined")
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Set by Mezz_BuildTypeRelease" FORCE)
    else(Mezz_BuildTypeRelease)
        if(Mezz_BuildTypeSmall)
            message( STATUS "${PROJECT_NAME} - Enabling Mezz_BuildTypeSmall; Disabling Mezz_BuildTypeDebug and Mezz_BuildTypeRelease.")
            set(Mezz_BuildTypeDebug_Arguments "${D}MEZZDEBUG_undefined")
            set(Mezz_BuildTypeRelease_Arguments "${D}MEZZRELEASE_undefined")
            set(Mezz_BuildTypeSmall_Arguments "${D}MEZZSMALL")
            set(Mezz_BuildTypeDebug_Defined "MEZZDEBUG_undefined")
            set(Mezz_BuildTypeRelease_Defined "MEZZRELEASE_undefined")
            set(Mezz_BuildTypeSmall_Defined "MEZZSMALL = 1")
            set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Set by Mezz_BuildTypeSmall" FORCE)
        else(Mezz_BuildTypeSmall)
            message( STATUS "${PROJECT_NAME} - No Build type selected Disabling Mezz_BuildTypeDebug, Mezz_BuildTypeRelease and Mezz_BuildTypeSmall.")
            set(Mezz_BuildTypeDebug_Arguments "${D}MEZZDEBUG_undefined")
            set(Mezz_BuildTypeRelease_Arguments "${D}MEZZRELEASE_undefined")
            set(Mezz_BuildTypeSmall_Arguments "${D}MEZZSMALL_undefined")
            set(Mezz_BuildTypeDebug_Defined "MEZZDEBUG_undefined")
            set(Mezz_BuildTypeRelease_Defined "MEZZRELEASE_undefined")
            set(Mezz_BuildTypeSmall_Defined "MEZZSMALL_undefined")
            set(CMAKE_BUILD_TYPE " " CACHE STRING "Set by default" FORCE)
        endif(Mezz_BuildTypeSmall)
    endif(Mezz_BuildTypeRelease)
endif(Mezz_BuildTypeDebug)

###############################################################################
# Now that we know our build type better, configure our library postfix.

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LibPostFix ${CMAKE_DEBUG_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(LibPostFix ${CMAKE_RELEASE_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(LibPostFix ${CMAKE_RELWITHDEBINFO_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(LibPostFix ${CMAKE_MINSIZEREL_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")

###############################################################################
# Set our other cmake variables that are dependent on the just set information

set(MezzanineLibFileName ${DynamicLibPrefix}Mezzanine${LibPostFix}${DynamicLibSuffix})
Message(STATUS "Expected name of compiled Mezzanine library: ${MezzanineLibFileName}")

set(MezzanineLinkName "Mezzanine")
if(${WIN32})
    #set(MezzanineLinkName lib${MezzanineLinkName}.dll.a)
    set(MezzanineLinkName ${MezzanineLibFileName})
endif(${WIN32})

###############################################################################
# Check For the Mezz_Profile option and set values accordingly
option(Mezz_Profile "Executables will gather runtime statistics." OFF)
if(${Mezz_Profile})
    message ( STATUS "${PROJECT_NAME} - Enabling gcc profiling and setting MEZZPROFILE")
    set(Mezz_Profile_Arguments "-pg ${D}MEZZPROFILE") # The pg makes this only work with gcc, clang and imitators
    set(Mezz_Profile_Defined "MEZZPROFILE = 1") # Is this required
else(${Mezz_Profile})
    message ( STATUS "${PROJECT_NAME} - Disabling gcc profiling and unsetting MEZZPROFILE")
    set( "${D}MEZZPROFILE_undefined")
    set(Mezz_Profile_Defined "MEZZPROFILE_undefined")
endif(${Mezz_Profile})

###############################################################################
# Check For the Mezz_Profile option and set values accordingly
option(Mezz_ThreadLog "Enables Macros for Thread issue logging." OFF)
if(${Mezz_ThreadLog})
    message ( STATUS "${PROJECT_NAME} - Enabling thread log instrumenting and setting MEZZTHREADLOG")
    set(Mezz_ThreadLog_Arguments "${D}MEZZTHREADLOG")
    set(Mezz_ThreadLog_Defined "MEZZTHREADLOG = 1") # Is this required
else(${Mezz_ThreadLog})
    message ( STATUS "${PROJECT_NAME} - Disabling thread log instrumenting and unsetting MEZZTHREADLOG")
    set(Mezz_ThreadLog_Arguments "${D}MEZZTHREADLOG_undefined")
    set(Mezz_ThreadLog_Defined "MEZZTHREADLOG_undefined")
endif(${Mezz_ThreadLog})

###############################################################################
# Check For the Mezz_Network option and set values accordingly
option(Mezz_Network "Enable Networking support." ON)
if(${Mezz_Network})
    message ( STATUS "${PROJECT_NAME} - Enabling Network subsystem, and setting MEZZNETWORK")
    set(Mezz_Network_Arguments "${D}MEZZNETWORK")
else(${Mezz_Network})
    message ( STATUS "${PROJECT_NAME} - Disabling Network subsystem, and unsetting MEZZNETWORK")
    set(Mezz_Network_Arguments "${D}MEZZNETWORK_undefined")
endif(${Mezz_Network})

###############################################################################
# Lua Library
option(Mezz_Lua51 "Include Support for Lua 5.1 Scripting Language" ON)
if(${Mezz_Lua51})
    set(LuaProject "Lua")
    set(LuaLinkName "lua")
    message ( STATUS "${PROJECT_NAME} - Enabling Support for Lua Scripting Language, and setting MEZZLUA51")
    set(Mezz_Lua51_Arguments "${D}MEZZLUA51")
else(${Mezz_Lua51})
    message ( STATUS "${PROJECT_NAME} - Disabling Support for Lua Scripting Language, and unsetting MEZZLUA51")
    set(Mezz_Lua51_Arguments "${D}MEZZLUA51_undefined")
endif(${Mezz_Lua51})

###############################################################################
# Ruby Library
option(Mezz_Ruby "Include Support for Ruby Scripting Language" OFF)
if(${Mezz_Ruby})
    set(RubyProject "Ruby")
    set(RubyLinkName "ruby")
    message ( STATUS "${PROJECT_NAME} - Enabling Support for Ruby Scripting Language, and setting MEZZRRUBY")
    set(Mezz_Ruby_Arguments "${D}MEZZRUBY")
else(${Mezz_Ruby})
    message ( STATUS "${PROJECT_NAME} - Disabling Support for Ruby Scripting Language, and unsetting MEZZRUBY")
    set(Mezz_Ruby_Arguments "${D}MEZZRUBY_undefined")
endif(${Mezz_Ruby})

###############################################################################
# Build Swig Library
option(Mezz_SWIG "Rerun swig/Rebuild script bindings before each build." OFF)
if(${Mezz_SWIG})
    message ( STATUS "${PROJECT_NAME} - Enabling Swig execution each build.")
    FIND_PACKAGE(SWIG REQUIRED)
    message ( "Swig location - ${SWIG_EXECUTABLE}")
else(${Mezz_SWIG})
    message ( STATUS "${PROJECT_NAME} - Disabling Swig execution each build.")
endif(${Mezz_SWIG})

###############################################################################
# Build Tools
option(Mezz_Tools_EntreLua "Build a Lua shell with access to the Mezzanine." ON)
if(${Mezz_Tools_EntreLua})
    message ( STATUS "${PROJECT_NAME} - Enabling EntreLua build.")
else(${Mezz_Tools_EntreLua})
    message ( STATUS "${PROJECT_NAME} - Disabling EntreLua build.")
endif(${Mezz_Tools_EntreLua})


###############################################################################
# DAGFrameScheduler

#How long should the default length on rolling averages and other multiframe periods be.
set(Mezz_FramesToTrack 10 CACHE STRING "How long should frame durations be tracked for")

#When looking for work use atomics to advance iterators past complete work units.
option(Mezz_DecacheWorkUnits "Use atomics to minimize how many WorkUnits are likely to be in CPU cache]." ON)
set(DecacheWork NODECACHEWORKUNIT)
if (Mezz_DecacheWorkUnits)
    set(DecacheWork DECACHEWORKUNIT)
endif (Mezz_DecacheWorkUnits)

add_definitions(
    ${D}_MEZZ_STATIC_BUILD_                   # Trailing slash indicates that compiler header should massage this before the library consumes
    ${D}_MEZZ_${MinimizeThreads}_
    ${D}_MEZZ_${DecacheWork}_
    ${D}_MEZZ_FRAMESTOTRACK_=${Mezz_FramesToTrack}
)

###############################################################################
# Misc simple defines
option(Mezz_ThrowOnSingletonFetch "When retrieving an invalid Singleton, should an exception be thrown" OFF)
if(${Mezz_ThrowOnSingletonFetch})
    message ( STATUS "${PROJECT_NAME} - Enabling exceptions on bad singleton fetches")
    set(Mezz_ThrowOnSingletonFetch_Arguments "${D}THROW_ON_FETCH_FAIL")
else(${Mezz_ThrowOnSingletonFetch})
    message ( STATUS "${PROJECT_NAME} - Disabling exceptions on bad singleton fetches")
    set(Mezz_ThrowOnSingletonFetch_Arguments "${D}THROW_ON_FETCH_FAIL_undefined")
endif(${Mezz_ThrowOnSingletonFetch})

# Allow the developer to disable mandatory compiler wanrings, in case it breaks stuff
option(Mezz_AllWarnings "Used atomics to minimize thread creation" ON)
if (Mezz_AllWarnings)
    if(MSVC)
    # Force to always compile with W4
        if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
            # Update if necessary
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
    endif()
endif (Mezz_AllWarnings)

# Make sure stdint.h is found for SDL and other subprojects
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)

CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif(HAVE_STDINT_H)

CHECK_INCLUDE_FILE("stddef.h" HAVE_STDDEF_H)
if(HAVE_STDDEF_H)
    add_definitions(-DHAVE_STDDEF_H)
endif(HAVE_STDDEF_H)

#######################################
# Ogre3d related values

set(OgreLibraryAllNames ${OgreLinkName} Plugin_ParticleFX)
set(Ogre_Arguments "")
set(OgreHeaderIncludeDir "")

# Setup rendersystem options
option (Mezz_BuildRenderSystemDirectX9 "Include Support for DirectX 9" OFF)
set(OGRE_BUILD_RENDERSYSTEM_D3D9 ${Mezz_BuildRenderSystemDirectX9} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_D3D9})
    message ( STATUS "${PROJECT_NAME} - Enabling DirectX 9 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX9_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/Direct3D9/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_Direct3D9)
else(${OGRE_BUILD_RENDERSYSTEM_D3D9})
    message ( STATUS "${PROJECT_NAME} - Disabling DirectX 9 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX9_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_D3D9})

option (Mezz_BuildRenderSystemDirectX11 "Include Support for DirectX 11" OFF)
set(OGRE_BUILD_RENDERSYSTEM_D3D11 ${Mezz_BuildRenderSystemDirectX11} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_D3D11})
    message ( STATUS "${PROJECT_NAME} - Enabling DirectX 11 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX11_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/Direct3D11/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_Direct3D11)
else(${OGRE_BUILD_RENDERSYSTEM_D3D11})
    message ( STATUS "${PROJECT_NAME} - Disabling DirectX 11 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX11_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_D3D11})

option (Mezz_BuildRenderSystemOpenGL "Include Support for OpenGL" ON)
set(OGRE_BUILD_RENDERSYSTEM_GL ${Mezz_BuildRenderSystemOpenGL} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GL})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGL Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGL_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GL/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GL)
else(${OGRE_BUILD_RENDERSYSTEM_GL})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGL Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_OPENGL_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GL})

option (Mezz_BuildRenderSystemOpenGL3Plus "Include Support for OpenGL3+" ON)
set(OGRE_BUILD_RENDERSYSTEM_GL3PLUS ${Mezz_BuildRenderSystemOpenGL3Plus} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GL3PLUS})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGL3+ Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGL3PLUS_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GL3Plus/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GL3Plus)
else(${OGRE_BUILD_RENDERSYSTEM_GL3PLUS})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGL3+ Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_OPENGL3PLUS_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GL3PLUS})

option (Mezz_BuildRenderSystemOpenGLES "Include Support for OpenGLES" OFF)
set(OGRE_BUILD_RENDERSYSTEM_GLES ${Mezz_BuildRenderSystemOpenGLES} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GLES})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGLES Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGLES_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GLES/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GLES)
else(${OGRE_BUILD_RENDERSYSTEM_GLES})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGLES Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_OPENGLES_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GLES})

option (Mezz_BuildRenderSystemOpenGLES2 "Include Support for OpenGLES2" OFF)
set(OGRE_BUILD_RENDERSYSTEM_GLES2 ${Mezz_BuildRenderSystemOpenGLES2} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GLES2})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGLES2 Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGLES2_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GLES2/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GLES2)
else(${OGRE_BUILD_RENDERSYSTEM_GLES2})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGLES2 Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGLES2_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GLES2})

if(${Mezz_BuildTypeDebug})
    message ( STATUS "${PROJECT_NAME} - Will we use Ogre Debug libraries")
    #set(OgreLinkName OgreMain_d)
    #set(OgresD _d)
    set(OgreLinkName OgreMain)
    set(OgresD "")
else(${Mezz_BuildTypeDebug})
    message ( STATUS "${PROJECT_NAME} - Will we use Ogre Release libraries")
    set(OgreLinkName OgreMain)
    set(OgresD "")
endif(${Mezz_BuildTypeDebug})

#This must be set manually each time ogre is upgraded.
set(Mezz_UseOgreVersion 1.10 CACHE STRING "The version of the ogre libraries to link against")
set(OgreDepName Ogre)
set(OgreLibFileName ${StaticLibPrefix}${OgreLinkName}${StaticLibSuffix})
set(OgreLinuxSoName ${OgreLibFileName}.${Mezz_UseOgreVersion})                #This value is gibberish on non-Linux platforms

set(OgreConfigHeaderIncludeDir "${ProjectBinaryDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/include/")
Message(STATUS "Putting Ogre3d config.h in: ${OgreConfigHeaderIncludeDir}")

set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/OgreMain/include")
set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/PlugIns/ParticleFX/include")

Message(STATUS "Looking for Ogre3d headers in: ${OgreHeaderIncludeDir}")

# Ogre Plugins
#set(OgrePluginCGPM ${StaticLibPrefix}Plugin_CgProgramManager${OgresD}${StaticLibSuffix})
#set(OgrePluginRenderGL ${StaticLibPrefix}RenderSystem_GL${OgresD}${StaticLibSuffix})
#set(OgrePluginParticle ${StaticLibPrefix}Plugin_ParticleFX${OgresD}${StaticLibSuffix})

###############################################################################
# Doxygen options
option (Mezz_Doc "Refresh Doxygen documentation during a build" OFF)

if(Mezz_Doc)
    Message(STATUS "Starting Doxygen Configuration.")
    if(${WIN32})
        set(CatCommand "type")
    else(${WIN32})
        set(CatCommand "cat")
    endif(${WIN32})

    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_custom_target(MezzanineDocumentation ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/Doxyfile
            COMMAND ${CatCommand} ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/DoxygenWarnings.txt 1>&2
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/closed.png
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/clclosed.png
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/closed.png
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/closed.png
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/open.png
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/clopen.png
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/favicon.ico
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/favicon.ico
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/open.png
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/open.png
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/red_x.png
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search/close.png
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/img/Magnifier.png
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search/mag_sel.png
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/html/search.css
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search/search.css

            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/tab_a.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/tab_b.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/tab_h.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/tab_s.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search/search_m.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search/search_l.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search/search_r.png
            COMMAND ${CMAKE_COMMAND} -E remove
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/search_r.png
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/Doxyfile
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/layout.xml
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/html/DoxygenWarnings.txt
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/html/DoxyStyle.css
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/html/search.css
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/html/DoxyFooter.html
                    ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/doxyfiles/html/DoxyHeader.html
        )
    else(DOXYGEN_FOUND)
        Message(STATUS "Doxygen not found.")
    endif(DOXYGEN_FOUND)
    Message(STATUS "Finishing Doxygen Configuration.")
endif(Mezz_Doc)

###############################################################################
# Network Library Define
if(${Mezz_Network})
    if(${WIN32})
        set(NetworkLibrary ws2_32)
    endif(${WIN32})
    # set windows and linux options here if required
endif(${Mezz_Network})

###############################################################################
# Misc library linking names
set(Bullet_LibraryNames
    # Extras
    "ConvexDecomposition"
    "GIMPACTUtils"
    "BulletWorldImporter"
    "BulletFileLoader"
    # Core Libs
    "BulletSoftBody" 
    "BulletDynamics" 
    "BulletCollision" 
    "LinearMath"
    # Defunct
    #"BulletSoftBodySolvers_OpenCL"
    #"BulletSoftBodySolvers_CPU"
    #"MiniCL"
    #"BulletMultiThreaded"
)

set(SoundLibraryNames
    "oggvorbis"
)

set(SDLLibraryNames
    "SDL2-static"
)

###############################################################################
# Perform Common Tasks for build options and configuration.

# Check and set a macro for endianess.
if(NOT MSVC AND NOT ANDROID)
    # This doesn't work on VS 2010
    # MSVC only builds for intel anyway
    include(TestBigEndian)
    test_big_endian(Mezz_Platform_Big_Endian)
    if(Mezz_Platform_Big_Endian)
        set(Mezz_Endian_Arguments "${D}MEZZ_BIG_ENDIAN")
    else(Mezz_Platform_Big_Endian)
        set(Mezz_Endian_Arguments "${D}MEZZ_LITTLE_ENDIAN")
    endif(Mezz_Platform_Big_Endian)
endif(NOT MSVC AND NOT ANDROID)

set(CommonBuildFlags "${Mezz_BuildTypeDebug_Arguments} ${Mezz_BuildTypeRelease_Arguments} ${Mezz_BuildTypeSmall_Arguments} ${Mezz_Profile_Arguments} ${Mezz_ThreadLog_Arguments} ${Mezz_Network_Arguments} ${Mezz_Lua51_Arguments} ${Mezz_Ruby_Arguments} ${Mezz_ThrowOnSingletonFetch_Arguments} ${Mezz_Endian_Arguments} ${Ogre_Arguments} -std=c++14")
separate_arguments(CommonBuildFlags UNIX_COMMAND "${CommonBuildFlags}")

###############################################################################
# Perform Platform Specific Tasks for build options and configuration.

# Force cmake to use Clang on MacOS
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #include(CMakeForceCompiler)
    #CMAKE_FORCE_C_COMPILER(clang GNU)
    #CMAKE_FORCE_CXX_COMPILER(clang++ GNU)
    #set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# Add subprojects

Message(STATUS "Starting Subproject Configuration.")
add_subdirectory (Mezzanine)
add_subdirectory (EngineDemo)
add_subdirectory (Catch)
add_subdirectory (UnitTests)
add_subdirectory (Tools/Armadillo)
if(${Mezz_Tools_EntreLua})
    add_subdirectory (Tools/EntreLua)
endif(${Mezz_Tools_EntreLua})
Message(STATUS "Finishing Subproject Configuration.")

###############################################################################
# Hide Variables not directly related to the Mezzanine Engine
mark_as_advanced( FORCE

    # Mezzanine options
    Mezz_UseOgreVersion Mezz_Profile Mezz_ThrowOnSingletonFetch
    Mezz_MinimizeThreadsEachFrame Mezz_FramesToTrack Mezz_DecacheWorkUnits
    Mezz_Lua51IncludeDirectory

    # Misc Options
    CMAKE_BACKWARDS_COMPATIBILITY CMAKE_BUILD_TYPE CMAKE_CODEBLOCKS_EXECUTABLE CMAKE_INSTALL_PREFIX CMAKE_DEBUG_POSTFIX CMAKE_GNUtoMS
    CMAKE_CXX_COMPILER CMAKE_C_COMPILER CMAKE_RC_COMPILER
    EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH
    NVIDIA_OPENCL_INCLUDES NVIDIA_OPENCL_ICD_CFG NVIDIA_OPENCL_LIBRARIES
    OGG_LIBRARY VORBISFILE_LIBRARY VORBIS_LIBRARY

    # Stuff that only appears on MacOS, I think
    AUDIOTOOLBOX_LIBRARY
    AUDIOUNIT
    OpenGL_LIBRARY
    CARBON_LIBRARY
    COCOA_LIBRARY
    COREAUDIO
    FORCEFEEDBACK
    IOKIT
    CMAKE_CONFIGURATION_TYPES
    CMAKE_OSX_ARCHITECTURES
    CMAKE_OSX_DEPLOYMENT_TARGET
    CMAKE_OSX_SYSROOT

    # Bullet options
    USE_DOUBLE_PRECISION
    USE_MSVC_RUNTIME_LIBRARY_DLL
    USE_MSVC_INCREMENTAL_LINKING
    USE_CUSTOM_VECTOR_MATH
    USE_DX11
    USE_MSVC_SSE
    USE_MSVC_FAST_FLOATINGPOINT
    INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES
    INTERNAL_CREATE_MSVC_RELATIVE_PATH_PROJECTFILES
    INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES
    INTEL_OPENCL_INCLUDES
    INTEL_OPENCL_LIBRARIES
    INTEL_OPENCL_ICD_CFG
    NVIDIA_OPENCL_INCLUDES
    NVIDIA_OPENCL_LIBRARIES
    NVIDIA_OPENCL_ICD_CFG
    INTERNAL_UPDATE_SERIALIZATION_STRUCTURES
    BUILD_EXTRAS
    BUILD_MULTITHREADING
    AMD_OPENCL_BASE_DIR
    USE_MULTITHREADED_BENCHMARK

    BUILD_AMD_OPENCL_DEMOS
    BUILD_INTEL_OPENCL_DEMOS
    BUILD_NVIDIA_OPENCL_DEMOS
    BUILD_MINICL_OPENCL_DEMOS
    BUILD_CPU_DEMOS
    USE_GLUT
    BUILD_DEMOS
    INSTALL_LIBS
    INSTALL_EXTRA_LIBS
    BUILD_UNIT_TESTS

    WINDRES WINE

    #SWIG
    SWIG_DIR SWIG_EXECUTABLE SWIG_VERSION
)

message("${PROJECT_NAME} - Finishing Configuration")
