
#include "pch.h"

#include <time.h>

#include "OriginalCubicMazeFactory.h"

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
/*       ------------------------------------------------------------       */
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
std::shared_ptr<CubicMaze> OriginalCubicMazeFactory::MakeMaze(int width, int height, int depth)
{
	int		ok_to_continue;  /*  flag set to one when maze is completed */
	int		count = 0;
	char	Mask;	/*  status variable:  see above for description.    */
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
	SETBIT( mazeArray[1][1][1], USEDBIT );
	++count;
	SETBIT( mazeArray[1][1][1], RIGHTBIT  );
	SETBIT( mazeArray[1][1][1], BOTTOMBIT );
	SETBIT( mazeArray[1][1][1], FRONTBIT  );
	SETBIT( mazeArray[0][1][1], RIGHTBIT  );
	SETBIT( mazeArray[1][0][1], BOTTOMBIT );
	SETBIT( mazeArray[1][1][0], FRONTBIT  );


	ok_to_continue = 1;

	while (ok_to_continue)
	{
		Mask = 63;  /*  111111  */
		if ( p.w==1		 || GETBIT(mazeArray[p.w-1][ p.h ][ p.d ],USEDBIT) )	Mask -=  1;
		if ( p.h==1		 || GETBIT(mazeArray[ p.w ][p.h-1][ p.d ],USEDBIT) )	Mask -=  2;
		if ( p.d==1		 || GETBIT(mazeArray[ p.w ][ p.h ][p.d-1],USEDBIT) )	Mask -=  4;
		if ( p.w==size.w || GETBIT(mazeArray[p.w+1][ p.h ][ p.d ],USEDBIT) )	Mask -=  8;
		if ( p.h==size.h || GETBIT(mazeArray[ p.w ][p.h+1][ p.d ],USEDBIT) )	Mask -= 16;
		if ( p.d==size.d || GETBIT(mazeArray[ p.w ][ p.h ][p.d+1],USEDBIT) )	Mask -= 32;

		if ( Mask )
			ok_to_continue = Move( Mask, mazeArray, size, &p, &count );
		else
			Relocate( mazeArray, size, &p );
	}

	if ( in.d == 0 )
		RESETBIT( mazeArray[in.w][in.h][0], FRONTBIT );
	else if ( in.d == size.d+1 )
		RESETBIT( mazeArray[in.w][in.h][size.d], FRONTBIT );
	else if ( in.w == 0 )
		RESETBIT( mazeArray[0][in.h][in.d], RIGHTBIT );
	else if ( in.w == size.w+1 )
		RESETBIT( mazeArray[size.w][in.h][in.d], RIGHTBIT );
	else if ( in.h == 0 )
		RESETBIT( mazeArray[in.w][0][in.d], BOTTOMBIT );
	else if ( in.h == size.h+1 )
		RESETBIT( mazeArray[in.w][size.h][in.d], BOTTOMBIT );

	if ( out.d == 0 )
		RESETBIT( mazeArray[out.w][out.h][0], FRONTBIT );
	else if ( out.d == size.d+1 )
		RESETBIT( mazeArray[out.w][out.h][size.d], FRONTBIT );
	else if ( out.w == 0 )
		RESETBIT( mazeArray[0][out.h][out.d], RIGHTBIT );
	else if ( out.w == size.w+1 )
		RESETBIT( mazeArray[size.w][out.h][out.d], RIGHTBIT );
	else if ( out.h == 0 )
		RESETBIT( mazeArray[out.w][0][out.d], BOTTOMBIT );
	else if ( out.h == size.h+1 )
		RESETBIT( mazeArray[out.w][size.h][out.d], BOTTOMBIT );

	//cubicMaze->BuildWalls();

	return cubicMaze;
}

