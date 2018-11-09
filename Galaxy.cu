#include "Galaxy.h"
#include <ctime>
#include "tbb/parallel_for.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <sstream>



__global__ void calcDistance_GPU(Vector2D* forces, Vector2D* particles, int count)
{

	int idx = threadIdx.x + blockIdx.x * blockDim.x;
	if (idx < count)
	{
		Vector2D& thisForce = forces[idx];
		Vector2D& thisParticle = particles[idx];

		for (int i = 0; i < count; i++)
		{
			if (i == idx) continue;


			//timestep per calculation
			double TIME = 0.0015;
			{
				//prevents NaN problems
				if (thisForce.x != thisForce.x)
					thisForce.x = 0;

				if (thisForce.y != thisForce.y)
					thisForce.y = 0;
			}

			double acc_x = forces[idx].x;
			double acc_y = forces[idx].y;
			//prevents points from accelerating too far from the center
			double max = 1.0 / 25;
			if (acc_x >= max)
			{
				acc_x = max;
			}
			if (acc_x < -max)
			{
				acc_x = -max;
			}

			if (acc_y >= max)
			{
				acc_y = max;
			}
			if (acc_y < -max)
			{
				acc_y = -max;
			}

			//velocities

			thisParticle.vx += acc_x * TIME;
			thisParticle.vy += acc_y * TIME;

			//positions
			thisParticle.x += thisParticle.vx;
			thisParticle.y += thisParticle.vy;

			//bounce particles off the borders
			if (thisParticle.x >= 0.99)
			{
				thisParticle.x = 0.99;
				thisParticle.vx *= -0.5;
			}
			if (thisParticle.x <= -0.99)
			{
				thisParticle.x = -0.99;
				thisParticle.vx *= -0.5;
			}
			if (thisParticle.y >= 0.99)
			{
				thisParticle.y = 0.99;
				thisParticle.vy *= -0.5;
			}
			if (thisParticle.y <= -0.99)
			{
				thisParticle.y = -0.99;
				thisParticle.vy *= -0.5;
			}


		}
	}


}



Galaxy::Galaxy()
{
	NUMBER_PARTICLES = 300;

	allParticles.reserve(NUMBER_PARTICLES);
	allParticles = particle.generateParticles(0, 0, NUMBER_PARTICLES, 0.5, 80000,0,0);
	
	//max is bot right corner , min is top left corner
	max = new Vector2D(1, -1, 0, 0);
	min = new Vector2D(-1, 1, 0, 0);
	root = new QuadNode(*min, *max, QuadNode::NONE, nullptr);
}

