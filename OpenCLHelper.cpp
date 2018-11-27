#include "OpenCLHelper.h"


cl::Program* CreateProgram(const std::string& file, unsigned int processor)
{

	std::vector<cl::Device> devices = get_running_devices();
	//keep as cpu so its not so bad when you fuck up the algorithm. When confident, change to gpu
	cl::Device device;
	if (processor == 0)
		device = devices.front();
	else
		device = devices.back();

	auto name2 = device.getInfo<CL_DEVICE_NAME>();


//	auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
//	auto name = device.getInfo<CL_DEVICE_NAME>();
	cl_int err = 0;
	std::ifstream fp(file);
	std::string src(std::istreambuf_iterator<char>(fp), (std::istreambuf_iterator<char>()));
	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1) );
	cl::Context* context = new  cl::Context(device, &err);
	cl::Program* program = new  cl::Program(*context, sources, &err);

	auto context2 = program->getInfo<CL_PROGRAM_CONTEXT>();
	auto devices2 = context2.getInfo<CL_CONTEXT_DEVICES>();

	err = program->build("-cl-std=CL1.2");

	return program;

}

std::vector<cl::Device> get_running_devices()
{

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;

	cl::Device cpu;
	cl::Device gpu;

	cl::Platform::get(&platforms);




	_ASSERT(platforms.size() > 0);
	for (unsigned int i = 0; i < platforms.size(); i++)
	{
		cl::Platform default_platform = platforms[i];

		default_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		_ASSERT(devices.size() > 0);


		std::vector<cl::Device> curr;
		default_platform.getDevices(CL_DEVICE_TYPE_CPU, &curr);
		if (curr.size() > 0)
		{
			cl_uint curr_value = 0;
			clGetDeviceInfo(curr[0](), CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &curr_value, NULL);

			cl_uint cpu_value = 0;
			clGetDeviceInfo(cpu(), CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &cpu_value, NULL);

			if (curr_value > cpu_value) {
				cpu = curr[0];

			}


		}

		default_platform.getDevices(CL_DEVICE_TYPE_GPU, &curr);
		if (curr.size() > 0)
		{
			cl_uint curr_value = 0;
			clGetDeviceInfo(curr[0](), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &curr_value, NULL);
			cl_uint gpu_value = 0;
			clGetDeviceInfo(gpu(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &gpu_value, NULL);

			if (curr_value > gpu_value) {
				gpu = curr[0];
			}
		}
	}
	std::vector<cl::Device> running_devices{ cpu, gpu };
	//std::cout << "cpu: " << running_devices[0].getInfo<CL_DEVICE_NAME>() 
	//	<< "\ngpu: " << running_devices[1].getInfo<CL_DEVICE_NAME>() << "\t work units: " << gpu.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "\n";
	//
	std::cout << "cpu: " << running_devices[0].getInfo<CL_DEVICE_NAME>()
		<< "\ngpu: " << running_devices[1].getInfo<CL_DEVICE_NAME>() << "\n";

	
	return running_devices;

}

