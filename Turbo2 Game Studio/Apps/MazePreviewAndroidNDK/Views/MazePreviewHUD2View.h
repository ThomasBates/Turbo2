
#pragma once

#include <pch.h>

#include <TurboGroupView.h>
#include <ITurboDebug.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewHUD2ViewModel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::View;

class MazePreviewHUD2View : public TurboGroupView
{
public:
    //	Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD2View(
            std::shared_ptr<ITurboDebug> debug,
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewHUD2ViewModel> viewModel);
    virtual ~MazePreviewHUD2View(){}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<MazePreviewHUD2ViewModel> _viewModel;

    std::shared_ptr<ITurboView> _menuControlView;
    std::shared_ptr<ITurboView> _infoControlView;
    std::shared_ptr<ITurboSceneSprite> _watermark;
    std::shared_ptr<ITurboSceneText> _signageText;
};
