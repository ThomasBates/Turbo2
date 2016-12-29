
#pragma once

#include <ITurboSceneBuilder.h>
#include <ITurboApplicationPlatform.h>

#include <CubicMaze.h>

class OriginalMazeSceneBuilder: public ITurboSceneBuilder
{
private:
	std::shared_ptr<ITurboApplicationPlatform> _platform;

	//	Local Support Methods  -----------------------------------------------------------------------------------------
	void BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> maze);

	std::shared_ptr<ITurboSceneObject> BuildCorner(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture);
	std::shared_ptr<ITurboSceneObject> BuildWEdge(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture);
	std::shared_ptr<ITurboSceneObject> BuildHEdge(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture);
	std::shared_ptr<ITurboSceneObject> BuildDEdge(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> texture);
	std::shared_ptr<ITurboSceneObject> BuildRightWall(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> leftTexture, std::shared_ptr<ITurboSceneObjectTexture> rightTexture);
	std::shared_ptr<ITurboSceneObject> BuildBottomWall(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> topTexture, std::shared_ptr<ITurboSceneObjectTexture> bottomTexture);
	std::shared_ptr<ITurboSceneObject> BuildFrontWall(int w, int h, int d, std::shared_ptr<ITurboSceneObjectTexture> backTexture, std::shared_ptr<ITurboSceneObjectTexture> frontTexture);

	std::shared_ptr<ITurboSceneObject> CreateCornerObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture);
	std::shared_ptr<ITurboSceneObject> CreateEdgeObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture);
	std::shared_ptr<ITurboSceneObject> CreateWallObject(MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture1, std::shared_ptr<ITurboSceneObjectTexture> texture2);

public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	OriginalMazeSceneBuilder(std::shared_ptr<ITurboApplicationPlatform> platform);

	//  ITurboSceneBuilder Methods  ------------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene();
	virtual void FreeScene(std::shared_ptr<ITurboScene> scene);

};

