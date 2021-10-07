cmake_minimum_required(VERSION 2.6)
project(game)

add_executable(
    game
    ../../source/gamemgr.cpp
    ../../source/gameobj.cpp
    ../../source/main.cpp
    ../../source/scriptmgr.cpp
    ../../../../add_on/scriptbuilder/scriptbuilder.cpp
    ../../../../add_on/scripthandle/scripthandle.cpp
    ../../../../add_on/scriptstdstring/scriptstdstring.cpp
    ../../../../add_on/weakref/weakref.cpp
)

if(NOT VITA)
 add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../angelscript/projects/cmake angelscript)
 include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../angelscript/include)
endif()

# Mods made 10-7-2021
# This tells the "game" sample to build itself as a Vita VPK.
# This also references the Vita's default AngelScript installation directory
#  to statically link against the library.
# Excellent sample block for those looking to adapt the other samples/include 
#  AngelScript in their Vita project.
if(VITA)
    include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    set(VITA_APP_NAME "libAngelscript Test Game")
    set(VITA_TITLEID "ANGELSCPT")
    set(VITA_VERSION "01.00")
    set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVITA -DPSP2 -g -Wall -O0 -fno-lto -ftree-vectorize")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVITA -DPSP2 -D__psp2__ -g -Wall -O0 -fno-lto -ftree-vectorize")

# STATIC LINKING
    include(GNUInstallDirs)
    find_library(ANGELSCRIPT_LIBRARY_NAME NAMES libAngelscript.a libangelscript.a)
    set(AS_HEADER_LOC ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/angelscript)
    message("INCLUDING: ${AS_HEADER_LOC}")
    target_include_directories(game BEFORE PUBLIC ${AS_HEADER_LOC})
# STATIC LINKING

# Script bundling
    file(GLOB_RECURSE resources "${PROJECT_SOURCE_DIR}/../../../game/bin/*.as")
    set(data_VPKSHADOW "")
    if(BUILD_VITA)
    foreach(resource ${resources})
        get_filename_component(filename ${resource} NAME)
        
        file(RELATIVE_PATH reldir ${PROJECT_SOURCE_DIR} ${resource})
        string(CONCAT data_VPKSHADOW "${data_VPKSHADOW}FILE;${reldir};${filename};")
    endforeach()
    endif()
# Script bundling

    target_link_libraries(game debugnet SceLibKernel_stub SceSysmodule_stub SceNetCtl_stub SceNet_stub)
    vita_create_self(eboot.bin "${PROJECT_SOURCE_DIR}/../../../game/bin/game" SAFE)

    vita_create_vpk(libAngelscript_test.vpk ${VITA_TITLEID} eboot.bin 
        VERSION ${VITA_VERSION}
        NAME ${VITA_APP_NAME}
        ${data_VPKSHADOW}
    )
endif()

target_link_libraries(game ${ANGELSCRIPT_LIBRARY_NAME})

set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin)
