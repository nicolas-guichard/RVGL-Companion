#!/bin/bash

set -xue

qmake RVGL_Companion.pro CONFIG+=Release
make

set +xue
