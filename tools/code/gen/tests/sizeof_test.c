#include "codegen_utils.h"

void print_usage(char *cmd) {
  printf("  Generates compilable C file which prints out sizeofs for the gathered structs.\n");
  printf("  syntax:\n   %s libpath prefix suffix outfile.c\n",cmd);
}

int main(int argc, char *argv[]) {

  header_data *hdata;
  int n;
  int i;
  FILE *out;

  if (argc != 5) {
    print_usage(argv[0]);
    return 1;
  }

  hdata = calloc(MAX_HEADERS,sizeof(header_data));

  for (i = 0; i < MAX_HEADERS; i++) {
    hdata[i].defs = calloc(MAX_HEADER_DEFS,sizeof(struct_data));
    hdata[i].targets = calloc(MAX_TARGETS,sizeof(cgen_target));
  }

  n = gather_struct_definitions(hdata,"kr",argv[1]);

  out = fopen(argv[4],"w+");

  if (!out) {
    fprintf(stderr,"Error opening file %s\n",argv[4]);
    exit(1);
  }

  codegen_sizeof(hdata,n,argv[2],argv[3],out);

  for (i = 0; i < MAX_HEADERS; i++) {
    free(hdata[i].defs);
    free(hdata[i].targets);
  }

  free(hdata);
  fclose(out);

  return 0;
}
