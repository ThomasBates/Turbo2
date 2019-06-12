
#include <pch.h>

#include <sensorManager.h>

#include <AndroidNDKGameController_Legacy.h>

using namespace Turbo::Platform::AndroidNDK;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKGameController_Legacy::AndroidNDKGameController_Legacy(
		android_app* app,
		std::shared_ptr<ITurboDebug> debug) :
		_android_app(app),
		_debug(debug)
{
	_android_app->controller = this;
	_android_app->onInputEvent = AndroidNDKGameController_Legacy::HandleInputEvents;

	InitSensors();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameController Methods ---------------------------------------------------------------------------------------

NavigationInfo* AndroidNDKGameController_Legacy::GetNavigationInfo()
{
    ProcessEvents();

    double time = _performance_monitor.GetCurrentTime();
    _navInfo.DeltaTime = time - _navInfo.Time;
    _navInfo.Time = time;

    return &_navInfo;
}

void AndroidNDKGameController_Legacy::Suspend()
{
    SuspendSensors();
}

void AndroidNDKGameController_Legacy::Resume()
{
    ResumeSensors();
}

//  ITurboGameController Methods ---------------------------------------------------------------------------------------
//	Event Handler Methods ----------------------------------------------------------------------------------------------

/**
 * Process the next input event.
 */
int32_t AndroidNDKGameController_Legacy::HandleInputEvents(android_app* app, AInputEvent* event)
{
	AndroidNDKGameController_Legacy* controller = (AndroidNDKGameController_Legacy*)app->controller;

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
        controller->DebugLogEvent(event);

		ndk_helper::GESTURE_STATE doubleTapState = controller->_doubletap_detector.Detect(event);
		ndk_helper::GESTURE_STATE dragState      = controller->_drag_detector.Detect(event);
		ndk_helper::GESTURE_STATE pinchState     = controller->_pinch_detector.Detect(event);

		// Double tap detector has a priority over other detectors
		if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION)
		{
			controller->DoubleTap();
		}
		else
		{
			// Handle drag state
			if (dragState & ndk_helper::GESTURE_STATE_START)
			{
				// Otherwise, start dragging
				controller->StartDrag();
			}
			else if (dragState & ndk_helper::GESTURE_STATE_MOVE)
			{
				controller->Drag();
			}
			else if (dragState & ndk_helper::GESTURE_STATE_END)
			{
				controller->EndDrag();
			}

			// Handle pinch state
			if (pinchState & ndk_helper::GESTURE_STATE_START)
			{
				// Start new pinch
				controller->StartPinch();
			}
			else if (pinchState & ndk_helper::GESTURE_STATE_MOVE)
			{
				// Multi touch
				// Start new pinch
				controller->Pinch();
			}
		}
		return 1;
	}
	return 0;
}

//	Event Handler Methods ----------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------
// Sensor handlers
//-------------------------------------------------------------------------
void AndroidNDKGameController_Legacy::InitSensors()
{
    _doubletap_detector.SetConfiguration(_android_app->config);
    _drag_detector.SetConfiguration(_android_app->config);
    _pinch_detector.SetConfiguration(_android_app->config);

//	_tap_camera.SetFlip(1.f, -1.f, -1.f);
//	_tap_camera.SetPinchTransformFactor(10.f, 10.f, 8.f);

	_sensor_manager = ndk_helper::AcquireASensorManagerInstance(_android_app);
    _accelerometer_sensor = ASensorManager_getDefaultSensor(_sensor_manager, ASENSOR_TYPE_ACCELEROMETER);
    _sensor_event_queue = ASensorManager_createEventQueue(_sensor_manager, _android_app->looper, LOOPER_ID_USER, NULL, NULL);
}

bool AndroidNDKGameController_Legacy::ProcessEvents()
{
	// Read all pending events.
	int id;

	//  If the timeout is zero, returns immediately without blocking.
	//  If the timeout is negative, waits indefinitely until an event appears.
	int timeout = _isRunning ? 0 : -1;
	int events;
	android_poll_source* source;

	// If not animating, we will block forever waiting for events.
	// If animating, we loop until all events are read, then continue
	// to draw the next frame of animation.
	if ((id = ALooper_pollAll(timeout, NULL, &events, (void**)&source)) >= 0)
	{
		// Process this event.
		if (source != NULL)
		{
			source->process(_android_app, source);
		}

		ProcessSensors(id);

		// Check if we are exiting.
		if (_android_app->destroyRequested != 0)
		{
			_navInfo.Terminate = true;
			return false;
		}
	}

	return true;
}

void AndroidNDKGameController_Legacy::ProcessSensors(int32_t id)
{
    // If a sensor has data, process it now.
    if (id == LOOPER_ID_USER)
    {
        if (_accelerometer_sensor != NULL)
        {
            ASensorEvent event;
            while (ASensorEventQueue_getEvents(_sensor_event_queue, &event, 1) > 0)
            {
            }
        }
    }
}

