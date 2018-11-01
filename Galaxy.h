#pragma once
#include "Tree.h"
#include <GLFW/glfw3.h>
#include "ParticleData.h"
#include <iostream>
#include "Tree.h"
#include <Windows.h>

class Galaxy
{
public:
	Galaxy();
	Galaxy(double x, double y, double _centerMass, int _NUM_P, double vel_x, double vel_y);
	~Galaxy();
	void displayParticles(std::vector<ParticleData*> arr);
	void displayParticles(std::vector<ParticleData*> arr1, std::vector<ParticleData*> arr2);

	void recursiveBoxes(QuadNode& qt, double factor);
	void displayQuadrant(QuadNode& quad);
	void displayQuadrant(QuadNode& quad, QuadNode& second);

	int running_display();
	int two_running_display(Galaxy& second);

	void add_galaxy(Galaxy& galaxy);
	double clockToMilliseconds(clock_t ticks);
	static std::vector<ParticleData*> renegades;

private:
	ParticleData particle; 
	std::vector<ParticleData*> allParticles;
	QuadNode * root;
	double x;
	double y;
	int NUMBER_PARTICLES;
	Vector2D* max;
	Vector2D* min;

};