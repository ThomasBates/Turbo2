
#include <stdio.h>
#include <stdlib.h>

#include "OriginalMazeFactory.h"
#include "OriginalMaze.h"

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
/*                                    ^ ¿                                   */
/*                                    |/                                    */
/*                        000001   <--*--> W 001000                         */
/*                                   /|                                     */
/*                                  À v                                     */
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
IMaze *OriginalMazeFactory::MakeMaze(int width, int height, int depth)
{
	int		ok_to_continue;  /*  flag set to one when maze is completed */
	int		count = 0;
	char	Mask;	/*  status variable:  see above for description.    */
	location	p = { 1, 1, 1 };
	
	Array3D		maze = NULL;
	location	size = { width, height, depth };
	location	in = { width/2, height/2, 0 };
	location	out = { width/2, height/2, depth };
	
	AllocateMaze( &maze, size );

	OriginalMaze *originalMaze = new OriginalMaze;
	originalMaze->SetMaze(maze);
	originalMaze->SetSize(size);
	originalMaze->SetEntrance(in);
	originalMaze->SetExit(out);


	/* -------------- */
	/* Now trace maze */
	/* -------------- */
	SETBIT( maze[1][1][1], USEDBIT );
	++count;
	SETBIT( maze[1][1][1], RIGHTBIT  );
	SETBIT( maze[1][1][1], BOTTOMBIT );
	SETBIT( maze[1][1][1], FRONTBIT  );
	SETBIT( maze[0][1][1], RIGHTBIT  );
	SETBIT( maze[1][0][1], BOTTOMBIT );
	SETBIT( maze[1][1][0], FRONTBIT  );


	ok_to_continue = 1;

	while (ok_to_continue)
	{
		Mask = 63;  /*  111111  */
		if ( p.w==1		 || GETBIT(maze[p.w-1][ p.h ][ p.d ],USEDBIT) )	Mask -=  1;
		if ( p.h==1		 || GETBIT(maze[ p.w ][p.h-1][ p.d ],USEDBIT) )	Mask -=  2;
		if ( p.d==1		 || GETBIT(maze[ p.w ][ p.h ][p.d-1],USEDBIT) )	Mask -=  4;
		if ( p.w==size.w || GETBIT(maze[p.w+1][ p.h ][ p.d ],USEDBIT) )	Mask -=  8;
		if ( p.h==size.h || GETBIT(maze[ p.w ][p.h+1][ p.d ],USEDBIT) )	Mask -= 16;
		if ( p.d==size.d || GETBIT(maze[ p.w ][ p.h ][p.d+1],USEDBIT) )	Mask -= 32;

		if ( Mask )
			ok_to_continue = Move( Mask, maze, size, &p, &count );
		else
			Relocate( maze, size, &p );
	}

	if ( in.d == 0 )
		RESETBIT( maze[in.w][in.h][0], FRONTBIT );
	else if ( in.d == size.d+1 )
		RESETBIT( maze[in.w][in.h][size.d], FRONTBIT );
	else if ( in.w == 0 )
		RESETBIT( maze[0][in.h][in.d], RIGHTBIT );
	else if ( in.w == size.w+1 )
		RESETBIT( maze[size.w][in.h][in.d], RIGHTBIT );
	else if ( in.h == 0 )
		RESETBIT( maze[in.w][0][in.d], BOTTOMBIT );
	else if ( in.h == size.h+1 )
		RESETBIT( maze[in.w][size.h][in.d], BOTTOMBIT );

	if ( out.d == 0 )
		RESETBIT( maze[out.w][out.h][0], FRONTBIT );
	else if ( out.d == size.d+1 )
		RESETBIT( maze[out.w][out.h][size.d], FRONTBIT );
	else if ( out.w == 0 )
		RESETBIT( maze[0][out.h][out.d], RIGHTBIT );
	else if ( out.w == size.w+1 )
		RESETBIT( maze[size.w][out.h][out.d], RIGHTBIT );
	else if ( out.h == 0 )
		RESETBIT( maze[out.w][0][out.d], BOTTOMBIT );
	else if ( out.h == size.h+1 )
		RESETBIT( maze[out.w][size.h][out.d], BOTTOMBIT );

	return originalMaze;
}

