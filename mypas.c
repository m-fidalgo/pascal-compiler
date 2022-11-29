/**@<mypas.c>::**/
#include <stdio.h>
#include <mypas.h>

FILE *source;

extern double acc;

int main (int argc, const char *argv[])
{
  source = stdin;
  if(argc > 1) source = fopen(argv[1], "r");
  lookahead = gettoken (source);
  mypas();

  return 0;
}
