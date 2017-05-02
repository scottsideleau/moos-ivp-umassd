#!/bin/bash

# Make the storage directory
mkdir /root/artifacts

# Run the workflow and copy the output to the storage directory
pushd /opt/moos-ivp-umassd/missions/crypto
./repro_launch.sh
cp data_from_runs/*/secret_message.alog /root/artifacts/secret_message.alog
cp .encrypt/plaintext /root/artifacts/plaintext
cp .decrypt/ciphertext /root/artifacts/ciphertext
cp .decrypt/cleartext /root/artifacts/cleartext
popd
