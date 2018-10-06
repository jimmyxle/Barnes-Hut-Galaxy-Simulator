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
	srand(time(NULL)); 

	int max = n;
	double factor = 0.0; //coefficient for initial velocities

	for (int i = 0; i < max; i++)
	{
		double r = R*sqrt( rand() / (double)RAND_MAX );
		double angle = (rand() / (double)RAND_MAX) * 2 * PI;
		/*
		double x =	factor*((rand() % 20 - 10) / 10.0);
		double y =	factor*((rand() % 20 - 10) / 10.0);
		*/
		double x = a + r*cos(angle);
		double y = b + r*sin(angle);

		//double vx =  r*cos(angle);
		double vx = factor + r * cos(angle);

		//double vy =   r*sin(angle);
		double vy = factor + r * sin(angle);
		std::cout <<"("<<vx<<","<< vy << ")\n";

		//while (vy == 0.0)
		//	vy = ((rand() % 4 - 2) / 10.0);

		double m = rand() % 10 + 0.1;
		//double m = .0;
		arr.push_back(new ParticleData(x, y, m, vx, vy));
	}


	return arr;
}

void ParticleData::calcDistance(Vector2D force, double TIME)
{
	//TIME *= 2;
	TIME = 200;
	//so i got the force. ill divide this by the mass and get the accel
	double acc_x = force.x / mState;
	double acc_y = force.y / mState;
	//std::cout<<"acc_x:" << acc_x << "\n";
	//velocity to be added
	double vel_x = acc_x * TIME ;
	double vel_y = acc_y * TIME ;

	//distance to be added
	double dist_x = vel_x * TIME;
	double dist_y = vel_y * TIME;
	
	this->xy->vx += vel_x;
	this->xy->vy += vel_y;

	//std::cout << "BEFORE: (" << xy->vx << "," << xy->vy << ")\n";

	//change velocities
	this->xy->x += dist_x;
	this->xy->y += dist_y ;
	//std::cout << "AFTER(" << xy->vx << "," << xy->vy << ")\n";
	

	//change x/y position
	if (xy->x >=0.99)
	{
		xy->x = -0.95;
	//	xy->vx *= -0.5;
	}
	if (xy->x <= -0.99)
	{
		xy->x = 0.95;
		//xy->vx *= -0.5;
	}
		
	if (xy->y >= 0.99)
	{
		xy->y = -0.95;
		//xy->vy *= -0.5;
	}
	if (xy->y <= -0.99)
	{
		xy->y = 0.95;
		//xy->vy *= -0.5;

	}


}

