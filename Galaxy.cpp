#include "Galaxy.h"
#include <ctime>
#include <thread>
#include "OpenCLHelper.h"



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

Galaxy::Galaxy(double _x, double _y, double _centerMass, int _NUM_P,
	double vel_x, double vel_y, double radius)
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
	double boxSize = 3;
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
void Galaxy::add_galaxy(Galaxy& galaxy, double vel_x, double vel_y)
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
		double x = (arr[0])->xy->x;
		double y = (arr[0])->xy->y;
		glVertex2d(x, y);
		init = !init;
	} while (init);

	glEnd();
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);

	for (int i = 1; i < SIZE; i++)
	{
		double x = (arr[i])->xy->x;
		double y = (arr[i])->xy->y;
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

		double x = (arr1[0])->xy->x;
		double y = (arr1[0])->xy->y;
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
		double x = (arr1[i])->xy->x;
		double y = (arr1[i])->xy->y;
		glVertex2d(x, y);
	}

	glColor3f(0, 255, 255);

	for (int i = SIZE / 2 + 1; i < SIZE; i++)
	{
		double x = (arr1[i])->xy->x;
		double y = (arr1[i])->xy->y;
		glVertex2d(x, y);
	}
	glEnd();
	glPopMatrix();
}


void Galaxy::recursiveBoxes(QuadNode& qt, double factor)
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


double Galaxy::clockToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

int Galaxy::running_display()
{
	std::cout << "running display mode" << std::endl;

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
	std::vector<Vector2D> forces1(NUMBER_PARTICLES);
	std::vector<Vector2D> positions1(NUMBER_PARTICLES);

	/* opencl context*/
	auto program = CreateProgram("calc_dist.cl", 0); //set to 0 for cpu, set to 1 for gpu
		//create context
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo <CL_CONTEXT_DEVICES>();
	_ASSERT(devices.size() > 0);
	auto& device = devices.back();

	/* clock stuff */
	clock_t deltaTime = 0;
	unsigned int frames = 0;
	std::clock_t end;
	std::clock_t begin;
	std::clock_t time;

	while (!glfwWindowShouldClose(window))
	{
		begin = clock();
		//start = begin;

		for (unsigned int i = 0; i < max; i++)
		{
			forces1[i].reset();
		//	positions1[i].reset();
		}

		//task parallel
		depth = root->buildTree(allParticles, NUMBER_PARTICLES);
		for (unsigned int i = 0; i < max; i++)
		{
			positions1[i] = *(allParticles[i]->xy);
		}
		//data parallelism
		std::thread cmd_th(&QuadNode::computeMassDistribution, root);
		displayParticles(allParticles, window);

		cmd_th.join();
		//display_th.join();



		/**/
		//calc forces 
		//data parallel

		for (unsigned int i = 0; i < max; i++)
		{
			root->calcForce(*(allParticles[i]), (forces1[i]));

		}

		/*
		here we od the opencl call
		*/
		
		cl_int err = 0;

		cl::Kernel kernel(program, "calc_dist");
		auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device, &err);

		auto numWorkGroups = max / workGroupSize;

		cl::Buffer buf_force(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(Vector2D)*max, forces1.data(), &err);
	
		cl::Buffer buf_pos(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(Vector2D)*max, positions1.data(), &err);
		cl::Buffer outBuf_pos(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(Vector2D)*numWorkGroups,NULL, &err);
		
		/*
		0. __global Vector2D* in_force
		1. __local Vector2D* local_force
		2. __global Vector2D* out_force
		3. __global Vector2D* in_pos
		4. __local Vector2D* local_pos
		5. __global vector2D* out_pos
		*/
		err = kernel.setArg(0, buf_force);
		kernel.setArg(1, sizeof(Vector2D)*workGroupSize, nullptr);

		err = kernel.setArg(2, buf_pos);
		kernel.setArg(3, sizeof(Vector2D)*workGroupSize, nullptr);
		err = kernel.setArg(4, outBuf_pos);


		std::vector<Vector2D> outVec(max);

		//create command queueue
		cl::CommandQueue queue(context, device);
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange( max ), cl::NDRange(workGroupSize));

		queue.enqueueReadBuffer(outBuf_pos, CL_TRUE, 0, sizeof(Vector2D)*outVec.size(), outVec.data());

		//allParticles[i]->calcDistance(forces1[i]);

		//allParticles[0]->calcDistance(forces1[0]);

		for (unsigned int i = 0; i < max; i++)
		{
			*(allParticles[i]->xy) = outVec[i];
		}

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
	std::cout << "running display mode" << std::endl;
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
	//glfwMakeContextCurrent(window);

	std::vector<Vector2D> forces(NUMBER_PARTICLES);
	size_t max = NUMBER_PARTICLES;

	clock_t deltaTime = 0;
	unsigned int frames = 0;


	std::clock_t end;
	std::clock_t start;
	std::clock_t time;


	while (!glfwWindowShouldClose(window))
	{
		start = clock();

		//task parallel
		root->buildTree(allParticles, NUMBER_PARTICLES);

		//data parallel

		std::thread cmd_th(&QuadNode::computeMassDistribution, root);
		
		displayParticles2(allParticles, second.allParticles, window);

		cmd_th.join();


		//displayParticles2(allParticles, second.allParticles);
		//displayQuadrant(*root, *second.root);

		size_t max = allParticles.size();


		for (unsigned int i = 0; i < max; i++)
		{
			forces[i].reset();
		}

		//data parallel 

		for (unsigned int i = 0; i < max; i++)
		{
			root->calcForce(*(allParticles[i]), (forces[i]));
			allParticles[i]->calcDistance(forces[i]);
		}
		
		

		//do center last
	//	root->calcForce(*(allParticles[0]), (forces[0]));

	//	allParticles[0]->calcDistance(forces[0]);

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

