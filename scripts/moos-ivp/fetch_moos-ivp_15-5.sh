#! /bin/bash

NPROC=`getconf _NPROCESSORS_ONLN`

pushd /opt
git clone https://github.com/moos-ivp/MOOS-IvP-releases.git && \
mv MOOS-IvP-releases moos-ivp && \
cd moos-ivp && \
git checkout tags/15.5 && \
sed -i s/j12/j${NPROC}/ build-ivp.sh && \
sed -i s/O3/Os/ build-ivp.sh && \
sed -i s/j12/j${NPROC}/ build-moos.sh && \
./build.sh
popd
