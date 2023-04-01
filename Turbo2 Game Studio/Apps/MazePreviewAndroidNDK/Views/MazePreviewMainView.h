
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewMainViewModel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::View;

class MazePreviewMainView : public TurboGroupView
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMainView(
            std::shared_ptr<ITurboDebug> debug,
            std::string name,
            const std::shared_ptr<ITurboViewRendererAccess>& rendererAccess,
            std::shared_ptr<MazePreviewMainViewModel> viewModel);
    virtual ~MazePreviewMainView() {}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<MazePreviewMainViewModel> _viewModel;

    std::shared_ptr<ITurboView> _sceneView;
    std::shared_ptr<ITurboView> _hud1View;
    std::shared_ptr<ITurboView> _hud2View;
    std::shared_ptr<ITurboView> _mainControlView;
    std::shared_ptr<ITurboView> _motionControlView;
    std::shared_ptr<ITurboView> _strafeControlView;
    std::shared_ptr<ITurboView> _directionControlView;

    std::shared_ptr<ITurboSceneSprite> _watermark;
//    std::shared_ptr<ITurboSceneSound> _background;
};
