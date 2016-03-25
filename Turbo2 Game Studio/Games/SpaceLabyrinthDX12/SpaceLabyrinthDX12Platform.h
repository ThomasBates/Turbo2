
#pragma once

#include "IApplicationDX12.h"
#include "IImage.h"
#include "ISpaceLabyrinthPlatform.h"
#include "ISpaceLabyrinthRenderer.h"

#include "Content\Sample3DSceneRenderer.h"
#include "Content\SpaceLabyrinthDX12OriginalRenderer.h"

using namespace Application_DX12;

namespace SpaceLabyrinthDX12
{
	class SpaceLabyrinthDX12Platform : public ISpaceLabyrinthPlatform
	{
	private:
		IApplicationDX12 *_applicationDX12;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<ISpaceLabyrinthRenderer> _sceneRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		Camera			*_camera;

		LARGE_INTEGER	_frequency;
		LARGE_INTEGER	_startCount;
		LARGE_INTEGER	_lastCount;
		float			_time;
		float			_deltaTime;

		//  Local Support Methods
		virtual int LoadTextures();
		virtual int LoadTexture(int id, const char *fileName);
		virtual IImage *LoadImage(const char *fileName);

	public:
		//  Constructors and Destructors
		SpaceLabyrinthDX12Platform(IApplication *application);
		~SpaceLabyrinthDX12Platform();

		//  ISpaceLabyrinthFactory Methods
		virtual int		Initialize(Camera *camera);
		virtual void	SetPlatformResources(IPlatformResources *platformResources);
		//	virtual int		Reset();
		virtual int		Resize(int width, int height);
		virtual int		BeginDraw();
		virtual int		EndDraw();
		virtual int		BeginUpdate();
		virtual int		EndUpdate();
		virtual int		BeginRender();
		virtual int		EndRender();
		virtual int		Finalize();

		virtual BOOL	GetNavigationInfo(NavInfo *navInfo);
		virtual float	GetTime() { return _time; }
		virtual float	GetDeltaTime() { return _deltaTime; }

		virtual int		DrawCorner(MazeObject *corner);
		virtual int		DrawEdge(MazeObject *edge);
		virtual int		DrawWall(MazeObject *wall);

		//virtual BOOL DrawWall(float left, float top, float back, float right, float bottom, float front);
		//virtual int MoveCamera(float x, float y, float z);
		//virtual int RotateCamera(float x, float y, float z);
	};
}