#include <GLFW/glfw3.h>
#include "ParticleData.h"
#include <iostream>
#include "Tree.h"
#include <Windows.h>
/*
Note: displayQuadrant only shows one box so far, ill have to go through my tree 
recusively to find the other quadrants
*/

void displayParticles(std::vector<ParticleData*> arr ) {
	glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glPointSize(1);
	glBegin(GL_POINTS);

	for (auto &particle : arr)
	{
		double x = (particle)->xy->x;
		double y = (particle)->xy->y;
		glVertex2d(x, y);
	}
	glEnd();
	glPopMatrix();
}

void recursiveBoxes( QuadNode& qt, double factor)
{
	//std::cout << "0\t";
	//std::cout << factor * qt.getVector(0).x <<"\t"<< factor*qt.getVector(0).y << std::endl;
	//std::cout << "1\t";

	//4 children, 0 NE, 2SE, 
//std::cout << factor * qt.getVector(1).x <<"\t"<< factor*qt.getVector(1).y << std::endl;
	glBegin(GL_POLYGON);
	glVertex2d( factor*qt.getVector(0).x, factor*qt.getVector(0).y);
	glVertex2d( factor*qt.getVector(2).x, factor*qt.getVector(0).y);
	glVertex2d(factor*qt.getVector(2).x, factor*qt.getVector(2).y);
	glVertex2d(factor*qt.getVector(0).x, factor*qt.getVector(2).y);

	//std::cout << "3\t";

	//std::cout << factor * qt.getVector(3).x << "\t" << factor * qt.getVector(3).y << std::endl;


	//std::cout << "2\t";

	//std::cout << factor * qt.getVector(2).x << "\t" << factor * qt.getVector(2).y << std::endl;
	/*
	glVertex2d(factor*qt.getVector(3).x, factor*qt.getVector(3).y);
	glVertex2d(factor*qt.getVector(2).x, factor*qt.getVector(2).y);

	glVertex2d(factor*qt.getVector(2).x, factor*qt.getVector(2).y);
	glVertex2d(factor*qt.getVector(0).x, factor*qt.getVector(0).y);
	//std::cout << "\n";
	*/
	glEnd();

	for (std::vector<QuadNode*>::iterator it = (qt).nodeArr.begin();
		it != (qt).nodeArr.end(); it++)
	{
		recursiveBoxes( *(*it) , 1.0 );
	}
}


void displayQuadrant( QuadNode& quad)  
{
	//glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(255, 0, 0);
	glPointSize(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	 recursiveBoxes(quad, 1.0);
	

	 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();
}

int display(std::vector<ParticleData*> allParticles, QuadNode* root, double TIME, int NUMBER_PARTICLES)
{
	Vector2D target;
	GLFWwindow* window;
	if (!glfwInit())
	{
		std::cout << "Error Initializing GLFW" << std::endl;
		return -1;
	}
	window = glfwCreateWindow(500, 500, "Barnes-Hut Tree", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		root->buildTree(allParticles, NUMBER_PARTICLES);

		root->computeMassDistribution();

		displayParticles(allParticles);

		displayQuadrant(*root);


		for (std::vector<ParticleData*>::iterator it = allParticles.begin(); it != allParticles.end(); it++)
		{

			target = root->calcForce(*(*it));
			(*it)->calcDistance(target, TIME);
		}

		Sleep(33);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
}

int running_display(std::vector<ParticleData*> allParticles, QuadNode* root, double TIME, int NUMBER_PARTICLES)
{
	Vector2D target;

	GLFWwindow* window;
	if (!glfwInit())
	{
		std::cout << "Error Initializing GLFW" << std::endl;
		return -1;
	}
	window = glfwCreateWindow(500, 500, "Barnes-Hut Tree", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		root->buildTree(allParticles, NUMBER_PARTICLES);
		root->computeMassDistribution();

		displayParticles(allParticles);

		for (std::vector<ParticleData*>::iterator it = allParticles.begin(); it != allParticles.end(); it++)
		{
			target = root->calcForce(*(*it));
			(*it)->calcDistance(target, TIME);
		}

		Sleep(33);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
}




int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------
	const int NUMBER_PARTICLES = 5;
	const double TIME = 0.1;

	//setup
	ParticleData particle;
	std::vector<ParticleData*> allParticles;
	allParticles.reserve(NUMBER_PARTICLES);
	allParticles = particle.generateParticles(NUMBER_PARTICLES, 1);

	Vector2D* max = new Vector2D(1, -1, 0, 0);
	Vector2D* min = new Vector2D(-1, 1, 0, 0);

	QuadNode* root = new QuadNode(*min, *max, QuadNode::NONE, nullptr);




	//-----------------------
	bool repeat = true;
	int a = 0;

	while (repeat)
	{
		/*
		
		for (int i = 0; i < NUMBER_PARTICLES; i++)
		{
			std::cout << "particle#: " << i << "\n";
			root->insert(*allParticles[i]);
		}
		*/
		//running_display(allParticles, root, TIME);

		//std::cout << "center of mass calculations\n" << std::endl;


		display(allParticles, root, TIME, NUMBER_PARTICLES);

	

		//reset all quadnodes






		std::cout << "Enter '1' to repeat..." << std::endl;

		std::cin >> a;
		if (a != 1)
		{
			repeat = false;
		}

	}
	//----------------------
	
	std::cout << "Enter a key to continue..." << std::endl;

	std::cin >> a; 

	
	return 0;
}