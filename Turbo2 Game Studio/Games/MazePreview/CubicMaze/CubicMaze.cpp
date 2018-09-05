
#include <pch.h>

#include <CubicMaze.h>

CubicMaze::CubicMaze(int width, int height, int depth)
{
	Size(CubicMazeLocation(width, height, depth));
	AllocateMazeArray();
}

CubicMaze::~CubicMaze()
{
	DeallocateMazeArray();
}

void CubicMaze::Cell(int w, int h, int d, CubicMazeCell cell)
{
	if ((w >= 0) && (w < _size.W) &&
		(h >= 0) && (h < _size.H) &&
		(d >= 0) && (d < _size.D))
	{
		_mazeArray[w][h][d] = cell;
	}
}

CubicMazeCell *CubicMaze::Cell(int w, int h, int d)
{
	if ((w >= 0) && (w < _size.W) &&
		(h >= 0) && (h < _size.H) &&
		(d >= 0) && (d < _size.D))
	{
		return &_mazeArray[w][h][d];
	}
	return NULL;
}

void CubicMaze::Cell(CubicMazeLocation location, CubicMazeCell cell)
{
	Cell(location.W, location.H, location.D, cell);
}

CubicMazeCell *CubicMaze::Cell(CubicMazeLocation location)
{
	return Cell(location.W, location.H, location.D);
}

void CubicMaze::AllocateMazeArray()
{
	_mazeArray = (CubicMazeCellArray3D)malloc(_size.W * sizeof(CubicMazeCellArray2D));

	if (_mazeArray != NULL)
	{
		for (int i = 0; i < _size.W; i++)
			_mazeArray[i] = NULL;

		for (int i = 0; i < _size.W; i++)
		{
			_mazeArray[i] = (CubicMazeCellArray2D)malloc(_size.H * sizeof(CubicMazeCellArray1D));

			if (_mazeArray[i] == NULL)
			{
				DeallocateMazeArray();
				return;
			}

			for (int j = 0; j < _size.H; j++)
				_mazeArray[i][j] = NULL;

			for (int j = 0; j < _size.H; j++)
			{
				_mazeArray[i][j] = (CubicMazeCellArray1D)malloc(_size.D * sizeof(CubicMazeCell));

				if (_mazeArray[i][j] == NULL)
				{
					DeallocateMazeArray();
					return;
				}

				for (int k = 0; k < _size.D; k++)
					_mazeArray[i][j][k] = CubicMazeCell(CubicMazeCellWallType::None);

			}
		}
	}
}

void CubicMaze::DeallocateMazeArray()
{
	if (_mazeArray != NULL)
	{
		for (int i = 0; i < _size.W; i++)
		{
			if (_mazeArray[i] != NULL)
			{
				for (int j = 0; j < _size.H; j++)
				{
					if (_mazeArray[i][j] != NULL)
						free(_mazeArray[i][j]);
				}
				free(_mazeArray[i]);
			}
		}
		free(_mazeArray);
		_mazeArray = NULL;
	}
}

void CubicMaze::ClearMazeArray()
{
	for (int i = 0; i < _size.W; i++)
	for (int j = 0; j < _size.H; j++)
	for (int k = 0; k < _size.D; k++)
		_mazeArray[i][j][k].Initialize(CubicMazeCellWallType::None);
}

