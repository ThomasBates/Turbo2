
#include <pch.h>

#include <sensorManager.h>

#include <AndroidNDKGameController_DualPads.h>

using namespace Turbo::Platform::AndroidNDK;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKGameController_DualPads::AndroidNDKGameController_DualPads(
		android_app* app,
		std::shared_ptr<ITurboDebug> debug) :
		_android_app(app),
		_debug(debug)
{
	_android_app->controller = this;
	_android_app->onInputEvent = AndroidNDKGameController_DualPads::HandleInputEvents;

	InitSensors();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameController Methods ---------------------------------------------------------------------------------------

NavigationInfo* AndroidNDKGameController_DualPads::GetNavigationInfo()
{
    ProcessEvents();

    double time = _performance_monitor.GetCurrentTime();
    _navInfo.DeltaTime = time - _navInfo.Time;
    _navInfo.Time = time;

    return &_navInfo;
}

void AndroidNDKGameController_DualPads::Suspend()
{
    SuspendSensors();
}

void AndroidNDKGameController_DualPads::Resume()
{
    ResumeSensors();
}

//  ITurboGameController Methods ---------------------------------------------------------------------------------------
//	Event Handler Methods ----------------------------------------------------------------------------------------------

/**
 * Process the next input event.
 */
int32_t AndroidNDKGameController_DualPads::HandleInputEvents(android_app* app, AInputEvent* event)
{
	auto controller = (AndroidNDKGameController_DualPads*)app->controller;

	return controller->HandleInputEvent(event);
}

int32_t AndroidNDKGameController_DualPads::HandleInputEvent(AInputEvent *event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		DebugLogMotionEvent(event);
		return HandleMotionEvent(event);
	}

	return 0;
}

int32_t AndroidNDKGameController_DualPads::HandleMotionEvent(AInputEvent *event)
{
	int32_t action = AMotionEvent_getAction(event);
	int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;
	size_t pointerIndex = (size_t )(action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	int32_t pointerID;
	float x;
	float y;

	switch (actionCode)
	{
		//	First touch down
		case AMOTION_EVENT_ACTION_DOWN:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);
			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);

			if (y < 540*3)
			{
				if (!_screenPointer.IsActive)
				{
					_screenPointer.IsActive = true;
					_screenPointer.ID = pointerID;
					_screenPointer.Index = pointerIndex;
					_navInfo.Pointer = true;
					_navInfo.PointerX = x;
					_navInfo.PointerY = y;
				}
			}
			else if (x < 540)
			{
				if (!_movePointer.IsActive)
				{
					_movePointer.IsActive = true;
					_movePointer.ID = pointerID;
					_movePointer.Index = pointerIndex;
					_movePointer.CenterX = x;
					_movePointer.CenterY = y;
					_navInfo.MovePadActive = true;
                    _navInfo.MovePadX = 0.0f;
                    _navInfo.MovePadY = 0.0f;
				}
			}
			else if (x >= 540)
			{
				if (!_lookPointer.IsActive)
				{
					_lookPointer.IsActive = true;
					_lookPointer.ID = pointerID;
					_lookPointer.Index = pointerIndex;
					_lookPointer.CenterX = x;
					_lookPointer.CenterY = y;
					_navInfo.LookPadActive = true;
                    _navInfo.LookPadX = 0.0f;
                    _navInfo.LookPadY = 0.0f;
				}
			}
			break;

		// 	Last touch up
		case AMOTION_EVENT_ACTION_UP:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);

			if (_screenPointer.IsActive && (pointerID == _screenPointer.ID))
			{
				_screenPointer.IsActive = false;
				_navInfo.Pointer = false;
			}
			if (_movePointer.IsActive && (pointerID == _movePointer.ID))
			{
				_movePointer.IsActive = false;
				_navInfo.MovePadActive = false;
			}
			if (_lookPointer.IsActive && (pointerID == _lookPointer.ID))
			{
				_lookPointer.IsActive = false;
				_navInfo.LookPadActive = false;
			}
			break;

		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);
			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);

            if (y < 540*3)
            {
                if (!_screenPointer.IsActive)
                {
                    _screenPointer.IsActive = true;
                    _screenPointer.ID = pointerID;
                    _screenPointer.Index = pointerIndex;
                    _navInfo.Pointer = true;
                    _navInfo.PointerX = x;
                    _navInfo.PointerY = y;
                }
            }
            else if (x < 540)
            {
                if (!_movePointer.IsActive)
                {
                    _movePointer.IsActive = true;
                    _movePointer.ID = pointerID;
                    _movePointer.Index = pointerIndex;
                    _movePointer.CenterX = x;
                    _movePointer.CenterY = y;
                    _navInfo.MovePadActive = true;
                    _navInfo.MovePadX = 0.0f;
                    _navInfo.MovePadY = 0.0f;
                }
            }
            else if (x >= 540)
            {
                if (!_lookPointer.IsActive)
                {
                    _lookPointer.IsActive = true;
                    _lookPointer.ID = pointerID;
                    _lookPointer.Index = pointerIndex;
                    _lookPointer.CenterX = x;
                    _lookPointer.CenterY = y;
                    _navInfo.LookPadActive = true;
                    _navInfo.LookPadX = 0.0f;
                    _navInfo.LookPadY = 0.0f;
                }
            }

			UpdatePointerIndex(event, &_screenPointer);
			UpdatePointerIndex(event, &_movePointer);
			UpdatePointerIndex(event, &_lookPointer);
			break;

		case AMOTION_EVENT_ACTION_POINTER_UP:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);

            if (_screenPointer.IsActive && (pointerID == _screenPointer.ID))
            {
                _screenPointer.IsActive = false;
                _navInfo.Pointer = false;
            }
            if (_movePointer.IsActive && (pointerID == _movePointer.ID))
            {
                _movePointer.IsActive = false;
                _navInfo.MovePadActive = false;
            }
            if (_lookPointer.IsActive && (pointerID == _lookPointer.ID))
            {
                _lookPointer.IsActive = false;
                _navInfo.LookPadActive = false;
            }

			UpdatePointerIndex(event, &_screenPointer);
			UpdatePointerIndex(event, &_movePointer);
			UpdatePointerIndex(event, &_lookPointer);
			break;

	    case AMOTION_EVENT_ACTION_CANCEL:
            _screenPointer.IsActive = false;
            _navInfo.Pointer = false;

            _movePointer.IsActive = false;
            _navInfo.MovePadActive = false;

            _lookPointer.IsActive = false;
            _navInfo.LookPadActive = false;
	        break;

		case AMOTION_EVENT_ACTION_MOVE:
			if (_screenPointer.IsActive)
			{
				pointerIndex = _screenPointer.Index;
				_navInfo.PointerX = AMotionEvent_getX(event, pointerIndex);
				_navInfo.PointerY = AMotionEvent_getY(event, pointerIndex);
			}
			if (_movePointer.IsActive)
			{
				pointerIndex = _movePointer.Index;
				x = AMotionEvent_getX(event, pointerIndex);
				y = AMotionEvent_getY(event, pointerIndex);
				_navInfo.MovePadX = fmin(fmax((x - _movePointer.CenterX) / 200.0f, -1.0f), 1.0f);
				_navInfo.MovePadY = fmin(fmax((y - _movePointer.CenterY) / 200.0f, -1.0f), 1.0f);
			}
			if (_lookPointer.IsActive)
			{
				pointerIndex = _lookPointer.Index;
				x = AMotionEvent_getX(event, pointerIndex);
				y = AMotionEvent_getY(event, pointerIndex);
				_navInfo.LookPadX = fmin(fmax((x - _lookPointer.CenterX) / 200.0f, -1.0f), 1.0f);
				_navInfo.LookPadY = fmin(fmax((y - _lookPointer.CenterY) / 200.0f, -1.0f), 1.0f);
			}
			break;

		default:
			break;
	}
	return 1;
}

