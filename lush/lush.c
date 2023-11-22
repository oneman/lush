#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pak_header_s
{
  char id[4];
  int offset;
  int size;
} pak_header_t;

typedef struct pak_file_s
{
  char name[56];
  int offset;
  int size;
} pak_file_t;



