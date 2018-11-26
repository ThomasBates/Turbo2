//
// Created by ejdeh on 2018-10-29.
//

#ifndef MAZEPREVIEWANDROIDNDK_MAZEPREVIEWNATIVEACTIVITY_H
#define MAZEPREVIEWANDROIDNDK_MAZEPREVIEWNATIVEACTIVITY_H


//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <vector>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include <ITurboGameRenderer.h>
//#include "GLContext.h"
#include <AndroidNDKRenderer.h>

using namespace Turbo::Game;

class AndroidNDKGameEngine
{
    std::shared_ptr<ITurboGameRenderer> _renderer;
    AndroidNDKRenderer renderer_;

    ndk_helper::GLContext* gl_context_;

    bool initialized_resources_;
    bool has_focus_;

    ndk_helper::DoubletapDetector doubletap_detector_;
    ndk_helper::PinchDetector pinch_detector_;
    ndk_helper::DragDetector drag_detector_;
    ndk_helper::PerfMonitor monitor_;

    ndk_helper::TapCamera tap_camera_;

    android_app* _android_app;

    ASensorManager* sensor_manager_;
    const ASensor* accelerometer_sensor_;
    ASensorEventQueue* sensor_event_queue_;

    void UpdateFPS(float fps);
    void ShowUI();
    void TransformPosition(ndk_helper::Vec2& vec);

public:
    static void HandleCmd(struct android_app* app, int32_t cmd);
    static int32_t HandleInput(android_app* app, AInputEvent* event);

    AndroidNDKGameEngine(android_app* app, std::shared_ptr<ITurboGameRenderer> _renderer);
    virtual ~AndroidNDKGameEngine() {}

    void SetState(android_app* app);
    int InitDisplay(android_app* app);
    void LoadResources();
    void UnloadResources();
    void DrawFrame();
    void TermDisplay();
    void TrimMemory();
    bool IsReady();

    void UpdatePosition(AInputEvent* event, int32_t index, float& x, float& y);

    void InitSensors();
    void ProcessSensors(int32_t id);
    void SuspendSensors();
    void ResumeSensors();
};


#endif //MAZEPREVIEWANDROIDNDK_MAZEPREVIEWNATIVEACTIVITY_H