void AndroidNDKGameController_DualPads::UpdatePointerIndex(const AInputEvent *event,
														   PointerInfo *pointer)
{
	if (pointer->IsActive)
	{
		int32_t pointerIndex = GetPointerIndex(event, pointer->ID);
		if (pointerIndex < 0)
		{
			//	throw
			return;
		}
		pointer->Index = (size_t)pointerIndex;
	}
}

int32_t AndroidNDKGameController_DualPads::GetPointerIndex(const AInputEvent *event, int32_t id)
{
	size_t count = AMotionEvent_getPointerCount(event);
	for (size_t index = 0; index < count; ++index)
	{
		if (id == AMotionEvent_getPointerId(event, index))
			return (int32_t)index;
	}
	return -1;
}

void AndroidNDKGameController_DualPads::DebugLogMotionEvent(AInputEvent *event)
{
	int32_t action = AMotionEvent_getAction(event);
	int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;
	size_t index = (size_t)(action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
	size_t count = AMotionEvent_getPointerCount(event);
	int32_t pointerID;
	float x;
	float y;

	switch (actionCode)
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
			_debug->Send(debugDebug, debugController) 	<< "action = " << actionCode << ", "
														 << "count = " << count << ".\n";
			break;
	}
}

//	Event Handler Methods ----------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------
// Sensor handlers
//-------------------------------------------------------------------------
void AndroidNDKGameController_DualPads::InitSensors()
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

bool AndroidNDKGameController_DualPads::ProcessEvents()
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

void AndroidNDKGameController_DualPads::ProcessSensors(int32_t id)
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

void AndroidNDKGameController_DualPads::ResumeSensors()
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

void AndroidNDKGameController_DualPads::SuspendSensors()
{
	_isRunning = false;

    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if (_accelerometer_sensor != NULL)
    {
        ASensorEventQueue_disableSensor(_sensor_event_queue, _accelerometer_sensor);
    }
}

void AndroidNDKGameController_DualPads::DoubleTap()
{
	//_tap_camera.Reset(true);
}

void AndroidNDKGameController_DualPads::StartDrag()
{
	ndk_helper::Vec2 v;
	_drag_detector.GetPointer(v);

	_navInfo.Pointer = true;
	v.Value(_navInfo.PointerX, _navInfo.PointerY);

//	TransformPosition(v);
//	_tap_camera.BeginDrag(v);
}

void AndroidNDKGameController_DualPads::Drag()
{
	ndk_helper::Vec2 v;
	_drag_detector.GetPointer(v);

    _navInfo.Pointer = true;
    v.Value(_navInfo.PointerX, _navInfo.PointerY);

//	TransformPosition(v);
//	_tap_camera.Drag(v);
}

void AndroidNDKGameController_DualPads::EndDrag()
{
    _navInfo.Pointer = false;

	//_tap_camera.EndDrag();
}

void AndroidNDKGameController_DualPads::StartPinch()
{
//	ndk_helper::Vec2 v1;
//	ndk_helper::Vec2 v2;
//	_pinch_detector.GetPointers(v1, v2);
//	TransformPosition(v1);
//	TransformPosition(v2);
//	_tap_camera.BeginPinch(v1, v2);
}

void AndroidNDKGameController_DualPads::Pinch()
{
//	ndk_helper::Vec2 v1;
//	ndk_helper::Vec2 v2;
//	_pinch_detector.GetPointers(v1, v2);
//	TransformPosition(v1);
//	TransformPosition(v2);
//	_tap_camera.Pinch(v1, v2);
}
