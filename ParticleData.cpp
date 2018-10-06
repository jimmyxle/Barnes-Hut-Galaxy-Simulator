#include "ParticleData.h"
#include <cassert>
#include <thread>
#include <mutex>

#define PI 3.1415926535897;
std::mutex _mu;


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


		double m = rand() % 10 + 0.1;
		std::lock_guard<std::mutex> lock(_mu);
		arr.push_back(new ParticleData(x, y, m, vx, vy));
		
	}

}
std::vector<ParticleData*> ParticleData::generateParticles(double a, double b, int n, double R )
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); 

	int max = std::move(n);

	//control thread
	const int nThreads = 4;
	std::thread t_arr[nThreads];

	for (int i = 0; i < nThreads; i++)
	{
		int start = i * max / nThreads;
		int end = (i + 1)*max / nThreads;
		if (i == nThreads - 1)
			end = max;
		t_arr[i] = std::thread( &ParticleData::createParticle, this , std::ref(a), std::ref(b), std::ref(R), std::ref(start), std::ref(end), std::ref(arr));
	}
	for (int i = 0; i < nThreads; i++)
	{
		t_arr[i].join();
	}

	if (arr.size() != 0)
	{
		for (int i = 0; i < max; i++)
		{
			std::cout << i<<"\t" ;
			arr[i]->printParticle();
			std::cout << "\n";
		}
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

