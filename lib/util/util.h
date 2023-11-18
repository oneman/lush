#if !defined(_util_util_H)
# define _util_util_H (1)
#include <stdint.h>
#include <unistd.h>
#include <krad/app/diff.h>
#include <krad/gen/metainfo.h>

typedef int (struct_cb)(const struct_info *s, int start);
typedef int (member_cb)(const member_info *m);

char *enum_to_str(int idx, int value);
int str_to_enum(int idx, char *str);
int enum_to_index(int idx, int value);
int info_helpers(void *info, int idx, int type);
int patch_path(kr_patch *patch, kr_path *path, int idx);
int patch_get_path(char *path, size_t sz, kr_patch *patch,
 int idx, void *info);
int path_get_type(kr_path *path, int idx);
int info_pack_unpack_ebml(kr_ebml *ebml, void *info, int idx, int ftype);
int info_pack_json(char *json, void *info, int max, int idx);
int metainfo_to_jschema(char *json, int max, int idx);
int info_unpack_json(char *json, void *info, int idx);
int info_to_text(char *text, void *info, int max, int idx);
int info_diff(kr_diffset *diffset, void *info1, void *info2, int idx);
int meta_loop(struct_cb *scb, member_cb *mcb);
char *path_enum_str(kr_path *path, int val, int idx);
int path_enum_val(kr_path *path, char *str, size_t len, int idx);
#endif
