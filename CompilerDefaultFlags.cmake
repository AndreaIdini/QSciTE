#----------------------------------------------------------------------------
# QSciTE cmake file
# Initializes the standard flags for the compilers
# created by CMake with custom values.
# 
# José Henriques 2014
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#Append some switches when building with Visual Studio
IF(DEFINED MSVC)
    # Enable multiple processor build if at least Visual Studio 2008 is used
    IF(NOT MSVC_VERSION LESS 1500)
        SET(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} /MP")
        SET(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} /MP")
    ENDIF(NOT MSVC_VERSION LESS 1500)

    SET(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} /wd4355 /wd4251 /wd4275 /wd4996")
    SET(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} /wd4355 /wd4251 /wd4275 /wd4996")
ENDIF(DEFINED MSVC)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# Set default C flags for gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS_DEBUG_INIT "-O0 -ggdb3")
    SET(CMAKE_C_FLAGS_RELEASE_INIT "-O2 -DNDEBUG")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O1 -ggdb3")

    IF(NOT NoWall)
        SET(CMAKE_C_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS_DEBUG_INIT} -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-missing-field-initializers")
        SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT} -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-missing-field-initializers")
    ENDIF(NOT NoWall)
ENDIF(CMAKE_COMPILER_IS_GNUCC)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# Set default C++ flags for gcc
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS_DEBUG_INIT "-O0 -ggdb3")
    SET(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2 -DNDEBUG")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O1 -ggdb3")

    IF(NOT NoWall)
        SET(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS_DEBUG_INIT} -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-missing-field-initializers")
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT} -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-missing-field-initializers")
    ENDIF(NOT NoWall)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# Set default Studio C/C++ flags for Xcode:
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )

    # This is needed for new versions of Xcode 5/clang where the libc++ is the default
    # In the future, new versions of all dependencies (boost, collada dom, etc...) need to 
    # be compiled with both versions (libstdc++ and libc++) or simply update everything to libc++;
    set( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libstdc++")

endif()
#----------------------------------------------------------------------------