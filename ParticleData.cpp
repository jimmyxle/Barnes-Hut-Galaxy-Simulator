#include "ParticleData.h"
#include <cassert>
#include <thread>
#include <mutex>
#include <ctime>

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
	int start, int end, std::vector<ParticleData*> &arr)
{
	for (int i = start; i < end; i++)
	{
		double r = radius * sqrt(rand() / (double)RAND_MAX);
		double angle = (rand() / (double)RAND_MAX) * 2 * PI;

		double x = a + r * cos(angle);
		double y = b + r * sin(angle);

		double vx = r * cos(angle);

		double vy = r * sin(angle);


		double m = rand() % 20 + 0.1;
		arr.push_back(new ParticleData(x, y, m, vx, vy));
		
	}

}
std::vector<ParticleData*> ParticleData::generateParticles(double a, double b, int n, double R )
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); 
	std::clock_t begin = clock();

	int max = std::move(n);

	//better when not parallel

	/*
	//control thread

	const int nThreads = 1;
	std::thread t_arr0[nThreads];
	for (int i = 0; i < nThreads; i++)
	{
		int start = i * max / nThreads;
		int end = (i + 1)*max / nThreads;
		if (i == nThreads - 1)
			end = max;

		t_arr0[i] = std::thread( &ParticleData::createParticle, this ,
			std::ref(a), std::ref(b), std::ref(R), std::ref(start), std::ref(end), std::ref(arr));
	}
	*/

	
	
	createParticle(a, b, R, 0, max, arr);
	std::clock_t end = clock();
	double time = (end - begin);
	std::cout <<"particle generate time: " << time << std::endl;;
	return arr;
}

void ParticleData::calcDistance(Vector2D force)
{
	double TIME = 200; //set this arbitrarily 
	double acc_x = force.x / mState;
	double acc_y = force.y / mState;
	//velocity to be added
	double vel_x = acc_x * TIME ;
	double vel_y = acc_y * TIME ;

	//distance to be added
	double dist_x = vel_x * TIME;
	double dist_y = vel_y * TIME;
	
	this->xy->vx += vel_x;
	this->xy->vy += vel_y;


	//change velocities
	this->xy->x += dist_x;
	this->xy->y += dist_y ;	

	//change x/y position
	if (xy->x >=0.99)
	{
		xy->x = -0.95;
	}
	if (xy->x <= -0.99)
	{
		xy->x = 0.95;
	}
		
	if (xy->y >= 0.99)
	{
		xy->y = -0.95;
	}
	if (xy->y <= -0.99)
	{
		xy->y = 0.95;

	}

}
