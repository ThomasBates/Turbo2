# build native_app_glue as a static lib
cmake_minimum_required(VERSION 3.4.1)

include(AndroidNdkModules)
android_ndk_import_module_native_app_glue()

add_library(AndroidNDK
  STATIC
    ../../../Platform/AndroidNDK/AndroidNDKGameApplication.cpp
    ../../../Platform/AndroidNDK/AndroidNDKRenderer.cpp
    ../../../Platform/AndroidNDK/gestureDetector.cpp
    ../../../Platform/AndroidNDK/gl3stub.cpp
    ../../../Platform/AndroidNDK/GLContext.cpp
    ../../../Platform/AndroidNDK/interpolator.cpp
    ../../../Platform/AndroidNDK/JNIHelper.cpp
    ../../../Platform/AndroidNDK/perfMonitor.cpp
    ../../../Platform/AndroidNDK/sensorManager.cpp
    ../../../Platform/AndroidNDK/shader.cpp
    ../../../Platform/AndroidNDK/tapCamera.cpp
    ../../../Platform/AndroidNDK/vecmath.cpp
)

include_directories(../MazePreview/src/main/cpp)
include_directories(../../../Platform/AndroidNDK/)
include_directories(../../../Library/TurboGame)
include_directories(../../../Library/TurboGraphics)
include_directories(../../../Library/TurboMath)
include_directories(../../../Library/TurboScene)

set_target_properties(AndroidNDK
  PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:AndroidNDK,INCLUDE_DIRECTORIES>
)

target_include_directories(AndroidNDK
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AndroidNDK
  PUBLIC
    native_app_glue
    GLESv2
    EGL
    log
    android
    atomic
)
