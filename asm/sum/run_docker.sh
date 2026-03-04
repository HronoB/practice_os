#!/bin/bash
docker build -t asm-linux-env .
docker run -it --rm \
  -v "$(pwd):/workspace" \
  -w /workspace \
  asm-linux-env \
  bash -c "make clean && make run"




