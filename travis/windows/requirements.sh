#!/bin/bash

set -xue

echo "deb http://pkg.mxe.cc/repos/apt/debian wheezy main" \
    | sudo tee /etc/apt/sources.list.d/mxeapt.list
sudo apt-key adv --keyserver x-hkp://keys.gnupg.net \
    --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB

sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"

sudo apt-get update

sudo apt-get --yes install upx-ucl autoconf automake \
    autopoint bash bison bzip2 flex gettext git g++ \
    gperf intltool libffi-dev libgdk-pixbuf2.0-dev \
    libtool libltdl-dev libssl-dev libxml-parser-perl make \
    openssl p7zip-full patch perl pkg-config python ruby scons \
    sed unzip wget xz-utils g++-7-multilib libc6-dev-i386

if [ "$COMPANION_PLATFORM" = "windows32" ]; then
    MXE_TARGET=i686-w64-mingw32.static
fi

if [ "$COMPANION_PLATFORM" = "windows64" ]; then
    MXE_TARGET=x86_64-w64-mingw32.static
fi

MXE2_TARGET=$(echo "$MXE_TARGET" | sed 's/_/-/g')
sudo apt-get --yes install \
    mxe-${MXE2_TARGET}-qt \
    mxe-${MXE2_TARGET}-miniupnpc

mkdir temp
mkdir win32
mkdir win32/i386
mkdir win32/x64
cd temp
curl -L http://www.7-zip.org/a/7z1604-extra.7z --output 7z1604-extra.7z
7z x 7z1604-extra.7z
mv ./7za.exe ../win32/i386/7za.exe
mv ./x64/7za.exe ../win32/x64/7za.exe
cd ..
rm -r temp

set +xue
