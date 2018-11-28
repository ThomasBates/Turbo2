//  ============================================================================
//  TeapotGame.cpp
//  ============================================================================

#include <pch.h>

#include <TeapotGame.h>

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TeapotGame::TeapotGame()
{
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------

std::shared_ptr<ITurboGameState> TeapotGame::GameState()
{
	return nullptr;
}

void TeapotGame::GameState(std::shared_ptr<ITurboGameState> gameState)
{
}

//  ITurboGameLevel Properties -----------------------------------------------------------------------------------------
//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------

void TeapotGame::Initialize()
{
	_scene = nullptr;
}

void TeapotGame::Update(NavigationInfo navInfo)
{
	_sceneChanged = false;
}

//  ITurboGameLevel Methods --------------------------------------------------------------------------------------------
