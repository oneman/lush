#include "codegen_utils.h"

void print_usage(char *cmd) {
  printf("  Generates JSchema for the structs gathered.\n");
  printf("  syntax:\n   %s libpath prefix suffix outfile.yml\n",cmd);
}

int main(int argc, char *argv[]) {

  header_data *hdata;
  struct_data *defs[MAX_HEADERS * MAX_HEADER_DEFS];
  int n;
  int i;
  int j;
  int idx;
  FILE *out;

  if (argc != 5) {
    print_usage(argv[0]);
    return 1;
  }

  memset(defs,0,sizeof(defs));
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

  for (i = 0; i < n; i++) {
    for (j = 0; j < hdata[i].def_count; j++) {
      if ( (idx = codegen_string_to_enum(hdata[i].defs[j].info.name)) ) {
        defs[idx-1] = &hdata[i].defs[j];
      }
    }
  }

  codegen_jschema(defs,MAX_HEADERS * MAX_HEADER_DEFS,argv[2],argv[3],out);

  for (i = 0; i < MAX_HEADERS; i++) {
    free(hdata[i].defs);
    free(hdata[i].targets);
  }

  free(hdata);
  fclose(out);

  return 0;
}
