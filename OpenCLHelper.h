#pragma once
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS


#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
cl::Program CreateProgram(const std::string& file, unsigned int processor);
std::vector<cl::Device> get_running_devices();