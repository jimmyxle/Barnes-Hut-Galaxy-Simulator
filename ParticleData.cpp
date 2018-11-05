#include "ParticleData.h"
#include <cassert>
#include <thread>
#include <ctime>
#include <sstream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>


const double PI = 3.1415926535897;
const double G_CONST = 6.674 * pow(10.0, -11.0);

ParticleData::ParticleData()
{
	std::cout << "default constructor for particle data used" << std::endl;
	xy = nullptr;
	mState = NULL;
}

ParticleData::ParticleData(double _x, double _y, double mstate)
{
	xy = new Vector2D(_x, _y);
	mState = mstate;
}

ParticleData::ParticleData(double _x, double _y, double mstate, double _vx, double _vy)
{
	xy = new Vector2D(_x, _y, _vx, _vy);
	mState = mstate;

}

void ParticleData::reset()
{
	xy->x = 0;
	xy->y = 0;
	xy->vx = 0;
	xy->vy = 0;
	mState = 0;
}

bool ParticleData::isNull() const
{
	std::cout << "null\n\n";
	return ( (*xy).x == 0 && (*xy).y == 0) && mState && (xy->vx == 0 && xy->vy ==0); //what? Didn't know this worked this way
}

void ParticleData::printParticle()
{
	if (this == nullptr)
	{
		std::cerr << "nullptr" << std::endl;
		return;
	}
	xy->print();
	std::cout <<" mass: "<< mState ;
}

/*
	Random angular positions and orbital velocities
*/
void ParticleData::createParticle(double a, double b, double radius, 
	int start, int end, std::vector<ParticleData*> &arr, double _centerMass, 
	double vel_x, double vel_y)
{
	//add the sun
	arr.push_back(new ParticleData(a,b, _centerMass ,vel_x,vel_y));

	for (int i = 1; i < end; i++)
	{
		double r = 1 * radius * sqrt(rand() / (double)RAND_MAX);
		double angle = (rand() / (double)RAND_MAX) * 2 * PI;

		double x = a + r * cos(angle);
		double y = b + r * sin(angle);

		//distance between center and particle
		double DIST = sqrt((a - x)*(a - x) + (b - y)*(b - y));
		double orbital_vel = sqrt(G_CONST*_centerMass / DIST);

		//adjust the orbital velocities
		double FACTOR = 0.1;
		double vx, vy;
		vx =  orbital_vel * cos(angle + (PI / 2) ) * FACTOR;
		vy =  orbital_vel * sin(angle + (PI / 2) ) * FACTOR;

		//random masses and push into vector
		double m = rand()%20 +0.1;
		arr.push_back(new ParticleData(x, y, m, vx, vy));
	}
}
std::vector<ParticleData*> ParticleData::generateParticles(double a,
	double b, int n, double R, double _centerMass, double vel_x, double vel_y)
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); 
	int max = std::move(n);
	createParticle(a, b, R, 0, max, arr, std::move(_centerMass), vel_x, vel_y);
	return arr;
}

void ParticleData::calcDistance(Vector2D force)
{
		//timestep per calculation
		double TIME = 0.3; 
		{
			//prevents NaN problems
			if (force.x != force.x)
				force.x = 0;

			if (force.y != force.y)
				force.y = 0;
		}

		double acc_x = force.x / mState;
		double acc_y = force.y / mState;
		//prevents points from accelerating too far from the center
		double max = 1.0 / 25;
		if (acc_x >= max)
		{
			acc_x = max;
		}
		if (acc_x < -max)
		{
			acc_x = -max;
		}

		if (acc_y >= max)
		{
			acc_y = max;
		}
		if (acc_y < -max)
		{
			acc_y = -max;
		}

		//velocities
		this->xy->vx += acc_x * TIME;
		this->xy->vy += acc_y * TIME;

		//positions
		this->xy->x += this->xy->vx;
		this->xy->y += this->xy->vy;

		//bounce particles off the borders
		if (xy->x >= 0.99)
		{
			xy->x = 0.99;
			xy->vx *= -0.5;
		}
		if (xy->x <= -0.99)
		{
			xy->x = -0.99;
			xy->vx *= -0.5;
		}
		if (xy->y >= 0.99)
		{
			xy->y = 0.99;
			xy->vy *= -0.5;
		}
		if (xy->y <= -0.99)
		{
			xy->y = -0.99;
			xy->vy *= -0.5;
		}
}

