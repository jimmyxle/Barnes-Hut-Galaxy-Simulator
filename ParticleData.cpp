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

void ParticleData::reset()
{
	xy->x = 0;
	xy->y = 0;
	mState = NULL;
}

bool ParticleData::isNull() const
{
	return ( (*xy).x == 0 && (*xy).y == 0) && mState; //what? Didn't know this worked this way
}

void ParticleData::printParticle()
{
	if (this == nullptr)
	{
		std::cerr << "nullptr" << std::endl;
		return;
	}
	std::cout << "( " << (*xy).x << ", " << (*xy).y << "): "<< mState ;
}

std::vector<ParticleData*> ParticleData::generateParticles( int n, double factor )
{
	std::vector<ParticleData*> arr;
	arr.reserve(n);
	srand(time(NULL)); //seed? 
	int max = n;


	if(n == 2)
	{
		arr.push_back(new ParticleData(0.5, 0.5, 1.0));
		arr.push_back(new ParticleData(-0.5, 0.5, 1.0));
		
	}
	else
	{
		for (int i = 0; i < max; i++)
		{
			double x = factor* ((rand() % 20 - 10) / 10.0);
			double y = factor*((rand() % 20 - 10) / 10.0);
			double m = rand() % 10;
			arr.push_back(new ParticleData(x, y, m));
		}
	}


	for (int i = 0; i < max; i++)
	{
		std::cout << i <<". ";
		(*arr[i]).printParticle();
		std::cout << std::endl;
	}

	return arr;
}