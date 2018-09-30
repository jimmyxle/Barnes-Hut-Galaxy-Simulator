#include "ParticleData.h"
#include <cassert>



ParticleData::ParticleData()
{
	xy = Vector2D();
	mState = 1;
}

ParticleData::ParticleData(double _x, double _y, double mstate)
{
	xy = Vector2D(_x, _y);
	mState = mstate;
}

void ParticleData::reset()
{
	xy.x = 0;
	xy.y = 0;
	mState = NULL;
}

bool ParticleData::isNull() const
{
	return ( xy.x == 0 && xy.y == 0) && mState; //what? Didn't know this worked this way
}

void ParticleData::printParticle()
{
	if (this == nullptr)
	{
		std::cerr << "nullptr" << std::endl;
		return;
	}
	std::cout << "( " << xy.x << ", " << xy.y << "): "<< mState ;
}

std::vector<ParticleData*> ParticleData::generateParticles( int n)
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); //seed? 

	int max = n;
	for (int i = 0; i < max; i++)
	{
		double x = (rand() % 20 - 10) / 10.0;
		double y = (rand() % 20 - 10) / 10.0;
		double m = rand() % 10;
		arr.push_back(new ParticleData(x, y, m));
	}
	for (int i = 0; i < max; i++)
	{
		std::cout << i <<". ";
		(*arr[i]).printParticle();
		std::cout << std::endl;
	}

	return arr;
}