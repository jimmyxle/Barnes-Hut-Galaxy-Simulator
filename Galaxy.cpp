#include "Galaxy.h"
//#include <thread>
#include <ctime>
#include "tbb/parallel_for.h"





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

Galaxy::Galaxy(double _x, double _y, double _centerMass, int _NUM_P, double vel_x, double vel_y)
{
	NUMBER_PARTICLES = _NUM_P;

	allParticles.reserve(NUMBER_PARTICLES );
	//pass x,y which are the center of the galaxy
	//number of particles to make
	//radius of galaxy initially
	x = _x;
	y = _y;
	allParticles = particle.generateParticles(x, y, NUMBER_PARTICLES, 0.20,
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
	this adds each other galaxies forces together without me making too many quadrants
*/
void Galaxy::add_galaxy(Galaxy& galaxy)
{
	/*
	galaxy.allParticles.push_back(allParticles[0]);
	this->allParticles.push_back(galaxy.allParticles[0]);
	/**/
	
	
	for (auto it = allParticles.begin(); it != allParticles.end(); it++)
	{
		galaxy.allParticles.push_back(*it);
	}

	/*

	
	for (auto it = galaxy.allParticles.begin(); it != galaxy.allParticles.end(); it++)
	{
		this->allParticles.push_back(*it);
	}
	
	/**/

}

//following functions are for drawing points
void Galaxy::displayParticles(std::vector<ParticleData*> arr) 
{
	glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);


	glPointSize(3);
	glBegin(GL_POINTS);



	int SIZE = arr.size();
	bool init = true;
	do
	{
		glColor3f(0, 255, 0);


		double x = (arr[0])->xy->x;
		double y = (arr[0])->xy->y;
		glVertex2d(x, y);
		init = !init;
	} while (init);

	glPointSize(1);
	glColor3f(1, 1, 1);



	//data parallel
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
	glColor3f(1, 1, 1);

	glPointSize(2);
	glBegin(GL_POINTS);



	int SIZE = arr1.size();
	bool init = true;
	do
	{
		glColor3f(0, 0, 255);

		double x = (arr1[0])->xy->x;
		double y = (arr1[0])->xy->y;
		glVertex2d(x, y);

		x = (arr2[0])->xy->x;
		y = (arr2[0])->xy->y;
		glVertex2d(x, y);


		init = !init;
	} while (init);

	

	glPointSize(1);
	glColor3f(1, 1, 1);

	//data parallel
	for (int i = 1; i < SIZE/2; i++)
	{
		double x = (arr1[i])->xy->x;
		double y = (arr1[i])->xy->y;
		glVertex2d(x, y);
	}

	/**/


	glColor3f(0, 255, 0);

	for (int i = 1 ; i < SIZE/2; i++)
	{
		double x = (arr2[i])->xy->x;
		double y = (arr2[i])->xy->y;
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




	std::vector<Vector2D> forces1(NUMBER_PARTICLES);

	size_t max = NUMBER_PARTICLES;

	clock_t deltaTime = 0;
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

		root->computeMassDistribution(); //tbb parallel


		//uncomment these to show particles/quadrants
		displayParticles(allParticles);
		//displayQuadrant(*root);

		for (int i = 0; i < max; i++)
		{
			forces1[i].reset();
		}

		/*attract to center*/
		
	
		/**/
		//calc forces 
		
		tbb::parallel_for(size_t(0), max, [&](size_t i) {
			root->calcForce(*(allParticles[i]), i, (forces1[i]) );	
		});

		
		//data parallel

		tbb::parallel_for(size_t(1), max, [&](size_t i) {
			allParticles[i]->calcDistance(forces1[i]);
		});
		//do center last
		allParticles[0]->calcDistance(forces1[0]);


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
	std::vector<Vector2D> forces2(NUMBER_PARTICLES);


	size_t max = NUMBER_PARTICLES;

	clock_t deltaTime = 0;
	unsigned int frames = 0;
	//double  frameRate = 60;
	//double  averageFrameTimeMilliseconds = 33.333;

	std::clock_t end;
	std::clock_t start;
	double time;


	while (!glfwWindowShouldClose(window))
	{
		start = clock();

		//task parallel
		root->buildTree(allParticles, NUMBER_PARTICLES);
		second.root->buildTree( second.allParticles, second.NUMBER_PARTICLES);

		//data parallel
		root->computeMassDistribution();
		second.root->computeMassDistribution();

		//displayParticles(allParticles);
		displayParticles(allParticles, second.allParticles);
		//displayQuadrant(*root, *second.root);

		size_t max = allParticles.size()/2;


		for (int i = 0; i < max; i++)
		{
			forces[i].reset();
			forces2[i].reset();
		}

		//data parallel 

		
		tbb::parallel_for(size_t(1), max, [&](size_t i) 
		{
			
			root->calcForce(*(allParticles[i]), i, (forces[i]));
			second.root->calcForce(*(second.allParticles[i]), i, (forces2[i]));

			allParticles[i]->calcDistance(forces[i]);
			second.allParticles[i]->calcDistance(forces2[i]);
		});


		//do center last
		allParticles[0]->calcDistance(forces[0]);
		second.allParticles[0]->calcDistance(forces2[0]);



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


double Galaxy::clockToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}