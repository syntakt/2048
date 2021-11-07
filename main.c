/**
 * 2048 in C89
 * 2048/main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include "term.h"

typedef struct i_grid
{
  int s;
  int *g;
} grid_t;

enum
{
  UP,
  LEFT,
  DOWN,
  RIGHT
};

static int _Spawnable[] =
{
  2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048
};

static int _Score = 0;

static void
term_clear( void )
{
  system( "clear" );
}

static void
term_render( grid_t *_grid )
{
  int i, j, b;

  printf( "SCORE: %d\n", _Score );

  for ( i = 0; i < _grid->s; i++ ) {
    for ( j = 0; j < _grid->s; j++ ) {
      b = _grid->g[i * _grid->s + j];

      if ( b > 0 )
        printf( "%-*d|", 4, b );
      else printf( "%*s", 5, "|" );
    }

    printf( "\n" );
  }
  
  fflush( stdout );
}

static int
term_input( )
{
  int in = getchar( );

  switch ( in ) {
  case 'i': return 0;
  case 'j': return 1;
  case 'k': return 2;
  case 'l': return 3;
  default:  return term_input( );
  }
}

static int
full( grid_t *_grid )
{
  int i;

  for ( i = 0; i < _grid->s * _grid->s; i++ ) {
    if ( _grid->g[i] == 0 )
      return 0;
  }

  return 1;
}

static void
spawn( grid_t *_grid, int _range )
{
  int i, v;

  i = rand() % ( _grid->s * _grid->s );
  v = rand() % _range;

  if ( full( _grid ) )
    return;

  if ( _grid->g[i] != 0 )
    return spawn( _grid, _range );
    
  _grid->g[i] = _Spawnable[v];
}

static int
move_index( grid_t *_grid, int _in, int _j, int _k, int _a )
{
  switch ( _in ) {
  case UP:    
  case DOWN:  return ( ( _k + _a ) * _grid->s + _j );
  case LEFT:  
  case RIGHT: return ( ( _k + _a ) + _grid->s * _j );
  }

  return -1;
}

static int
move( grid_t *_grid, int _in )
{
  int orig = _Score;
  int i, j, k, d;
  int *c, *n;

  /**
   * i: Loops through the same row/col `i` times.
   * j: The inactive axis
   * k: The index of the axis that a move is being applied to
   * d: Loop direction (is it increasing or decreasing)
   * c: Current Cell
   * n: Next Cell
   */

  /* This function slides rows/cols vertically or horizontally.
     Direction is controlled by if loop `k` is decreasing or increasing (k++ or k--)
     controlled by `d` */

  switch ( _in ) {
  case UP:    d = -1; break;
  case DOWN:  d =  1; break;
  case LEFT:  d = -1; break;
  case RIGHT: d =  1; break;
  }

  for ( i = 0; i < _grid->s; i++ ) {    
    for ( j = 0; j < _grid->s; j++ ) {
      for ( k = ( d > 0 ) ? 0 : _grid->s - 1; ( d > 0 ) ? ( k < _grid->s - 1 ) : ( k > 0 ); k += d ) {
        c = &_grid->g[ move_index( _grid, _in, j, k, 0 ) ];
        n = &_grid->g[ move_index( _grid, _in, j, k, d ) ];

        /* If the next cell is empty move into it */
        if ( *n == 0 ) {
          *n = *c;
          *c = 0;
        /* If the next cell is the same number, add the current cell into it */
        } else if ( *n == *c ) {
          *n += *c;
          _Score += ( *c * 2 ); 
          *c = 0;
        }
      }
    }
  }

  /* TODO: Fix, this don't work */
  return ( full( _grid ) && _Score == orig ) ? 1 : 0;
}

int
main( void )
{
  grid_t grid;
  int stuck = 0;

  grid.s = 4; /* Grid Scale */
  grid.g = calloc( sizeof( int ), grid.s * grid.s );

  srand( time( NULL ) );
  term_init( );
  
  do {
    term_clear( );

    spawn( &grid, 1 );
    term_render( &grid );

    stuck = move( &grid, term_input( ) );
  } while ( !stuck );
 
  term_shutdown( );
  free( grid.g );

  return 0;
}