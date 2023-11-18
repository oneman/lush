#if !defined(_app_name_H)
# define _app_name_H (1)

#define KR_NAME_LEN 40

typedef struct kr_names kr_names;

typedef struct {
  int nnodes;
} kr_names_setup;

typedef struct {
  char buf[KR_NAME_LEN];
  int len;
} kr_name;

#define KR_NAME_NO_DELETE 1

typedef struct {
  int type;
  int perm;
  void *user;
} kr_name_ctx;

ssize_t kr_name_snprint(kr_name *name, char *str, size_t max);
int kr_name_has_ctx_type(kr_name *name, int type);
kr_name *kr_names_iter(kr_names *names, int *pos);
kr_name *kr_name_reverse(kr_names *names, void *user);
kr_name *kr_name_find(kr_names *names, char *str, size_t sz);
kr_name_ctx *kr_name_resolve_type(kr_names *names, int type, char *str, size_t sz);
kr_name_ctx *kr_name_resolve(kr_names *names, char *str, size_t sz);
kr_name_ctx *kr_name_resolve2(kr_names *names, char *str, size_t sz, char *str2,
 size_t sz2);
int kr_name_unlink(kr_names *names, kr_name *name, kr_name_ctx *ctx);
int kr_name_link(kr_names *names, kr_name *name, kr_name_ctx *ctx);
kr_name *kr_name_attach(kr_names *names, kr_name *name, kr_name *name2);
int kr_name_remove(kr_names *names, kr_name *name);
kr_name *kr_name_add_str(kr_names *names, char *name, size_t sz);
kr_name *kr_name_add(kr_names *names, kr_name *name);
int kr_names_destroy(kr_names *names);
kr_names *kr_names_create(kr_names_setup *setup);

#endif
