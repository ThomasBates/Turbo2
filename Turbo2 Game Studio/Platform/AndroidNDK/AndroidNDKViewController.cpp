
#include <pch.h>
#include <perfMonitor.h>

#include <AndroidNDKViewController.h>
#include <ITurboGroupView.h>

using namespace Turbo::Platform::AndroidNDK;

static AndroidNDKViewController *_controller;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

AndroidNDKViewController::AndroidNDKViewController(
		android_app* app,
		std::shared_ptr<ITurboDebug> debug,
		std::shared_ptr<ITurboView> view) :
		_android_app(app),
		_debug(debug)
{
	AddControls(view);

	_controller = this;
	_android_app->onInputEvent = AndroidNDKViewController::HandleInputEvents;

	InitializeSensors();
	InitializeControls();
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboViewController Methods ---------------------------------------------------------------------------------------

NavigationInfo* AndroidNDKViewController::GetNavigationInfo()
{
    ProcessEvents();

    double time = ndk_helper::PerfMonitor::GetCurrentTime();
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
	return _controller->HandleInputEvent(event);
}

int32_t AndroidNDKViewController::HandleInputEvent(AInputEvent *event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		return HandleMotionEvent(event);
	}

	return 0;
}

int32_t AndroidNDKViewController::HandleMotionEvent(AInputEvent *event)
{
	int32_t action = AMotionEvent_getAction(event);
	int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;
	size_t pointerIndex = (size_t )(action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
	size_t count = AMotionEvent_getPointerCount(event);
	int32_t pointerID = AMotionEvent_getPointerId(event, pointerIndex);
	float x = AMotionEvent_getX(event, pointerIndex);
	float y = AMotionEvent_getY(event, pointerIndex);

	switch (actionCode)
	{
		//	First touch down
		case AMOTION_EVENT_ACTION_DOWN:
			_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
					<< "DOWN: "
					<< "index = " << pointerIndex << ", "
					<< "id = " << pointerID << ", "
					<< "at (" << x << ", " << y << "), "
					<< "count = " << count << "\n";

			for (auto &controlView : _controlViews)
			{
				if (controlView->IsActive())
					continue;

				if (!controlView->Contains(x, y))
					continue;

				_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
						<< "  Activating Control: " << controlView->Name() << "\n";
				controlView->IsActive(true);
				controlView->CurrentPoint(x, y);

				_activeControlViews[pointerID] = controlView;
			}
			break;

		// 	Last touch up
		case AMOTION_EVENT_ACTION_UP:
			_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
				<< "UP: "
				<< "index = " << pointerIndex << ", "
				<< "id = " << pointerID << ", "
				<< "at (" << x << ", " << y << "), "
				<< "count = " << count << "\n";

			if (_activeControlViews.find(pointerID) != _activeControlViews.end())
			{
                auto controlView = _activeControlViews[pointerID];

				_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
														  << "  Deactivating Control: " << controlView->Name() << "\n";
                controlView->IsActive(false);

	    		_activeControlViews.erase(pointerID);
            }
			break;

		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
				<< "POINTER_DOWN: "
				<< "index = " << pointerIndex << ", "
				<< "id = " << pointerID << ", "
				<< "at (" << x << ", " << y << "), "
				<< "count = " << count << "\n";

			for (auto& controlView : _controlViews)
			{
				if (controlView->IsActive())
					continue;

                if (!controlView->Contains(x, y))
                    continue;

				_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
														  << "  Activating Control: " << controlView->Name() << "\n";
				controlView->IsActive(true);
				controlView->CurrentPoint(x, y);

				_activeControlViews[pointerID] = controlView;
			}
			break;

		case AMOTION_EVENT_ACTION_POINTER_UP:
			_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
				<< "POINTER_UP: "
				<< "index = " << pointerIndex << ", "
				<< "id = " << pointerID << ", "
				<< "at (" << x << ", " << y << "), "
				<< "count = " << count << "\n";

            if (_activeControlViews.find(pointerID) != _activeControlViews.end())
            {
                auto controlView = _activeControlViews[pointerID];

				_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
														  << "  Deactivating Control: " << controlView->Name() << "\n";
                controlView->IsActive(false);

                _activeControlViews.erase(pointerID);
            }
			break;

		case AMOTION_EVENT_ACTION_MOVE:
			for (int index = 0; index < count; index++)
			{
				pointerID = AMotionEvent_getPointerId(event, index);
				x = AMotionEvent_getX(event, index);
				y = AMotionEvent_getY(event, index);

				_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
					<< "MOVE: "
					<< "index = " << index << ", "
					<< "id = " << pointerID << ", "
					<< "at (" << x << ", " << y << "), "
					<< "count = " << count << "\n";

				if (_activeControlViews.find(pointerID) != _activeControlViews.end())
				{
					auto controlView = _activeControlViews[pointerID];

					_debug->Send(debugDebug, debugController) << "  Updating Control: " << controlView->Name() << "\n";
					controlView->CurrentPoint(x, y);
				}
			}
			break;

		case AMOTION_EVENT_ACTION_CANCEL:
			_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
				<< "CANCEL: "
				<< "index = " << pointerIndex << ", "
				<< "id = " << pointerID << ", "
				<< "count = " << count << "\n";

			for (auto& controlView : _controlViews)
			{
				_debug->Send(debugDebug, debugController) << "AndroidNDKViewController::HandleMotionEvent: "
					<< "  Deactivating Control: " << controlView->Name() << "\n";
				controlView->IsActive(false);
			}
			_activeControlViews.clear();
			break;

		default:
			break;
	}
	return 1;
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
    _debug->Send(debugVerbose, debugController) << "--> ProcessEvents()\n";

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
            _debug->Send(debugVerbose, debugController) << "<-- ProcessEvents(): false\n";
			return false;
		}
	}

    _debug->Send(debugVerbose, debugController) << "<-- ProcessEvents(): true\n";
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

    _controlViews.clear();
    _navInfo.Controls.clear();
}
