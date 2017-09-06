
#include <pch.h>

#include <Level05CubicMazeFactory.h>

//  ICubicMazeFactory Methods ------------------------------------------------------------------------------------------

std::shared_ptr<CubicMaze> Level05CubicMazeFactory::MakeMaze(int width, int height, int depth)
{
	std::shared_ptr<CubicMaze> cubicMaze = std::shared_ptr<CubicMaze>(new CubicMaze(3, 1, 1));
	CubicMazeLocation	size = cubicMaze->Size();

	//  I'm laying out this array visually - it's not the same as mazeArray.
	int wallcodes[1][1][3] =
	{
		{
			{ 0x101111, 0x001111, 0x011111 }
		}
	};

	for (int h = 0; h < size.H; h++)
	for (int w = 0; w < size.W; w++)
	for (int d = 0; d < size.D; d++)
	{
		int wallcode = wallcodes[h][size.D - 1 - d][w];
		CubicMazeCell *cell = cubicMaze->Cell(w, h, d);

		cell->LeftWall.Type		= (CubicMazeCellWallType)((wallcode & 0xF00000) >> 20);
		cell->RightWall.Type	= (CubicMazeCellWallType)((wallcode & 0x0F0000) >> 16);
		cell->TopWall.Type		= (CubicMazeCellWallType)((wallcode & 0x00F000) >> 12);
		cell->BottomWall.Type	= (CubicMazeCellWallType)((wallcode & 0x000F00) >>  8);
		cell->FrontWall.Type	= (CubicMazeCellWallType)((wallcode & 0x0000F0) >>  4);
		cell->BackWall.Type		= (CubicMazeCellWallType)((wallcode & 0x00000F) >>  0);
	}

	return cubicMaze;
}

//  ICubicMazeFactory Methods ------------------------------------------------------------------------------------------
