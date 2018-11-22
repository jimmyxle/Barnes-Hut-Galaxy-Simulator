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
	ParticleData(float _x, float _y, float mstate);
	ParticleData(float _x, float _y, float mstate, float _vx, float _vy);

	~ParticleData() = default;
	void reset();
	bool isNull() const;
	void printParticle();
	std::vector<ParticleData*> generateParticles(float a, 
		float b,int n, float R, float _centerMass, float vel_x, float vel_y);
	void calcDistance(Vector2D force);

	void createParticle(float a, float b, float radius, 
		int start, int end, std::vector<ParticleData*> &arr,
		float _centerMass, float vel_x, float vel_y);

	//__global__ void calcDistance_CUDA(Vector2D* forces, Vector2D* particles, int count);


	Vector2D* xy;
	float mState;
};

