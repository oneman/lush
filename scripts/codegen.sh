#!/bin/sh

LIBPATH='lib/'
GEN_COMMON='lib/gen/'
TOOLS_PATH='tools/'

function update_embedded {
  cd web/rack
  rm -f interface.js
  cat src/js/*.js > interface.js
  ../../tools/kr_js_proto >> interface.js
  rm -f style.css
  cat src/css/*.css > style.css
  cd ../..
}

function run_mime_gen {
  echo ./tools/code/gen/mime_gen ${1}/mime.types ${2}/gen/mime.c ${2}/gen/mime.h
  eval ./tools/code/gen/mime_gen ${1}/mime.types ${2}/gen/mime.c ${2}/gen/mime.h
}

function run_limits_gen {
  echo ./tools/code/gen/limits_gen ${1}/radio/limits.h ${1}/gen/limits_gen.h
  eval ./tools/code/gen/limits_gen ${1}/radio/limits.h ${1}/gen/limits_gen.h
}

function run_codegen_precheck {
  cd tools/code/gen/bootstrap
  rm bootstrapped.c
  rm bootstrapped.h
  make
  cd ../
  make
  cd ../../../
}

function run_codegen {
  funcs="${1}/gen/info_functions.h"
  types="${1}/gen/info_types.h"
  typesc="${1}/gen/info_types.c"
  metah="${1}/gen/metainfo.h"
  metac="${1}/gen/metainfo.c"
  echo ./tools/code/gen/info_gen $1 $funcs $types $typesc
  eval ./tools/code/gen/info_gen $1 $funcs $types $typesc
  echo ./tools/code/gen/meta_gen $1 $metah $metac
  eval ./tools/code/gen/meta_gen $1 $metah $metac
  echo ./tools/code/gen/wrap_gen $1
  eval ./tools/code/gen/wrap_gen $1
  #echo ./tools/code/gen/codegen_auto $1 $2 $3 $4
  #eval tools/code/gen/codegen_auto $1 $2 $3 $4
  #echo ./tools/code/gen/codegen_cfg $tools
  #eval tools/code/gen/codegen_cfg $tools
}

path=$(realpath $LIBPATH)
pre="''"
suff="''"
gencommon=$(realpath $GEN_COMMON)
tools=$(realpath $TOOLS_PATH)
cgen_script_path="$(pwd)/scripts/codegen.sh"

if [ ! -f $cgen_script_path ]; then
  echo 'You are not running codegen.sh from KR root dir. Please do so.'
  exit
fi

run_codegen_precheck

for dir in $(find lib/ -type d -not -path '*/\.*' -not -path '*gen*'); do
  gendir="${dir}/gen"
  mkdir -p $gendir
  rm -rf $gendir/*.c
  rm -rf $gendir/*.h
done

rm -rf $gencommon/*.c
rm -rf $gencommon/*.h
mkdir -p $gencommon

run_codegen $path $pre $suff $gencommon

update_embedded

run_mime_gen $tools $path
run_limits_gen $path

echo 'All Done!'
