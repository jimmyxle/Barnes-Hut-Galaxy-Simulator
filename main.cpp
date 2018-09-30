#include <GLFW/glfw3.h>
#include "ParticleData.h"
#include <iostream>
#include "Tree.h"

/*
Note: displayQuadrant only shows one box so far, ill have to go through my tree 
recusively to find the other quadrants
*/

void displayParticles(std::vector<ParticleData*> arr ) {
	glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glPointSize(5);
	glBegin(GL_POINTS);

	for (auto &particle : arr)
	{
		double x = (*particle).xy.x;
		double y = (*particle).xy.y;
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();
}

void recursiveBoxes( QuadNode& qt)
{
	if (qt.nodeArr.size() == 0)
	{
		glVertex2d(qt.getVector(0).x, qt.getVector(0).y);
		glVertex2d(qt.getVector(1).x, qt.getVector(1).y);

		glVertex2d(qt.getVector(1).x, qt.getVector(1).y);
		glVertex2d(qt.getVector(3).x, qt.getVector(3).y);

		glVertex2d(qt.getVector(3).x, qt.getVector(3).y);
		glVertex2d(qt.getVector(2).x, qt.getVector(2).y);

		glVertex2d(qt.getVector(2).x, qt.getVector(2).y);
		glVertex2d(qt.getVector(0).x, qt.getVector(0).y);
	}
	else
	{
		recursiveBoxes( *(qt).nodeArr[0] );
		recursiveBoxes( *(qt).nodeArr[1] );
		recursiveBoxes( *(qt).nodeArr[2] );
		recursiveBoxes( *(qt).nodeArr[3] );
	}
}


void displayQuadrant( QuadNode& quad)  
{
	//glClearColor(0, 0, 0, 0);
	//clear color and depth buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(255, 0, 0);
	glPointSize(2);
	glBegin(GL_LINES);

	recursiveBoxes(quad);

	glEnd();
	glPopMatrix();
}





int main()
{
	std::cout << "Start" << std::endl;
	//-----------------------





	//-----------------------
	
	const int NUMBER_PARTICLES = 5;

	ParticleData particle;
	std::vector<ParticleData*> allParticles = particle.generateParticles(NUMBER_PARTICLES);

	
	Vector2D* max = new Vector2D(1, -1);
	Vector2D* min = new Vector2D(-1, 1);

	QuadNode* root = new QuadNode(*(min), *(max), nullptr);


	for (int i = 0; i < NUMBER_PARTICLES; i++)
	{
		root->insert( *allParticles[i] );
	}



	
	//qt->insert()
	

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
		//function
		displayParticles(allParticles);
		displayQuadrant(*root);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();	







	//----------------------
	
	std::cout << "Enter a key to continue..." << std::endl;

	int a = 0;
	std::cin >> a; 

	
	return 0;
}