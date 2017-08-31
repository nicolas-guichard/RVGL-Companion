#!/bin/bash

set -xue

MXE_DIR=./mxe

if [ "$COMPANION_PLATFORM" = "windows32" ]; then
    MXE_TARGET=i686-w64-mingw32.static
fi

if [ "$COMPANION_PLATFORM" = "windows64" ]; then
    MXE_TARGET=x86_64-w64-mingw32.static
fi

export PATH=${MXE_DIR}/usr/bin/:$PATH
export LD_LIBRARY_PATH=${MXE_DIR}/usr/${MXE_TARGET}/lib:$LD_LIBRARY_PATH
export CPATH=${MXE_DIR}/usr/${MXE_TARGET}/include:$CPATH

${MXE_TARGET}-qmake-qt5 RVGL_Companion.pro CONFIG+=Release
make

set +xue
