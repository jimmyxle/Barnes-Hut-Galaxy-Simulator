#pragma once
#include "Vector.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>




/*
This holds info about each particle's X&Y and their mass
*/



struct ParticleData
{
public:
	ParticleData();
	ParticleData(double _x, double _y, double mstate);
	ParticleData(double _x, double _y, double mstate, double _vx, double _vy);

	~ParticleData() = default;
	void reset();
	bool isNull() const;
	void printParticle();
	std::vector<ParticleData*> generateParticles(double a, 
		double b,int n, double R, double _centerMass, double vel_x, double vel_y);
	void calcDistance(Vector2D force);

	void createParticle(double a, double b, double radius, 
		int start, int end, std::vector<ParticleData*> &arr,
		double _centerMass, double vel_x, double vel_y);

	//__global__ void calcDistance_CUDA(Vector2D* forces, Vector2D* particles, int count);


	Vector2D* xy;
	double mState;
};

