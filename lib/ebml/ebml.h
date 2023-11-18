#if !defined(_ebml_ebml_H)
# define _ebml_ebml_H (1)

#include <stddef.h>
#include <inttypes.h>

typedef struct kr_ebml kr_ebml2_t;
typedef struct kr_ebml kr_ebml;

struct kr_ebml {
  size_t pos;
  size_t len;
  uint8_t *buf;
  uint8_t *bufstart;
};

size_t kr_ebml_sizeof(void);
kr_ebml *kr_ebml_init(uint8_t *mem);
size_t kr_ebml_position(kr_ebml *ebml);

int kr_ebml2_destroy(kr_ebml **ebml);
kr_ebml *kr_ebml2_create(void);
int kr_ebml2_advance(kr_ebml *ebml, size_t bytes);
int kr_ebml2_set_buffer(kr_ebml *ebml, uint8_t *buffer, size_t len);
int kr_ebml_pack(kr_ebml *ebml, void *buffer, size_t len);

void kr_ebml2_revpack2(kr_ebml *ebml, void *buffer);
void kr_ebml2_revpack4(kr_ebml *ebml, void *buffer);

int kr_ebml_pack_header(kr_ebml *e, char *doctype, uint32_t ver, uint32_t read_version);
int kr_ebml_pack_element(kr_ebml *ebml, uint32_t element);
int kr_ebml2_start_element(kr_ebml *ebml, uint32_t element, uint8_t **pos);
int kr_ebml2_finish_element(kr_ebml *ebml, uint8_t *element_position);
int kr_ebml_pack_data_size(kr_ebml *ebml, uint64_t data_size);
int kr_ebml_pack(kr_ebml *ebml, void *buffer, size_t len);
int kr_ebml_pack_element(kr_ebml *ebml, uint32_t element);
int kr_ebml_pack_void(kr_ebml *ebml, uint64_t length);
int kr_ebml_pack_data(kr_ebml *e, uint32_t element, void *data, uint64_t len);
int kr_ebml_pack_string(kr_ebml *ebml, uint32_t element, char *string);
int kr_ebml_pack_float(kr_ebml *ebml, uint32_t element, float number);
int kr_ebml_pack_double(kr_ebml *ebml, uint32_t element, double number);
int kr_ebml_pack_int8(kr_ebml *ebml, uint32_t element, int8_t number);
int kr_ebml_pack_uint8(kr_ebml *ebml, uint32_t element, uint8_t number);
int kr_ebml_pack_int16(kr_ebml *ebml, uint32_t element, int16_t number);
int kr_ebml_pack_uint16(kr_ebml *ebml, uint32_t element, uint16_t number);
int kr_ebml_pack_int32(kr_ebml *ebml, uint32_t element, int32_t number);
int kr_ebml_pack_uint32(kr_ebml *ebml, uint32_t element, uint32_t number);
int kr_ebml_pack_int64(kr_ebml *kr_ebml, uint32_t element, int64_t number);
int kr_ebml_pack_uint64(kr_ebml *kr_ebml, uint32_t element, uint64_t number);

int kr_ebml2_unpack_id (kr_ebml2_t *ebml, uint32_t *element, uint64_t *data_size);

int kr_ebml2_unpack_data (kr_ebml2_t *ebml, void *data, size_t len);
int kr_ebml2_unpack_string (kr_ebml2_t *ebml, char *string, size_t len);
int kr_ebml2_unpack_int8 (kr_ebml2_t *ebml, int8_t *integer, size_t len);
int kr_ebml2_unpack_uint8 (kr_ebml2_t *ebml, uint8_t *integer, size_t len);
int kr_ebml2_unpack_int16 (kr_ebml2_t *ebml, int16_t *integer, size_t len);
int kr_ebml2_unpack_uint16 (kr_ebml2_t *ebml, uint16_t *integer, size_t len);
int kr_ebml2_unpack_int32 (kr_ebml2_t *ebml, int32_t *integer, size_t len);
int kr_ebml2_unpack_uint32 (kr_ebml2_t *ebml, uint32_t *integer, size_t len);
int kr_ebml2_unpack_int64 (kr_ebml2_t *ebml, int64_t *integer, size_t len);
int kr_ebml2_unpack_uint64 (kr_ebml2_t *ebml, uint64_t *integer, size_t len);
int kr_ebml2_unpack_float (kr_ebml2_t *ebml, float *real, size_t len);
int kr_ebml2_unpack_double (kr_ebml2_t *ebml, double *real, size_t len);

int kr_ebml2_unpack_element_data (kr_ebml2_t *ebml, uint32_t *id, void *data, size_t maxlen);
int kr_ebml2_unpack_element_string (kr_ebml2_t *ebml, uint32_t *id, char *string, size_t maxlen);
int kr_ebml2_unpack_element_int8 (kr_ebml2_t *ebml, uint32_t *id, int8_t *integer);
int kr_ebml2_unpack_element_uint8 (kr_ebml2_t *ebml, uint32_t *id, uint8_t *integer);
int kr_ebml2_unpack_element_int16 (kr_ebml2_t *ebml, uint32_t *id, int16_t *integer);
int kr_ebml2_unpack_element_uint16 (kr_ebml2_t *ebml, uint32_t *id, uint16_t *integer);
int kr_ebml2_unpack_element_int32 (kr_ebml2_t *ebml, uint32_t *id, int32_t *integer);
int kr_ebml2_unpack_element_uint32 (kr_ebml2_t *ebml, uint32_t *id, uint32_t *integer);
int kr_ebml2_unpack_element_int64 (kr_ebml2_t *ebml, uint32_t *id, int64_t *integer);
int kr_ebml2_unpack_element_uint64 (kr_ebml2_t *ebml, uint32_t *id, uint64_t *integer);
int kr_ebml2_unpack_element_float (kr_ebml2_t *ebml, uint32_t *id, float *real);
int kr_ebml2_unpack_element_double (kr_ebml2_t *ebml, uint32_t *id, double *real);

int kr_ebml2_unpack_header (kr_ebml2_t *ebml, char *doctype, int doctype_maxlen,
                            uint32_t *version, uint32_t *read_version);

int kr_ebml2_at_end (kr_ebml2_t *ebml);

#endif
