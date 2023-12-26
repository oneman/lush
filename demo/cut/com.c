void comcut(uint8_t *bs, size_t sz) {
  size_t i;
  i = 0;
  printf("  comp len %lu: ", sz);
  for (i = 0; i < sz; i++) {
    printf("%c", bs[i]);
  }
  printf("\n");
}
