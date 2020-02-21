
#pragma once

#include <pch.h>

#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewDirectionControlViewModel.h>

using namespace Turbo::View;

class MazePreviewDirectionControlView : public TurboGroupView
{
public:
    //	Constructors and Destructors ---------------------------------------------------------------------------
    MazePreviewDirectionControlView(
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewDirectionControlViewModel> viewModel);
    virtual ~MazePreviewDirectionControlView(){}

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<ITurboViewRendererAccess> _rendererAccess;
    std::shared_ptr<MazePreviewDirectionControlViewModel> _viewModel;

    std::shared_ptr<ITurboView> _directionControlView;
};
