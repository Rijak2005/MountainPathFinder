#!/usr/bin/env bash

# Usage:
#   ./build.sh            # configure and build
#   ./build.sh clean      # remove the build directory

set -e

BUILD_DIR="build"

if [[ "$1" == "clean" ]]; then
    echo "Cleaning build directory"
    rm -rf "$BUILD_DIR"
    exit 0
fi

echo "Running autoreconf"
autoreconf -i -f

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo "Configuring"
../configure

# Build 
echo "Building"
make

echo "Build complete. Binary is at $(pwd)/mountainpath"