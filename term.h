#ifndef _TERM_h
#define _TERM_h 1

#include <termios.h>
#include <stdio.h>

static struct termios old;

static void
term_init( void ) 
{
  struct termios term;
  tcgetattr(0, &term);
  tcgetattr(0, &old);
  term.c_lflag &= ~ICANON;
  term.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &term);
}

static void
term_shutdown( void )
{
  tcsetattr(0, TCSANOW, &old);
}

#endif /* _TERM_h */