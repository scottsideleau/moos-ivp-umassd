FROM scottsideleau/moos-ivp-umassd
MAINTAINER Scott R. Sideleau <scott@sideleau.com>

# Install GNU Plot
RUN apt-get update && apt-get install -y \
  gnuplot

# Add the run script to the container
ADD run_repro_launch.sh /root/run_repro_launch.sh

# Run the 'reproducible' workflow
ENTRYPOINT ["/bin/bash", "/root/run_repro_launch.sh"]

