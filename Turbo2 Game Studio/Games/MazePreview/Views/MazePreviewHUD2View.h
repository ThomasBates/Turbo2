
#pragma once

#include <pch.h>

#include <TurboView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewHUD2ViewModel.h>

using namespace Turbo::View;

class MazePreviewHUD2View : public TurboView
{
public:
    //	Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD2View(
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
    std::shared_ptr<MazePreviewHUD2ViewModel> _viewModel;

    std::shared_ptr<ITurboSceneSprite> _test1;
    std::shared_ptr<ITurboSceneSprite> _test2;
};
