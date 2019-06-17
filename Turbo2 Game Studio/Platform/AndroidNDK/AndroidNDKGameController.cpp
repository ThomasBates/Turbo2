
#include <pch.h>

#include <sensorManager.h>

#include <AndroidNDKGameController.h>
#include <TurboSceneNavigationControl_Base.h>
#include <TurboSceneNavigationControl_Button.h>
#include <TurboSceneNavigationControl_Last.h>

using namespace Turbo::Platform::AndroidNDK;
using namespace Turbo::Scene;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKGameController::AndroidNDKGameController(
		android_app* app,
		std::shared_ptr<ITurboDebug> debug) :
		_android_app(app),
		_debug(debug)
{
	_android_app->controller = this;
	_android_app->onInputEvent = AndroidNDKGameController::HandleInputEvents;

	InitializeSensors();
	InitializeControls();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameController Methods ---------------------------------------------------------------------------------------

NavigationInfo* AndroidNDKGameController::GetNavigationInfo()
{
    ProcessEvents();

    double time = _performance_monitor.GetCurrentTime();
    _navInfo.DeltaTime = time - _navInfo.Time;
    _navInfo.Time = time;

    return &_navInfo;
}

void AndroidNDKGameController::Suspend()
{
    SuspendSensors();
}

void AndroidNDKGameController::Resume()
{
    ResumeSensors();
}

//  ITurboGameController Methods ---------------------------------------------------------------------------------------
//	Event Handler Methods ----------------------------------------------------------------------------------------------

/**
 * Process the next input event.
 */
int32_t AndroidNDKGameController::HandleInputEvents(android_app* app, AInputEvent* event)
{
	auto controller = (AndroidNDKGameController*)app->controller;

	return controller->HandleInputEvent(event);
}

int32_t AndroidNDKGameController::HandleInputEvent(AInputEvent *event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		DebugLogMotionEvent(event);
		return HandleMotionEvent(event);
	}

	return 0;
}

int32_t AndroidNDKGameController::HandleMotionEvent(AInputEvent *event)
{
	int32_t action = AMotionEvent_getAction(event);
	int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;
	size_t pointerIndex = (size_t )(action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	int32_t pointerID;
	float x;
	float y;
	std::shared_ptr<ITurboSceneNavigationControl> activeControl;

	switch (actionCode)
	{
		//	First touch down
		case AMOTION_EVENT_ACTION_DOWN:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);
			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);

			for (auto& control : _navInfo.Controls)
            {
                if (control->IsActive() || !control->Contains(x,y))
                    continue;

                control->IsActive(true);
                control->CurrentPoint(x, y);

                _activeControls[pointerID] = control;
                _activeIndexes[pointerID] = pointerIndex;
            }
			break;

		// 	Last touch up
		case AMOTION_EVENT_ACTION_UP:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);

			activeControl = _activeControls[pointerID];
			activeControl->IsActive(false);

			_activeControls.erase(pointerID);
			_activeIndexes.erase(pointerID);
			break;

		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);
			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);

			for (auto& control : _navInfo.Controls)
			{
				if (control->IsActive() || !control->Contains(x,y))
					continue;

				control->IsActive(true);
				control->CurrentPoint(x, y);

				_activeControls[pointerID] = control;
				_activeIndexes[pointerID] = pointerIndex;
			}

			UpdatePointerIndexes(event);
			break;

		case AMOTION_EVENT_ACTION_POINTER_UP:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);

			activeControl = _activeControls[pointerID];
			activeControl->IsActive(false);

			_activeControls.erase(pointerID);
			_activeIndexes.erase(pointerID);

			UpdatePointerIndexes(event);
			break;

	    case AMOTION_EVENT_ACTION_CANCEL:
			for (auto& control : _navInfo.Controls)
			{
				control->IsActive(false);
			}
			_activeControls.clear();
			_activeIndexes.clear();
	        break;

		case AMOTION_EVENT_ACTION_MOVE:
			for (auto& element : _activeControls)
			{
				pointerID = element.first;
				activeControl = element.second;
				pointerIndex = _activeIndexes[pointerID];

				x = AMotionEvent_getX(event, pointerIndex);
				y = AMotionEvent_getY(event, pointerIndex);

				activeControl->CurrentPoint(x, y);
			}
			break;

		default:
			break;
	}
	return 1;
}

void AndroidNDKGameController::UpdatePointerIndexes(const AInputEvent *event)
{
	_activeIndexes.clear();
	for (auto& element : _activeControls)
	{
		int32_t pointerID = element.first;
		int32_t pointerIndex = GetPointerIndex(event, pointerID);
		if (pointerIndex >= 0)
		{
			_activeIndexes[pointerID] = (size_t)pointerIndex;
		}
	}
}

int32_t AndroidNDKGameController::GetPointerIndex(const AInputEvent *event, int32_t id)
{
	size_t count = AMotionEvent_getPointerCount(event);
	for (size_t index = 0; index < count; ++index)
	{
		if (id == AMotionEvent_getPointerId(event, index))
			return (int32_t)index;
	}
	return -1;
}

void AndroidNDKGameController::DebugLogMotionEvent(AInputEvent *event)
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
void AndroidNDKGameController::InitializeSensors()
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

void AndroidNDKGameController::InitializeControls()
{
	_navInfo.Controls.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Last(_debug, TurboGameControlType::Look,   0.0f, 1080.0f,    0.0f, 1440.0f,  0.1f)));
	_navInfo.Controls.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Last(_debug, TurboGameControlType::Look, 540.0f, 1080.0f, 1440.0f, 1920.0f, -1.0f)));
    _navInfo.Controls.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationControl_Button(TurboGameControlType::Move, 0.0f,  540.0f, 1440.0f, 1920.0f)));
}

bool AndroidNDKGameController::ProcessEvents()
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

void AndroidNDKGameController::ProcessSensors(int32_t id)
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

void AndroidNDKGameController::ResumeSensors()
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

void AndroidNDKGameController::SuspendSensors()
{
	_isRunning = false;

    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if (_accelerometer_sensor != NULL)
    {
        ASensorEventQueue_disableSensor(_sensor_event_queue, _accelerometer_sensor);
    }
}
