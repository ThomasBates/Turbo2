
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewRootViewModel.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::View;

class MazePreviewRootView : public TurboGroupView
{
public:
    //  Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewRootView(
            std::shared_ptr<ITurboDebug> debug,
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewRootViewModel> viewModel);
    virtual ~MazePreviewRootView() {}

    //	ITurboView Methods ---------------------------------------------------------------------------------------------
    virtual void Load();
    virtual void Render();

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboDebug> _debug;
    std::shared_ptr<MazePreviewRootViewModel> _viewModel;

    std::shared_ptr<ITurboView> _mainView;
    std::shared_ptr<ITurboView> _menuView;
    std::shared_ptr<ITurboView> _infoView;
};
