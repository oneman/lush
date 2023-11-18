static int validate_local_client_header(uint8_t *header, size_t sz);
static int validate_local_client(kr_app_server_client *client);

static int validate_local_client_header(uint8_t *header, size_t sz) {
  kr_ebml2_t ebml;
  int ret;
  char doctype[32];
  uint32_t version;
  uint32_t read_version;
  size_t app_client_doctype_len;
  app_client_doctype_len = strlen(KR_APP_CLIENT_DOCTYPE);
  kr_ebml2_set_buffer(&ebml, header, sz);
  ret = kr_ebml2_unpack_header(&ebml, doctype, 32, &version, &read_version);
  if (ret > 0) {
    if ((version == KR_APP_DOCTYPE_VERSION)
     && (read_version == KR_APP_DOCTYPE_READ_VERSION)
     && (strlen(doctype) == app_client_doctype_len)
     && (strncmp(doctype, KR_APP_CLIENT_DOCTYPE, app_client_doctype_len) == 0)) {
        return ret;
    } else {
      printke("App Server: Header validation failed - mismatch %u %u %s",
       version, read_version, doctype);
    }
  } else {
    printke("App Server: Header validation failed -  %d", ret);
  }
  return -1;
}

static int validate_local_client(kr_app_server_client *client) {
  kr_ebml2_t ebml;
  int ret;
  ret = validate_local_client_header(client->in->rd_buf, client->in->len);
  if (ret > 0) {
    kr_io2_pulled(client->in, ret);
    client->type = KR_APP_CLIENT_LOCAL_VALID;
    kr_ebml2_set_buffer(&ebml, client->out->buf, client->out->space);
    kr_ebml_pack_header(&ebml, KR_APP_SERVER_DOCTYPE, KR_APP_DOCTYPE_VERSION,
     KR_APP_DOCTYPE_READ_VERSION);
    if (ebml.pos < 1) {
      printke("App Server: Error packing client header");
    } else {
      printk("ret is %d ebmlpos is %d", ret, ebml.pos);
      kr_io2_advance(client->out, ebml.pos);
      return 0;
    }
  }
  printke("App Server: Invalid client!");
  return -1;
}
