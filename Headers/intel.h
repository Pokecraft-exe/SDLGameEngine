#pragma once

#include "game.h"
#include <sycl/sycl.hpp>
#define FPGA_HARDWARE 1
#if FPGA_HARDWARE || FPGA_EMULATOR || FPGA_SIMULATOR
#include <sycl/ext/intel/fpga_extensions.hpp>
#endif
