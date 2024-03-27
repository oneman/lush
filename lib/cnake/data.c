int samebytes(u8 *aa, u8 *bb, size_t len) {
  if ((memcmp(aa, bb, len)) == 0) return 1;
  return 0;
}

size_t byte_scan(u8 *dat, size_t sz) {
  size_t n;
  size_t z;
  z = png_data_scan(dat, sz);
  if (sz == z) return z;
  /*printf("Binary data: %lu\n\n", sz);*/
  return 0;
  for (n = 0; n < sz;) {
    //usleep(10000);
    //printf("n %u sz %u\n", n, sz);
    u8 b = dat[n];
    z = text_len(dat + n, sz - n);
    if (z == 0) { printf("%02X", b); n++; continue; }
    for (int i = 0; z > i; i++) putchar(dat[n] + i);
    n += z;
    continue;
  }
  return sz;
}

size_t data_scan(u8 *dat, size_t sz) {
  printf("    Size: ");
  if (sz > 1000 * 1000) {
    printf("%lu.%lum\n",  sz/1000000, (sz % 1000000) / 1000);
  } else {
    if (sz < 1000) printf("%lubytes\n", sz);
    if (sz >= 1000) printf("%luk\n",  sz/1000);
  }
  int n = text_len(dat, sz);
  if (n == sz) {
    printf("    Type: Text\n");
    return text_scan(dat, sz);
  }
  printf("    Type: Binary\n");
  return byte_scan(dat, sz);
}
