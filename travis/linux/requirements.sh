#!/bin/bash

set -xue

export DEBIAN_FRONTEND=noninteractive

sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo apt-add-repository -y "ppa:beineri/opt-qt591-trusty"

sudo apt-get -yq update

sudo apt-get -yq --no-install-suggests \
    --no-install-recommends --force-yes install \
    g++-7 build-essential qt59base qt59quickcontrols2\
    qt59declarative qt59quickcontrols iptables-dev \
    libevent-dev libnfnetlink-dev uuid-dev

sudo rm /usr/bin/g++
sudo ln -s /usr/bin/g++-7 /usr/bin/g++

git clone https://github.com/miniupnp/miniupnp
mv miniupnp/miniupnpc miniupnpc
rm -rf miniupnp
cd miniupnpc
CONFIG_OPTIONS="--ipv6 --igd2" make -j3
sudo make install
cd ..

set +xue
