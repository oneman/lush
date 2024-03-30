#ifndef C_TEXT
#define C_TEXT

/* A proper normal sentence is a sequence of alphabetic words no
 * longer than 78, most often less than 26. The final word must
 * be a period, and maybe decorated with exclimation or question
 * marking. All non-alphabetic marks such as emoji, emoticons,
 * dashes, arrows unicode characters or doodles must be translated
 * into alphabetic representations in exploded view.
 *
 * document chain?
 *
 */

#define UNICODES 1112064
#define PHY_C 299792458

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u64;
#define fubar exit(1);

typedef struct {
  size_t sz;
  u64 spaces;
  u64 letters;
  u64 numbers;
  u64 dodads;
  u64 uppercase_letters;
  u64 vowels;
  u64 consonants;
  u64 sylables;
  u64 words;
  u64 n[128];
} text_nfo;

void anal_text(u8 *buf, size_t sz) {
  if (!buf) fubar;
  if (sz < 1) fubar;
  /* Assumptions: We have one line of valid ascii text */
  text_nfo nfo;
  nfo.sz = sz;
  int n = 0;
  for (n = 0; n < sz; n++) {
    char c = buf[n];
    if (c == ' ') {
      nfo.spaces++;
      continue;
    }
    if (isalpha(c)) {
      nfo.letters++;
    } else if (isdigit(c)) {
      nfo.numbers++;
    }
  }
  printf("Length: %lu\n", nfo.sz);
  printf("Numbers: %lu\n", nfo.numbers);
  printf("Letters: %lu\n", nfo.letters);
  printf("Spaces: %lu\n", nfo.spaces);
  printf("Length: %lu characters\n", nfo.sz);
}

void anal_test() {
  char *customer_statement = "\
Let's do that thing with, as an inline example, this silly sentence, the one\
your reading right here starting with Let's and ending soon after here with\
some immediate regrets.";
  anal_text(customer_statement, strlen(customer_statement));
}

/*
13312 - 19893 CJK Ideographs Extension A
19968 - 40869 CJK Ideographs
44032 - 55203 Hangul Syllables
55296 - 56191 Non-Private Use High Surrogates
56192 - 56319 Private Use High Surrogates
56320 - 57343 Low Surrogates
57344 - 63743 The Private Use Area
983040 - 1048573 Private Use
1048576 - 1114109 Private Use*/

u8 is_ascii_blank(u8 byte) {
  if (byte == SP) return 1;
  if (byte == LF) return 1;
  if (byte == CR) return 1;
  if (byte == VT) return 1;
  if (byte == HT) return 1;
  if (byte == FF) return 1;
  return 0;
}

u8 is_ascii_char(u8 byte) {
  if ((byte > SP) && (byte < DEL)) {
    return 1;
  }
  return 0;
}

int is_unicode_head(u8 byte) {
  if (byte <= 191) return 0;
  if (byte >= 248) return 0;
  if (byte <= 223) return 2;
  if (byte <= 239) return 3;
  if (byte <= 247) return 4;
  return 0;
}

int is_unicode_tail(u8 byte) {
  if ((byte >= 128) && (byte <= 191)) return 1;
  return 0;
}

int is_unicode_neckbeard(u8 head, u8 neck) {
  if (head == 224) {
    if ((neck >= 160) && (neck <= 191)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  if (head == 237) {
    if ((neck >= 128) && (neck <= 159)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  if (head == 240) {
    if ((neck >= 144) && (neck <= 191)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  if (head == 244) {
    if ((neck >= 128) && (neck <= 143)) return 1;
    printk("unicode neckbeard error!");
    return 0;
  }
  return is_unicode_tail(neck);
}

size_t text_len(u8 *buf, size_t sz) {
  u8 byte;
  size_t i;
  for (i = 0; i < sz; i++) {
    byte = buf[i];
    if (is_ascii_char(byte)) continue;
    if (is_ascii_blank(byte)) continue;
    int u = is_unicode_head(byte);
    if (u < 2) break;
    if ((i + u) > sz) break;
    if (u == 2) {
      if (is_unicode_tail(buf[i + 1])) {
        i += 1;
        continue;
      }
      break;
    } else if (u == 3) {
      if ((is_unicode_neckbeard(byte, buf[i + 1]))
       && (is_unicode_tail(buf[i + 2]))) {
        i += 2;
        continue;
      }
      break;
    } else if (u == 4) {
      if ((is_unicode_neckbeard(byte, buf[i + 1]))
       && (is_unicode_tail(buf[i + 2]))
       && (is_unicode_tail(buf[i + 3]))) {
        i += 3;
        continue;
      }
      break;
    }
  }
  return i;
}
#endif
