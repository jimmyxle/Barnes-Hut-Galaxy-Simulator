#include "Tree.h"
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include "Galaxy.h"
#include <assert.h>
#include "tbb/parallel_for.h"
#include "tbb/task_group.h"

/*
static/global variables go here 
*/

const double _THETA = 0.9; 
//close to 1.0, determines # calcs.
//Smaller number, more calcs

const double G_CONST = 6.674 * pow(10.0, -11.0);
//prevents dividing by zero
const double SOFT_CONST = 0.01;

std::vector<ParticleData*> Galaxy::renegades;


QuadNode::QuadNode()
{
	std::cout << "Default constructor used" << std::endl;
	particle = nullptr;
	center = botRight  = topLeft = Vector2D();
	parent = nullptr;
	nodeArr.reserve(4);
	divided = false;
	numParticles = 0;
	numSubdivisions = 0;
	QUADRANT = NONE;

}

QuadNode::QuadNode(const Vector2D &min, const Vector2D &max, 
	Quadrant quad, QuadNode* _parent, int level_given)
{
	particle = nullptr;

	topLeft = min;
	center = Vector2D( (min.x + (max.x-min.x)/2.0) , (min.y + (max.y-min.y)/2.0),0,0 );
	botRight = max;

	COM = Vector2D();
	totalMass = 0;

	parent = _parent;
	nodeArr.reserve(4);

	divided = false;
	numParticles = 0;
	numSubdivisions = 0;
	QUADRANT = quad;

	level = level_given;


}
QuadNode::~QuadNode()
{
	if (nodeArr.size() != 0)
	{
		for (int i = 0; i < 4; i++)
		{
			delete  nodeArr[i];
		}
	}
	nodeArr.clear();
	particle = nullptr;
	parent = nullptr;
}


bool QuadNode::contains(ParticleData &_particle)
{
	double p_x = (&_particle)->xy->x;
	double p_y = (&_particle)->xy->y;

	if (p_x >= this->topLeft.x &&
		p_x < this->botRight.x && 
		p_y >= this->botRight.y &&
		p_y < this->topLeft.y)
	{
		return true;
	}
	else
	{
		return false;

	}
}


