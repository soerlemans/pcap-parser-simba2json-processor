#!/usr/bin/env bash


# Cd into the directory where the script is located.
cd "$(dirname "")"
pwd -P


# Download the test data:
cat files.txt | while read LINE
do
    echo "Fetching $LINE"
    wget "http://ftp.moex.ru/pub/SIMBA/Spectra/prod/pcap/${LINE}"
    echo
done
