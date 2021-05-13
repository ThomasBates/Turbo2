
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewInfoViewModel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::View;

class MazePreviewInfoView : public TurboGroupView
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewInfoView(
            std::shared_ptr<ITurboDebug> debug,
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewInfoViewModel> viewModel);
    virtual ~MazePreviewInfoView() {}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<MazePreviewInfoViewModel> _viewModel;

    std::shared_ptr<ITurboView> _sceneView;
    std::shared_ptr<ITurboView> _closeControlView;
    std::shared_ptr<ITurboSceneText> _infoText;
};
