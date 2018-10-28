#include "ParticleData.h"
#include <cassert>
#include <thread>
#include <ctime>
#include <sstream>


#define PI 3.1415926535897;
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
	give random coordinates and velocities to particles. I would use a mutex to 
	push onto the arr vector if this part was parallelized
*/
void ParticleData::createParticle(double a, double b, double radius, 
	int start, int end, std::vector<ParticleData*> &arr, double _centerMass)
{
	arr.push_back(new ParticleData(a,b, _centerMass ,0,0));

	for (int i = 1; i < end; i++)
	{
		double r = 1 * radius * sqrt(rand() / (double)RAND_MAX);
		double angle = (rand() / (double)RAND_MAX) * 2 * PI;

		double x = a + r * cos(angle);
		double y = b + r * sin(angle);


		double DIST = sqrt((a - x)*(a - x) + (b - y)*(b - y));
		double vel = sqrt(G_CONST*_centerMass / DIST);

		
		double vx, vy;
		vx = (a - x) / DIST * vel;
		vy = -(b - y) / DIST * vel;
		
		/**/
		double m = rand()%50 +0.1;
		arr.push_back(new ParticleData(x, y, m, vx, vy));
		
	}

}
std::vector<ParticleData*> ParticleData::generateParticles(double a,
	double b, int n, double R, double _centerMass)
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); 
	int max = std::move(n);
	
	
	createParticle(a, b, R, 0, max, arr, std::move(_centerMass));
	return arr;
}

void ParticleData::calcDistance(Vector2D force)
{
		//timestep per calculation
		double TIME = 0.15; 
		{
			if (force.x != force.x)
				force.x = 0;

			if (force.y != force.y)
				force.y = 0;
		}

		double acc_x = force.x / mState;
		double acc_y = force.y / mState;

		
		//velocity to be added

		this->xy->vx += acc_x * TIME;
		this->xy->vy += acc_y * TIME;

		//change velocities
		this->xy->x += this->xy->vx*TIME;
		this->xy->y += this->xy->vy*TIME;

		/*here*/
		if (xy->x >= 0.99)
		{
			xy->x = -0.95;
			xy->vx *= 0.5;

		}
		if (xy->x <= -0.99)
		{
			xy->x = 0.95;
			xy->vx *= 0.5;

		}

		if (xy->y >= 0.99)
		{
			xy->y = -0.95;
			xy->vy *= 0.5;

		}
		if (xy->y <= -0.99)
		{
			xy->y = 0.95;
			xy->vy *= 0.5;

		}
	
	

}
