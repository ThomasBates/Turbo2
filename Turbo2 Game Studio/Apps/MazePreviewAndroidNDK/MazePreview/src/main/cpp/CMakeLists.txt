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

get_filename_component(TurboFolder ../../../../../.. ABSOLUTE)

# now build app's shared lib
add_library(${PROJECT_NAME}
		SHARED
		android_main.cpp
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
		)

add_library(Games_Teapots
		STATIC
		${TurboFolder}/Games/Teapots/TeapotGame.cpp
		${TurboFolder}/Games/Teapots/TeapotMesh.cpp
		${TurboFolder}/Games/Teapots/TeapotPlayer.cpp
		)

add_library(Library_TurboCore
		STATIC
		${TurboFolder}/Library/TurboCore/TurboDebug/TurboDebug.cpp
		${TurboFolder}/Library/TurboCore/TurboDebug/TurboDebugMemoryLogger.cpp
		)

add_library(Library_TurboGame
		STATIC
		${TurboFolder}/Library/TurboGame/TurboGameControllerViewModel.cpp
		${TurboFolder}/Library/TurboGame/TurboGameMotionEffects.cpp
		${TurboFolder}/Library/TurboGame/TurboGameState.cpp
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
		${TurboFolder}/Library/TurboScene/TurboSceneFont.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneMaterial.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneMesh.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneNavigationControl_Base.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneNavigationControl_Button.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneNavigationControl_Last.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneNavigationControl_Set.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneObject.cpp
		${TurboFolder}/Library/TurboScene/TurboScenePixelShader.cpp
		${TurboFolder}/Library/TurboScene/TurboScenePlacement.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneSoundEffect.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneSprite.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneText.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneTexture.cpp
		${TurboFolder}/Library/TurboScene/TurboSceneVertexShader.cpp
		)

add_library(Platform_AndroidNDK
		STATIC
		${TurboFolder}/Platform/AndroidNDK/AndroidNDKAudio.cpp
		${TurboFolder}/Platform/AndroidNDK/AndroidNDKDebugLogCatLogger.cpp
		${TurboFolder}/Platform/AndroidNDK/AndroidNDKGameApplication.cpp
		${TurboFolder}/Platform/AndroidNDK/AndroidNDKGameController.cpp
		${TurboFolder}/Platform/AndroidNDK/AndroidNDKIOService.cpp
		${TurboFolder}/Platform/AndroidNDK/JNIHelper.cpp
		${TurboFolder}/Platform/AndroidNDK/perfMonitor.cpp
		)

add_library(Platform_OpenGLES
		STATIC
		${TurboFolder}/Platform/OpenGLES/gl3stub.cpp
		${TurboFolder}/Platform/OpenGLES/OpenGLESRenderer.cpp
		${TurboFolder}/Platform/OpenGLES/OpenGLESContext.cpp
		)

include_directories(.)
include_directories(${TurboFolder}/Games/Teapots)
include_directories(${TurboFolder}/Games/MazePreview)
include_directories(${TurboFolder}/Games/MazePreview/CubicMaze)
include_directories(${TurboFolder}/Games/MazePreview/Level00)
include_directories(${TurboFolder}/Library/TurboCore)
include_directories(${TurboFolder}/Library/TurboCore/TurboDebug)
include_directories(${TurboFolder}/Library/TurboGame)
include_directories(${TurboFolder}/Library/TurboGraphics)
include_directories(${TurboFolder}/Library/TurboMath)
include_directories(${TurboFolder}/Library/TurboScene)
include_directories(${TurboFolder}/Platform/AndroidNDK)
include_directories(${TurboFolder}/Platform/OpenGLES)

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
		Games_Teapots
		Games_MazePreview
		Library_TurboCore
		Library_TurboGame
		Library_TurboGraphics
		Library_TurboMath
		Library_TurboScene
        Platform_AndroidNDK
        Platform_OpenGLES
		)

target_link_libraries(Games_MazePreview
		PRIVATE
		Library_TurboCore
		Library_TurboGame
		Library_TurboGraphics
		Library_TurboMath
		Library_TurboScene
		)

target_link_libraries(Library_TurboGame
		PUBLIC
		Library_TurboScene
		)

target_link_libraries(Library_TurboScene
		PUBLIC
		Library_TurboMath
		)

target_link_libraries(Platform_AndroidNDK
		PUBLIC
		native_app_glue
		log
		android
		atomic
		Library_TurboCore
		Library_TurboGame
		)

target_link_libraries(Platform_OpenGLES
		PUBLIC
		GLESv2
		EGL
		atomic
		Library_TurboCore
		Library_TurboGame
		Library_TurboGraphics
		)
