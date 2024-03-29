
#pragma once

#include <pch.h>

#include <ITurboDebug.h>
#include <ITurboGameLevel.h>
#include <ITurboGameMotionEffects.h>
#include <ITurboSceneBuilder.h>
#include <ICubicMazeObjectInteractions.h>
#include <ICubicMazeSceneBuilder.h>

#include <CubicMaze.h>
#include <MazeOptions.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::Math;
using namespace Turbo::Scene;

class Level00Helper
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	Level00Helper(
		std::shared_ptr<ITurboSceneObject> player,
		std::shared_ptr<CubicMaze> maze,
		std::shared_ptr<ITurboGameMotionEffects> motionEffects,
		std::shared_ptr<ICubicMazeSceneBuilder> sceneBuilder,
		std::shared_ptr<ICubicMazeObjectInteractions> objectInteractions,
		std::shared_ptr<MazeOptions> mazeOptions,
		std::vector<std::shared_ptr<ITurboSceneObject>>* keys,
		std::vector<std::shared_ptr<ITurboSceneObject>>* hazards) :

		_player(player),
		_maze(maze),
		_motionEffects(motionEffects),
		_sceneBuilder(sceneBuilder),
		_objectInteractions(objectInteractions),
		_mazeOptions(mazeOptions),
		_keys(keys),
		_hazards(hazards)
	{
	}

	//  Public Methods -------------------------------------------------------------------------------------------------
	void CreateKeys(CubicMazeLocation* firstKeyLocation, std::string keyTextureName);
	void CreateHazards(CubicMazeLocation* firstHazardLocation, std::string hazardTextureName);

	std::shared_ptr<ITurboScene> BuildScene(NavigationInfo* navInfo);

	void CreateSign(std::shared_ptr<ITurboScene> scene, CubicMazeLocation signLocation, CubicMazeCellWallSide wallSide, std::string signTextureName);
	void ClearSignage();
	void AddSignage(float x, float y, float z, std::string signage);
	std::string GetSignage(std::shared_ptr<ITurboSceneObject> player);

	bool Update(NavigationInfo* navInfo, TurboGameLevelState* levelState);
	bool UpdateKeys(NavigationInfo* navInfo, std::string keyHitSoundName, bool doHitTest);
	bool UpdateHazards(NavigationInfo* navInfo, std::string hazardHitSoundName, bool doHitTest);


private:
	std::shared_ptr<ITurboSceneObject>					_player;
	std::shared_ptr<CubicMaze>							_maze;
	std::shared_ptr<ITurboGameMotionEffects>			_motionEffects;
	std::shared_ptr<ICubicMazeSceneBuilder>				_sceneBuilder;
	std::shared_ptr<ICubicMazeObjectInteractions>		_objectInteractions;
	std::shared_ptr<MazeOptions>						_mazeOptions;
	std::vector<std::shared_ptr<ITurboSceneObject>>*	_keys;
	std::vector<std::shared_ptr<ITurboSceneObject>>*	_hazards;
	std::vector<std::tuple<TurboVector3D,std::string>>	_signageMap;

	double		_failTime;

	//	Private Methods ------------------------------------------------------------------------------------------------
	TurboVector3D Position(CubicMazeLocation * location);
	TurboVector3D RandomPosition();
	TurboVector3D RandomDirection();
	TurboVector3D RandomAngularVelocity();
};

