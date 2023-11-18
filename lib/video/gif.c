#include <string.h>
#include <stdlib.h>
#include <gif_lib.h>
#include <krad/app/debug.h>
#include <krad/image/convert.h>
#include "gif.h"

struct kr_gif {
  uint8_t *data;
  size_t sz;
  size_t pos;
  int frame_num;
  int alpha;
  int tc;
  int error;
  uint8_t *px;
  GraphicsControlBlock gcb;
  GifFileType *gif;
};

static int gif_read(GifFileType *gif, GifByteType *data, int len) {
  kr_gif *kg;
  kg = (kr_gif *)gif->UserData;
  if (kg->pos + len > kg->sz) {
    len = kg->sz - kg->pos;
  }
  memcpy(data, kg->data + kg->pos, len);
  kg->pos += len;
  return len;
}

static inline int decode_line(kr_gif *kg, ColorMapObject *cm, uint8_t *outline,
 uint8_t *scanline, int line, int r, int c, int w, int h) {
  int ret;
  int i;
  int j;
  int outpx;
  int alpha;
  GifColorType *color;
  i = line;
  if ((i >= r) && (i < r + h)) {
    ret = DGifGetLine(kg->gif, scanline, w);
    if (ret == GIF_ERROR) {
      kg->error = 1;
      printke("Gif: getline");
      return -1;
    }
  }
  for (j = 0; j < kg->gif->SWidth; j++) {
    if ((j >= c) && (j < c + w) && (i >= r) && (i < r + h)) {
      if ((kg->gcb.TransparentColor > -1)
       && (scanline[j-c] == kg->gcb.TransparentColor)) {
        scanline[j-c] = kg->px[i * kg->gif->SWidth + j];
        if (kg->frame_num == 1) {
          kg->alpha = 0;
        }
        alpha = kg->alpha;
      } else {
        alpha = 255;
      }
      color = &cm->Colors[scanline[j-c]];
      if (kg->gcb.DisposalMode == DISPOSE_DO_NOT) {
        kg->px[i * kg->gif->SWidth + j] = scanline[j-c];
      }
      if (kg->gcb.DisposalMode == DISPOSE_BACKGROUND) {
      }
    } else {
      color = &cm->Colors[kg->px[i * kg->gif->SWidth + j]];
      alpha = kg->alpha;
    }
    if (outline != NULL) {
      outpx = j * 4;
      outline[outpx + 0] = color->Blue;
      outline[outpx + 1] = color->Green;
      outline[outpx + 2] = color->Red;
      outline[outpx + 3] = alpha;
    }
  }
  return 0;
}

static inline uint8_t *get_outline(kr_image *image, int num) {
  if (image == NULL) return NULL;
  return image->px[0] + (image->ps[0] * num);
}

static int decode_frame(kr_gif *kg, kr_frame *frame) {
  uint8_t *scan;
  ColorMapObject *cm;
  int il_offset[] = { 0, 4, 2, 1 };
  int il_jumps[] = { 8, 8, 4, 2 };
  int ret;
  int i;
  int j;
  GifImageDesc *gi;
  kr_image *image;
  if (frame == NULL) {
    image = NULL;
  } else {
    image = &frame->image;
    image->info.w = kg->gif->SWidth;
    image->info.h = kg->gif->SHeight;
    image->info.fmt = KR_PIXELS_ARGB;
    image->ps[0] = image->info.w * 4;
  }
  ret = DGifGetImageDesc(kg->gif);
  if (ret == GIF_ERROR) {
    printke("Gif: get image desc");
    kg->error = 1;
    return -1;
  }
  gi = &kg->gif->Image;
  cm = gi->ColorMap ? gi->ColorMap : kg->gif->SColorMap;
  if (cm == NULL) {
    kg->error = 1;
    printke("Gif: Colormap missing");
    return -1;
  }
  scan = alloca(gi->Width);
  memset(scan, 0, gi->Width);
  if ((gi->Left + gi->Width > kg->gif->SWidth) || (gi->Top + gi->Height > kg->gif->SHeight)) {
    printke("Image %d is not confined to screen", kg->frame_num);
    kg->error = 1;
    return -1;
  }
  if (frame != NULL) {
    frame->info.tc = kg->tc;
    /*frame->td = kg->gcb.DelayTime * 10;*/
    /*printk("Image %d tc %"PRIu64" ms dur %u at (%d, %d) [%dx%d]", kg->frame_num, frame->tc,
     frame->td, gi->Left, gi->Top, gi->Width, gi->Height);*/
    kg->frame_num++;
  }
  kg->tc += kg->gcb.DelayTime * 10;
  if (gi->Interlace) {
    for (i = 0; i < 4; i++) {
      for (j = il_offset[i]; j < kg->gif->SHeight;  j += il_jumps[i]) {
        ret = decode_line(kg, cm, get_outline(image, j), scan, j, gi->Top, gi->Left,
         gi->Width, gi->Height);
        if (ret != 0) return -1;
      }
    }
  } else {
    for (i = 0; i < kg->gif->SHeight; i++) {
      ret = decode_line(kg, cm, get_outline(image, i), scan, i, gi->Top, gi->Left,
       gi->Width, gi->Height);
      if (ret != 0) return -1;
    }
  }
  return 1;
}

