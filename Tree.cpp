#include "Tree.h"
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include "Galaxy.h"
#include <assert.h>
#include <stack>

/*
static/global variables go here 
*/

const float _THETA = 1.0f; 
//close to 1.0, determines # calcs.
//Smaller number, more calcs

const float G_CONST = 6.674e-11f;
//prevents dividing by zero
const float SOFT_CONST = 0.01F;

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
	COM_calculated = false;

}

QuadNode::QuadNode(const Vector2D &min, const Vector2D &max, 
	Quadrant quad, QuadNode* _parent, int level_given)
{
	particle = nullptr;

	topLeft = min;
	center = Vector2D( (min.x + (max.x-min.x)/2.0f) , (min.y + (max.y-min.y)/2.0f),0,0 );
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
	COM_calculated = false;


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
	float p_x = (&_particle)->xy->x;
	float p_y = (&_particle)->xy->y;

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



		//check each quadrant and insert if it contains the right coordinates
		if (this->nodeArr[0]->contains( newParticle)) 
		{

			max_depth = this->nodeArr[0]->insert(newParticle);
		}
		else if (this->nodeArr[1]->contains(newParticle)) 
		{
		max_depth = this->nodeArr[1]->insert(newParticle);

		}
		else if (this->nodeArr[2]->contains(newParticle)) 
		{
	 max_depth = this->nodeArr[2]->insert(newParticle);


		}
		else if (this->nodeArr[3]->contains(newParticle)) 
		{
			max_depth = this->nodeArr[3]->insert(newParticle);

		}
		else
		{
			//particle can't be found for some reason so add to 
			//vector of renegade particles so we still calculate their 
			//force
			Galaxy::renegades.push_back(&newParticle);
		}
	}
	else if (this->numParticles == 1)
	{
		if (this->nodeArr.size() == 0 && this->divided == false)
		{
			

			this->subdivide();
		}
		//particle is in the exact same position as another particle
		//move it slightly
		if (particle != nullptr)
		{
			if ((particle)->xy->x == newParticle.xy->x &&
				this->particle->xy->y == newParticle.xy->y)
			{
				if (&newParticle.xy->x <= 0)
					newParticle.xy->x += 0.01f;
				else
					newParticle.xy->x -= 0.01f;

				if (&newParticle.xy->y <= 0)
					newParticle.xy->y += 0.01f;
				else
					newParticle.xy->y -= 0.01f;
			}
		}

		// move old particle because the quadrant was subdivided
		if (this->nodeArr[0]->contains( *particle ))
		{
	
				max_depth = this->nodeArr[0]->insert(*particle);
				this->particle = nullptr;
		
		}
		else if (this->nodeArr[1]->contains( *(this->particle) ))
		{
		
				max_depth = this->nodeArr[1]->insert(*particle);
				this->particle = nullptr;
			
		}
		else if (this->nodeArr[2]->contains( *(this->particle)))
		{


				max_depth = this->nodeArr[2]->insert(*particle);
				this->particle = nullptr;
		
		}
		else if (this->nodeArr[3]->contains( *(this->particle)))
		{

		
				max_depth = this->nodeArr[3]->insert(*particle);
				this->particle = nullptr;
	
		}
		else
		{	
			Galaxy::renegades.push_back(&newParticle);
		}

		//add new particle
		if (this->nodeArr[0]->contains(newParticle))
		{
		
				max_depth = this->nodeArr[0]->insert(newParticle);
		
		}
		else if (this->nodeArr[1]->contains(newParticle))
		{
	
				max_depth = this->nodeArr[1]->insert(newParticle);
		
		}
		else if (this->nodeArr[2]->contains(newParticle))
		{
			
				max_depth = this->nodeArr[2]->insert(newParticle);
		
		}
		else if (this->nodeArr[3]->contains(newParticle))
		{
			
				max_depth = this->nodeArr[3]->insert(newParticle);
		
		}
		else
		{
			Galaxy::renegades.push_back(&newParticle);
		}
	
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
			for (unsigned int i = 0; i < nodeArr.size(); i++) {
					nodeArr[i]->computeMassDistribution();
					this->totalMass += (nodeArr[i])->totalMass;
					this->COM.x += (nodeArr[i])->totalMass * (nodeArr[i])->COM.x;
					this->COM.y += (nodeArr[i])->totalMass * (nodeArr[i])->COM.y;
				}

			COM.x /= totalMass;
			COM.y /= totalMass;
		}
	}
}

