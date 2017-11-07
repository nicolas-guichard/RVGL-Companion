#!/bin/bash

set -xue

MXE_DIR=/usr/lib/mxe

if [ "$COMPANION_PLATFORM" = "windows32" ]; then
    MXE_TARGET=i686-w64-mingw32.static
    QMAKE_TARGET=x86
fi

if [ "$COMPANION_PLATFORM" = "windows64" ]; then
    MXE_TARGET=x86_64-w64-mingw32.static
    QMAKE_TARGET=x86_64
fi

export PATH=${MXE_DIR}/usr/bin/:$PATH
export LD_LIBRARY_PATH=${MXE_DIR}/usr/${MXE_TARGET}/lib

sudo ln -s ${MXE_DIR}/usr/${MXE_TARGET}/lib/libadvapi32.a ${MXE_DIR}/usr/${MXE_TARGET}/lib/libAdvapi32.a

${MXE_DIR}/usr/bin/${MXE_TARGET}-qmake-qt5 RVGL_Companion.pro CONFIG+=Release -spec win32-g++ QMAKE_TARGET.arch=${QMAKE_TARGET}
make

set +xue