static int find_frame(kr_gif *kg) {
  int ret;
  GifRecordType record_type;
  int extcode;
  GifByteType *extdata;
  if (kg == NULL) return -1;
  for (;;) {
    ret = DGifGetRecordType(kg->gif, &record_type);
    if (ret == GIF_ERROR) {
      kg->error = 1;
      printke("Gif: Get record type fail");
      return -1;
    }
    switch (record_type) {
      case IMAGE_DESC_RECORD_TYPE:
        return 1;
      case TERMINATE_RECORD_TYPE:
        return 0;
      case EXTENSION_RECORD_TYPE:
        ret = DGifGetExtension(kg->gif, &extcode, &extdata);
        if (ret == GIF_ERROR) {
          kg->error = 1;
          printke("Gif: get extension");
          return -1;
        }
        if (extcode == GRAPHICS_EXT_FUNC_CODE) {
          ret = DGifExtensionToGCB(extdata[0], extdata + 1, &kg->gcb);
          if (ret == GIF_ERROR) {
            kg->error = 1;
            printke("Gif: ExtensionToGCB");
            return -1;
          }
        }
        while (extdata != NULL) {
          ret = DGifGetExtensionNext(kg->gif, &extdata);
          if (ret == GIF_ERROR) {
            kg->error = 1;
            printke("Gif: Extention next");
            return -1;
          }
        }
        break;
      default:
        kg->error = 1;
        printke("Gif: Unknown record type");
        return -1;
    }
  }
}

int kr_gif_skip_frame(kr_gif *kg) {
  int ret;
  if (kg == NULL) return -1;
  ret = find_frame(kg);
  if (ret == 1) {
    return decode_frame(kg, NULL);
  }
  return ret;
}

int kr_gif_get_frame(kr_gif *kg, kr_frame *frame) {
  int ret;
  if (kg == NULL) return -1;
  if (frame == NULL) return -1;
  ret = find_frame(kg);
  if (ret == 1) {
    return decode_frame(kg, frame);
  }
  return ret;
}

int kr_gif_get_info(kr_gif *kg, kr_gif_info *info) {
  if (kg == NULL) return -1;
  if (info == NULL) return -1;
  info->w = kg->gif->SWidth;
  info->h = kg->gif->SHeight;
  return 0;
}

int kr_gif_close(kr_gif *kg) {
  int ret;
  int code;
  if (kg == NULL) return -1;
  ret = DGifCloseFile(kg->gif, &code);
  if (ret == GIF_ERROR) {
    printke("Gif: failure on decoder close: %s", GifErrorString(code));
  }
  free(kg->px);
  free(kg);
  kg = NULL;
  return 0;
}

kr_gif *kr_gif_open(kr_data *data) {
  kr_gif *kg;
  kg = kr_allocz(1, sizeof(kr_gif));
  kg->data = data->buf;
  kg->sz = data->info.sz;
  kg->gcb.TransparentColor = NO_TRANSPARENT_COLOR;
  kg->gif = DGifOpen(kg, gif_read, &kg->error);
  if (kg->gif == NULL) {
    free(kg);
    return NULL;
  }
  kg->px = kr_alloc(kg->gif->SWidth * kg->gif->SHeight);
  memset(kg->px, kg->gif->SBackGroundColor, kg->gif->SWidth * kg->gif->SHeight);
  kg->alpha = 255;
  return kg;
}