int QuadNode::insert(ParticleData &newParticle)
{
	//if exact same particle, return
	if (&newParticle == particle)
		return level;

	//checks if particle is within the quadrant
	if(!this->contains( newParticle ) )
	{
		std::stringstream ss;
		ss << "this particle"
				<< newParticle.xy->x << "," << newParticle.xy->y
				<< "is outside node ("
				<< "min.x=" << topLeft.x << ", "
				<< "max.x=" << botRight.x << ", "
				<< "min.y=" << topLeft.y << ", "
				<< "max.y=" << botRight.y << ")";
		throw std::runtime_error(ss.str());
	}
	int max_depth = this->level;
	
	if (this->numParticles > 1)
	{
		//quadrant has a particle, so subdivide if not yet
		if (nodeArr.size() == 0 && divided == false)
		{
			this->subdivide();
		}

		tbb::task_group g;

		//check each quadrant and insert if it contains the right coordinates
		if (this->nodeArr[0]->contains( newParticle)) 
		{

			g.run([&]{ max_depth = this->nodeArr[0]->insert(newParticle); });
		}
		else if (this->nodeArr[1]->contains(newParticle)) 
		{
			g.run([&] { max_depth = this->nodeArr[1]->insert(newParticle); });

		}
		else if (this->nodeArr[2]->contains(newParticle)) 
		{
			g.run([&] { max_depth = this->nodeArr[2]->insert(newParticle); });


		}
		else if (this->nodeArr[3]->contains(newParticle)) 
		{
			g.run([&] { max_depth = this->nodeArr[3]->insert(newParticle); });

		}
		else
		{
			//particle can't be found for some reason so add to 
			//vector of renegade particles so we still calculate their 
			//force
			Galaxy::renegades.push_back(&newParticle);
		}
		g.wait();
	}
	else if (this->numParticles == 1)
	{
		if (this->nodeArr.size() == 0 && this->divided == false)
		{
			this->subdivide();
		}
		//particle is in the exact same position as another particle
		//move it slightly
		tbb::task_group g;
		if (particle != nullptr)
		{
			if ((particle)->xy->x == newParticle.xy->x &&
				this->particle->xy->y == newParticle.xy->y)
			{
				if (&newParticle.xy->x <= 0)
					newParticle.xy->x += 0.01;
				else
					newParticle.xy->x -= 0.01;

				if (&newParticle.xy->y <= 0)
					newParticle.xy->y += 0.01;
				else
					newParticle.xy->y -= 0.01;
			}
		}

		// move old particle because the quadrant was subdivided
		if (this->nodeArr[0]->contains( *particle ))
		{
			g.run([&] { 
				max_depth = this->nodeArr[0]->insert(*particle);
				this->particle = nullptr;
			});
		}
		else if (this->nodeArr[1]->contains( *(this->particle) ))
		{
			g.run([&] {
				max_depth = this->nodeArr[1]->insert(*particle);
				this->particle = nullptr;
			});
		}
		else if (this->nodeArr[2]->contains( *(this->particle)))
		{

			g.run([&] {
				max_depth = this->nodeArr[2]->insert(*particle);
				this->particle = nullptr;
			});
		}
		else if (this->nodeArr[3]->contains( *(this->particle)))
		{

			g.run([&] {
				max_depth = this->nodeArr[3]->insert(*particle);
				this->particle = nullptr;
			});
		}
		else
		{	
			Galaxy::renegades.push_back(&newParticle);
		}

		//add new particle
		if (this->nodeArr[0]->contains(newParticle))
		{
			g.run([&] {
				max_depth = this->nodeArr[0]->insert(newParticle);
			});
		}
		else if (this->nodeArr[1]->contains(newParticle))
		{
			g.run([&] {
				max_depth = this->nodeArr[1]->insert(newParticle);
			});
		}
		else if (this->nodeArr[2]->contains(newParticle))
		{
			g.run([&] {
				max_depth = this->nodeArr[2]->insert(newParticle);
			});
		}
		else if (this->nodeArr[3]->contains(newParticle))
		{
			g.run([&] {
				max_depth = this->nodeArr[3]->insert(newParticle);
			});
		}
		else
		{
			Galaxy::renegades.push_back(&newParticle);
		}
		g.wait();
	}
	else if (this->numParticles == 0)
	{	
		//add the particle
		this->particle = &newParticle;
	}
	numParticles++;
	return max_depth;
}
//helper function
Vector2D QuadNode::getVector(int n)
{
	switch (n)
	{
	case 0:
		return topLeft;
		break;
	case 1:
		return center;
		break;
	case 2:
		return botRight;
		break;
	default:
		std::cout << "Error in getvector()" << std::endl;
		std::string str = "Error in getVector()";
		throw std::invalid_argument(str);
		break;
	}
}


void QuadNode::subdivide()
{
	Vector2D new_min = Vector2D(center.x, topLeft.y );
	Vector2D new_max = Vector2D(botRight.x, center.y);
	nodeArr.push_back(  new QuadNode(new_min, new_max, NE , this, level+1) ) ;

	//NW
	new_min = topLeft;
	new_max = center;
	nodeArr.push_back(new QuadNode(new_min, new_max,NW,  this, level+1));

	//SE

	new_min = center;
	new_max = botRight;
	nodeArr.push_back(new QuadNode(new_min, new_max, SE,this, level+1));

	//SW
	new_min = Vector2D(topLeft.x, center.y);
	new_max = Vector2D(center.x , botRight.y);
	nodeArr.push_back(new QuadNode(new_min, new_max, SW, this, level +1));

	this->divided = true;
	this->numSubdivisions++;
	
}

void QuadNode::computeMassDistribution()
{
	if (numParticles == 1)
	{
		//make sure particle exists
		assert(particle); 
		COM.x = particle->xy->x;
		COM.y = particle->xy->y;
		totalMass = particle->mState;
	}
	else
	{
		totalMass = 0;
		COM.reset();
		if (nodeArr.size() > 0)
		{
			//task parallel
			tbb::parallel_for(size_t(0), nodeArr.size(), [&](size_t i){
					nodeArr[i]->computeMassDistribution();
					this->totalMass += (nodeArr[i])->totalMass;
					this->COM.x += (nodeArr[i])->totalMass*(nodeArr[i])->COM.x;
					this->COM.y += (nodeArr[i])->totalMass*(nodeArr[i])->COM.y;
				});

			COM.x /= totalMass;
			COM.y /= totalMass;
		}
	}
}

