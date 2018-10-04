#include "ParticleData.h"
#include <cassert>

#define PI 3.1415926535897;


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

std::vector<ParticleData*> ParticleData::generateParticles(double a, double b, int n, double R )
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); //seed? 
	int max = n;

	/*
	r = R * sqrt(random())
	theta = random() * 2 * PI

	x = r * cos(theta)
	y = r * sin(theta)
	*/


	for (int i = 0; i < max; i++)
	{
		
		double r = R*sqrt( rand() / (double)RAND_MAX );
		double angle = (rand() / (double)RAND_MAX) * 2 * PI;
		/*
		double x =	factor*((rand() % 20 - 10) / 10.0);
		double y =	factor*((rand() % 20 - 10) / 10.0);
		*/
		double x = a + r*cos(angle) ;
		double y = b + r*sin(angle)  ;

		//double vx =  r*cos(angle);
		double vx = 0;

		//double vy =   r*sin(angle);
		double vy = 0;
		//while (vy == 0.0)
		//	vy = ((rand() % 4 - 2) / 10.0);

		double m = rand() % 10 + 0.1;
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
	double temp_vx = xy->vx;
	double temp_vy = xy->vy;
	/*
	double vel_x = acc_x * time + temp_vx;
	double vel_y = acc_y * time + temp_vy;
	*/

	double vel_x = acc_x * time + temp_vx;
	double vel_y = acc_y * time + temp_vy;

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

ParticleData* ParticleData::renegadeHandler()
{
	xy->x += 0.5;
	xy->y += 0.5;
	return this;

}