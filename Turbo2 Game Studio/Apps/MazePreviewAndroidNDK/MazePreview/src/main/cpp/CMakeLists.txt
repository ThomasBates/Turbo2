#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)
project(MazePreviewNativeActivity LANGUAGES C CXX)

include(AndroidNdkModules)
android_ndk_import_module_native_app_glue()

# set up common compile options
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -frtti")

get_filename_component(AppFolder ../../../.. ABSOLUTE)

get_filename_component(TurboFolder ../../../../../.. ABSOLUTE)

# now build app's shared lib
add_library(${PROJECT_NAME}
        SHARED
        android_main.cpp

        ${AppFolder}/Views/MazePreviewRootView.cpp
        ${AppFolder}/Views/MazePreviewMainView.cpp
        ${AppFolder}/Views/MazePreviewHUD1View.cpp
        ${AppFolder}/Views/MazePreviewHUD2View.cpp
        ${AppFolder}/Views/MazePreviewDirectionControlView.cpp
        ${AppFolder}/Views/MazePreviewMainControlView.cpp
        ${AppFolder}/Views/MazePreviewMotionControlView.cpp
        ${AppFolder}/Views/MazePreviewMenuView.cpp
        ${AppFolder}/Views/MazePreviewInfoView.cpp
        )

add_library(Games_MazePreview
        STATIC
        ${TurboFolder}/Games/MazePreview/MazePreview.cpp

        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMaze.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeCornerMesh.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeEdgeMesh.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeFactory.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeMotionEffects_WithGravity.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeMotionEffects_WithoutGravity.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeObjectInteractions.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneBuilder.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneBuilder_Castle.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneBuilder_Flat.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneBuilder_Laboratory.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneBuilder_Metal.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneBuilder_Random.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSceneObject.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeSignMesh.cpp
        ${TurboFolder}/Games/MazePreview/CubicMaze/CubicMazeWallMesh.cpp

        ${TurboFolder}/Games/MazePreview/Level00/Level00.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level00CubicMazeFactory.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level00Helper.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level00Player.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level01.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level02.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level03.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level04.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level05.cpp
        ${TurboFolder}/Games/MazePreview/Level00/Level05CubicMazeFactory.cpp

        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewRootViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewMainViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewHUD1ViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewHUD2ViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewDirectionControlViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewMainControlViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewMotionControlViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewMenuViewModel.cpp
        ${TurboFolder}/Games/MazePreview/ViewModels/MazePreviewInfoViewModel.cpp
        )

add_library(Library_TurboCore
        STATIC
        ${TurboFolder}/Library/TurboCore/TurboDebug/TurboDebug.cpp
        ${TurboFolder}/Library/TurboCore/TurboDebug/TurboDebugMemoryLogger.cpp
        ${TurboFolder}/Library/TurboCore/TurboSystem/TurboEvent.cpp
        ${TurboFolder}/Library/TurboCore/TurboSystem/TurboEventHandler.cpp
        ${TurboFolder}/Library/TurboCore/TurboPresentation/TurboNotifyPropertyChanged.cpp
        ${TurboFolder}/Library/TurboCore/TurboPresentation/TurboCommand.cpp
        ${TurboFolder}/Library/TurboCore/TurboPresentation/TurboTypedCommand.cpp
        )

add_library(Library_TurboGame
        STATIC
        ${TurboFolder}/Library/TurboGame/TurboGameAggregateRenderer.cpp
        ${TurboFolder}/Library/TurboGame/TurboGameMotionEffects.cpp
        ${TurboFolder}/Library/TurboGame/TurboGameNullAudioRenderer.cpp
        ${TurboFolder}/Library/TurboGame/TurboGameState.cpp
        ${TurboFolder}/Library/TurboGame/ViewModels/TurboSceneViewModel.cpp
        ${TurboFolder}/Library/TurboGame/Views/TurboSceneView.cpp
        )

add_library(Library_TurboGraphics
        STATIC
        ${TurboFolder}/Library/TurboGraphics/TurboBitmap.cpp
        ${TurboFolder}/Library/TurboGraphics/TurboCanvasRGBA32.cpp
        )

add_library(Library_TurboMath
        STATIC
        ${TurboFolder}/Library/TurboMath/TurboMatrix4x4.cpp
        ${TurboFolder}/Library/TurboMath/TurboVector2D.cpp
        ${TurboFolder}/Library/TurboMath/TurboVector3D.cpp
        )

add_library(Library_TurboScene
        STATIC
        ${TurboFolder}/Library/TurboScene/TurboScene.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneArialFont.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneFont.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneMaterial.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneMesh.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneNavigationButtonControl.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneNavigationPadControl.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneNavigationSetControl.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneNavigationStickControl.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneNavigationTapControl.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneObject.cpp
        ${TurboFolder}/Library/TurboScene/TurboScenePixelShader.cpp
        ${TurboFolder}/Library/TurboScene/TurboScenePlacement.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneSound.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneSprite.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneText.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneTexture.cpp
        ${TurboFolder}/Library/TurboScene/TurboSceneVertexShader.cpp
        )

add_library(Library_TurboSound
        STATIC
        ${TurboFolder}/Library/TurboSound/TurboWavFormat.cpp
        ${TurboFolder}/Library/TurboSound/TurboSound16BitCanvas.cpp
        ${TurboFolder}/Library/TurboSound/TurboSoundFloatCanvas.cpp
        )

