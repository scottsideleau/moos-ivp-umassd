FROM debian:jessie
MAINTAINER Scott R. Sideleau <scott@sideleau.com>

# Setup the base Debian 8 container
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git

# Setup the MOOS-IvP_15.5 environment
RUN \
  cd /opt && \
  git clone https://github.com/moos-ivp/MOOS-IvP-releases.git && \
  mv MOOS-IvP-releases moos-ivp && \
  cd moos-ivp && \
  git checkout tags/15.5 && \
  sed -i s/BUILD_GUI_CODE=\"ON\"/BUILD_GUI_CODE=\"OFF\"/ build-ivp.sh && \
  sed -i s/j12/j1/ build-ivp.sh && \
  sed -i s/O3/Os/ build-ivp.sh && \
  sed -i s/j12/j1/ build-moos.sh && \
  ./build.sh

# Add MOOS-IvP to the PATH
ENV PATH "$PATH:/opt/moos-ivp/bin"

