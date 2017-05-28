
#include "pch.h"

#include <time.h>

#include "IntroCubicMazeFactory.h"

//  ========================================================================  //
//  IMazeFactory Methods
//  ========================================================================  //

/* ======================================================================== */
/* Makemaze                                                                 */
/* ======================================================================== */
/* Description:                                                             */
/* ------------                                                             */
/*    Method to generate a maze with dimensions WIDTH x HEIGHT x DEPTH.     */
/*                                                                          */
/* Processing:                                                              */
/* -----------                                                              */
/*                                                                          */
/*    'Mask' is a status variable:  set initially to 63 (binary 111111)     */
/* for each position.  Surroundings are scanned and the bit corresponding   */
/* to a direction of a position that has already been occupied is reset     */
/* to zero (see below)                                                      */
/*                                                                          */
/*                             000010      100000                           */
/*                                       D                                  */
/*                                    ^ >                                   */
/*                                    |/                                    */
/*                        000001   <--*--> W 001000                         */
/*                                   /|                                     */
/*                                  < v                                     */
/*                                                                          */
/*                          000100    H 010000                              */
/*                                                                          */
/*     --------------------------------------------------------------       */
/*                                                                          */
/*    The numbers in the 'maze' array indicate which walls are present.     */
/* Using the lowest three bits, a set bit ( == 1 ) indicates that a wall    */
/* is present, and vice-versa.  The three bits control three walls of the   */
/* cell as follows:                                                         */
/*                      bit         wall                                    */
/*                     -----       ------                                   */
/*                       1          right                                   */
/*                       2         bottom                                   */
/*                       3          front                                   */
/*                                                                          */
/* ======================================================================== */
std::shared_ptr<CubicMaze> IntroCubicMazeFactory::MakeMaze(int width, int height, int depth)
{
	bool		ok_to_continue;  /*  flag set to one when maze is completed */
	int			count = 0;
	Direction	Mask;	/*  status variable:  see above for description.    */
	location	p = { 1, 1, 1 };
	
	width = 8;
	height = 3;
	depth = 8;

	Array3D		mazeArray = NULL;
	location	size = { width, height, depth };

	AllocateMazeArray( &mazeArray, size );

	std::shared_ptr<CubicMaze> cubicMaze = std::shared_ptr<CubicMaze>(new CubicMaze);
	cubicMaze->SetMazeArray(mazeArray);
	cubicMaze->SetSize(size);


	//  I'm laying out this array visually - it's not the same as mazeArray.
	byte wallcodes[4][9][9] =
	{
		//{
		//	{ 0,2,2,2,0,0,2,2,2 },
		//	{ 0,2,2,2,0,0,2,2,2 },
		//	{ 0,2,2,2,0,0,2,2,2 },
		//	{ 0,0,0,0,0,0,0,0,0 },
		//	{ 0,0,0,0,0,0,0,0,0 },
		//	{ 0,2,2,2,0,0,2,2,2 },
		//	{ 0,2,2,2,0,0,2,2,2 },
		//	{ 0,2,2,2,0,0,2,2,2 },
		//	{ 0,0,0,0,0,0,0,0,0 }
		//},
		{
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,2,2,2,2,2,2,2,2 },
			{ 0,0,0,0,0,0,0,0,0 }
		},
		{
			{ 1,6,6,6,6,6,4,4,5 },
			{ 1,2,2,3,2,3,0,0,1 },
			{ 1,2,2,3,2,3,0,0,1 },
			{ 1,2,6,6,2,2,4,4,5 },
			{ 1,2,2,2,2,2,0,0,1 },
			{ 1,0,4,5,2,3,4,4,5 },
			{ 1,0,0,1,2,3,0,0,1 },
			{ 1,0,0,0,2,2,0,0,1 },
			{ 0,4,4,4,4,4,4,4,4 }
		},
		{
			{ 0,0,0,0,0,1,4,4,5 },
			{ 0,0,0,0,0,1,0,0,1 },
			{ 0,0,0,0,0,1,0,0,1 },
			{ 0,0,0,0,0,1,4,4,5 },
			{ 0,0,0,0,0,1,0,0,1 },
			{ 1,4,4,5,0,1,4,4,5 },
			{ 1,0,0,1,0,1,0,0,1 },
			{ 1,0,0,1,0,1,0,0,1 },
			{ 0,4,4,4,0,0,4,4,4 }
		},
		{
			{ 0,0,0,0,0,1,6,6,7 },
			{ 0,0,0,0,0,1,2,2,3 },
			{ 0,0,0,0,0,1,2,2,3 },
			{ 0,0,0,0,0,1,6,6,3 },
			{ 0,0,0,0,0,1,2,2,3 },
			{ 1,6,6,7,0,1,6,6,3 },
			{ 1,2,2,3,0,1,2,2,3 },
			{ 1,2,2,3,0,1,2,2,3 },
			{ 0,4,4,4,0,0,4,4,4 }
		}
	};


	for (int h = 0; h < 4; h++)
	for (int w = 0; w < 9; w++)
	for (int d = 0; d < 9; d++)
	{
		byte wallcode = wallcodes[h][8-d][w];

		if (wallcode & 1)
		{
			mazeArray[w][h][d].right = true;
		}
		if (wallcode & 2)
		{
			mazeArray[w][h][d].bottom = true;
		}
		if (wallcode & 4)
		{
			mazeArray[w][h][d].front = true;
		}
	}

	return cubicMaze;
}

