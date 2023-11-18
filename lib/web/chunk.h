#if !defined(_web_chunk_H)
# define _web_chunk_H (1)

#define KR_CHUNK_TRAILER "\r\n"
#define KR_CHUNK_TRAILER_LEN 2
#define KR_CHUNKING_TRAILER "0\r\n\r\n"
#define KR_CHUNKING_TRAILER_LEN 5
#define KR_CHUNK_HDR_MAX_LEN 10

typedef enum {
  KR_CHUNKED_INIT = 0,
  KR_CHUNKED_CHUNK,
  KR_CHUNKED_DONE
} kr_chunked_state;

typedef struct {
  uint64_t len;
  uint64_t header_len;
  uint64_t nchunks;
  uint64_t nbytes;
  uint64_t overhead;
  kr_chunked_state state;
} kr_chunked_tracker;

#endif
