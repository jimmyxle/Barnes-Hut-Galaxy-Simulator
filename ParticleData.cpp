#include "ParticleData.h"
#include <cassert>
#include <thread>
#include <ctime>
#include <sstream>




const float PI = 3.1415926f;
const float G_CONST = 6.674e-11f;

ParticleData::ParticleData()
{
	std::cout << "default constructor for particle data used" << std::endl;
	xy = nullptr;
	mState = NULL;
}

ParticleData::ParticleData(float _x, float _y, float mstate)
{
	xy = new Vector2D(_x, _y);
	mState = mstate;
}

ParticleData::ParticleData(float _x, float _y, float mstate, float _vx, float _vy)
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
void ParticleData::createParticle(float a, float b, float radius, 
	int start, int end, std::vector<ParticleData*> &arr, float _centerMass, 
	float vel_x, float vel_y)
{
	//add the sun
	arr.push_back(new ParticleData(a,b, _centerMass ,vel_x,vel_y));

	for (int i = 1; i < end; i++)
	{
		float r = 1 * radius * sqrt(rand() / (float)RAND_MAX);
		float angle = (rand() / (float)RAND_MAX) * 2 * PI;

		float x = a + r * cos(angle);
		float y = b + r * sin(angle);

		//distance between center and particle
		float DIST = sqrt((a - x)*(a - x) + (b - y)*(b - y));
		float orbital_vel = sqrt(G_CONST*_centerMass / DIST);

		//adjust the orbital velocities
		float FACTOR = 0.05f;
		float vx, vy;
		vx =  orbital_vel * cos(angle + (PI / 2) ) * FACTOR;
		vy =  orbital_vel * sin(angle + (PI / 2) ) * FACTOR;

		//random masses and push into vector
		float m = rand()%20 +0.1f;
		arr.push_back(new ParticleData(x, y, m, vx, vy));
	}
}
std::vector<ParticleData*> ParticleData::generateParticles(float a,
	float b, int n, float R, float _centerMass, float vel_x, float vel_y)
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
		float TIME = 0.3f; 
		{
			//prevents NaN problems
			if (force.x != force.x)
				force.x = 0;

			if (force.y != force.y)
				force.y = 0;
		}

		float acc_x = force.x / mState;
		float acc_y = force.y / mState;
		//prevents points from accelerating too far from the center
		float max = 1.0f / 25;
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
		if (xy->x >= 0.99f)
		{
			xy->x = 0.99f;
			xy->vx *= -0.5f;
		}
		if (xy->x <= -0.99f)
		{
			xy->x = -0.99f;
			xy->vx *= -0.5f;
		}
		if (xy->y >= 0.99f)
		{
			xy->y = 0.99f;
			xy->vy *= -0.5f;
		}
		if (xy->y <= -0.99f)
		{
			xy->y = -0.99f;
			xy->vy *= -0.5f;
		}
}

