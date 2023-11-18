#if !defined (_web_crate_H)
#define _web_crate_H (1)

#include <krad/web/http/http.h>

const kr_http_response_type kr_res_to_http_res_type(kr_response res);
int kr_http_request_to_crate(kr_crate *crate, kr_http_request *request);

#endif
