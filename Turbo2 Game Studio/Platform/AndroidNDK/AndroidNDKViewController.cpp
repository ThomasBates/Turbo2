
#include <pch.h>

#include <AndroidNDKViewController.h>
#include <ITurboGroupView.h>

using namespace Turbo::Platform::AndroidNDK;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKViewController::AndroidNDKViewController(
		android_app* app,
		std::shared_ptr<ITurboDebug> debug,
		std::shared_ptr<ITurboView> view) :
		_android_app(app),
		_debug(debug)
{
	AddControls(view);

	_android_app->controller = this;
	_android_app->onInputEvent = AndroidNDKViewController::HandleInputEvents;

	InitializeSensors();
	InitializeControls();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboViewController Methods ---------------------------------------------------------------------------------------

NavigationInfo* AndroidNDKViewController::GetNavigationInfo()
{
    ProcessEvents();

    double time = _performance_monitor.GetCurrentTime();
    _navInfo.DeltaTime = time - _navInfo.Time;
    _navInfo.Time = time;

    return &_navInfo;
}

void AndroidNDKViewController::Suspend()
{
    SuspendSensors();
}

void AndroidNDKViewController::Resume()
{
    ResumeSensors();
}

//  ITurboViewController Methods ---------------------------------------------------------------------------------------
//	Event Handler Methods ----------------------------------------------------------------------------------------------

/**
 * Process the next input event.
 */
int32_t AndroidNDKViewController::HandleInputEvents(android_app* app, AInputEvent* event)
{
	auto controller = (AndroidNDKViewController*)app->controller;

	return controller->HandleInputEvent(event);
}

int32_t AndroidNDKViewController::HandleInputEvent(AInputEvent *event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		DebugLogMotionEvent(event);
		return HandleMotionEvent(event);
	}

	return 0;
}

int32_t AndroidNDKViewController::HandleMotionEvent(AInputEvent *event)
{
	int32_t action = AMotionEvent_getAction(event);
	int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;
	size_t pointerIndex = (size_t )(action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	int32_t pointerID;
	float x;
	float y;
	std::shared_ptr<ITurboControlView> activeControl;

	switch (actionCode)
	{
		//	First touch down
		case AMOTION_EVENT_ACTION_DOWN:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);
			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);

			for (auto& control : _controlViews)
            {
				if (control->IsActive())
				{
					_debug->Send(debugDebug, debugController) << "Control \"" << control->Name() << "\" is already active.\n";
					continue;
				}

				if (!control->Contains(x, y))
				{
					_debug->Send(debugDebug, debugController) << "Control \"" << control->Name() << "\" was not hit.\n";
					continue;
				}

				_debug->Send(debugDebug, debugController) << "Control \"" << control->Name() << "\" was hit.\n";
				control->IsActive(true);
                control->CurrentPoint(x, y);

                _activeControlViews[pointerID] = control;
                _activeIndexes[pointerID] = pointerIndex;
            }
			break;

		// 	Last touch up
		case AMOTION_EVENT_ACTION_UP:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);

			if (_activeControlViews.find(pointerID) != _activeControlViews.end())
			{
                activeControl = _activeControlViews[pointerID];
                activeControl->IsActive(false);

	    		_activeControlViews.erase(pointerID);
    			_activeIndexes.erase(pointerID);
            }
			break;

		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);
			x = AMotionEvent_getX(event, pointerIndex);
			y = AMotionEvent_getY(event, pointerIndex);

			for (auto& control : _controlViews)
			{
				if (control->IsActive())
					continue;

                if (!control->Contains(x, y))
                    continue;

				control->IsActive(true);
				control->CurrentPoint(x, y);

				_activeControlViews[pointerID] = control;
				_activeIndexes[pointerID] = pointerIndex;
			}

			UpdatePointerIndexes(event);
			break;

		case AMOTION_EVENT_ACTION_POINTER_UP:
			pointerID = AMotionEvent_getPointerId(event, pointerIndex);

            if (_activeControlViews.find(pointerID) != _activeControlViews.end())
            {
                activeControl = _activeControlViews[pointerID];
                activeControl->IsActive(false);

                _activeControlViews.erase(pointerID);
                _activeIndexes.erase(pointerID);
            }

			UpdatePointerIndexes(event);
			break;

	    case AMOTION_EVENT_ACTION_CANCEL:
			for (auto& control : _controlViews)
			{
				control->IsActive(false);
			}
			_activeControlViews.clear();
			_activeIndexes.clear();
	        break;

		case AMOTION_EVENT_ACTION_MOVE:
			for (auto& element : _activeControlViews)
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

void AndroidNDKViewController::UpdatePointerIndexes(const AInputEvent *event)
{
	_activeIndexes.clear();
	for (auto& element : _activeControlViews)
	{
		int32_t pointerID = element.first;
		int32_t pointerIndex = GetPointerIndex(event, pointerID);
		if (pointerIndex >= 0)
		{
			_activeIndexes[pointerID] = (size_t)pointerIndex;
		}
	}
}

int32_t AndroidNDKViewController::GetPointerIndex(const AInputEvent *event, int32_t id)
{
	size_t count = AMotionEvent_getPointerCount(event);
	for (size_t index = 0; index < count; ++index)
	{
		if (id == AMotionEvent_getPointerId(event, index))
			return (int32_t)index;
	}
	return -1;
}

void AndroidNDKViewController::DebugLogMotionEvent(AInputEvent *event)
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
void AndroidNDKViewController::InitializeSensors()
{
}

void AndroidNDKViewController::InitializeControls()
{
//	_controlViews.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(_debug, TurboViewControlType::Look,   0.0f, 1080.0f,    0.0f, 1440.0f,  0.1f)));
//	_controlViews.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationPadControl(_debug, TurboViewControlType::Look, 540.0f, 1080.0f, 1440.0f, 1920.0f, -1.0f)));
//    _controlViews.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationButtonControl(      TurboViewControlType::Move,   0.0f,  540.0f, 1440.0f, 1920.0f)));
//    _controlViews.push_back(std::shared_ptr<ITurboSceneNavigationControl>(new TurboSceneNavigationSetControl(         TurboViewControlType::Move)));
}

bool AndroidNDKViewController::ProcessEvents()
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
			//_navInfo.Terminate = true;
			return false;
		}
	}

	return true;
}

void AndroidNDKViewController::ProcessSensors(int32_t id)
{
}

void AndroidNDKViewController::ResumeSensors()
{
    _isRunning = true;
}

void AndroidNDKViewController::SuspendSensors()
{
	_isRunning = false;
}

void AndroidNDKViewController::AddControls(std::shared_ptr<ITurboView> control)
{
	auto touch = std::dynamic_pointer_cast<ITurboControlView>(control);
	if (touch != nullptr)
	{
		AddControl(touch);
		return;
	}

	auto group = std::dynamic_pointer_cast<ITurboGroupView>(control);
	if (group != nullptr)
	{
		for (auto& child : group->Views())
			AddControls(child);
	}
}

void AndroidNDKViewController::AddControl(std::shared_ptr<ITurboControlView> controlView)
{
    _controlViews.push_back(controlView);
    auto viewModel = controlView->ControlViewModel();
    auto control = viewModel->Control();
    _navInfo.Controls.push_back(control);
}

void AndroidNDKViewController::ClearControls()
{
    _activeControlViews.clear();
    _activeIndexes.clear();

    _controlViews.clear();
    _navInfo.Controls.clear();
}
