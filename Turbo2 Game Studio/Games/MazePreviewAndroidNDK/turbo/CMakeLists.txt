# build native_app_glue as a static lib
cmake_minimum_required(VERSION 3.4.1)

#include(AndroidNdkModules)
#android_ndk_import_module_native_app_glue()

add_library(TurboCore
  STATIC
    ../../../Library/TurboCore/TurboDebug/TurboDebug.cpp
    ../../../Library/TurboCore/TurboDebug/TurboDebugMemoryLogger.cpp
)

add_library(TurboGame
        STATIC
        ../../../Library/TurboGame/TurboGameMotionEffects.cpp
        ../../../Library/TurboGame/TurboGameState.cpp
        )

add_library(TurboGraphics
        STATIC
        ../../../Library/TurboGraphics/TurboBitmap.cpp
        ../../../Library/TurboGraphics/TurboCanvasRGBA32.cpp
        )

add_library(TurboMath
  STATIC
    ../../../Library/TurboMath/TurboMatrix4x4.cpp
    ../../../Library/TurboMath/TurboVector2D.cpp
    ../../../Library/TurboMath/TurboVector3D.cpp
)

add_library(TurboScene
  STATIC
    ../../../Library/TurboScene/TurboScene.cpp
    ../../../Library/TurboScene/TurboSceneFont.cpp
    ../../../Library/TurboScene/TurboSceneMaterial.cpp
    ../../../Library/TurboScene/TurboSceneMesh.cpp
    ../../../Library/TurboScene/TurboSceneObject.cpp
    ../../../Library/TurboScene/TurboScenePixelShader.cpp
    ../../../Library/TurboScene/TurboScenePlacement.cpp
    ../../../Library/TurboScene/TurboSceneSoundEffect.cpp
    ../../../Library/TurboScene/TurboSceneSprite.cpp
    ../../../Library/TurboScene/TurboSceneText.cpp
    ../../../Library/TurboScene/TurboSceneTexture.cpp
    ../../../Library/TurboScene/TurboSceneVertexShader.cpp
)

include_directories(../MazePreview/src/main/cpp)
include_directories(../../../Library/TurboCore/TurboDebug)
include_directories(../../../Library/TurboGame)
include_directories(../../../Library/TurboGraphics)
include_directories(../../../Library/TurboMath)
include_directories(../../../Library/TurboScene)

set_target_properties(TurboCore
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:TurboCore,INCLUDE_DIRECTORIES>
)

set_target_properties(TurboGame
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:TurboGame,INCLUDE_DIRECTORIES>
)

# set_target_properties(TurboGraphics
#   PROPERTIES
#     CXX_STANDARD 11
#     CXX_STANDARD_REQUIRED YES
#     CXX_EXTENSIONS NO
#     INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:TurboGraphics,INCLUDE_DIRECTORIES>
# )

set_target_properties(TurboMath
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:TurboMath,INCLUDE_DIRECTORIES>
)

set_target_properties(TurboScene
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:TurboScene,INCLUDE_DIRECTORIES>
)

target_include_directories(TurboCore
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(TurboGame
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_include_directories(TurboGraphics
#   PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )

target_include_directories(TurboMath
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(TurboScene
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TurboCore
  PUBLIC
    native_app_glue
    GLESv2
    EGL
    log
    android
    atomic
)

target_link_libraries(TurboGame
  PUBLIC
    native_app_glue
    GLESv2
    EGL
    log
    android
    atomic
)

# target_link_libraries(TurboGraphics
#   PUBLIC
#     native_app_glue
#     GLESv2
#     EGL
#     log
#     android
#     atomic
# )

target_link_libraries(TurboMath
  PUBLIC
    native_app_glue
    GLESv2
    EGL
    log
    android
    atomic
)

target_link_libraries(TurboScene
  PUBLIC
    native_app_glue
    GLESv2
    EGL
    log
    android
    atomic
    TurboMath
)