void AndroidNDKGameController_Legacy::ResumeSensors()
{
    // When our app gains focus, we start monitoring the accelerometer.
    if (_accelerometer_sensor != NULL)
    {
        ASensorEventQueue_enableSensor(_sensor_event_queue, _accelerometer_sensor);
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate(_sensor_event_queue, _accelerometer_sensor,
                                       (1000L / 60) * 1000);
    }

    _isRunning = true;
}

void AndroidNDKGameController_Legacy::SuspendSensors()
{
	_isRunning = false;

    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if (_accelerometer_sensor != NULL)
    {
        ASensorEventQueue_disableSensor(_sensor_event_queue, _accelerometer_sensor);
    }
}

void AndroidNDKGameController_Legacy::DoubleTap()
{
	//_tap_camera.Reset(true);
}

void AndroidNDKGameController_Legacy::StartDrag()
{
//	ndk_helper::Vec2 v;
//	_drag_detector.GetPointer(v);
//
//	_navInfo.Pointer = true;
//	v.Value(_navInfo.PointerX, _navInfo.PointerY);

//	TransformPosition(v);
//	_tap_camera.BeginDrag(v);
}

void AndroidNDKGameController_Legacy::Drag()
{
//	ndk_helper::Vec2 v;
//	_drag_detector.GetPointer(v);
//
//    _navInfo.Pointer = true;
//    v.Value(_navInfo.PointerX, _navInfo.PointerY);

//	TransformPosition(v);
//	_tap_camera.Drag(v);
}

void AndroidNDKGameController_Legacy::EndDrag()
{
//    _navInfo.Pointer = false;

	//_tap_camera.EndDrag();
}

void AndroidNDKGameController_Legacy::StartPinch()
{
//	ndk_helper::Vec2 v1;
//	ndk_helper::Vec2 v2;
//	_pinch_detector.GetPointers(v1, v2);
//	TransformPosition(v1);
//	TransformPosition(v2);
//	_tap_camera.BeginPinch(v1, v2);
}

void AndroidNDKGameController_Legacy::Pinch()
{
//	ndk_helper::Vec2 v1;
//	ndk_helper::Vec2 v2;
//	_pinch_detector.GetPointers(v1, v2);
//	TransformPosition(v1);
//	TransformPosition(v2);
//	_tap_camera.Pinch(v1, v2);
}

void AndroidNDKGameController_Legacy::DebugLogEvent(AInputEvent *event)
{
	int32_t action = AMotionEvent_getAction(event);
    int32_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
	int32_t flags = action & AMOTION_EVENT_ACTION_MASK;
    int32_t count = AMotionEvent_getPointerCount(event);
    int32_t pointerID;
    float x;
    float y;

	switch (flags)
    {
        case AMOTION_EVENT_ACTION_DOWN:
            pointerID = AMotionEvent_getPointerId(event, index);
            x = AMotionEvent_getX(event, index);
            y = AMotionEvent_getY(event, index);

            _debug->Send(debugDebug, debugController) 	<< "DOWN: "
														<< "index = " << index << ", "
														<< "id = " << pointerID << ", "
														<< "at (" << x << ", " << y << "), "
														<< "count = " << count << ".\n";
            break;

        case AMOTION_EVENT_ACTION_UP:
            pointerID = AMotionEvent_getPointerId(event, index);
            x = AMotionEvent_getX(event, index);
            y = AMotionEvent_getY(event, index);

            _debug->Send(debugDebug, debugController) 	<< "UP: "
														<< "index = " << index << ", "
														<< "id = " << pointerID << ", "
														<< "at (" << x << ", " << y << "), "
														<< "count = " << count << ".\n";
            break;

        case AMOTION_EVENT_ACTION_MOVE:
            _debug->Send(debugDebug, debugController)	<< "MOVE: "
														<< "count = " << count << ".\n";
            break;

        case AMOTION_EVENT_ACTION_POINTER_DOWN:
            pointerID = AMotionEvent_getPointerId(event, index);
            x = AMotionEvent_getX(event, index);
            y = AMotionEvent_getY(event, index);

            _debug->Send(debugDebug, debugController) 	<< "POINTER_DOWN: "
														<< "index = " << index << ", "
														<< "id = " << pointerID << ", "
														<< "at (" << x << ", " << y << "), "
														<< "count = " << count << ".\n";
            break;

        case AMOTION_EVENT_ACTION_POINTER_UP:
            pointerID = AMotionEvent_getPointerId(event, index);
            x = AMotionEvent_getX(event, index);
            y = AMotionEvent_getY(event, index);

            _debug->Send(debugDebug, debugController) 	<< "POINTER_UP: "
														<< "index = " << index << ", "
														<< "id = " << pointerID << ", "
														<< "at (" << x << ", " << y << "), "
														<< "count = " << count << ".\n";
            break;

        default:
            _debug->Send(debugDebug, debugController) 	<< "action = " << flags << ", "
														<< "count = " << count << ".\n";
            break;
    }
}
