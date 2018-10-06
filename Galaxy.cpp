#include "Galaxy.h"
Galaxy::Galaxy()
{
	NUMBER_PARTICLES = 500;
	TIME = 10; //to find initial velocities + distance

	allParticles.reserve(NUMBER_PARTICLES);
	allParticles = particle.generateParticles(0, 0, NUMBER_PARTICLES, 0.5);
	//x , y , num, radius

	max = new Vector2D(1, -1, 0, 0);
	min = new Vector2D(-1, 1, 0, 0);

	root = new QuadNode(*min, *max, QuadNode::NONE, nullptr);

}


/*       */
void Galaxy::displayParticles(std::vector<ParticleData*> arr) {
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


int Galaxy::display()
{
	std::cout << "normal display mode" << std::endl;

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
		std::cout << "New cycle" << std::endl;


		for (int i = 0; i < NUMBER_PARTICLES; i++)
		{
			std::cout << i << ". ";
			(*allParticles[i]).printParticle();
			std::cout << std::endl;
		}
		root->buildTree(allParticles, NUMBER_PARTICLES);



		root->computeMassDistribution();

		displayParticles(allParticles);
		displayQuadrant(*root);


		for (std::vector<ParticleData*>::iterator it = allParticles.begin(); it != allParticles.end(); it++)
		{

			target = root->calcForce(*(*it));
			(*it)->calcDistance(target, TIME);
		}

		Sleep(33);//should be 33

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
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
	window = glfwCreateWindow(750, 750, "Barnes-Hut Tree", NULL, NULL);
	if (!window)
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		//use processes to run on 4 children
		root->buildTree(allParticles, NUMBER_PARTICLES);

		//data parallelism
		root->computeMassDistribution();

		displayParticles(allParticles);

		std::vector<Vector2D> forces;
		forces.reserve(NUMBER_PARTICLES);

		//data parallel on this
		for (std::vector<ParticleData*>::iterator it = allParticles.begin(); it != allParticles.end(); it++)
		{
			target = root->calcForce(*(*it));
			forces.push_back(target);
		}
		//data parallel on this

		for (int i = 0; i < NUMBER_PARTICLES; i++)
		{
			allParticles[i]->calcDistance( std::move(forces[i]), TIME);
		}
		Sleep(1000);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
}
