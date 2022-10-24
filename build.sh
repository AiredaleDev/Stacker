#!/bin/sh

set -xe

gcc src/*.c -g -Wall -Werror -lreadline -o stkr
