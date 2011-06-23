#!/bin/bash

usage() {
  [ "$1" != "" ] && echo $1
  echo 'Usage: ./enable_breakpad_linux.sh <tulip_install_dir> <symbols_dir>'
  echo 'You must have the following executables installed on your system and available in your PATH:'
  echo '. dump_syms : The Google breakpad executable used to dump symbols tables.'
  echo '. strip : The GNU executable used to strip debugging symbols from executables.'
  echo ''
  echo 'This script will look into the <tulip_install_dir> subfolders to find shared libraries and executables. And for each of them it will do the following:'
  echo '1. Run the dump_syms utility to output symbol table into <symbols_dir>/file.sym'
  echo '2. Strip the executable/library from its debugging symbols.'
  exit 1
}

[ "$#" != "2" ] && usage
install_dir=$1
output_dir=$2

dump_syms_exe=$(which dump_syms)
strip_exe=$(which strip)

[ "$dump_syms_exe" = "" ] && usage 'dymp_syms executable was not found in your PATH'
[ "$strip_exe" = "" ] && usage 'strip executable was not found in your PATH'

[ ! -e "$install_dir" ] && usage 'Provided installation directory does not exist'

parse_dir() {
  echo -e '\e[01;34mParsing '$(pwd)'\e[00m'
  for f in $(file * | grep ELF | cut -f 1 -d ':'); do
    out_file=$output_dir/$(basename $f '.so').sym
    echo -e '  \e[00;32mDumping\e[00m symbols from '$f' to '$out_file'.'
    $dump_syms_exe $f > $out_file
    echo -e '  \e[00;32mStripping\e[00m debugging symbols from '$f'.'
    $strip_exe -g $f
  done

  for f in *; do
    if [ -d "$f" ]; then cd "$f" && parse_dir && cd ..; fi
  done
}

orig_pwd=$(pwd)

cd $install_dir
parse_dir 2> >(while read line; do echo -e '\e[01;31m'$line'\e[0m'; done)