void OriginalMazeFactory::FreeMaze(IMaze *maze)
{
	Array3D mazeArray = maze->GetMaze();
	location size = maze->GetSize();

	DeallocateMaze(&mazeArray, size);
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
int OriginalMazeFactory::Move(char mask, Array3D maze, location size, location *p, int *count)
{
	int q;		/*  q:  choice of direction of motion
			    (see above for bit/direction relationship)      */


	for ( q=0; q==0; q = ( 1 << (rand()%6) ) & mask );

	switch (q)
	{
		case 1:		/*  move LEFT  */
			++*count;
			--p->w;
			SETBIT(maze[p->w][p->h][p->d],USEDBIT);
			/* turn off right wall in new cell.  */
		  RESETBIT( maze[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( maze[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 2:		/*  move UP  */
			++*count;
			--p->h;
			SETBIT( maze[p->w][p->h][p->d], USEDBIT );
			/* turn off bottom wall in new cell.  */
			SETBIT( maze[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
		  RESETBIT( maze[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( maze[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 4:		/*  move BACK  */
			++*count;
			--p->d;
			SETBIT( maze[p->w][p->h][p->d], USEDBIT );
			/* turn off front wall in new cell.  */
			SETBIT( maze[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
		  RESETBIT( maze[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( maze[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 8:		/*  move RIGHT  */
			++*count;
			++p->w;
			SETBIT( maze[p->w][p->h][p->d], USEDBIT );
			/* turn off right wall in last cell.  */
			SETBIT( maze[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], FRONTBIT  );
		  RESETBIT( maze[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 16:	/*  move DOWN  */
			++*count;
			++p->h;
			SETBIT( maze[p->w][p->h][p->d], USEDBIT );
			/* turn off bottom wall in last cell.*/
			SETBIT( maze[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( maze[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
		  RESETBIT( maze[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][p->d-1], FRONTBIT  );
			break;

		case 32:	/*  move FRONT  */
			++*count;
			++p->d;
			SETBIT( maze[p->w][p->h][p->d], USEDBIT );
			/* turn off front wall in last cell.*/
			SETBIT( maze[ p->w ][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], BOTTOMBIT );
			SETBIT( maze[ p->w ][ p->h ][ p->d ], FRONTBIT  );
			SETBIT( maze[p->w-1][ p->h ][ p->d ], RIGHTBIT  );
			SETBIT( maze[ p->w ][p->h-1][ p->d ], BOTTOMBIT );
		  RESETBIT( maze[ p->w ][ p->h ][p->d-1], FRONTBIT  );
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
void OriginalMazeFactory::Relocate(Array3D maze, location size, location *p)
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

	while ( !GETBIT( maze[ p->w ][ p->h ][ p->d ], USEDBIT ) );
}

void OriginalMazeFactory::AllocateMaze(Array3D *maze, location size)
{
	int	i, j, k;


	if ( ( *maze = (Array3D)malloc( (size.w+1) * sizeof(Array2D) ) ) != NULL )
	{
		for ( i=0; i<=size.w; i++ )
			(*maze)[i] = NULL;

		for ( i=0; i<=size.w; i++ )
		{
			if ( ( (*maze)[i] = (Array2D)malloc( (size.h+1) * sizeof(Array1D) ) ) == NULL )
			{
				DeallocateMaze( maze, size );
				return;
			}
			else
			{
				for ( j=0; j<=size.h; j++ )
					(*maze)[i][j] = NULL;

				for ( j=0; j<=size.h; j++ )
				{
					if ( ( (*maze)[i][j] = (Array1D)malloc( size.d + 1 ) ) == NULL )
					{
						DeallocateMaze( maze, size );
						return;
					}
					else
					{
						for ( k=0; k<=size.d; k++ )
							(*maze)[i][j][k] = 0;
					}
				}
			}
		}
	}
}

void OriginalMazeFactory::DeallocateMaze(Array3D *maze, location size)
{
	int	i, j;


	if ( *maze != NULL )
	{
		for ( i=0; i<=size.w; i++ )
		{
			if ( (*maze)[i] != NULL )
			{
				for ( j=0; j<=size.h; j++ )
				{
					if ( (*maze)[i][j] != NULL )
						free( (*maze)[i][j] );
				}
				free( (*maze)[i] );
			}
		}
		free( *maze );
		*maze = NULL;
	}
}

void OriginalMazeFactory::ClearMaze(Array3D maze, location size)
{
	int	i, j, k;

	for ( i=0; i<=size.w; i++ )
	for ( j=0; j<=size.h; j++ )
	for ( k=0; k<=size.d; k++ )
		maze[i][j][k] = 0;
}

