#include "ParticleData.h"
#include <cassert>



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

std::vector<ParticleData*> ParticleData::generateParticles( int n, double factor )
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); //seed? 
	int max = n;

	for (int i = 0; i < max; i++)
	{

		double x =	factor*((rand() % 20 - 10) / 10.0);
		double y =	factor*((rand() % 20 - 10) / 10.0);
		double vx = factor*((rand() % 20 - 10) / 10.0);
		double vy = factor*((rand() % 20 - 10) / 10.0);

		double m = rand() % 10 + 0.5;
		arr.push_back(new ParticleData(x, y, m, vx, vy));
	}
	

	/*
	for (int i = 0; i < max; i++)
	{
		std::cout << i <<". ";
		(*arr[i]).printParticle();
		std::cout << std::endl;
	}
	*/

	return arr;
}

void ParticleData::calcDistance(Vector2D force, double time)
{
	//so i got the force. ill divide this by the mass and get the accel
	double acc_x = force.x / mState;
	double acc_y = force.y / mState;

	//s =  vit + 1/2at^2

	double vel_x = acc_x / time;
	double vel_y = acc_y / time;

	double temp_vx = xy->vx;
	double temp_vy = xy->vy;


	
	
	double dist_x = temp_vx*time + 0.5*(acc_x * time * time);
	double dist_y = temp_vy*time + 0.5*(acc_y * time * time);
	
	//change velocities
	this->xy->vx += vel_x;
	this->xy->vy += vel_y;

	//change x/y position

	this->xy->x += dist_x;
	if (xy->x > 1)
		xy->x = -1;
	if (xy->x < -1)
		xy->x = 1;
	this->xy->y += dist_y;
	if (xy->y > 1)
		xy->y = -1;
	if (xy->y < -1)
		xy->y = 1;



}