Galaxy::Galaxy(double _x, double _y, double _centerMass, int _NUM_P,
	double vel_x, double vel_y, double radius)
{
	NUMBER_PARTICLES = _NUM_P;

	allParticles.reserve(NUMBER_PARTICLES );
	//pass x,y which are the center of the galaxy
	//number of particles to make
	//radius of galaxy initially
	x = _x;
	y = _y;
	allParticles = particle.generateParticles(x, y, NUMBER_PARTICLES, radius,
		_centerMass, vel_x, vel_y);
	double boxSize = 3 ;
	max = new Vector2D(boxSize, -boxSize, 0, 0);
	min = new Vector2D(-boxSize, boxSize, 0, 0);

	root = new QuadNode(*min, *max, QuadNode::NONE, nullptr);
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
void Galaxy::displayParticles(std::vector<ParticleData*> arr) 
{
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

void Galaxy::displayParticles(std::vector<ParticleData*> arr1, std::vector<ParticleData*> arr2)
{
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
	for (int i = 1; i < SIZE/2; i++)
	{
		double x = (arr1[i])->xy->x;
		double y = (arr1[i])->xy->y;
		glVertex2d(x, y);
	}

	glColor3f(0, 255, 255);

	for (int i = SIZE/2+1 ; i < SIZE; i++)
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

int Galaxy::running_display()
{
	std::cout << "running display mode" << std::endl;
	Vector2D target;

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
	glfwMakeContextCurrent(window);

	//std::vector<Vector2D> forces1(NUMBER_PARTICLES);

	size_t max = NUMBER_PARTICLES;
	Vector2D* forces1[ 150 ];

	for (unsigned int i = 0; i < max; i++)
	{
		forces1[i] = new Vector2D();
	}

	double deltaTime = 0;
	unsigned int frames = 0;
	double  frameRate = 30;
	double  averageFrameTimeMilliseconds = 33.333;

	std::clock_t end;
	std::clock_t begin;
	std::clock_t start;
	double time;

	while (!glfwWindowShouldClose(window))
	{
		begin = clock();
		start = begin;


		//task parallel
		root->buildTree(allParticles, NUMBER_PARTICLES);

		//data parallelism
		root->computeMassDistribution();

		//uncomment these to show particles/quadrants
		displayParticles(allParticles);
		//displayQuadrant(*root);

		/**/
		//calc forces 
		//data parallel

		tbb::parallel_for(size_t(0), max, [&](size_t i) {
			root->calcForce(*(allParticles[i]),  *(forces1[i]) );	
		});

		/*here we try to do cuda first*/

		//make array of forces
		//Vector2D *h_force = new Vector2D[max];
		Vector2D *d_force;

		Vector2D *h_particles = new Vector2D[max];
		Vector2D *d_particles;
		
		for (unsigned int i = 0; i < max; i++)
		{
			//h_force[i] = *forces1[i];
			h_particles[i] = *(allParticles[i])->xy;
		}
		
		if (cudaMalloc(&d_force, sizeof(Vector2D)*max) != cudaSuccess)
		{
			//delete[] d_force;
			std::stringstream ss;
			ss << "Can't malloc d_force_x.";
			throw std::runtime_error(ss.str());
		}
		if (cudaMalloc(&d_particles, sizeof(Vector2D)*max) != cudaSuccess)
		{
			delete[] d_force;
			std::stringstream ss;
			ss << "Can't malloc d_force_x.";
			throw std::runtime_error(ss.str());
		}

		if (cudaMemcpy(d_force, forces1, sizeof(Vector2D)*max, cudaMemcpyHostToDevice) != cudaSuccess)
		{
			cudaFree(d_force);
	//		delete[] forces1;
			cudaFree(d_particles);
			delete[] h_particles;
			std::stringstream ss;
			ss << "Can't copy  host force_x.";
			throw std::runtime_error(ss.str());
		}

		if (cudaMemcpy(d_particles, h_particles, sizeof(Vector2D)*max, cudaMemcpyHostToDevice) != cudaSuccess)
		{
			cudaFree(d_force);
	//		delete[] forces1;
			cudaFree(d_particles);
			delete[] h_particles;
			std::stringstream ss;
			ss << "Can't copy  host force_x.";
			throw std::runtime_error(ss.str());
		}

		 //kernel 
	//kernal <<< 1231,13123,1321 >>();
		calcDistance_GPU<<<((max / 32) + 1), 32>>>(d_force, d_particles, max);
		//cudaDeviceSynchronize();



		if (cudaMemcpy(h_particles , d_particles, sizeof(Vector2D)*max, cudaMemcpyDeviceToHost) != cudaSuccess)
		{
			cudaFree(d_force);
			//delete[] forces1;
			cudaFree(d_particles);
			delete[] h_particles;
			std::stringstream ss;
			ss << "Can't copy device d_force_x.";
			throw std::runtime_error(ss.str());
		}
		/*
		std::cout << "host_particles\n";
		for (unsigned int i = 0; i < max; i++)
		{
			std::cout << h_particles[i].x<<", "<<h_particles[i].y<<"\n";
		}
		std::cout << "allParticles before\n";

		for (unsigned int i = 0; i < max; i++)
		{
			std::cout << (allParticles[i])->xy->x << ", " << allParticles[i]->xy->y << "\n";
		}
		*/

		
		for (unsigned int i = 0; i < max; i++)
		{
			*(allParticles[i])->xy = h_particles[i];
		}
		

		cudaFree(d_force);
		cudaFree(d_particles);
		delete[] h_particles;
		//delete[] h_force;

		/**/

		//do center last
		allParticles[0]->calcDistance(*forces1[0]);


		for (unsigned int i = 0; i < max; i++)
		{
			forces1[i]->reset();
		}
	

		/* end calc forces*/
		glfwSwapBuffers(window);
		glfwPollEvents();

		end = clock();
		time = (end - start);
		deltaTime += time;

		frames++;

		if (clockToMilliseconds(deltaTime) > 1000.0) { //every second
			frameRate = (double)frames*0.5 + frameRate * 0.5; //more stable
			frames = 0;
			deltaTime -= CLOCKS_PER_SEC;
			averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);

			std::cout << "\tFrameTime was:\t[" << averageFrameTimeMilliseconds <<"]"<< std::endl;
			std::cout << "time per cycle: \t[" << time << "]" << std::endl;
		}

	}
	glfwTerminate();
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
	glfwMakeContextCurrent(window);

	std::vector<Vector2D> forces(NUMBER_PARTICLES);
	//size_t max = NUMBER_PARTICLES;

	clock_t deltaTime = 0;
	unsigned int frames = 0;


	std::clock_t end;
	std::clock_t start;
	double time;


	while (!glfwWindowShouldClose(window))
	{
		start = clock();

		//task parallel
		root->buildTree(allParticles, NUMBER_PARTICLES);

		//data parallel
		root->computeMassDistribution();

		displayParticles(allParticles, second.allParticles);
		//displayQuadrant(*root, *second.root);

		size_t max = allParticles.size();


		for (unsigned int i = 0; i < max; i++)
		{
			forces[i].reset();
		}

		//data parallel 

		tbb::parallel_for(size_t(1), max, [&](size_t i) 
		{
			root->calcForce(*(allParticles[i]),  (forces[i]));
			allParticles[i]->calcDistance(forces[i]);
		});

		//do center last
		root->calcForce(*(allParticles[0]),  (forces[0]));
		allParticles[0]->calcDistance(forces[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();

		end = clock();;
		time = (end - start);
		deltaTime += time;

		frames++;

		if ( (deltaTime) >= 1.0) { //every second
			std::cout << 1000.0 / double(frames) << std::endl;

			double fps = double(frames) / deltaTime;

			std::cout << "\t fps was:\t[" << fps << "]" << std::endl;

			frames = 0;

			std::cout << "time per cycle: \t[" << time << "]" << std::endl;

		}

	}
	glfwTerminate();
}

/**/
double Galaxy::clockToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}
/**/