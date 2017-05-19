
#pragma once

#include <ITurboSceneBuilder.h>

#include <CubicMaze.h>

class OriginalMazeSceneBuilder: public ITurboSceneBuilder
{
public:
	//  Constructors and Destructors -----------------------------------------------------------------------------------
	OriginalMazeSceneBuilder();

	//  ITurboSceneBuilder Methods -------------------------------------------------------------------------------------
	virtual std::shared_ptr<ITurboScene> BuildScene();

private:
	//	Local Support Methods ------------------------------------------------------------------------------------------
	void	BuildSceneObjects(std::shared_ptr<ITurboScene> scene, std::shared_ptr<CubicMaze> maze);

	std::shared_ptr<ITurboSceneMesh> BuildCornerMesh();
	std::shared_ptr<ITurboSceneMesh> BuildEdgeMesh();
	std::shared_ptr<ITurboSceneMesh> BuildRightWallMesh();
	std::shared_ptr<ITurboSceneMesh> BuildBottomWallMesh();
	std::shared_ptr<ITurboSceneMesh> BuildFrontWallMesh();

	void	BuildCorner(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void	BuildWEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void	BuildHEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void	BuildDEdge(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> material);
	void	BuildRightWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> leftMaterial, std::shared_ptr<ITurboSceneMaterial> rightMaterial);
	void	BuildBottomWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> topMaterial, std::shared_ptr<ITurboSceneMaterial> bottomMaterial);
	void	BuildFrontWall(std::shared_ptr<ITurboScene> scene, int w, int h, int d, std::shared_ptr<ITurboSceneMesh> mesh, std::shared_ptr<ITurboSceneMaterial> backMaterial, std::shared_ptr<ITurboSceneMaterial> frontMaterial);
};

