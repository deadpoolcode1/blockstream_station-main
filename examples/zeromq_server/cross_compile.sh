#!/bin/bash

if [ "$1" = "yes" ]; then
  unset LD_LIBRARY_PATH
  source /opt/petalinux/2022.2/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi
fi

shift
"$@"
