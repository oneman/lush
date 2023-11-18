#include <stdio.h>
#include <string.h>

char *mime_to_ext(char *mime) {
  if (!mime) return NULL;
  if (!strncmp(mime, "text/html", 9)) {
    return "html";
  }
  if (!strncmp(mime, "text/css", 8)) {
    return "css";
  }
  if (!strncmp(mime, "text/xml", 8)) {
    return "xml";
  }
  if (!strncmp(mime, "image/gif", 9)) {
    return "gif";
  }
  if (!strncmp(mime, "image/jpeg", 10)) {
    return "jpeg";
  }
  if (!strncmp(mime, "application/javascript", 22)) {
    return "js";
  }
  if (!strncmp(mime, "application/atom+xml", 20)) {
    return "atom";
  }
  if (!strncmp(mime, "application/rss+xml", 19)) {
    return "rss";
  }
  if (!strncmp(mime, "text/mathml", 11)) {
    return "mml";
  }
  if (!strncmp(mime, "text/plain", 10)) {
    return "txt";
  }
  if (!strncmp(mime, "text/vnd.sun.j2me.app-descriptor", 32)) {
    return "jad";
  }
  if (!strncmp(mime, "text/vnd.wap.wml", 16)) {
    return "wml";
  }
  if (!strncmp(mime, "text/x-component", 16)) {
    return "htc";
  }
  if (!strncmp(mime, "image/png", 9)) {
    return "png";
  }
  if (!strncmp(mime, "image/tiff", 10)) {
    return "tif";
  }
  if (!strncmp(mime, "image/vnd.wap.wbmp", 18)) {
    return "wbmp";
  }
  if (!strncmp(mime, "image/x-icon", 12)) {
    return "ico";
  }
  if (!strncmp(mime, "image/x-jng", 11)) {
    return "jng";
  }
  if (!strncmp(mime, "image/x-ms-bmp", 14)) {
    return "bmp";
  }
  if (!strncmp(mime, "image/svg+xml", 13)) {
    return "svg";
  }
  if (!strncmp(mime, "image/webp", 10)) {
    return "webp";
  }
  if (!strncmp(mime, "application/font-woff", 21)) {
    return "woff";
  }
  if (!strncmp(mime, "application/java-archive", 24)) {
    return "jar";
  }
  if (!strncmp(mime, "application/json", 16)) {
    return "json";
  }
  if (!strncmp(mime, "application/mac-binhex40", 24)) {
    return "hqx";
  }
  if (!strncmp(mime, "application/msword", 18)) {
    return "doc";
  }
  if (!strncmp(mime, "application/pdf", 15)) {
    return "pdf";
  }
  if (!strncmp(mime, "application/postscript", 22)) {
    return "ps";
  }
  if (!strncmp(mime, "application/rtf", 15)) {
    return "rtf";
  }
  if (!strncmp(mime, "application/vnd.apple.mpegurl", 29)) {
    return "m3u8";
  }
  if (!strncmp(mime, "application/vnd.ms-excel", 24)) {
    return "xls";
  }
  if (!strncmp(mime, "application/vnd.ms-fontobject", 29)) {
    return "eot";
  }
  if (!strncmp(mime, "application/vnd.ms-powerpoint", 29)) {
    return "ppt";
  }
  if (!strncmp(mime, "application/vnd.wap.wmlc", 24)) {
    return "wmlc";
  }
  if (!strncmp(mime, "application/vnd.google-earth.kml+xml", 36)) {
    return "kml";
  }
  if (!strncmp(mime, "application/vnd.google-earth.kmz", 32)) {
    return "kmz";
  }
  if (!strncmp(mime, "application/x-7z-compressed", 27)) {
    return "7z";
  }
  if (!strncmp(mime, "application/x-cocoa", 19)) {
    return "cco";
  }
  if (!strncmp(mime, "application/x-java-archive-diff", 31)) {
    return "jardiff";
  }
  if (!strncmp(mime, "application/x-java-jnlp-file", 28)) {
    return "jnlp";
  }
  if (!strncmp(mime, "application/x-makeself", 22)) {
    return "run";
  }
  if (!strncmp(mime, "application/x-perl", 18)) {
    return "pl";
  }
  if (!strncmp(mime, "application/x-pilot", 19)) {
    return "prc";
  }
  if (!strncmp(mime, "application/x-rar-compressed", 28)) {
    return "rar";
  }
  if (!strncmp(mime, "application/x-redhat-package-manager", 36)) {
    return "rpm";
  }
  if (!strncmp(mime, "application/x-sea", 17)) {
    return "sea";
  }
  if (!strncmp(mime, "application/x-shockwave-flash", 29)) {
    return "swf";
  }
  if (!strncmp(mime, "application/x-stuffit", 21)) {
    return "sit";
  }
  if (!strncmp(mime, "application/x-tcl", 17)) {
    return "tcl";
  }
  if (!strncmp(mime, "application/x-x509-ca-cert", 26)) {
    return "der";
  }
  if (!strncmp(mime, "application/x-xpinstall", 23)) {
    return "xpi";
  }
  if (!strncmp(mime, "application/xhtml+xml", 21)) {
    return "xhtml";
  }
  if (!strncmp(mime, "application/xspf+xml", 20)) {
    return "xspf";
  }
  if (!strncmp(mime, "application/zip", 15)) {
    return "zip";
  }
  if (!strncmp(mime, "application/octet-stream", 24)) {
    return "bin";
  }
  if (!strncmp(mime, "application/octet-stream", 24)) {
    return "deb";
  }
  if (!strncmp(mime, "application/octet-stream", 24)) {
    return "dmg";
  }
  if (!strncmp(mime, "application/octet-stream", 24)) {
    return "iso";
  }
  if (!strncmp(mime, "application/octet-stream", 24)) {
    return "msi";
  }
  if (!strncmp(mime, "application/vnd.openxmlformats-officedocument.wordprocessingml.document", 71)) {
    return "docx";
  }
  if (!strncmp(mime, "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", 65)) {
    return "xlsx";
  }
  if (!strncmp(mime, "application/vnd.openxmlformats-officedocument.presentationml.presentation", 73)) {
    return "pptx";
  }
  if (!strncmp(mime, "audio/midi", 10)) {
    return "mid";
  }
  if (!strncmp(mime, "audio/mpeg", 10)) {
    return "mp3";
  }
  if (!strncmp(mime, "audio/ogg", 9)) {
    return "ogg";
  }
  if (!strncmp(mime, "audio/ogg", 9)) {
    return "opus";
  }
  if (!strncmp(mime, "audio/x-m4a", 11)) {
    return "m4a";
  }
  if (!strncmp(mime, "audio/x-realaudio", 17)) {
    return "ra";
  }
  if (!strncmp(mime, "video/3gpp", 10)) {
    return "3gpp";
  }
  if (!strncmp(mime, "video/mp2t", 10)) {
    return "ts";
  }
  if (!strncmp(mime, "video/mp4", 9)) {
    return "mp4";
  }
  if (!strncmp(mime, "video/mpeg", 10)) {
    return "mpeg";
  }
  if (!strncmp(mime, "video/quicktime", 15)) {
    return "mov";
  }
  if (!strncmp(mime, "video/webm", 10)) {
    return "webm";
  }
  if (!strncmp(mime, "video/x-flv", 11)) {
    return "flv";
  }
  if (!strncmp(mime, "video/x-m4v", 11)) {
    return "m4v";
  }
  if (!strncmp(mime, "video/x-mng", 11)) {
    return "mng";
  }
  if (!strncmp(mime, "video/x-ms-asf", 14)) {
    return "asx";
  }
  if (!strncmp(mime, "video/x-ms-wmv", 14)) {
    return "wmv";
  }
  if (!strncmp(mime, "video/x-msvideo", 15)) {
    return "avi";
  }
  return NULL;
}

