#!/bin/sh

CERTDIR=tests/cert

rm -rf ${CERTDIR}
mkdir -p ${CERTDIR}

openssl genrsa -des3 -passout pass:pass 2048 > ${CERTDIR}/server.key
openssl req -passin pass:pass -new -key ${CERTDIR}/server.key -x509 -days 356 -out ${CERTDIR}/server.crt << EOF
IO
Krad
Krad
Krad
Krad
Krad
Krad
EOF
openssl rsa -in ${CERTDIR}/server.key -passin pass:pass -out ${CERTDIR}/server.key

openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout client-key.pem -out client-cert.pem
openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout server-key.pem -out server-cert.pem
