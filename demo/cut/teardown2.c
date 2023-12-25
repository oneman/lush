void teardown(uint8_t *bs, size_t sz) {
  char b;
  int i;
  int high;
  int high2;
  int n[256];
  int u2 = 0;
  int u3 = 0;
  int u4 = 0;
  int ub = 0;
  int utb = 0;
  high = 0;
  high2 = 0;
  printf("! %p %lu bytes\n", bs, sz);
  for (i = 0; i < 256; i++) {
    n[i] = 0;
  }
  for (i = 0; i < sz; i++) {
    n[bs[i]]++;
    if (bs[i] < 128) continue;
    high2++;
    b = bs[i];
    if ((b & 0b11110000) == 0b11110000) {
      u4 += 4;
      ub += 21;
      i += 3;
      continue;
    }
    if ((b & 0b11100000) == 0b11100000) {
      u3 += 3;
      ub += 16;
      i += 2;
      continue;
    }
    if ((b & 0b11000000) == 0b11000000) {
      u2 += 2;
      ub += 11;
      i += 1;
      continue;
    }
  }
  for (i = 0; i < 256; i++) {
    if (isascii_cc(i)) {
      printf("CC %s: %d\n", ascii_cc_str[i], n[i]);
      continue;
    }
    if (i == SP) {
      printf("SP ' ': %d\n", n[i]);
      continue;
    }
    if (i == DEL) {
      printf("DL '': %d\n", n[i]);
      continue;
    }
    if (isascii_text(i)) {
     printf("TX %d %c: %d\n", i, i, n[i]);
     continue;
    }
    if (i > 127) {
     printf("HI %d: %d\n", i, n[i]);
     high += n[i];
     continue;
    }
  }
  utb = u4 + u3 + u2;
  //printf("total high characters: %d\n", high);
  //printf("total high2 characters: %d\n", high2);
  printf("total unicode characters: %d\n", high2);
  printf("4byte unicode chars: %d\n", u4);
  printf("3byte unicode chars: %d\n", u3);
  printf("2byte unicode chars: %d\n", u2);
  printf("total unicode bytes: %d\n", utb);
  printf("total ascii bytes: %lu\n", sz - utb);
  printf("total bytes: %lu\n", sz);
}
