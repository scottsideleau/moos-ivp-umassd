FROM scottsideleau/moos-ivp-umassd
MAINTAINER Scott R. Sideleau <scott@sideleau.com>

# Install WolfSSL build dependencies
RUN apt-get update && apt-get install -y \
  autoconf \
  autoconf-archive \
  automake \ 
  libtool

# Install WolfSSL (note Debian "Sid" has a package available)
RUN \
  cd /opt && \
  git clone https://github.com/wolfSSL/wolfssl.git && \
  cd wolfssl && \
  ./autogen.sh && \
  ./configure \
			--disable-silent-rules \
      --disable-dependency-tracking \
			--disable-bump \
      --disable-examples \
      --disable-fortress \
      --disable-md5 \
      --disable-sniffer \
      --disable-webserver \
      --enable-aesccm \
      --enable-aesgcm \
      --enable-alpn \
      --enable-blake2 \
      --enable-camellia \
      --enable-certgen \
      --enable-certreq \
      --enable-chacha \
      --enable-crl \
      --enable-crl-monitor \
      --enable-curve25519 \
      --enable-dtls \
      --enable-dh \
      --enable-ecc \
      --enable-eccencrypt \
      --enable-ed25519 \
      --enable-filesystem \
      --enable-hc128 \
      --enable-hkdf \
      --enable-inline \
      --enable-ipv6 \
      --enable-jni \
      --enable-keygen \
      --enable-ocsp \
      --enable-opensslextra \
      --enable-pwdbased \
      --enable-poly1305 \
      --enable-psk \
      --enable-rabbit \
      --enable-ripemd \
      --enable-savesession \
      --enable-savecert \
			--enable-sessioncerts \
      --enable-sha512 \
      --enable-sni \
      --enable-supportedcurves && \
  make && \
  make install

# Add WolfSSL to the LD_LIBRARY_PATH
ENV LD_LIBRARY_PATH "/usr/local/lib:$LD_LIBRARY_PATH"

# Setup the moos-ivp-umassd environment
RUN \
  cd /opt/moos-ivp-umassd && \
  git pull -a && \
  ./build.sh -DBUILD_WOLFSSL=true

# Add the run script to the container
ADD run_repro_launch.sh /root/run_repro_launch.sh

# Run the 'crypto' workflow
ENTRYPOINT ["/bin/bash", "/root/run_repro_launch.sh"]

