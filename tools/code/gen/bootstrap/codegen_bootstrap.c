#include "codegen_bootstrap_utils.h"

void print_usage(char *cmd) {
  printf("syntax:\n%s path prefix suffix outpath\n",cmd);
}

int main(int argc, char *argv[]) {

  header_data *hdata;
  char outc[256];
  char outh[256];
  char rpath[PATH_MAX];
  char *res;
  int n;
  FILE *header;
  FILE *bstrap_file;

  if (argc != 5) {
    print_usage(argv[0]);
    return 0;
  }

  hdata = hdata_init(MAX_HEADERS);
  res = realpath(argv[4],rpath);
  if (!res) return -1;

  snprintf(outc,sizeof(outc),"%s/bootstrapped.c",rpath);
  snprintf(outh,sizeof(outh),"%s/bootstrapped.h",rpath);

  header = fopen(outh,"w+");
  bstrap_file = fopen(outc,"w+");

  if (!header || !bstrap_file) {
    fprintf(stderr,"Invalid output file!\n");
    return 1;
  }

  n = gather_struct_definitions(hdata,"types",argv[1]);

  codegen_bootstrap(hdata,n,argv[2],argv[3],header);
  fprintf(header,"\ntypedef struct {\n  int type;\n  void *actual;\n} uber_St;\n\n");
  codegen_enum_utils(hdata,n,argv[2],argv[3],bstrap_file);

  fclose(header);
  fclose(bstrap_file);

  hdata_free(hdata, MAX_HEADERS);

  return 0;
}
