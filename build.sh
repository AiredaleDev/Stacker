#!/bin/sh

set -xe

gcc src/*.c -Wall -Werror -lreadline -o horth
