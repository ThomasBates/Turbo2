
#pragma once

#include <pch.h>

#include <TurboGroupView.h>
#include <ITurboViewRendererAccess.h>
#include <MazePreviewMotionControlViewModel.h>

using namespace Turbo::View;

class MazePreviewMotionControlView : public TurboGroupView
{
public:
    //	Constructors and Destructors -----------------------------------------------------------------------------------
    MazePreviewMotionControlView(
            std::string name,
            std::shared_ptr<ITurboViewRendererAccess> rendererAccess,
            std::shared_ptr<MazePreviewMotionControlViewModel> viewModel);
    virtual ~MazePreviewMotionControlView(){}

protected:
    virtual void UpdateLayout(TurboVector2D position, TurboVector2D size);

private:
    std::shared_ptr<MazePreviewMotionControlViewModel> _viewModel;

    std::shared_ptr<ITurboView> _forwardControlView;
    std::shared_ptr<ITurboView> _backwardControlView;
};
