/*
int kr_media_source_close(kr_media_source *media) {
  if (!media) return -1;
  return kr_file2_close(media->file);
}

kr_media_source *kr_media_source_open(kr_studio *studio,
 kr_media_source_setup *setup) {
  kr_studio_path *path;
  kr_file2_info finfo;
  *//*kr_demuxer_setup dmx_setup;*//*
  if (!studio || !setup) return NULL;
  if (setup->len > sizeof(path->info.media.len)) return NULL;
  path = kr_pool_slice(studio->pool);
  if (path == NULL) return NULL;
  path->info.type = KR_STUDIO_MEDIA_SOURCE;
  path->info.media.len = setup->len;
  strncpy(path->info.media.path, setup->path, path->info.media.len);
  path->media.file = kr_file2_open(studio->fset, setup->path, setup->len);
  if (path->media.file == NULL) {
    kr_pool_release(studio->pool, path);
    return NULL;
  }
  if (kr_file2_get_info(path->media.file, &finfo) != 0) {
    kr_file2_close(path->media.file);
    kr_pool_release(studio->pool, path);
    return NULL;
  }
  *//*dmx_setup.buf = kr_file2_get_data(path->media.file);
  dmx_setup.sz = finfo.sz;
  dmx_setup.pkt = &path->media.pkt;
  if (kr_demuxer_init(studio->dmxr, &dmx_setup) < 0) {
    kr_file2_close(path->media.file);
    kr_pool_release(studio->pool, path);
    return NULL;
  }*//*
  return &path->media;
}*/
