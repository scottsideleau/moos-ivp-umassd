FROM scottsideleau/moos-ivp:15.5
MAINTAINER Scott R. Sideleau <scott@sideleau.com>

# Setup the moos-ivp-umassd environment
RUN \
  cd /opt && \
  git clone https://github.com/scottsideleau/moos-ivp-umassd.git && \
  cd moos-ivp-umassd && \
  ./build.sh

# Add moos-ivp-umassd to the PATH
ENV PATH "$PATH:/opt/moos-ivp-umassd/bin"

# Add moos-ivp-umassd to the IVP_BEHAVIOR_DIRS
ENV IVP_BEHAVIOR_DIRS "$IVP_BEHAVIOR_DIRS:/opt/moos-ivp-umassd/lib"

