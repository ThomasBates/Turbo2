
#pragma once

#include <ITurboSceneObject.h>

using namespace Turbo::Scene;

/*
struct MazeWalls
{
	bool right : 1;
	bool bottom : 1;
	bool front : 1;
	bool used : 1;

	MazeWalls()
	{
		Initialize(false);
	}

	MazeWalls(bool isOn)
	{
		Initialize(isOn);
	}

	void Initialize(bool isOn)
	{
		right = isOn;
		bottom = isOn;
		front = isOn;
		used = isOn;
	}
};
*/

enum CubicMazeCellWallType 
{ 
	None, 
	Wall, 
	InvisibleWall, 
	Entrance, 
	EntranceLocked,
	EntranceBack, 
	Exit, 
	ExitLocked,
	ExitBack,
	CellWallTypeCount
};

enum CubicMazeType
{
	Unknown,
	Cube,
	Layered
};

struct CubicMazeCellWall
{
	CubicMazeCellWallType Type;
	ITurboSceneObject *SceneObject;
	int PortalIndex;
};

struct CubicMazeCell
{
	CubicMazeCellWall LeftWall;
	CubicMazeCellWall RightWall;
	CubicMazeCellWall TopWall;
	CubicMazeCellWall BottomWall;
	CubicMazeCellWall FrontWall;
	CubicMazeCellWall BackWall;
	bool Used;

	CubicMazeCell()
	{
		Initialize(CubicMazeCellWallType::None);
	}

	CubicMazeCell(CubicMazeCellWallType cellWallType)
	{
		Initialize(cellWallType);
	}

	void Initialize(CubicMazeCellWallType cellWallType)
	{
		LeftWall.Type = cellWallType;
		RightWall.Type = cellWallType;
		TopWall.Type = cellWallType;
		BottomWall.Type = cellWallType;
		FrontWall.Type = cellWallType;
		BackWall.Type = cellWallType;

		LeftWall.SceneObject = NULL;
		RightWall.SceneObject = NULL;
		TopWall.SceneObject = NULL;
		BottomWall.SceneObject = NULL;
		FrontWall.SceneObject = NULL;
		BackWall.SceneObject = NULL;

		LeftWall.PortalIndex = 0;
		RightWall.PortalIndex = 0;
		TopWall.PortalIndex = 0;
		BottomWall.PortalIndex = 0;
		FrontWall.PortalIndex = 0;
		BackWall.PortalIndex = 0;

		Used = false;
	}
};

struct CubicMazeDirection
{
	bool left : 1;
	bool up : 1;
	bool back : 1;
	bool right : 1;
	bool down : 1;
	bool front : 1;

	CubicMazeDirection()
	{
		Initialize(false);
	}

	CubicMazeDirection(bool isOn) 
	{
		Initialize(isOn);
	}

	void Initialize(bool isOn)
	{
		left = isOn;
		up = isOn;
		back = isOn;
		right = isOn;
		down = isOn;
		front = isOn;
	}
};


struct CubicMazeLocation
{
	int W, H, D;

	CubicMazeLocation() : 
		W(0), H(0), D(0) 
	{
	}

	CubicMazeLocation(int w, int h, int d) : 
		W(w), H(h), D(d) 
	{
	}
};

inline std::ostream& operator<<(std::ostream& os, const CubicMazeLocation& location)
{
	os << "(" << location.W << "," << location.H << "," << location.D << ")";
	return os;
}

typedef	CubicMazeCell*			CubicMazeCellArray1D;
typedef	CubicMazeCellArray1D*	CubicMazeCellArray2D;
typedef	CubicMazeCellArray2D*	CubicMazeCellArray3D;

struct CubicMazePath
{
	CubicMazeLocation	location;
	CubicMazeCell		mask;
	CubicMazePath		*prev;
	CubicMazePath		*next;
};

class CubicMaze
{
private:
	CubicMazeCellArray3D	_mazeArray;
	CubicMazeLocation		_size;
//	CubicMazeLocation		_entrance;
//	CubicMazeLocation		_exit;
	CubicMazePath			_solution;

public:
	//	Constructors ---------------------------------------------------------------------------------------------------
	CubicMaze(int width, int height, int depth);
	~CubicMaze();

	//	Properties -----------------------------------------------------------------------------------------------------
	//virtual CubicMazeCellArray3D MazeArray() { return _mazeArray; }
	//virtual void MazeArray(CubicMazeCellArray3D mazeArray) { _mazeArray = mazeArray; }

	virtual CubicMazeLocation Size() { return _size; }
	virtual void Size(CubicMazeLocation size) { _size = size; }

	virtual CubicMazePath Solution() { return _solution; }
	virtual void Solution(CubicMazePath solution) { _solution = solution; }

	//	Methods --------------------------------------------------------------------------------------------------------
	void Cell(int w, int h, int d, CubicMazeCell cell);
	CubicMazeCell *Cell(int w, int h, int d);

	void Cell(CubicMazeLocation location, CubicMazeCell cell);
	CubicMazeCell *Cell(CubicMazeLocation location);

private:
	void AllocateMazeArray();
	void DeallocateMazeArray();
	void ClearMazeArray();
};


#define CELLSIZE ((float)2.0)
#define CELLHALF ((float)1.0)
#define WALLSIZE ((float)0.2)
#define WALLHALF ((float)0.1)
#define PORTALSIZE ((float)1.0)
#define PORTALHALF ((float)0.5)

