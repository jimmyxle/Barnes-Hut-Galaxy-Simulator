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
	//ParticleData(int _x, int _y, int _vx, int _vy, int mstate);
	ParticleData(double _x, double _y, double mstate);
	~ParticleData() = default;
	void reset();
	bool isNull() const;
	void printParticle();
	std::vector<ParticleData*> generateParticles(int n);


	Vector2D xy;
//	int vx; //velocities
//	int vy;

	double mState;
};