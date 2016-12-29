
#include <pch.h>

#include <TurboSceneObjectMesh.h>

void TurboSceneObjectMesh::AddVertex(Vector3D position, Vector3D color)
{
	TurboSceneObjectVertex vertex;

	vertex.Position = position;
	vertex.Color = color;

	vertex.UsesNormal = false;
	vertex.UsesColor = true;
	vertex.UsesTexture = false;

	_vertices.push_back(vertex);

	if (_minExtent.X > position.X)	_minExtent.X = position.X;
	if (_maxExtent.X < position.X)	_maxExtent.X = position.X;
	if (_minExtent.Y > position.Y)	_minExtent.Y = position.Y;
	if (_maxExtent.Y < position.Y)	_maxExtent.Y = position.Y;
	if (_minExtent.Z > position.Z)	_minExtent.Z = position.Z;
	if (_maxExtent.Z < position.Z)	_maxExtent.Z = position.Z;
}

void TurboSceneObjectMesh::AddVertex(Vector3D position, Vector3D normal, Vector2D textureCoordinate)
{
	TurboSceneObjectVertex vertex;
	
	vertex.Position = position;
	vertex.Color = Vector3D(abs(normal.X), abs(normal.Y), abs(normal.Z));	//  Temporary for testing
	vertex.Normal = normal;
	vertex.Texture = textureCoordinate;

	vertex.UsesNormal = true;
	vertex.UsesColor = true;	//  Temporary for testing
	vertex.UsesTexture = true;

	_vertices.push_back(vertex);

	if (_minExtent.X > position.X)	_minExtent.X = position.X;
	if (_maxExtent.X < position.X)	_maxExtent.X = position.X;
	if (_minExtent.Y > position.Y)	_minExtent.Y = position.Y;
	if (_maxExtent.Y < position.Y)	_maxExtent.Y = position.Y;
	if (_minExtent.Z > position.Z)	_minExtent.Z = position.Z;
	if (_maxExtent.Z < position.Z)	_maxExtent.Z = position.Z;
}

void TurboSceneObjectMesh::AddTriangle(int vertex1, int vertex2, int vertex3)
{
	TurboSceneObjectTriangle triangle = CreateTriangle(vertex1, vertex2, vertex3);

	triangle.UsesTexture = false;

	_triangles.push_back(triangle);
}

void TurboSceneObjectMesh::AddTriangle(int vertex1, int vertex2, int vertex3, std::shared_ptr<ITurboSceneObjectTexture> texture)
{
	TurboSceneObjectTriangle triangle = CreateTriangle(vertex1, vertex2, vertex3);

	triangle.Texture = texture;
	triangle.UsesTexture = true;

	_triangles.push_back(triangle);
}

TurboSceneObjectTriangle TurboSceneObjectMesh::CreateTriangle(int vertex1, int vertex2, int vertex3)
{
	TurboSceneObjectTriangle triangle;

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

	triangle.UsesTexture = false;
	
	return triangle;
}

