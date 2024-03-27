int samebytes(u8 *aa, u8 *bb, size_t len) {
  if ((memcmp(aa, bb, len)) == 0) return 1;
  return 0;
}

size_t binary_scan(u8 *buf, size_t sz) {
  size_t n;
  size_t z;
  z = png_data_scan(buf, sz);
  if (sz == z) return z;
  /*printf("Binary data: %lu\n\n", sz);*/
  return 0;
  for (n = 0; n < sz;) {
    //usleep(10000);
    //printf("n %u sz %u\n", n, sz);
    u8 b = buf[n];
    z = text_len(buf + n, sz - n);
    if (z == 0) { printf("%02X", b); n++; continue; }
    for (int i = 0; z > i; i++) putchar(buf[n] + i);
    n += z;
    continue;
  }
  return sz;
}

size_t data_scan(uint8_t *buf, size_t sz) {
  int n = text_len(buf, sz);
  if (n == sz) return text_scan(buf, sz);
  return binary_scan(buf, sz);
}
