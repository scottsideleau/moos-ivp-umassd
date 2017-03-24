#! /bin/bash

NPROC=`getconf _NPROCESSORS_ONLN`
ROBOT=false

for ARGI; do
  if [ "$ARGI" = "--robot" -o "$ARGI" = "-r" ]; then
    ROBOT=true
  fi
done

pushd /opt

git clone https://github.com/moos-ivp/MOOS-IvP-releases.git && \
mv MOOS-IvP-releases moos-ivp && \
cd moos-ivp && \
git checkout tags/15.5 && \
sleep 1

sed -i s/j12/j${NPROC}/ build-ivp.sh && \
sed -i s/O3/Os/ build-ivp.sh && \
sleep 1

if [ "$ROBOT" = true ]; then
  sed -i s/BUILD_GUI_CODE=\"ON\"/BUILD_GUI_CODE=\"OFF\"/ build-ivp.sh
fi
sleep 1

sed -i s/j12/j${NPROC}/ build-moos.sh && \
sleep 1

./build.sh

popd
