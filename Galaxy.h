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
	Galaxy(double x, double y);
	~Galaxy();
	void displayParticles(std::vector<ParticleData*> arr);
	void recursiveBoxes(QuadNode& qt, double factor);
	void displayQuadrant(QuadNode& quad);

	int display();
	int running_display();
	static std::vector<ParticleData*> renegades;
	ParticleData particle; //temp

private:
	std::vector<ParticleData*> allParticles;
	QuadNode * root;
	double x;
	double y;
	int NUMBER_PARTICLES;
	double TIME;
	Vector2D* max;
	Vector2D* min;
};