void IntroCubicMazeFactory::FreeMaze(std::shared_ptr<CubicMaze> cubicMaze)
{
	Array3D mazeArray = cubicMaze->GetMazeArray();
	location size = cubicMaze->GetSize();

	DeallocateMazeArray(&mazeArray, size);
}


//  ========================================================================  //
//  Local Support Methods
//  ========================================================================  //


void IntroCubicMazeFactory::AllocateMazeArray(Array3D *mazeArray, location size)
{
	int	i, j, k;


	if ((*mazeArray = (Array3D)malloc((size.w + 1) * sizeof(Array2D))) != NULL)
	{
		for (i = 0; i <= size.w; i++)
			(*mazeArray)[i] = NULL;

		for (i = 0; i <= size.w; i++)
		{
			if (((*mazeArray)[i] = (Array2D)malloc((size.h + 1) * sizeof(Array1D))) == NULL)
			{
				DeallocateMazeArray(mazeArray, size);
				return;
			}
			else
			{
				for (j = 0; j <= size.h; j++)
					(*mazeArray)[i][j] = NULL;

				for (j = 0; j <= size.h; j++)
				{
					if (((*mazeArray)[i][j] = (Array1D)malloc(size.d + 1)) == NULL)
					{
						DeallocateMazeArray(mazeArray, size);
						return;
					}
					else
					{
						for (k = 0; k <= size.d; k++)
							(*mazeArray)[i][j][k] = MazeWalls();
					}
				}
			}
		}
	}
}

void IntroCubicMazeFactory::DeallocateMazeArray(Array3D *mazeArray, location size)
{
	int	i, j;


	if (*mazeArray != NULL)
	{
		for (i = 0; i <= size.w; i++)
		{
			if ((*mazeArray)[i] != NULL)
			{
				for (j = 0; j <= size.h; j++)
				{
					if ((*mazeArray)[i][j] != NULL)
						free((*mazeArray)[i][j]);
				}
				free((*mazeArray)[i]);
			}
		}
		free(*mazeArray);
		*mazeArray = NULL;
	}
}

void IntroCubicMazeFactory::ClearMazeArray(Array3D mazeArray, location size)
{
	int	i, j, k;

	for (i = 0; i <= size.w; i++)
	for (j = 0; j <= size.h; j++)
	for (k = 0; k <= size.d; k++)
		mazeArray[i][j][k] = MazeWalls();
}

