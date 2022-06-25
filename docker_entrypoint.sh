#!/bin/bash
git clone https://github.com/predragdraganovic/ForestTrailGL.git /code
cd /code && git checkout cmake_fix
cd /output && cmake /code
cmake --build .
