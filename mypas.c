/**@<mypas.c>::**/
#include <stdio.h>
#include <symtab.h>
#include <mypas.h>

FILE *source;

extern double acc;

int main (int argc, const char *argv[])
{
  source = stdin;
  if(argc > 1) source = fopen(argv[1], "r");
  lookahead = gettoken (source);
  mypas();
  symtab_list();

  return 0;
}
