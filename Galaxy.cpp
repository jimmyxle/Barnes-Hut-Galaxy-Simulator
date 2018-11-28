#include "Galaxy.h"


Galaxy::Galaxy()
{
	NUMBER_PARTICLES = 300;

	allParticles.reserve(NUMBER_PARTICLES);
	allParticles = particle.generateParticles(0, 0, NUMBER_PARTICLES, 0.5, 80000, 0, 0);

	//max is bot right corner , min is top left corner
	max = new Vector2D(1, -1, 0, 0);
	min = new Vector2D(-1, 1, 0, 0);
	root = new QuadNode(*min, *max, QuadNode::NONE, nullptr, 0);
	depth = 0;
}

Galaxy::Galaxy(float _x, float _y, float _centerMass, int _NUM_P,
	float vel_x, float vel_y, float radius)
{
	NUMBER_PARTICLES = _NUM_P;

	allParticles.reserve(NUMBER_PARTICLES);
	//pass x,y which are the center of the galaxy
	//number of particles to make
	//radius of galaxy initially
	x = _x;
	y = _y;
	allParticles = particle.generateParticles(x, y, NUMBER_PARTICLES, radius,
		_centerMass, vel_x, vel_y);
	float boxSize = 3;
	max = new Vector2D(boxSize, -boxSize, 0, 0);
	min = new Vector2D(-boxSize, boxSize, 0, 0);

	root = new QuadNode(*min, *max, QuadNode::NONE, nullptr, 0);
	depth = 0;
}

Galaxy::~Galaxy()
{
	delete root;
	delete max;
	delete min;
	for (auto it = allParticles.begin(); it != allParticles.end(); it++)
	{
		delete (*it);
	}
}

/*
this adds ones planets to the other list and change num planets var in main
*/
void Galaxy::add_galaxy(Galaxy& galaxy, float vel_x, float vel_y)
{
	for (auto it = galaxy.allParticles.begin(); it != galaxy.allParticles.end(); it++)
	{
		allParticles.push_back(*it);
	}
	NUMBER_PARTICLES *= 2;
}

//following functions are for drawing points
void Galaxy::displayParticles(std::vector<ParticleData*> arr, GLFWwindow* window)
{
	glfwMakeContextCurrent(window);

	glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0, 255, 0);
	glPointSize(4);
	glBegin(GL_POINTS);

	int SIZE = arr.size();
	bool init = true;
	do
	{
		float x = (arr[0])->xy->x;
		float y = (arr[0])->xy->y;
		glVertex2d(x, y);
		init = !init;
	} while (init);

	glEnd();
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);

	for (int i = 1; i < SIZE; i++)
	{
		float x = (arr[i])->xy->x;
		float y = (arr[i])->xy->y;
		glVertex2d(x, y);
	}

	glEnd();
	glPopMatrix();
}

void Galaxy::displayParticles2(std::vector<ParticleData*> arr1, 
	std::vector<ParticleData*> arr2, GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
	glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(4);

	glBegin(GL_POINTS);

	int SIZE = arr1.size();

	bool init = true;
	do
	{
		glColor3f(255, 0, 0);

		float x = (arr1[0])->xy->x;
		float y = (arr1[0])->xy->y;
		glVertex2d(x, y);

		x = (arr1[SIZE / 2])->xy->x;
		y = (arr1[SIZE / 2])->xy->y;
		glVertex2d(x, y);


		init = !init;
	} while (init);

	glEnd();

	glPointSize(1);
	glBegin(GL_POINTS);

	glColor3f(0, 255, 0);
	for (int i = 1; i < SIZE / 2; i++)
	{
		float x = (arr1[i])->xy->x;
		float y = (arr1[i])->xy->y;
		glVertex2d(x, y);
	}

	glColor3f(0, 255, 255);

	for (int i = SIZE / 2 + 1; i < SIZE; i++)
	{
		float x = (arr1[i])->xy->x;
		float y = (arr1[i])->xy->y;
		glVertex2d(x, y);
	}
	glEnd();
	glPopMatrix();
}


void Galaxy::recursiveBoxes(QuadNode& qt, float factor)
{
	glBegin(GL_POLYGON);
	glVertex2d(factor*qt.getVector(0).x, factor*qt.getVector(0).y);
	glVertex2d(factor*qt.getVector(2).x, factor*qt.getVector(0).y);
	glVertex2d(factor*qt.getVector(2).x, factor*qt.getVector(2).y);
	glVertex2d(factor*qt.getVector(0).x, factor*qt.getVector(2).y);


	glEnd();

	for (std::vector<QuadNode*>::iterator it = (qt).nodeArr.begin();
		it != (qt).nodeArr.end(); it++)
	{
		recursiveBoxes(*(*it), 1.0);
	}
}