char *ext_to_mime(char *ext) {
  if (!ext) return NULL;
  if (!strncmp(ext, "html", 4)) {
    return "text/html";
  }
  if (!strncmp(ext, "htm", 3)) {
    return "text/html";
  }
  if (!strncmp(ext, "shtml", 5)) {
    return "text/html";
  }
  if (!strncmp(ext, "css", 3)) {
    return "text/css";
  }
  if (!strncmp(ext, "xml", 3)) {
    return "text/xml";
  }
  if (!strncmp(ext, "gif", 3)) {
    return "image/gif";
  }
  if (!strncmp(ext, "jpeg", 4)) {
    return "image/jpeg";
  }
  if (!strncmp(ext, "jpg", 3)) {
    return "image/jpeg";
  }
  if (!strncmp(ext, "js", 2)) {
    return "application/javascript";
  }
  if (!strncmp(ext, "atom", 4)) {
    return "application/atom+xml";
  }
  if (!strncmp(ext, "rss", 3)) {
    return "application/rss+xml";
  }
  if (!strncmp(ext, "mml", 3)) {
    return "text/mathml";
  }
  if (!strncmp(ext, "txt", 3)) {
    return "text/plain";
  }
  if (!strncmp(ext, "jad", 3)) {
    return "text/vnd.sun.j2me.app-descriptor";
  }
  if (!strncmp(ext, "wml", 3)) {
    return "text/vnd.wap.wml";
  }
  if (!strncmp(ext, "htc", 3)) {
    return "text/x-component";
  }
  if (!strncmp(ext, "png", 3)) {
    return "image/png";
  }
  if (!strncmp(ext, "tif", 3)) {
    return "image/tiff";
  }
  if (!strncmp(ext, "tiff", 4)) {
    return "image/tiff";
  }
  if (!strncmp(ext, "wbmp", 4)) {
    return "image/vnd.wap.wbmp";
  }
  if (!strncmp(ext, "ico", 3)) {
    return "image/x-icon";
  }
  if (!strncmp(ext, "jng", 3)) {
    return "image/x-jng";
  }
  if (!strncmp(ext, "bmp", 3)) {
    return "image/x-ms-bmp";
  }
  if (!strncmp(ext, "svg", 3)) {
    return "image/svg+xml";
  }
  if (!strncmp(ext, "svgz", 4)) {
    return "image/svg+xml";
  }
  if (!strncmp(ext, "webp", 4)) {
    return "image/webp";
  }
  if (!strncmp(ext, "woff", 4)) {
    return "application/font-woff";
  }
  if (!strncmp(ext, "jar", 3)) {
    return "application/java-archive";
  }
  if (!strncmp(ext, "war", 3)) {
    return "application/java-archive";
  }
  if (!strncmp(ext, "ear", 3)) {
    return "application/java-archive";
  }
  if (!strncmp(ext, "json", 4)) {
    return "application/json";
  }
  if (!strncmp(ext, "hqx", 3)) {
    return "application/mac-binhex40";
  }
  if (!strncmp(ext, "doc", 3)) {
    return "application/msword";
  }
  if (!strncmp(ext, "pdf", 3)) {
    return "application/pdf";
  }
  if (!strncmp(ext, "ps", 2)) {
    return "application/postscript";
  }
  if (!strncmp(ext, "eps", 3)) {
    return "application/postscript";
  }
  if (!strncmp(ext, "ai", 2)) {
    return "application/postscript";
  }
  if (!strncmp(ext, "rtf", 3)) {
    return "application/rtf";
  }
  if (!strncmp(ext, "m3u8", 4)) {
    return "application/vnd.apple.mpegurl";
  }
  if (!strncmp(ext, "xls", 3)) {
    return "application/vnd.ms-excel";
  }
  if (!strncmp(ext, "eot", 3)) {
    return "application/vnd.ms-fontobject";
  }
  if (!strncmp(ext, "ppt", 3)) {
    return "application/vnd.ms-powerpoint";
  }
  if (!strncmp(ext, "wmlc", 4)) {
    return "application/vnd.wap.wmlc";
  }
  if (!strncmp(ext, "kml", 3)) {
    return "application/vnd.google-earth.kml+xml";
  }
  if (!strncmp(ext, "kmz", 3)) {
    return "application/vnd.google-earth.kmz";
  }
  if (!strncmp(ext, "7z", 2)) {
    return "application/x-7z-compressed";
  }
  if (!strncmp(ext, "cco", 3)) {
    return "application/x-cocoa";
  }
  if (!strncmp(ext, "jardiff", 7)) {
    return "application/x-java-archive-diff";
  }
  if (!strncmp(ext, "jnlp", 4)) {
    return "application/x-java-jnlp-file";
  }
  if (!strncmp(ext, "run", 3)) {
    return "application/x-makeself";
  }
  if (!strncmp(ext, "pl", 2)) {
    return "application/x-perl";
  }
  if (!strncmp(ext, "pm", 2)) {
    return "application/x-perl";
  }
  if (!strncmp(ext, "prc", 3)) {
    return "application/x-pilot";
  }
  if (!strncmp(ext, "pdb", 3)) {
    return "application/x-pilot";
  }
  if (!strncmp(ext, "rar", 3)) {
    return "application/x-rar-compressed";
  }
  if (!strncmp(ext, "rpm", 3)) {
    return "application/x-redhat-package-manager";
  }
  if (!strncmp(ext, "sea", 3)) {
    return "application/x-sea";
  }
  if (!strncmp(ext, "swf", 3)) {
    return "application/x-shockwave-flash";
  }
  if (!strncmp(ext, "sit", 3)) {
    return "application/x-stuffit";
  }
  if (!strncmp(ext, "tcl", 3)) {
    return "application/x-tcl";
  }
  if (!strncmp(ext, "tk", 2)) {
    return "application/x-tcl";
  }
  if (!strncmp(ext, "der", 3)) {
    return "application/x-x509-ca-cert";
  }
  if (!strncmp(ext, "pem", 3)) {
    return "application/x-x509-ca-cert";
  }
  if (!strncmp(ext, "crt", 3)) {
    return "application/x-x509-ca-cert";
  }
  if (!strncmp(ext, "xpi", 3)) {
    return "application/x-xpinstall";
  }
  if (!strncmp(ext, "xhtml", 5)) {
    return "application/xhtml+xml";
  }
  if (!strncmp(ext, "xspf", 4)) {
    return "application/xspf+xml";
  }
  if (!strncmp(ext, "zip", 3)) {
    return "application/zip";
  }
  if (!strncmp(ext, "bin", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "exe", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "dll", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "deb", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "dmg", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "iso", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "img", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "msi", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "msp", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "msm", 3)) {
    return "application/octet-stream";
  }
  if (!strncmp(ext, "docx", 4)) {
    return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
  }
  if (!strncmp(ext, "xlsx", 4)) {
    return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  }
  if (!strncmp(ext, "pptx", 4)) {
    return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
  }
  if (!strncmp(ext, "mid", 3)) {
    return "audio/midi";
  }
  if (!strncmp(ext, "midi", 4)) {
    return "audio/midi";
  }
  if (!strncmp(ext, "kar", 3)) {
    return "audio/midi";
  }
  if (!strncmp(ext, "mp3", 3)) {
    return "audio/mpeg";
  }
  if (!strncmp(ext, "ogg", 3)) {
    return "audio/ogg";
  }
  if (!strncmp(ext, "opus", 4)) {
    return "audio/ogg";
  }
  if (!strncmp(ext, "m4a", 3)) {
    return "audio/x-m4a";
  }
  if (!strncmp(ext, "ra", 2)) {
    return "audio/x-realaudio";
  }
  if (!strncmp(ext, "3gpp", 4)) {
    return "video/3gpp";
  }
  if (!strncmp(ext, "3gp", 3)) {
    return "video/3gpp";
  }
  if (!strncmp(ext, "ts", 2)) {
    return "video/mp2t";
  }
  if (!strncmp(ext, "mp4", 3)) {
    return "video/mp4";
  }
  if (!strncmp(ext, "mpeg", 4)) {
    return "video/mpeg";
  }
  if (!strncmp(ext, "mpg", 3)) {
    return "video/mpeg";
  }
  if (!strncmp(ext, "mov", 3)) {
    return "video/quicktime";
  }
  if (!strncmp(ext, "webm", 4)) {
    return "video/webm";
  }
  if (!strncmp(ext, "flv", 3)) {
    return "video/x-flv";
  }
  if (!strncmp(ext, "m4v", 3)) {
    return "video/x-m4v";
  }
  if (!strncmp(ext, "mng", 3)) {
    return "video/x-mng";
  }
  if (!strncmp(ext, "asx", 3)) {
    return "video/x-ms-asf";
  }
  if (!strncmp(ext, "asf", 3)) {
    return "video/x-ms-asf";
  }
  if (!strncmp(ext, "wmv", 3)) {
    return "video/x-ms-wmv";
  }
  if (!strncmp(ext, "avi", 3)) {
    return "video/x-msvideo";
  }
  return NULL;
}
