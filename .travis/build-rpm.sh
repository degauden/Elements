#!/bin/bash
set -ex

# Environment
export VERBOSE=1
export CTEST_OUTPUT_ON_FAILURE=1

# Platform-specific configuration
source /etc/os-release

CMAKEFLAGS="-DINSTALL_DOC=ON"

if [ $NAME == 'Fedora' ] && [ $VERSION_ID -ge 30 ]; then
  PYTHON="python3"
  CMAKEFLAGS="$CMAKEFLAGS -DPYTHON_EXPLICIT_VERSION=3"
else
  PYTHON="python"
fi

# Dependencies
yum install -y @development-tools cmake gcc-c++ rpm-build ${PYTHON}-devel
yum install -y boost-devel ${PYTHON}-pytest log4cpp-devel doxygen CCfits-devel wcslib-devel
yum install -y pybind11-devel
yum install -y graphviz ${PYTHON}-sphinx ${PYTHON}-sphinxcontrib-apidoc

# Build
mkdir -p /build
cd /build
cmake -DCMAKE_INSTALL_PREFIX=/usr $CMAKEFLAGS /src
make $MAKEFLAGS rpm

