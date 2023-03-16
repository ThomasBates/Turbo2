
#pragma once

#include <pch.h>

#include <TurboView.h>
#include <ITurboDebug.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewHUD1ViewModel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::View;

class MazePreviewHUD1View : public TurboView
{
public:
    //	Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewHUD1View(
            std::shared_ptr<ITurboDebug> debug,
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
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<MazePreviewHUD1ViewModel> _viewModel;

    std::shared_ptr<ITurboSceneText> _roundText;
    std::shared_ptr<ITurboSceneText> _titleText;
    std::shared_ptr<ITurboSceneText> _keysText;
    std::shared_ptr<ITurboSceneText> _hazardText;
};
