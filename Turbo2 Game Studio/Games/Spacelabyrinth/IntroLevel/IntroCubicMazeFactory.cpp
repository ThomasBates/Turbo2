
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
	
	Array3D		mazeArray = NULL;
	location	size = { width, height, depth };
	location	in   = { width/2+1, height/2+1, 1 };
	location	out  = { width, height, depth+1 };
//	location	out  = { width/2+1, height/2+1, depth+2 };

	//time_t now;	/*  used with "time" to randomize seed for "rand"  */
	//time(&now);
	srand(time(0) % 0xFFFF );
	
	AllocateMazeArray( &mazeArray, size );

	std::shared_ptr<CubicMaze> cubicMaze = std::shared_ptr<CubicMaze>(new CubicMaze);
	cubicMaze->SetMazeArray(mazeArray);
	cubicMaze->SetSize(size);
	cubicMaze->SetEntrance(in);
	cubicMaze->SetExit(out);


	/* -------------- */
	/* Now trace maze */
	/* -------------- */
	mazeArray[1][1][1].used = true;
	++count;
	mazeArray[1][1][1].right = true;
	mazeArray[1][1][1].bottom = true;
	mazeArray[1][1][1].front = true;
	mazeArray[0][1][1].right = true;
	mazeArray[1][0][1].bottom = true;
	mazeArray[1][1][0].front = true;


	ok_to_continue = 1;

	while (ok_to_continue)
	{
		Mask = Direction(true);

		if (p.w == 1		||	mazeArray[p.w - 1][p.h][p.d].used)	Mask.left = false;
		if (p.h == 1		||	mazeArray[p.w][p.h - 1][p.d].used)	Mask.up = false;
		if (p.d == 1		||	mazeArray[p.w][p.h][p.d - 1].used)	Mask.back = false;
		if (p.w == size.w	||	mazeArray[p.w + 1][p.h][p.d].used)	Mask.right = false;
		if (p.h == size.h	||	mazeArray[p.w][p.h + 1][p.d].used)	Mask.down = false;
		if (p.d == size.d	||	mazeArray[p.w][p.h][p.d + 1].used)	Mask.front = false;

		if (Mask.left || Mask.right || Mask.up || Mask.down || Mask.front || Mask.back)
			ok_to_continue = Move(Mask, mazeArray, size, &p, &count);
		else
			Relocate( mazeArray, size, &p );
	}

	if (in.d == 0)
		mazeArray[in.w][in.h][0].front = false;
	else if (in.d == size.d + 1)
		mazeArray[in.w][in.h][size.d].front = false;
	else if (in.w == 0)
		mazeArray[0][in.h][in.d].right = false;
	else if (in.w == size.w + 1)
		mazeArray[size.w][in.h][in.d].right = false;
	else if (in.h == 0)
		mazeArray[in.w][0][in.d].bottom = false;
	else if (in.h == size.h + 1)
		mazeArray[in.w][size.h][in.d].bottom = false;

	if (out.d == 0)
		mazeArray[out.w][out.h][0].front = false;
	else if (out.d == size.d + 1)
		mazeArray[out.w][out.h][size.d].front = false;
	else if (out.w == 0)
		mazeArray[0][out.h][out.d].right = false;
	else if (out.w == size.w + 1)
		mazeArray[size.w][out.h][out.d].right = false;
	else if (out.h == 0)
		mazeArray[out.w][0][out.d].bottom = false;
	else if (out.h == size.h + 1)
		mazeArray[out.w][size.h][out.d].bottom = false;

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


/* ======================================================================== */
/* Move                                                                     */
/* ======================================================================== */
/*                                                                          */
/* Description:                                                             */
/* ------------                                                             */
/*    Moves position (w,h,d) to a new position in a random direction.       */
/* The motion is contingent on the status variable 'Mask' as described      */
/* in Makemaze.                                                             */
/*                                                                          */
/* Inputs:                                                                  */
/* -------                                                                  */
/*    Mask -- determines in which direction motion is possible              */
/*                                                                          */
/* Revision History:                                                        */
/* ------------------------------------------------------------------------ */
/* ======================================================================== */


