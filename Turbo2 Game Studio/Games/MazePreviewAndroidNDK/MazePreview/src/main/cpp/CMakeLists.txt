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

# set up common compile options
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror -fno-exceptions -fno-rtti")

# build the ndk-helper library into a common directory
# visible to all 3 projects avoiding duplicate builds.
get_filename_component(turboPlatform ../../../../../../Platform ABSOLUTE)
get_filename_component(ndkHelperSrc ../../../../ndk_helper ABSOLUTE)
get_filename_component(ndkHelperBin ${ndkHelperSrc}/ndkHelperBin ABSOLUTE)
add_subdirectory(${ndkHelperSrc} ${ndkHelperBin})

# build the turbo library into a common directory
get_filename_component(turboLibrary ../../../../../../Library ABSOLUTE)
get_filename_component(turboSrc ../../../../turbo ABSOLUTE)
get_filename_component(turboBin ${turboSrc}/turboBin ABSOLUTE)
add_subdirectory(${turboSrc} ${turboBin})

# now build app's shared lib
add_library(${PROJECT_NAME}
  SHARED
    android_main.cpp
		TeapotGame.cpp
		TeapotMesh.cpp
		TeapotPlayer.cpp
        )

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
  PROPERTIES  LINK_FLAGS "-u ANativeActivity_onCreate")

target_include_directories(${PROJECT_NAME}
		# ${CMAKE_CURRENT_SOURCE_DIR}
		PUBLIC ${turboLibrary}/TurboCore/TurboDebug
		PUBLIC ${turboLibrary}/TurboGame
		PUBLIC ${turboLibrary}/TurboGraphics
		PUBLIC ${turboLibrary}/TurboMath
		PUBLIC ${turboLibrary}/TurboScene
		PUBLIC ${turboPlatform}/AndroidNDK)

# add lib dependencies
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    AndroidNDK
	TurboCore
    TurboGame
	TurboGraphics
	TurboMath
    TurboScene
)