void QuadNode::computeMassDistribution_iterative()
{
	if (this == nullptr)
		return;
	const unsigned int MAX = 1000;
	std::stack<QuadNode*> stk1[MAX];
	std::stack<QuadNode*> stk2[MAX];
	QuadNode* ptr = this;
	QuadNode* temp = nullptr;
	stk1->push(ptr);
	
	//stk->push(ptr);
	while (!stk1->empty())
	{
		//pop an item from stk1 and push to stk2
		temp = stk1->top();
		stk1->pop();
		stk2->push(temp);
		//push children left -> right
		if (temp->nodeArr.size() > 0)
		{
			for (int i = 0; i <  4; i++)
				stk1->push(temp->nodeArr[i]);
		}
	
	}
	//std::cout << "size of stk2:" << stk2->size() << "\n";
	while (!stk2->empty())
	{
		temp = stk2->top();
		stk2->pop();

		if (temp->numParticles == 1)
		{
			assert( temp->particle );
			temp->COM.x = temp->particle->xy->x;
			temp->COM.y = temp->particle->xy->y;
			temp->totalMass = temp->particle->mState;
		}
		else
		{
			temp->COM.x = 0;
			temp->COM.y = 0;
			temp->totalMass = 0;

			if (temp->nodeArr.size() > 0)
			{
				for (int i = 0; i < 4; i++)
				{
					temp->COM.x += temp->nodeArr[i]->COM.x * temp->nodeArr[i]->totalMass;
					temp->COM.y += temp->nodeArr[i]->COM.y * temp->nodeArr[i]->totalMass;
					temp->totalMass += temp->nodeArr[i]->totalMass;
				}
				temp->COM.x /= temp->totalMass;
				temp->COM.y /= temp->totalMass;
			}
		}
	}
}




void QuadNode::calcForce(ParticleData& _particle, Vector2D &forces)
{
	Vector2D force1 = this->calcForceTree(_particle);
	if (int s = Galaxy::renegades.size() )
	{
		for (int i = 0; i<s; ++i)
		{
			Vector2D force4 = calcAcceleration(_particle, *Galaxy::renegades[i]);
			float FACTOR = 1.0;
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
		float x1 = COM.x;
		float y1 = COM.y;
		
		float x2 = _particle.xy->x;
		float y2 = _particle.xy->y;

		float mass1 = this->totalMass;
		float mass2 = _particle.mState;

		float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + SOFT_CONST);

		float d = this->botRight.x - this->topLeft.x;
		
		float theta = d / r;
		if (theta <= _THETA)
		{
			float k = G_CONST *  (mass1*mass2) / (r*r);

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

	const float &x1 = _particle1.xy->x;
	const float &y1 = _particle1.xy->y;

	const float &x2 = _particle2.xy->x;
	const float &y2 = _particle2.xy->y;

	const float &mass1 = _particle1.mState;
	const float &mass2 = _particle2.mState; 

	float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + SOFT_CONST);

	 if (r > 0)
	{
		float k = G_CONST * (mass2*mass1) / (r*r);

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
	center = Vector2D((min.x + (max.x - min.x) / 2.0f), (min.y + (max.y - min.y) / 2.0f), 0, 0);
	numParticles = 0;
	totalMass = 0;
	COM.x = 0;
	COM.y = 0;
	divided = false;
}