add_library(Library_TurboView
        STATIC
        ${TurboFolder}/Library/TurboView/TurboView.cpp
        ${TurboFolder}/Library/TurboView/ViewModels/TurboControlViewModel.cpp
        ${TurboFolder}/Library/TurboView/Views/TurboGroupView.cpp
        ${TurboFolder}/Library/TurboView/Views/TurboControlView.cpp
        )

add_library(Platform_AndroidNDK
        STATIC
        ${TurboFolder}/Platform/AndroidNDK/AndroidNDKDebugLogCatLogger.cpp
        ${TurboFolder}/Platform/AndroidNDK/AndroidNDKGameApplication.cpp
        ${TurboFolder}/Platform/AndroidNDK/AndroidNDKIOService.cpp
        ${TurboFolder}/Platform/AndroidNDK/AndroidNDKViewController.cpp
        ${TurboFolder}/Platform/AndroidNDK/JNIHelper.cpp
        ${TurboFolder}/Platform/AndroidNDK/perfMonitor.cpp
        )

add_library(Platform_OpenGLES
        STATIC
        ${TurboFolder}/Platform/OpenGLES/gl3stub.cpp
        ${TurboFolder}/Platform/OpenGLES/OpenGLESRenderer.cpp
        ${TurboFolder}/Platform/OpenGLES/OpenGLESContext.cpp
        )

add_library(Platform_Oboe
        STATIC
        ${TurboFolder}/Platform/Oboe/OboeAudioLoopingTrack.cpp
        ${TurboFolder}/Platform/Oboe/OboeAudioRenderer.cpp
        ${TurboFolder}/Platform/Oboe/OboeAudioSimpleTrack.cpp
        ${TurboFolder}/Platform/Oboe/OboeAudioTestTrack.cpp
        )

include_directories(.)
include_directories(${AppFolder}/Views)

include_directories(${TurboFolder}/Games/MazePreview)
include_directories(${TurboFolder}/Games/MazePreview/CubicMaze)
include_directories(${TurboFolder}/Games/MazePreview/Level00)
include_directories(${TurboFolder}/Games/MazePreview/ViewModels)

include_directories(${TurboFolder}/Library/TurboCore)
include_directories(${TurboFolder}/Library/TurboCore/TurboDebug)
include_directories(${TurboFolder}/Library/TurboCore/TurboPresentation)
include_directories(${TurboFolder}/Library/TurboCore/TurboSystem)
include_directories(${TurboFolder}/Library/TurboGame)
include_directories(${TurboFolder}/Library/TurboGame/ViewModels)
include_directories(${TurboFolder}/Library/TurboGame/Views)
include_directories(${TurboFolder}/Library/TurboGraphics)
include_directories(${TurboFolder}/Library/TurboMath)
include_directories(${TurboFolder}/Library/TurboScene)
include_directories(${TurboFolder}/Library/TurboSound)
include_directories(${TurboFolder}/Library/TurboView)
include_directories(${TurboFolder}/Library/TurboView/ViewModels)
include_directories(${TurboFolder}/Library/TurboView/Views)

include_directories(${TurboFolder}/Platform/AndroidNDK)
include_directories(${TurboFolder}/Platform/OpenGLES)
include_directories(${TurboFolder}/Platform/Oboe)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${PROJECT_NAME},INCLUDE_DIRECTORIES>
        )

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        LINK_FLAGS "-u ANativeActivity_onCreate"
        )

set_target_properties(Games_MazePreview
        PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:Games_MazePreview,INCLUDE_DIRECTORIES>
        )

# add lib dependencies
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Games_MazePreview
        Library_TurboCore
        Library_TurboGame
        Library_TurboGraphics
        Library_TurboMath
        Library_TurboScene
        Library_TurboSound
        Library_TurboView
        Platform_AndroidNDK
        Platform_OpenGLES
        Platform_Oboe
        )

target_link_libraries(Games_MazePreview
        PRIVATE
        Library_TurboCore
        Library_TurboGame
        Library_TurboGraphics
        Library_TurboMath
        Library_TurboScene
        Library_TurboSound
        Library_TurboView
        )

target_link_libraries(Library_TurboGame
        PUBLIC
        Library_TurboScene
        Library_TurboView
        )

target_link_libraries(Library_TurboScene
        PUBLIC
        Library_TurboMath
        )

target_link_libraries(Library_TurboView
        PUBLIC
        Library_TurboCore
        Library_TurboMath
        Library_TurboScene
        )

target_link_libraries(Platform_AndroidNDK
        PUBLIC
        native_app_glue
        log
        android
        atomic
        oboe
        Library_TurboCore
        Library_TurboGame
        Library_TurboView
        )

target_link_libraries(Platform_OpenGLES
        PUBLIC
        GLESv2
        EGL
        atomic
        Library_TurboCore
        Library_TurboGame
        Library_TurboGraphics
        Library_TurboView
        )

target_link_libraries(Platform_Oboe
        PUBLIC
        atomic
        Library_TurboCore
        Library_TurboGame
        Library_TurboSound
        Library_TurboView
        )


# Set the path to the Oboe directory.
set (OBOE_DIR D:/Development/ThirdParty/oboe)

# Add the Oboe library as a subdirectory in your project.
# add_subdirectory tells CMake to look in this directory to
# compile oboe source files using oboe's CMake file.
# ./oboe specifies where the compiled binaries will be stored
add_subdirectory (${OBOE_DIR} ../../../../oboe)

# Specify the path to the Oboe header files.
# This allows targets compiled with this CMake (application code)
# to see public Oboe headers, in order to access its API.
include_directories (${OBOE_DIR}/include)