void OriginalCubicMazeFactory::FreeMaze(std::shared_ptr<CubicMaze> cubicMaze)
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
int OriginalCubicMazeFactory::Move(char mask, Array3D mazeArray, location size, location *p, int *count)
{
	int q;		/*  q:  choice of direction of motion
			    (see above for bit/direction relationship)      */


	for ( q=0; q==0; q = ( 1 << (rand()%6) ) & mask );

	switch (q)
	{
		case 1:		/*  move LEFT  */
			++*count;
			--p->w;
			SETBIT(mazeArray[p->w][p->h][p->d],USEDBIT);
			/* turn off right wall in new cell.  */
		  RESETBIT( mazeArray[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( mazeArray[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 2:		/*  move UP  */
			++*count;
			--p->h;
			SETBIT( mazeArray[p->w][p->h][p->d], USEDBIT );
			/* turn off bottom wall in new cell.  */
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
		  RESETBIT( mazeArray[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( mazeArray[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 4:		/*  move BACK  */
			++*count;
			--p->d;
			SETBIT( mazeArray[p->w][p->h][p->d], USEDBIT );
			/* turn off front wall in new cell.  */
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
		  RESETBIT( mazeArray[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( mazeArray[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 8:		/*  move RIGHT  */
			++*count;
			++p->w;
			SETBIT( mazeArray[p->w][p->h][p->d], USEDBIT );
			/* turn off right wall in last cell.  */
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], FRONTBIT  );
		  RESETBIT( mazeArray[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 16:	/*  move DOWN  */
			++*count;
			++p->h;
			SETBIT( mazeArray[p->w][p->h][p->d], USEDBIT );
			/* turn off bottom wall in last cell.*/
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( mazeArray[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
		  RESETBIT( mazeArray[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 32:	/*  move FRONT  */
			++*count;
			++p->d;
			SETBIT( mazeArray[p->w][p->h][p->d], USEDBIT );
			/* turn off front wall in last cell.*/
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( mazeArray[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( mazeArray[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( mazeArray[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
		  RESETBIT( mazeArray[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;
	}

	return( *count < size.w * size.h * size.d );
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
void OriginalCubicMazeFactory::Relocate(Array3D mazeArray, location size, location *p)
{
	do
		if ( p->w == size.w )
			if ( p->h == size.h )
				if ( p->d == size.d )
					p->w = 1,
					p->h = 1,
					p->d = 1;
				else
					p->w = 1,
					p->h = 1,
					p->d++;
			else
				p->w = 1,
				p->h++;
		else
			p->w++;

	while ( !GETBIT( mazeArray[ p->w ][ p->h ][ p->d ], USEDBIT ) );
}

void OriginalCubicMazeFactory::AllocateMazeArray(Array3D *mazeArray, location size)
{
	int	i, j, k;


	if ( ( *mazeArray = (Array3D)malloc( (size.w+1) * sizeof(Array2D) ) ) != NULL )
	{
		for ( i=0; i<=size.w; i++ )
			(*mazeArray)[i] = NULL;

		for ( i=0; i<=size.w; i++ )
		{
			if ( ( (*mazeArray)[i] = (Array2D)malloc( (size.h+1) * sizeof(Array1D) ) ) == NULL )
			{
				DeallocateMazeArray( mazeArray, size );
				return;
			}
			else
			{
				for ( j=0; j<=size.h; j++ )
					(*mazeArray)[i][j] = NULL;

				for ( j=0; j<=size.h; j++ )
				{
					if ( ( (*mazeArray)[i][j] = (Array1D)malloc( size.d + 1 ) ) == NULL )
					{
						DeallocateMazeArray( mazeArray, size );
						return;
					}
					else
					{
						for ( k=0; k<=size.d; k++ )
							(*mazeArray)[i][j][k] = 0;
					}
				}
			}
		}
	}
}

void OriginalCubicMazeFactory::DeallocateMazeArray(Array3D *mazeArray, location size)
{
	int	i, j;


	if ( *mazeArray != NULL )
	{
		for ( i=0; i<=size.w; i++ )
		{
			if ( (*mazeArray)[i] != NULL )
			{
				for ( j=0; j<=size.h; j++ )
				{
					if ( (*mazeArray)[i][j] != NULL )
						free( (*mazeArray)[i][j] );
				}
				free( (*mazeArray)[i] );
			}
		}
		free( *mazeArray );
		*mazeArray = NULL;
	}
}

void OriginalCubicMazeFactory::ClearMazeArray(Array3D mazeArray, location size)
{
	int	i, j, k;

	for ( i=0; i<=size.w; i++ )
	for ( j=0; j<=size.h; j++ )
	for ( k=0; k<=size.d; k++ )
		mazeArray[i][j][k] = 0;
}