void QuadNode::computeMassDistribution_iterative()
{

}


void QuadNode::calcForce(ParticleData& _particle, Vector2D &forces)
{
	Vector2D force1 = this->calcForceTree(_particle);
	if (int s = Galaxy::renegades.size() )
	{
		for (int i = 0; i<s; ++i)
		{
			Vector2D force4 = calcAcceleration(_particle, *Galaxy::renegades[i]);
			double FACTOR = 1.0;
			force1.x += force4.x * FACTOR;
			force1.y += force4.y * FACTOR;
		}
	}
	
	forces.x += force1.x/_particle.mState;
	forces.y += force1.y/_particle.mState;
}

Vector2D QuadNode::calcForceTree(ParticleData& _particle)
{
	Vector2D force2;
	if (numParticles == 1)
	{
		force2 =  calcAcceleration(_particle, *particle );
	}
	else
	{
		double x1 = COM.x;
		double y1 = COM.y;
		
		double x2 = _particle.xy->x;
		double y2 = _particle.xy->y;

		double mass1 = this->totalMass;
		double mass2 = _particle.mState;

		double r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + SOFT_CONST);

		double d = this->botRight.x - this->topLeft.x;
		
		double theta = d / r;
		if (theta <= _THETA)
		{
			double k = G_CONST *  (mass1*mass2) / (r*r);

			force2.x += k*(x1-x2);
			force2.y += k*(y1-y2);
		}
		else
		{
			Vector2D partial;

			for (std::vector<QuadNode*>::iterator it = nodeArr.begin();
				it != nodeArr.end(); it++)
			{
				if (*it)
				{
					partial = (*it)->calcForceTree(_particle);
					force2.x += partial.x;
					force2.y += partial.y;
				}
			}

		}
	}
	return force2;
}


Vector2D QuadNode::calcAcceleration(ParticleData& _particle1, ParticleData& _particle2)
{
	Vector2D force3;

	if (&_particle1 == &_particle2)
		return force3;

	const double &x1 = _particle1.xy->x;
	const double &y1 = _particle1.xy->y;

	const double &x2 = _particle2.xy->x;
	const double &y2 = _particle2.xy->y;

	const double &mass1 = _particle1.mState;
	const double &mass2 = _particle2.mState; 

	double r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + SOFT_CONST);

	 if (r > 0)
	{
		double k = G_CONST * (mass2*mass1) / (r*r);

		force3.x += k * (x2 - x1) ;
		force3.y += k * (y2 - y1);
	}
	else
	{
		//add no force if two particles are too close together
		force3.x = force3.y = 0;
	}
	return force3;
}


int QuadNode::buildTree(std::vector<ParticleData*> &arr, int NUMBER_PARTICLES)
{
	if (parent != nullptr)
	{
		std::stringstream ss;
		ss << "only the root can call this.";
		throw std::runtime_error(ss.str());
	}
	this->reset(topLeft, botRight);

	if (arr.size() == 0)
	{
		std::stringstream ss;
		ss << "empty list";
		throw std::runtime_error(ss.str());
	}
	int new_depth = 0;
	int current_depth = 0;

	for (unsigned int i = 0; i < arr.size(); i++)
	{
		new_depth = insert(*arr[i]);
		if (new_depth > current_depth)
		{
			current_depth = new_depth;
		}
	}
	return current_depth;
}

void QuadNode::reset(const Vector2D &min, const Vector2D &max )
{
	if (parent != nullptr)
	{
		std::stringstream ss;
		ss << "only the root can call this.";
		throw std::runtime_error(ss.str());
	}

	if (nodeArr.size() != 0 )
	{
		for (int i = 0; i < 4; i++)
		{
			delete nodeArr[i];
			nodeArr[i] = nullptr;
		}
		nodeArr.clear();
	}
	topLeft = min;
	botRight = max;
	center = Vector2D((min.x + (max.x - min.x) / 2.0), (min.y + (max.y - min.y) / 2.0), 0, 0);
	numParticles = 0;
	totalMass = 0;
	COM.x = 0;
	COM.y = 0;
	divided = false;
}
