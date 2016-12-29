
#include "pch.h"

#include <OriginalMazeCornerObject.h>

//  Constructors and Destructors  --------------------------------------------------------------------------------------

OriginalMazeCornerObject::OriginalMazeCornerObject(std::shared_ptr<ITurboSceneObjectMesh> mesh, MazeObject mazeObject, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	const float cShadeFactor = 0.50;

	//std::shared_ptr<ITurboSceneObjectMesh> mesh = _platform->CreateMesh();

	Vector3D normal = Vector3D(0.0f, 0.0f, 1.0f) * cShadeFactor;
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));

	normal = Vector3D(0.0f, 0.0f, -1.0f) * cShadeFactor;
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));


	normal = Vector3D(0.0f, 1.0f, 0.0f) * cShadeFactor;
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));

	normal = Vector3D(0.0f, -1.0f, 0.0f) * cShadeFactor;
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));


	normal = Vector3D(1.0f, 0.0f, 0.0f) * cShadeFactor;
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(1.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Front), normal, Vector2D(1.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Top, mazeObject.Back), normal, Vector2D(0.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Left, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(0.0f, 0.0f));

	normal = Vector3D(-1.0f, 0.0f, 0.0f) * cShadeFactor;
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Front), normal, Vector2D(0.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Bottom, mazeObject.Back), normal, Vector2D(1.0f, 0.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Back), normal, Vector2D(1.0f, 1.0f));
	mesh->AddVertex(Vector3D(mazeObject.Right, mazeObject.Top, mazeObject.Front), normal, Vector2D(0.0f, 1.0f));

	mesh->AddTriangle(0, 1, 2, texture);
	mesh->AddTriangle(2, 3, 0, texture);

	mesh->AddTriangle(4, 5, 6, texture);
	mesh->AddTriangle(6, 7, 4, texture);

	mesh->AddTriangle(8, 9, 10, texture);
	mesh->AddTriangle(10, 11, 8, texture);

	mesh->AddTriangle(12, 13, 14, texture);
	mesh->AddTriangle(14, 15, 12, texture);

	mesh->AddTriangle(16, 17, 18, texture);
	mesh->AddTriangle(18, 19, 16, texture);

	mesh->AddTriangle(20, 21, 22, texture);
	mesh->AddTriangle(22, 23, 20, texture);

	Mesh(mesh);
}

//  Constructors and Destructors  --------------------------------------------------------------------------------------
//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------

//  ITurboSceneObject Methods  -----------------------------------------------------------------------------------------
