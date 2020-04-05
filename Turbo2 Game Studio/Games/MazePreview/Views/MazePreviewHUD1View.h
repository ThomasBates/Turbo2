
#pragma once

#include <pch.h>

#include <TurboView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewHUD1ViewModel.h>

using namespace Turbo::View;

class MazePreviewHUD1View : public TurboView
{
public:
    //	Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD1View(
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewHUD1ViewModel> viewModel);
    virtual ~MazePreviewHUD1View(){}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<MazePreviewHUD1ViewModel> _viewModel;

    std::shared_ptr<ITurboSceneSprite> _test1;
    std::shared_ptr<ITurboSceneSprite> _test2;
};