bool IntroCubicMazeFactory::Move(Direction mask, Array3D mazeArray, location size, location *p, int *count)
{
	Direction direction = Direction();

	while (true)
	{
		/*  q:  choice of direction of motion
		(see above for bit/direction relationship)      */
		int q = rand() % 6;

		if ((q == 0) && (mask.left))
		{
			direction.left = true;
			break;
		}
		if ((q == 1) && (mask.up))
		{
			direction.up = true;
			break;
		}
		if ((q == 2) && (mask.back))
		{
			direction.back = true;
			break;
		}
		if ((q == 3) && (mask.right))
		{
			direction.right = true;
			break;
		}
		if ((q == 4) && (mask.down))
		{
			direction.down = true;
			break;
		}
		if ((q == 5) && (mask.front))
		{
			direction.front = true;
			break;
		}
	}

	if (direction.left)
	{
		++*count;
		--p->w;
		mazeArray[p->w][p->h][p->d].used = true;
		/* turn off right wall in new cell.  */
		mazeArray[p->w][p->h][p->d].right = false;
		mazeArray[p->w][p->h][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d].front = true;
		mazeArray[p->w - 1][p->h][p->d].right = true;
		mazeArray[p->w][p->h - 1][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d - 1].front = true;
	}
	else if (direction.up)
	{
		++*count;
		--p->h;
		mazeArray[p->w][p->h][p->d].used = true;
		/* turn off bottom wall in new cell.  */
		mazeArray[p->w][p->h][p->d].right = true;
		mazeArray[p->w][p->h][p->d].bottom = false;
		mazeArray[p->w][p->h][p->d].front = true;
		mazeArray[p->w - 1][p->h][p->d].right = true;
		mazeArray[p->w][p->h - 1][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d - 1].front = true;
	}
	else if (direction.back)
	{
		++*count;
		--p->d;
		mazeArray[p->w][p->h][p->d].used = true;
		/* turn off front wall in new cell.  */
		mazeArray[p->w][p->h][p->d].right = true;
		mazeArray[p->w][p->h][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d].front = false;
		mazeArray[p->w - 1][p->h][p->d].right = true;
		mazeArray[p->w][p->h - 1][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d - 1].front = true;
	}
	else if (direction.right)
	{
		++*count;
		++p->w;
		mazeArray[p->w][p->h][p->d].used = true;
		/* turn off right wall in last cell.  */
		mazeArray[p->w][p->h][p->d].right = true;
		mazeArray[p->w][p->h][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d].front = true;
		mazeArray[p->w - 1][p->h][p->d].right = false;
		mazeArray[p->w][p->h - 1][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d - 1].front = true;
	}
	else if (direction.down)
	{
		++*count;
		++p->h;
		mazeArray[p->w][p->h][p->d].used = true;
		/* turn off bottom wall in last cell.*/
		mazeArray[p->w][p->h][p->d].right = true;
		mazeArray[p->w][p->h][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d].front = true;
		mazeArray[p->w - 1][p->h][p->d].right = true;
		mazeArray[p->w][p->h - 1][p->d].bottom = false;
		mazeArray[p->w][p->h][p->d - 1].front = true;
	}
	else if (direction.front)
	{
		++*count;
		++p->d;
		mazeArray[p->w][p->h][p->d].used = true;
		/* turn off front wall in last cell.*/
		mazeArray[p->w][p->h][p->d].right = true;
		mazeArray[p->w][p->h][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d].front = true;
		mazeArray[p->w - 1][p->h][p->d].right = true;
		mazeArray[p->w][p->h - 1][p->d].bottom = true;
		mazeArray[p->w][p->h][p->d - 1].front = false;

	}

	return(*count < size.w * size.h * size.d);
}

/* ======================================================================== */
/* Relocate                                                                 */
/* ======================================================================== */
/*                                                                          */
/* Description:                                                             */
/* ------------                                                             */
/*    Finds a new position to continue from somewhere else on the path.     */
/* Used when position (w,h,d) is blocked on all six sides.                  */
/*                                                                          */
/*                                                                          */
/* Revision History:                                                        */
/* ------------------------------------------------------------------------ */
/* ======================================================================== */
void IntroCubicMazeFactory::Relocate(Array3D mazeArray, location size, location *p)
{
	do
	{
		if (p->w == size.w)
		{
			if (p->h == size.h)
			{
				if (p->d == size.d)
				{
					p->w = 1;
					p->h = 1;
					p->d = 1;
				}
				else
				{
					p->w = 1;
					p->h = 1;
					p->d++;
				}
			}
			else
			{
				p->w = 1;
				p->h++;
			}
		}
		else
		{
			p->w++;
		}
	} while (!mazeArray[p->w][p->h][p->d].used);
}

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

