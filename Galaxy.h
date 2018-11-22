#pragma once
#include "Tree.h"
#include <GLFW/glfw3.h>
#include "ParticleData.h"
#include <iostream>
#include "Tree.h"
#include <Windows.h>
#include <ctime>
#include <thread>
#include "OpenCLHelper.h"



class Galaxy
{
public:
	Galaxy();
	Galaxy(float x, float y, float _centerMass, int _NUM_P,
		float vel_x, float vel_y, float radius);
	~Galaxy();
	void displayParticles(std::vector<ParticleData*> arr, GLFWwindow* window);
	void displayParticles2(std::vector<ParticleData*> arr1,
		std::vector<ParticleData*> arr2, GLFWwindow* window);

	void recursiveBoxes(QuadNode& qt, float factor);
	void displayQuadrant(QuadNode& quad);
	void displayQuadrant(QuadNode& quad, QuadNode& second);

	int running_display();
	int two_running_display(Galaxy& second);

	void add_galaxy(Galaxy& galaxy, float vel_x, float vel_y);
	float clockToMilliseconds(clock_t ticks);
	static std::vector<ParticleData*> renegades;

private:
	ParticleData particle; 
	std::vector<ParticleData*> allParticles;
	QuadNode * root;
	float x;
	float y;
	int NUMBER_PARTICLES;
	Vector2D* max;
	Vector2D* min;

	int depth;
};