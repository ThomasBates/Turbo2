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
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -fno-rtti")

get_filename_component(GamesFolder    ../../../../../../Games    ABSOLUTE)
get_filename_component(LibraryFolder  ../../../../../../Library  ABSOLUTE)
get_filename_component(PlatformFolder ../../../../../../Platform ABSOLUTE)

# now build app's shared lib
add_library(${PROJECT_NAME}
		SHARED
		android_main.cpp
		)

add_library(MazePreview
		STATIC
		${GamesFolder}/MazePreview/MazePreview.cpp

		${GamesFolder}/MazePreview/CubicMaze/CubicMaze.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeCornerMesh.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeEdgeMesh.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeFactory.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeMotionEffects.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeMotionEffects_WithGravity.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeMotionEffects_WithoutGravity.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeObjectInteractions.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneBuilder.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneBuilder_Castle.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneBuilder_Flat.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneBuilder_Laboratory.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneBuilder_Metal.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneBuilder_Random.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSceneObject.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeSignMesh.cpp
		${GamesFolder}/MazePreview/CubicMaze/CubicMazeWallMesh.cpp

		${GamesFolder}/MazePreview/Level00/Level00.cpp
		${GamesFolder}/MazePreview/Level00/Level00CubicMazeFactory.cpp
		${GamesFolder}/MazePreview/Level00/Level00Helper.cpp
		${GamesFolder}/MazePreview/Level00/Level00Player.cpp
		${GamesFolder}/MazePreview/Level00/Level01.cpp
		${GamesFolder}/MazePreview/Level00/Level02.cpp
		${GamesFolder}/MazePreview/Level00/Level03.cpp
		${GamesFolder}/MazePreview/Level00/Level04.cpp
		${GamesFolder}/MazePreview/Level00/Level05.cpp
		${GamesFolder}/MazePreview/Level00/Level05CubicMazeFactory.cpp
		)

add_library(Teapots
        STATIC
        ${GamesFolder}/Teapots/TeapotGame.cpp
        ${GamesFolder}/Teapots/TeapotMesh.cpp
        ${GamesFolder}/Teapots/TeapotPlayer.cpp
        )

add_library(TurboCore
		STATIC
		${LibraryFolder}/TurboCore/TurboDebug/TurboDebug.cpp
		${LibraryFolder}/TurboCore/TurboDebug/TurboDebugMemoryLogger.cpp
		)

add_library(TurboGame
		STATIC
		${LibraryFolder}/TurboGame/TurboGameMotionEffects.cpp
		${LibraryFolder}/TurboGame/TurboGameState.cpp
		)

add_library(TurboGraphics
		STATIC
		${LibraryFolder}/TurboGraphics/TurboBitmap.cpp
		${LibraryFolder}/TurboGraphics/TurboCanvasRGBA32.cpp
		)

add_library(TurboMath
		STATIC
		${LibraryFolder}/TurboMath/TurboMatrix4x4.cpp
		${LibraryFolder}/TurboMath/TurboVector2D.cpp
		${LibraryFolder}/TurboMath/TurboVector3D.cpp
		)

add_library(TurboScene
		STATIC
		${LibraryFolder}/TurboScene/TurboScene.cpp
		${LibraryFolder}/TurboScene/TurboSceneFont.cpp
		${LibraryFolder}/TurboScene/TurboSceneMaterial.cpp
		${LibraryFolder}/TurboScene/TurboSceneMesh.cpp
		${LibraryFolder}/TurboScene/TurboSceneObject.cpp
		${LibraryFolder}/TurboScene/TurboScenePixelShader.cpp
		${LibraryFolder}/TurboScene/TurboScenePlacement.cpp
		${LibraryFolder}/TurboScene/TurboSceneSoundEffect.cpp
		${LibraryFolder}/TurboScene/TurboSceneSprite.cpp
		${LibraryFolder}/TurboScene/TurboSceneText.cpp
		${LibraryFolder}/TurboScene/TurboSceneTexture.cpp
		${LibraryFolder}/TurboScene/TurboSceneVertexShader.cpp
		)

# TODO: Separate GL from NDK
add_library(AndroidNDK
		STATIC
		${PlatformFolder}/AndroidNDK/AndroidGLRenderer.cpp
		${PlatformFolder}/AndroidNDK/AndroidNDKAudio.cpp
		${PlatformFolder}/AndroidNDK/AndroidNDKDebugLogCatLogger.cpp
		${PlatformFolder}/AndroidNDK/AndroidNDKGameApplication.cpp
		${PlatformFolder}/AndroidNDK/AndroidNDKGameController_DualPads.cpp
		${PlatformFolder}/AndroidNDK/AndroidNDKGameController_LookAround.cpp
		${PlatformFolder}/AndroidNDK/AndroidNDKIOService.cpp
		${PlatformFolder}/AndroidNDK/gestureDetector.cpp
		${PlatformFolder}/AndroidNDK/gl3stub.cpp
		${PlatformFolder}/AndroidNDK/GLContext.cpp
		${PlatformFolder}/AndroidNDK/interpolator.cpp
		${PlatformFolder}/AndroidNDK/JNIHelper.cpp
		${PlatformFolder}/AndroidNDK/perfMonitor.cpp
		${PlatformFolder}/AndroidNDK/sensorManager.cpp
		${PlatformFolder}/AndroidNDK/shader.cpp
		${PlatformFolder}/AndroidNDK/tapCamera.cpp
		${PlatformFolder}/AndroidNDK/vecmath.cpp
		)

include_directories(.)
include_directories(${GamesFolder}/Teapots)
include_directories(${GamesFolder}/MazePreview)
include_directories(${GamesFolder}/MazePreview/CubicMaze)
include_directories(${GamesFolder}/MazePreview/Level00)
include_directories(${LibraryFolder}/TurboCore/TurboDebug)
include_directories(${LibraryFolder}/TurboGame)
include_directories(${LibraryFolder}/TurboGraphics)
include_directories(${LibraryFolder}/TurboMath)
include_directories(${LibraryFolder}/TurboScene)
include_directories(${PlatformFolder}/AndroidNDK)

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

set_target_properties(MazePreview
		PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
		INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:MazePreview,INCLUDE_DIRECTORIES>
		)

# add lib dependencies
target_link_libraries(${PROJECT_NAME}
		PRIVATE
		Teapots
		MazePreview
		TurboCore
		TurboGame
		TurboGraphics
		TurboMath
		TurboScene
		AndroidNDK
		)

target_link_libraries(MazePreview
		PRIVATE
		TurboCore
		TurboGame
		TurboGraphics
		TurboMath
		TurboScene
		)

target_link_libraries(TurboGame
		PUBLIC
		TurboScene
		)

target_link_libraries(TurboScene
		PUBLIC
		TurboMath
		)

target_link_libraries(AndroidNDK
		PUBLIC
		native_app_glue
		GLESv2
		EGL
		log
		android
		atomic
		TurboGame
		TurboGraphics
		)
