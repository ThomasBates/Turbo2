
#pragma once

#include "pch.h"
//#include <XInput.h>
#include <Common\StepTimer.h>
#include "INavigationController.h"

//using namespace Windows::Devices::Input;
//using namespace Windows::UI::Core;

using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
//using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
//using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;



namespace Application_DX12
{
	ref class TurboApplicationDX12NavigationControl 
	{
	internal:
		//  Constructors and Destructors  ------------------------------------------------------------------------------
		TurboApplicationDX12NavigationControl();
		//virtual ~TurboApplicationDX12NavigationControl();

		void SetTimeStampForFrame();
		NavigationInfo GetNavigationInfo();

	protected:
		//  Event Handler Methods  -------------------------------------------------------------------------------------
		void OnPointerPressed(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerMoved(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerReleased(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnPointerExited(_In_ CoreWindow^ sender, _In_ PointerEventArgs^ args);
		void OnKeyDown(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnKeyUp(_In_ CoreWindow^ sender, _In_ KeyEventArgs^ args);
		void OnMouseMoved(_In_ MouseDevice^ mouseDevice, _In_ MouseEventArgs^ args);

		//  Local Support Methods  -------------------------------------------------------------------------------------
		void ResetState();

	private:
		//CoreWindow^ _window;
		NavigationInfo _navInfo;

		DX::StepTimer _timer;

		DirectX::XMFLOAT3           m_velocity;             // How far we move in this frame.
		float                       m_pitch;
		float                       m_yaw;                  // Orientation euler angles in radians.

															// Properties of the Move control.
		DirectX::XMFLOAT2           m_moveUpperLeft;        // Bounding box where this control will activate.
		DirectX::XMFLOAT2           m_moveLowerRight;
		bool                        m_moveInUse;            // The move control is in use.
		uint32                      m_movePointerID;        // Id of the pointer in this control.
		DirectX::XMFLOAT2           m_moveFirstDown;        // Point where initial contact occurred.
		DirectX::XMFLOAT2           m_movePointerPosition;  // Point where the move pointer is currently located.
		DirectX::XMFLOAT3           m_moveCommand;          // The net command from the move control.

															// Properties of the Look control.
		bool                        m_lookInUse;            // The look control is in use.
		uint32                      m_lookPointerID;        // Id of the pointer in this control.
		DirectX::XMFLOAT2           m_lookLastPoint;        // Last point (from last frame).
		DirectX::XMFLOAT2           m_lookLastDelta;        // The delta used for smoothing between frames.

															// Properties of the Fire control.
		bool                        m_autoFire;
		bool                        m_firePressed;
		DirectX::XMFLOAT2           m_fireUpperLeft;        // Bounding box where this control will activate.
		DirectX::XMFLOAT2           m_fireLowerRight;
		bool                        m_fireInUse;            // The fire control in in use.
		UINT32                      m_firePointerID;        // Id of the pointer in this control.
		DirectX::XMFLOAT2           m_fireLastPoint;        // Last fire position.

															// Properties of the Mouse control.  This is a combination of Look and Fire.
		bool                        m_mouseInUse;
		uint32                      m_mousePointerID;
		DirectX::XMFLOAT2           m_mouseLastPoint;
		bool                        m_mouseLeftInUse;
		bool                        m_mouseRightInUse;

		bool                        m_buttonInUse;
		uint32                      m_buttonPointerID;
		DirectX::XMFLOAT2           m_buttonUpperLeft;
		DirectX::XMFLOAT2           m_buttonLowerRight;
		bool                        m_buttonPressed;
		bool                        m_pausePressed;

		// Input states for Keyboard.
		bool                        m_forward;
		bool                        m_back;                    // States for movement.
		bool                        m_left;
		bool                        m_right;
		bool                        m_up;
		bool                        m_down;
		bool                        m_pause;

	};
}