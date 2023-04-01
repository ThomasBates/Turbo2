
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <TurboDialogViewModel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::View;

class TurboDialogView : public TurboGroupView
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    TurboDialogView(
            std::shared_ptr<ITurboDebug> debug,
            std::string name,
            const std::shared_ptr<ITurboViewRendererAccess>& rendererAccess,
            std::shared_ptr<TurboDialogViewModel> viewModel);
    virtual ~TurboDialogView() {}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<TurboDialogViewModel> _viewModel;

    std::shared_ptr<ITurboSceneSprite> _background;

    //std::shared_ptr<ITurboView> _sceneView;
    std::shared_ptr<ITurboView> _okControlView;
    std::shared_ptr<ITurboView> _cancelControlView;

    std::shared_ptr<ITurboSceneText> _captionText;
    std::shared_ptr<ITurboSceneText> _dialogText;

    std::shared_ptr<ITurboSceneText> _okText;
    std::shared_ptr<ITurboSceneText> _cancelText;
};
