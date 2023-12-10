void teardown(uint8_t *bs, size_t sz) {
  int i;
  int bitcnt[256];
  printf("! %p %lu bytes\n", bs, sz);

  for (i = 0; i < 256; i++) {
    bitcnt[i] = 0;
  }
  for (i = 0; i < sz; i++) {
    bitcnt[bs[i]]++;
  }
  for (i = 0; i < 256; i++) {
    printf("%d: %d\n", i, bitcnt[i]);
  }
}
