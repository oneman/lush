#if !defined(_io_file_H)
# define _io_file_H (1)

typedef struct kr_file kr_file;

int file_exists(char *path);
int64_t file_size(char *path);

kr_file *kr_file_open_stdin();
kr_file *kr_file_open_stdout();

kr_file *kr_file_open(char *path);
kr_file *kr_file_create(char *path);

ssize_t kr_file_read(kr_file *file, void *buffer, size_t len);
ssize_t kr_file_write(kr_file *file, void *buffer, size_t len);

int64_t kr_file_size(kr_file *file);
char *kr_file_path(kr_file *file);
int kr_file_fd(kr_file *file);
int kr_file_close(kr_file **file);

#endif
