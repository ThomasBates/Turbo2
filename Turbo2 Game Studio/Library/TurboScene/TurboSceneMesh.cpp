
#include <pch.h>

#include <TurboSceneMesh.h>
#include <TurboColor.h>


using namespace Turbo::Scene;

//  ITurboSceneMesh Methods --------------------------------------------------------------------------------------------

void TurboSceneMesh::AddVertex(TurboVector3D position, TurboVector3D normal, TurboColor color, TurboVector2D textureUV)
{
    TurboSceneVertex vertex;

    vertex.Position = position;
    vertex.Normal = normal;
    vertex.Color = color;
    vertex.TextureUV = textureUV;

    _vertices.push_back(vertex);

    //	Set Min/Max Extent
    if (_minExtent.X > position.X)	_minExtent.X = position.X;
    if (_maxExtent.X < position.X)	_maxExtent.X = position.X;
    if (_minExtent.Y > position.Y)	_minExtent.Y = position.Y;
    if (_maxExtent.Y < position.Y)	_maxExtent.Y = position.Y;
    if (_minExtent.Z > position.Z)	_minExtent.Z = position.Z;
    if (_maxExtent.Z < position.Z)	_maxExtent.Z = position.Z;
}

void TurboSceneMesh::AddVertex(TurboVector3D position, TurboVector3D normal, TurboColor color)
{
    AddVertex(position, normal, color, TurboVector2D(0, 0));
}

void TurboSceneMesh::AddVertex(TurboVector3D position, TurboVector3D normal, TurboVector2D textureUV)
{
    AddVertex(position, normal, TurboColor(1,1,1,1), textureUV);
}

void TurboSceneMesh::AddTriangle(int vertex1, int vertex2, int vertex3)
{
	TurboSceneTriangle triangle {};

	int vertexCount = _vertices.size();

	if (vertex1 >= 0)
		triangle.Vertex1 = vertex1;
	else
		triangle.Vertex1 = vertexCount - vertex1;

	if (vertex2 >= 0)
		triangle.Vertex2 = vertex2;
	else
		triangle.Vertex2 = vertexCount - vertex2;

	if (vertex3 >= 0)
		triangle.Vertex3 = vertex3;
	else
		triangle.Vertex3 = vertexCount - vertex3;

	_triangles.push_back(triangle);
}

