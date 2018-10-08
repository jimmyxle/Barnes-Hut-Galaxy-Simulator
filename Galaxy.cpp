#include "Galaxy.h"
#include <thread>
#include <ctime>

const int nThreads = 300;

Galaxy::Galaxy()
{
	NUMBER_PARTICLES = 300;

	allParticles.reserve(NUMBER_PARTICLES);
	allParticles = particle.generateParticles(0, 0, NUMBER_PARTICLES, 0.5);
	
	//max is bot right corner , min is top left corner
	max = new Vector2D(1, -1, 0, 0);
	min = new Vector2D(-1, 1, 0, 0);
	root = new QuadNode(*min, *max, QuadNode::NONE, nullptr);
}

Galaxy::Galaxy(double x, double y)
{
	NUMBER_PARTICLES = 300;

	allParticles.reserve(NUMBER_PARTICLES);
	//pass x,y which are the center of the galaxy
	//number of particles to make
	//radius of galaxy initially
	allParticles = particle.generateParticles(x, y, NUMBER_PARTICLES, 0.3);

	max = new Vector2D(1, -1, 0, 0);
	min = new Vector2D(-1, 1, 0, 0);

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
	for (auto it = allParticles.begin(); it != allParticles.end(); it++)
	{
		galaxy.renegades.push_back(*it);
	}
	for (auto it = galaxy.allParticles.begin(); it != galaxy.allParticles.end(); it++)
	{
		this->renegades.push_back(*it);
	}

}

//following functions are for drawing points
void Galaxy::displayParticles(std::vector<ParticleData*> arr) 
{
	glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glPointSize(1);
	glBegin(GL_POINTS);




	//data parallel
	for (auto &particle : arr)
	{
		double x = (particle)->xy->x;
		double y = (particle)->xy->y;
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
	glPointSize(1);
	glBegin(GL_POINTS);

	//tried to make this parallel but it wouldn't show anything


	for (auto &particle : arr1)
	{
		double x = (particle)->xy->x;
		double y = (particle)->xy->y;
		glVertex2d(x, y);
	}

	glColor3f(0, 255, 0);

	for (auto &particle : arr2)
	{
		double x = (particle)->xy->x;
		double y = (particle)->xy->y;
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

	while (!glfwWindowShouldClose(window))
	{
		std::cout << "cycle begin.\n";
		std::clock_t begin = clock();


		//the loop

		//task parallel
		root->buildTree(allParticles, NUMBER_PARTICLES);

		//data parallelism
		root->computeMassDistribution();


		//uncomment these to show particles/quadrants
		displayParticles(allParticles);
		//		displayQuadrant(*root);

		//store forces calculated
		std::vector<Vector2D> forces1(NUMBER_PARTICLES);

		std::thread t_arr2[nThreads];
		int max = NUMBER_PARTICLES;

		//data parallel
		for (int i = 0; i<nThreads; i++)
		{
			t_arr2[i] = std::thread(&QuadNode::calcForce, root,std::ref(*(allParticles[i])),std::ref(i), std::ref(forces1[i]) ); 
		}
		
		for (int i = 0; i < nThreads; i++)
		{
			t_arr2[i].join();
		}

		std::thread t_arr3[nThreads];


		//data parallel
		for (int i = 0; i < nThreads; i++)
		{
			t_arr3[i] = std::thread(&ParticleData::calcDistance, allParticles[i],
			std::ref(forces1[i]) );
		}
		

		for (int i = 0; i < nThreads; i++)
		{
			t_arr3[i].join();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		std::clock_t end = clock();
		double time = (end - begin);
		std::cout<<"time per cycle: " << time << std::endl;

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
	std::clock_t begin;
	std::clock_t end;


	while (!glfwWindowShouldClose(window))
	{
		std::cout << "cycle begin.\n";
		begin = clock();

		//task parallel
		root->buildTree(allParticles, NUMBER_PARTICLES);
		second.root->buildTree( second.allParticles, second.NUMBER_PARTICLES);

		//data parallel
		root->computeMassDistribution();
		second.root->computeMassDistribution();

		displayParticles(allParticles, second.allParticles);
		displayQuadrant(*root, *second.root);



		//data parallel 
		int const nThreads2 = 600;
		std::thread t_arr2[nThreads2];

		for (int i = 0; i < nThreads; i += 2)
		{
			t_arr2[i] = std::thread(&QuadNode::calcForce, root,
				std::ref(*(allParticles[i])), std::ref(i), std::ref(forces[i]));

			t_arr2[i+1]= std::thread(&QuadNode::calcForce, (second.root),
				std::ref(*(second.allParticles[i])), std::ref(i), std::ref(forces2[i]));
		
		}
		/*
				for (int i = 0; i < NUMBER_PARTICLES; i++)
		{
			t_arr4[i] = std::thread(&QuadNode::calcForce, (second.root), 
				std::ref(*(second.allParticles[i])), std::ref(i), std::ref(forces2[i]));
		}
		*/


	
		for (int i = 0; i < nThreads; i++)
		{
			t_arr2[i].join();
		}
		/*
				for (int i = 0; i < nThreads; i++)
		{
			t_arr2[i].join();
			//t_arr4[i].join();
		}
		*/

		//data parallel
		std::thread t_arr3[nThreads2];
		// std::thread t_arr5[nThreads];

		//		not sure why this works 

		for (int i = 0; i < nThreads; i+=2)
		{
			t_arr3[i] = std::thread(&ParticleData::calcDistance, allParticles[i],
				std::ref(forces[i]));
			t_arr3[i+1] = std::thread(&ParticleData::calcDistance, second.allParticles[i],
				std::ref(forces2[i]));
		}
		for (int i = 0; i < nThreads; i++)
		{
			t_arr3[i].join();
		}
		/*
		//this is if the above doesn't work
				for (int i = 0; i < nThreads; i+=2)
		{

			t_arr5[i] = std::thread(&ParticleData::calcDistance, second.allParticles[i],
				std::ref(forces2[i]));
		}
		for (int i = 0; i < nThreads; i++)
		{
		t_arr5[i].join();
		}
		*/




		glfwSwapBuffers(window);
		glfwPollEvents();
		end = clock();
		double time = (end - begin);
		std::cout << "time per cycle: " << time << std::endl;

	}
	glfwTerminate();
}
