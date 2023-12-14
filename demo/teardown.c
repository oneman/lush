void teardown(uint8_t *bs, size_t sz) {
  int i;
  int n[256];
  printf("! %p %lu bytes\n", bs, sz);
  for (i = 0; i < 256; i++) {
    n[i] = 0;
  }
  for (i = 0; i < sz; i++) {
    n[bs[i]]++;
  }
  for (i = 0; i < 256; i++) {
    if (i == SP) {
      printf("SP ' ': %d\n", n[i]);
      continue;
    }
    if (isascii_cc(i)) {
      printf("CC %s: %d\n", ascii_cc_str[i], n[i]);
      continue;
    }
    if (isascii_text(i)) {
     printf("TX %d %c: %d\n", i, i, n[i]);
     continue;
    }
    if (i == DEL) {
      printf("DEL '': %d\n", n[i]);
      continue;
    }
    if (i > DEL) {
      printf("HIGH: %d\n", n[i]);
    }
  }
}
