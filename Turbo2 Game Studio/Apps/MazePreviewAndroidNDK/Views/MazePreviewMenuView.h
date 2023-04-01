
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <TurboDialogView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewMenuViewModel.h>
#include <Views/TurboSpinnerControlView.h>
#include <Views/TurboToggleControlView.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::View;

class MazePreviewMenuView : public TurboDialogView
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMenuView(
            const std::shared_ptr<ITurboDebug>& debug,
            std::string name,
            const std::shared_ptr<ITurboViewRendererAccess>& rendererAccess,
            const std::shared_ptr<MazePreviewMenuViewModel>& viewModel);
    virtual ~MazePreviewMenuView() {}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<MazePreviewMenuViewModel> _viewModel;

    std::shared_ptr<ITurboSceneText> _reverseControlsText;
    std::shared_ptr<ITurboSceneText> _soundEffectsText;
    std::shared_ptr<ITurboSceneText> _selectRoundText;
    std::shared_ptr<ITurboSceneText> _selectLevelText;
    std::shared_ptr<ITurboSceneText> _resetProgressText;

    std::shared_ptr<TurboToggleControlView> _reverseControlToggleView;
    std::shared_ptr<TurboToggleControlView> _soundEffectsToggleView;
    std::shared_ptr<TurboSpinnerControlView> _selectRoundSpinnerView;
    std::shared_ptr<TurboSpinnerControlView> _selectLevelSpinnerView;
    std::shared_ptr<TurboToggleControlView> _resetProgressToggleView;
};
