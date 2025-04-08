#!/bin/bash

set -e

cmake -DCMAKE_BUILD_TYPE=debug -S . -B ./build
