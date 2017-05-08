#! /bin/bash

NPROC=`getconf _NPROCESSORS_ONLN`
EXT=".bak"
ROBOT=false

# Handle command line options
for ARGI; do
  if [ "'$ARGI' = '--robot' -o '$ARGI' = '-r'" ]; then
    ROBOT=true
  fi
done

# Setup the 'sed' command for in-place file changes
if [ "$(uname) = 'Darwin'" ]; then
  # MacOS
  SED="sed -i $EXT"
else
  # Linux, etc.
  SED="sed -i"
fi

pushd /opt

git clone https://github.com/moos-ivp/MOOS-IvP-releases.git && \
mv MOOS-IvP-releases moos-ivp && \
cd moos-ivp && \
git checkout tags/15.5 && \
sleep 1

$SED s/j12/j${NPROC}/ build-ivp.sh && \
$SED s/O3/Os/ build-ivp.sh && \
sleep 1

if [ "'$ROBOT' = true" ]; then
  $SED s/BUILD_GUI_CODE=\"ON\"/BUILD_GUI_CODE=\"OFF\"/ build-ivp.sh
fi
sleep 1

$SED s/j12/j${NPROC}/ build-moos.sh && \
sleep 1

# OS-specific clean-up
if [ "$(uname) = 'Darwin'" ]; then
  # MacOS
  rm -f *$EXT
fi

./build.sh

popd

