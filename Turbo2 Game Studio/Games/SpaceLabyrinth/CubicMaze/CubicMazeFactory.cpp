
#include <pch.h>

#include <time.h>

#include <CubicMazeFactory.h>

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
std::shared_ptr<CubicMaze> CubicMazeFactory::MakeMaze(int width, int height, int depth)
{
	std::shared_ptr<CubicMaze> cubicMaze = std::shared_ptr<CubicMaze>(new CubicMaze(width, height, depth));
	CubicMazeLocation	size = cubicMaze->Size();

	srand(time(0) % 0xFFFF);

	int count = 0;
	CubicMazeLocation location(0, 0, 0);
	
	//	--------------	//
	//	Now trace maze	//
	//	--------------	//
	cubicMaze->Cell(location, CubicMazeCell(Wall));
	cubicMaze->Cell(location)->Used = true;
	++count;

	//	flag set to false when maze is completed
	bool mazeIsComplete = false;

	while (!mazeIsComplete)
	{
		//	status variable:  see above for description.
		CubicMazeDirection	mask(true);

		if (location.W == 0			|| cubicMaze->Cell(location.W - 1, location.H, location.D)->Used)	mask.left = false;
		if (location.H == 0			|| cubicMaze->Cell(location.W, location.H - 1, location.D)->Used)	mask.up = false;
		if (location.D == 0			|| cubicMaze->Cell(location.W, location.H, location.D - 1)->Used)	mask.back = false;
		if (location.W == size.W-1	|| cubicMaze->Cell(location.W + 1, location.H, location.D)->Used)	mask.right = false;
		if (location.H == size.H-1	|| cubicMaze->Cell(location.W, location.H + 1, location.D)->Used)	mask.down = false;
		if (location.D == size.D-1	|| cubicMaze->Cell(location.W, location.H, location.D + 1)->Used)	mask.front = false;

		if (mask.left || mask.right || mask.up || mask.down || mask.front || mask.back)
			mazeIsComplete = Move(mask, cubicMaze, size, &location, &count);
		else
			Relocate( cubicMaze, size, &location );
	}

	return cubicMaze;
}

void CubicMazeFactory::FreeMaze(std::shared_ptr<CubicMaze> cubicMaze)
{
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


bool CubicMazeFactory::Move(CubicMazeDirection mask, std::shared_ptr<CubicMaze> cubicMaze, CubicMazeLocation size, CubicMazeLocation *location, int *count)
{
	CubicMazeDirection direction = CubicMazeDirection();

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

	CubicMazeCell *oldCell = cubicMaze->Cell(*location);
	CubicMazeCell newCell = CubicMazeCell(Wall);
	newCell.Used = true;

	//	Move location in the determined direction.
	//	Turn off the wall in the new cell in the direction we moved from.
	if (direction.left)
	{
		--location->W;
		oldCell->LeftWall.Type = None;
		newCell.RightWall.Type = None;
	}
	else if (direction.up)
	{
		--location->H;
		oldCell->TopWall.Type = None;
		newCell.BottomWall.Type = None;
	}
	else if (direction.back)
	{
		--location->D;
		oldCell->BackWall.Type = None;
		newCell.FrontWall.Type = None;
	}
	else if (direction.right)
	{
		++location->W;
		oldCell->RightWall.Type = None;
		newCell.LeftWall.Type = None;
	}
	else if (direction.down)
	{
		++location->H;
		oldCell->BottomWall.Type = None;
		newCell.TopWall.Type = None;
	}
	else if (direction.front)
	{
		++location->D;
		oldCell->FrontWall.Type = None;
		newCell.BackWall.Type = None;
	}

	cubicMaze->Cell(*location, newCell);

	++*count;
	return(*count >= size.W * size.H * size.D);
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
void CubicMazeFactory::Relocate(std::shared_ptr<CubicMaze> cubicMaze, CubicMazeLocation size, CubicMazeLocation *location)
{
	do
	{
		location->W++;
		
		if (location->W >= size.W)
		{
			location->W = 0;
			location->H++;
		}
		
		if (location->H >= size.H)
		{
			location->H = 0;
			location->D++;
		}
		
		if (location->D >= size.D)
		{
			location->D = 0;
		}

	} while (!cubicMaze->Cell(*location)->Used);
}

