#include "structs_gathering.h"

void print_usage(char *cmd) {
  printf("syntax:\n%s path prefix suffix format\n",cmd);
  printf("Available formats:\n");
  printf("names --> prints structure names only.\n");
  printf("info --> prints info about the structs matched.\n");
  printf("header --> prints the body of the structs matched.\n");
  printf("members --> prints info about the members of the structs matched.\n");
  printf("members_data_info --> prints info about the members possible data values.\n");
  printf("targets --> prints names and codegen targets if they exist\n");
}

int main(int argc, char *argv[]) {

  header_data *hdata;
  int n;
  int i;

  if (argc != 5) {
    print_usage(argv[0]);
    return 0;
  }

  hdata = hdata_init(MAX_HEADERS);

  n = gather_struct_definitions(hdata,"types",argv[1]);

  for (i = 0; i < n; i++) {
    if (print_structs_defs(&hdata[i],argv[2],argv[3],argv[4])) {
      fprintf(stderr,"Invalid print format!\n");
      print_usage(argv[0]);
      return 0;
    }
  }

  hdata_free(hdata, MAX_HEADERS);

  return 0;
}
