
#pragma once

#include <pch.h>

#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewMainControlViewModel.h>

using namespace Turbo::View;

class MazePreviewMainControlView : public TurboGroupView
{
public:
    //	Constructors and Destructors ---------------------------------------------------------------------------
    MazePreviewMainControlView(
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewMainControlViewModel> viewModel);
    virtual ~MazePreviewMainControlView(){}

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboViewRendererAccess> _rendererAccess;
    std::shared_ptr<MazePreviewMainControlViewModel> _viewModel;

    std::shared_ptr<ITurboView> _mainControlView;
};
