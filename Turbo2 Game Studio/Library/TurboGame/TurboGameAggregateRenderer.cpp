
#include <pch.h>

#include <chrono>
#include <thread>
#include <cstring>

#include <TurboGameAggregateRenderer.h>

#include <ITurboGroupView.h>
#include <ITurboCanvas.h>
#include <ITurboImage.h>

#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>
#include <TurboCoreHelpers.h>
#include <TurboSceneMesh.h>
#include <TurboSceneTexture.h>
#include <TurboSceneArialFont.h>
#include "TurboGameAggregateRenderer.h"


using namespace Turbo::Core;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::OpenGLES;

//  Constructors & Destructors  ----------------------------------------------------------------------------------------

TurboGameAggregateRenderer::TurboGameAggregateRenderer(std::initializer_list<std::shared_ptr<ITurboGameRenderer>> initializer_list)
{
    for (auto& renderer : initializer_list)
    {
        _renderers.push_back(renderer);

        auto rendererAccess = renderer->RendererAccess();
        if (rendererAccess != nullptr)
            _rendererAccess.push_back(rendererAccess);
    }
}

//	ITurboGameRenderer Methods -----------------------------------------------------------------------------------------

void TurboGameAggregateRenderer::InitializeLoading()
{
    for (auto& renderer : _renderers)
        renderer->InitializeLoading();
}

void TurboGameAggregateRenderer::FinalizeLoading()
{
    for (auto& renderer : _renderers)
        renderer->FinalizeLoading();
}

void TurboGameAggregateRenderer::InitializeRendering()
{
    for (auto& renderer : _renderers)
        renderer->InitializeRendering();
}


void TurboGameAggregateRenderer::FinalizeRendering()
{
    for (auto& renderer : _renderers)
        renderer->FinalizeRendering();
}

void TurboGameAggregateRenderer::Reset()
{
    for (auto& renderer : _renderers)
        renderer->Reset();
}

//	ITurboViewRendererAccess Methods ----------------------------------------------------------------------------------

void TurboGameAggregateRenderer::LoadScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->LoadScene(scene);
}

void TurboGameAggregateRenderer::LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    if (sceneSprite == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->LoadSceneSprite(sceneSprite);
}

void TurboGameAggregateRenderer::LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText)
{
    if (sceneText == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->LoadSceneText(sceneText);
}

void TurboGameAggregateRenderer::LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound)
{
    if (sceneSound == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->LoadSceneSound(sceneSound);
}

void TurboGameAggregateRenderer::LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground)
{
    if (sceneBackground == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->LoadSceneBackground(sceneBackground);
}

void TurboGameAggregateRenderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->RenderScene(scene);
}

void TurboGameAggregateRenderer::RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    if (sceneSprite == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->RenderSceneSprite(sceneSprite);
}

void TurboGameAggregateRenderer::RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText)
{
    if (sceneText == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->RenderSceneText(sceneText);
}

void TurboGameAggregateRenderer::RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSound)
{
    if (sceneSound == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->RenderSceneSound(sceneSound);
}

void TurboGameAggregateRenderer::RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground)
{
    if (sceneBackground == nullptr)
        return;

    for (auto& rendererAccess : _rendererAccess)
        rendererAccess->RenderSceneBackground(sceneBackground);
}