void Galaxy::displayQuadrant(QuadNode& quad)
{

	glColor3f(255, 0, 0);
	glPointSize(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	recursiveBoxes(quad, 1.0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();
}
void Galaxy::displayQuadrant(QuadNode& quad, QuadNode& second)
{

	glColor3f(255, 0, 0);
	glPointSize(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	recursiveBoxes(quad, 1.0);
	recursiveBoxes(second, 1.0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();
}


float Galaxy::clockToMilliseconds(clock_t ticks) {
	return (ticks / (float)CLOCKS_PER_SEC)*1000.0f;
}


int Galaxy::running_display()
{
	std::cout << "Single galaxy mode:\t["<< NUMBER_PARTICLES <<"] particles." << std::endl;

	GLFWwindow* window;
	if (!glfwInit())
	{
		std::cout << "Error Initializing GLFW" << std::endl;
		return -1;
	}
	window = glfwCreateWindow(750, 750, "Barnes-Hut Tree: Single Galaxy", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* constants */
	size_t max = NUMBER_PARTICLES;

	/* arrays */
	std::vector<Vector2D> forces(NUMBER_PARTICLES);

	std::vector<cl_float4> positions(NUMBER_PARTICLES);
	std::vector<cl_float2> velocities(NUMBER_PARTICLES);

	std::vector<cl_float2> forces_copy(NUMBER_PARTICLES);
	
	std::vector<cl_float4> outVec(max);


	/* opencl context*/
	auto program2 = CreateProgram("force_check.cl", 0);
	auto context2 = program2->getInfo<CL_PROGRAM_CONTEXT>();
	auto devices2 = context2.getInfo<CL_CONTEXT_DEVICES>();
	_ASSERT(devices2.size() > 0);


	auto program = CreateProgram("calc_dist.cl", 1); //set to 0 for cpu, set to 1 for gpu

		//create context
	auto context = program->getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	_ASSERT(devices.size() > 0);



	auto& device = devices.back();
	auto& device_cpu = devices2.back();

	auto name = device.getInfo<CL_DEVICE_NAME>();
	auto name_cpu =  device_cpu.getInfo<CL_DEVICE_NAME>();

	cl_int err = 0;
	cl::Kernel kernel(*program, "calc_dist", &err);
	cl::Kernel kernel_cpu(*program2, "force_check", &err);
	//auto workgrpsz = kernel_cpu.getWorkGroupInfo < CL_KERNEL_WORK_GROUP_SIZE>(device_cpu, &err);
	//auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device, &err);
	auto workGroupSize = max;
	cl::CommandQueue queue(context, device);
	cl::CommandQueue queue_cpu(context2, device_cpu);

	/* clock stuff */
	clock_t deltaTime = 0;
	unsigned int frames = 0;
	std::clock_t end;
	std::clock_t begin;
	std::clock_t time;

	while (!glfwWindowShouldClose(window))
	{
		begin = clock();



		//task parallel
		depth = root->buildTree(allParticles, NUMBER_PARTICLES);
		/*
			x = x; y = y; vx = z; vy = w;
		
		*/
		for (unsigned int i = 0; i < max; i++)
		{
			positions[i].x = (allParticles[i]->xy->x);
			positions[i].y = (allParticles[i]->xy->y);
			positions[i].z = (allParticles[i]->xy->vx);
			positions[i].w = (allParticles[i]->xy->vy);

		}
		//data parallelism
		root->computeMassDistribution();
		
		displayParticles(allParticles, window);

		for (unsigned int i = 0; i < max; i++)
		{
			root->calcForce(*(allParticles[i]), (forces[i]));

			forces_copy[i].x = forces[i].x;
			forces_copy[i].y = forces[i].y;

		}
		cl::Buffer buf_force_copy(context2, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2)*max, forces_copy.data(), &err);
		cl::Buffer buf_out_force_copy(context2, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(cl_float2)*max, NULL, &err);

		err = kernel_cpu.setArg(0, buf_force_copy);
		err = kernel_cpu.setArg(1, buf_out_force_copy);

		err = queue_cpu.enqueueNDRangeKernel(kernel_cpu, cl::NullRange, cl::NDRange(max), cl::NDRange(workGroupSize));
		err = queue_cpu.enqueueReadBuffer(buf_out_force_copy, CL_TRUE, 0, sizeof(cl_float2)*max, forces_copy.data());
		/*
		here we do the opencl calls
		*/

			cl::Buffer buf_force(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2)*max, forces_copy.data(), &err);
			cl::Buffer buf_pos(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4)*max, positions.data(), &err);
			cl::Buffer outBuf_pos(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(cl_float4)*max, NULL, &err);
			err = kernel.setArg(0, buf_force);
			err = kernel.setArg(1, buf_pos);
			err = kernel.setArg(2, outBuf_pos);
			err = kernel.setArg(3, sizeof(cl_float2)*workGroupSize, nullptr); //local for forces
			err = kernel.setArg(4, sizeof(cl_float4)*workGroupSize, nullptr); //local for positions 

			err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(max), cl::NDRange(workGroupSize));
			err = queue.enqueueReadBuffer(outBuf_pos, CL_TRUE, 0, sizeof(cl_float4)*outVec.size(), outVec.data());

			for (unsigned int i = 0; i < max; i++)
			{
				(allParticles[i]->xy->x) = outVec[i].x;
				(allParticles[i]->xy->y) = outVec[i].y;
				(allParticles[i]->xy->vx) = outVec[i].z;
				(allParticles[i]->xy->vy) = outVec[i].w;

				if ((allParticles[i]->xy->x) >= 0.99f)
				{
					(allParticles[i]->xy->x) = 0.99f;
					(allParticles[i]->xy->vx) *= -1.0f;
				}
				if ((allParticles[i]->xy->x) <= -0.99f)
				{
					(allParticles[i]->xy->x) = -0.99f;
					(allParticles[i]->xy->vx) *= -1.0f;
				}
				if ((allParticles[i]->xy->y) >= 0.99f)
				{
					(allParticles[i]->xy->y) = 0.99f;
					(allParticles[i]->xy->vy) *= -1.0f;
				}
				if ((allParticles[i]->xy->y) <= -0.99f)
				{
					(allParticles[i]->xy->y) = -0.99f;
					(allParticles[i]->xy->vy) *= -1.0f;
				}
			}



			/* c++ wrapper class handles resource management
			https://stackoverflow.com/questions/17246541/releasing-opencl-memory-kernels-devices-etc
			*/

		

		
		/* end calc forces*/
		glfwSwapBuffers(window);
		glfwPollEvents();

		end = clock();
		time = (end - begin);
		deltaTime += time;

		frames++;

		if (clockToMilliseconds(deltaTime) > 1000.0) { //every second
			std::cout << "\t #frames was:\t[" << frames << "]" << std::endl;
		//	std::cout << "time per cycle: \t[" << time << "]" << std::endl;

			frames = 0;
			deltaTime = 0;
		}

	}
	glfwTerminate();
	return 0;
}





int Galaxy::two_running_display(Galaxy& second)
{
	std::cout << "Two Galaxies mode: ["<<NUMBER_PARTICLES<<"] particles" << std::endl;
	Vector2D target;

	GLFWwindow* window;
	if (!glfwInit())
	{
		std::cout << "Error Initializing GLFW" << std::endl;
		return -1;
	}
	window = glfwCreateWindow(750, 750, "Barnes-Hut Tree: Two Galaxies", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}

	size_t max = NUMBER_PARTICLES;

	/* vectors */
	std::vector<Vector2D> forces(NUMBER_PARTICLES);

	std::vector<cl_float4> positions(NUMBER_PARTICLES);
	std::vector<cl_float2> velocities(NUMBER_PARTICLES);

	std::vector<cl_float2> forces_copy(NUMBER_PARTICLES);

	auto program2 = CreateProgram("force_check.cl", 0);
	auto context2 = program2->getInfo<CL_PROGRAM_CONTEXT>();
	auto devices2 = context2.getInfo<CL_CONTEXT_DEVICES>();
	_ASSERT(devices2.size() > 0);

	/* opencl context*/
	auto program = CreateProgram("calc_dist.cl", 1); //set to 0 for cpu, set to 1 for gpu
	//create context
	auto context = program->getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	//auto devic   = context.getInfo<CL_CONTEXT_DEVICES>();
	

	_ASSERT(devices.size() > 0);
	auto& device = devices.back();
	auto& device_cpu = devices2.back();


	cl_int err = 0;
	cl::Kernel kernel(*program, "calc_dist", &err);
	cl::Kernel kernel_cpu(*program2, "force_check", &err);

	//auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device, &err);
	auto workGroupSize = max;
	std::vector<cl_float4> outVec(max);
	cl::CommandQueue queue(context, device);

	cl::CommandQueue queue_cpu(context2, device_cpu);


	clock_t deltaTime = 0;
	unsigned int frames = 0;
	std::clock_t end;
	std::clock_t start;
	std::clock_t time;

	std::clock_t test_start, test_end;
	while (!glfwWindowShouldClose(window))
	{
		start = clock();

		//task parallel
		//std::cout << " < ==== >\n";
		//test_start = clock();
		depth = root->buildTree(allParticles, NUMBER_PARTICLES);
	//	std::cout << "depth: " << this->depth << "\n";
		/*
		x = x; y = y; vx = z; vy = w;

		*/
		for (unsigned int i = 0; i < max; i++)
		{
			positions[i].x = (allParticles[i]->xy->x);
			positions[i].y = (allParticles[i]->xy->y);
			positions[i].z = (allParticles[i]->xy->vx);
			positions[i].w = (allParticles[i]->xy->vy);

		}
		//test_end = clock();
		//std::cout << "build tree took: " << test_end - test_start << "\n";

		//data parallel
		//test_start = clock();
		root->computeMassDistribution();

		//root->computeMassDistribution_iterative();
		//test_end = clock();
		//std::cout << "cmd took: " << test_end - test_start << "\n";

		//test_start = clock();
		displayParticles2(allParticles, second.allParticles, window);
		//test_end = clock();
		//std::cout << "display tree took: " << test_end - test_start << "\n";
	
		//displayQuadrant(*root, *second.root);

		//size_t max = allParticles.size();



		//data parallel 
		//test_start = clock();
		for (unsigned int i = 0; i < max; i++)
		{
			root->calcForce(*(allParticles[i]), (forces[i]));


			forces_copy[i].x = forces[i].x;
			forces_copy[i].y = forces[i].y;
		}
		//test_end = clock();
		//std::cout << "calcforce tree took: " << test_end - test_start << "\n";
		

		/*
		here we do the opencl call
		*/

		cl::Buffer buf_force_copy(context2, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2)*max, forces_copy.data(), &err);
		cl::Buffer buf_out_force_copy(context2, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(cl_float2)*max, NULL, &err);

		err = kernel_cpu.setArg(0, buf_force_copy);
		err = kernel_cpu.setArg(1, buf_out_force_copy);

		err = queue_cpu.enqueueNDRangeKernel(kernel_cpu, cl::NullRange, cl::NDRange(max), cl::NDRange(workGroupSize));
		err = queue_cpu.enqueueReadBuffer(buf_out_force_copy, CL_TRUE, 0, sizeof(cl_float2)*max, forces_copy.data());
		
		
	//	test_start = clock();
			cl::Buffer buf_force(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2)*max, forces_copy.data(), &err);
			cl::Buffer buf_pos(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4)*max, positions.data(), &err);
			cl::Buffer outBuf_pos(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(cl_float4)*max, NULL, &err);
			err = kernel.setArg(0, buf_force);
			err = kernel.setArg(1, buf_pos);
			err = kernel.setArg(2, outBuf_pos);
			err = kernel.setArg(3, sizeof(cl_float2)*workGroupSize, nullptr); //local for forces
			err = kernel.setArg(4, sizeof(cl_float4)*workGroupSize, nullptr); //local for positions 


			//create command queueue
			err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(max), cl::NDRange(workGroupSize));
			err = queue.enqueueReadBuffer(outBuf_pos, CL_TRUE, 0, sizeof(cl_float4)*outVec.size(), outVec.data());

			//queue.flush();
		//	queue.finish();

			for (unsigned int i = 0; i < max; i++)
			{
				(allParticles[i]->xy->x) = outVec[i].x;
				(allParticles[i]->xy->y) = outVec[i].y;
				(allParticles[i]->xy->vx) = outVec[i].z;
				(allParticles[i]->xy->vy) = outVec[i].w;

				if ((allParticles[i]->xy->x) >= 0.99f)
				{
					(allParticles[i]->xy->x) = 0.99f;
					(allParticles[i]->xy->vx) *= -0.5f;
				}
				if ((allParticles[i]->xy->x) <= -0.99f)
				{
					(allParticles[i]->xy->x) = -0.99f;
					(allParticles[i]->xy->vx) *= -0.5f;
				}
				if ((allParticles[i]->xy->y) >= 0.99f)
				{
					(allParticles[i]->xy->y) = 0.99f;
					(allParticles[i]->xy->vy) *= -0.5f;
				}
				if ((allParticles[i]->xy->y) <= -0.99f)
				{
					(allParticles[i]->xy->y) = -0.99f;
					(allParticles[i]->xy->vy) *= -0.5f;
				}

			}
		//	test_end = clock();
		//	std::cout << "calcdist took: " << test_end - test_start << "\n";



			/* c++ wrapper class handles resource management
			https://stackoverflow.com/questions/17246541/releasing-opencl-memory-kernels-devices-etc
			*/


			for(unsigned int i = 0; i < max; i++) forces[i].reset();
		

		glfwSwapBuffers(window);
		glfwPollEvents();

		end = clock();;
		time = (end - start);
		deltaTime += time;

		frames++;

		if ((deltaTime) >= 1000) { //every second


			std::cout << "\t fps was:\t[" << frames << "]" << std::endl;

			frames = 0;
			deltaTime = 0;


		}

	}
	glfwTerminate();
	return 0;
}

