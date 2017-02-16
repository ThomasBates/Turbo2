
#pragma once

#include <ITurboSceneBuilder.h>

#include <CubicMaze.h>

class OriginalMazeSceneBuilder: public ITurboSceneBuilder
{
public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	OriginalMazeSceneBuilder();

	//  ITurboSceneBuilder Methods  ------------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene();

private:
	//	Local Support Methods  -----------------------------------------------------------------------------------------
	void	BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> maze);

	void	BuildCorner(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material);
	void	BuildWEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material);
	void	BuildHEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material);
	void	BuildDEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> material);
	void	BuildRightWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> leftMaterial, std::shared_ptr<ITurboSceneObjectMaterial> rightMaterial);
	void	BuildBottomWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> topMaterial, std::shared_ptr<ITurboSceneObjectMaterial> bottomMaterial);
	void	BuildFrontWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneObjectMaterial> backMaterial, std::shared_ptr<ITurboSceneObjectMaterial> frontMaterial);
};

