# crypto
## A reproducible workflow demonstrating AES encrypted message transfer between two MOOS processes.

Using the 'alpha' mission from MOOS-IvP, create a workflow that demonstrates
encrypted messaging between MOOS processes  in local simulation, Docker 
containers, and in the physical world.

Docker containers referenced below are hosted on
[Docker Hub](https://hub.docker.com):

  * Base container: https://hub.docker.com/r/scottsideleau/moos-ivp/
  * crypto container: https://hub.docker.com/r/scottsideleau/moos-ivp-umassd/tags/

The Dockerfile source for each of the containers is located in this repository.

  * ../../dockerfiles/moos-ivp_15.5: a Docker container with MOOS-IvP 15.5 installed
  * ../../dockerfiles/umassd: a Docker container with moos-ivp-umassd installed
  * ../../dockerfiles/crypto: a Docker container featuring the crypto workflow

You may choose to build any/all of the provided docker containers by:

  1. Switching into any of the container source directories.

  ```bash
  cd ../../dockerfiles/crypto
  ```

  2. Issuing the Docker build command for the chosen container.

  ```bash
  docker build -t crypto .
  ```

A. Run the workflow using MOOS-IvP on local machine

  NOTE: These instructions assume a [MacOS](http://www.apple.com/macos/)
  10.11+ environment with [Homebrew](http://brew.sh/index.html).

  1. Install dependencies.

  ```bash
  brew install git cmake wolfssl
  ```

  2. Install [MOOS-IvP](http://moos-ivp.org) on your local computer.

  ```bash
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
  ```

  3. Install moos-ivp-umassd on your local computer.

  ```bash
  cd /opt && \
  git clone https://github.com/scottsideleau/moos-ivp-umassd.git && \
  cd moos-ivp-umassd && \
  ./build.sh
  ```

  4. Add necessary environmental variables.

  ```bash
  export PATH=$PATH:/opt/moos-ivp/bin:/opt/moos-ivp-umassd/bin
  export IVP_BEHAVIOR_DIRS:$IVP_BEHAVIOR_DIRS:/opt/moos-ivp-umassd/lib
  ```

  5. Run the workflow.

  ```bash
  cd missions/crypto
  ./repro_launch.sh
  ```

  6. Inspect the artifacts generated (e.g. 'diff plaintext cleartext' returns NULL).

  ```bash
  ls -alh data_from_runs/secret_message.alog
  ls -alh .encrypt/plaintext
  ls -alh .decrypt/ciphertext
  ls -alh .decrypt/cleartext
  ```

B. Run the workflow within a Docker container

  NOTE: These instructions assume a [MacOS](http://www.apple.com/macos/)
  10.11+ environment with [Homebrew](http://brew.sh/index.html).

  1. Install [Docker](https://www.docker.com) on your local computer.

  ```bash
  brew cask install docker
  ```

  2. Run Docker by selecting it from your Applciations/ directory.

  3. Run the workflow and copy the artifact to local directory.

  ```bash
  docker pull scottsideleau/moos-ivp-umassd
  docker run scottsideleau/moos-ivp-umassd:crypto
  cp `docker ps -alq`:/root/artifacts/secret_message.alog secret_message.alog
  cp `docker ps -alq`:/root/artifacts/plaintext plaintext
  cp `docker ps -alq`:/root/artifacts/ciphertext ciphertext
  cp `docker ps -alq`:/root/artifacts/cleartext cleartext
  ```
  TROUBLESHOOTING: If the workflow gets hung up waiting on the IvP Helm, it
  may be necessary to stop the Docker instance and try again.  You can stop the
  most recently running container with the 'stop' command:

  ```bash 
  docker stop `docker ps -alq`
  docker rm -v $(docker ps -a -q -f status=exited)
  ```

C. Compare the results and verify crypto

  NOTE: These instructions assume a [MacOS](http://www.apple.com/macos/)
  10.11+ environment with [Homebrew](http://brew.sh/index.html).

  1. Inspect the text files.

  ```bash
  ls -alh secret_message.alog
  ls -alh plaintext
  ls -alh ciphertext
  ls -alh cleartext
  ```

  2. Diff plaintext/cleartext and note message contents are identical.

  ```bash
  diff plaintext cleartext
  ```

  NOTE: By default, the "secret message" is defined in pWolfEncrypt's plug file,
  located in missions/crypto/plugs/app\_pWolfEncrypt.plug.  It is safe to replace 
  the "secret message" with one of your own and relaunch.

