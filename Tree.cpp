#include "Tree.h"
#include <cstdlib>
/*
static/global variables go here 



*/
QuadNode::QuadNode()
{
	std::cout << "Default constructor used" << std::endl;
	particle = nullptr;
	botLeft = botRight = topRight = topLeft = Vector2D();
	parent = nullptr;
	nodeArr.reserve(4);
	divided = false;
	numParticles = 0;
}

QuadNode::QuadNode(const Vector2D &min, const Vector2D &max, QuadNode* _parent)
{
	particle = nullptr;

	topLeft = min;
	topRight = Vector2D(max.x, min.y);
	botLeft = Vector2D(min.x, max.y);
	botRight = max;
	
	parent = _parent;
	nodeArr.reserve(4);

	divided = false;
	numParticles = 0;


	/*
	mass = 0;
	centerMass = Vector2D();
	center = Vector2D((min.x + (max.x - min.x) / 2.0), (min.y + (max.y - min.y) / 2.0));
	*/
}
QuadNode::~QuadNode()
{
	for (int i = 0; i < 4; i++)
	{
		auto temp = nodeArr[i];
		delete (&temp) ;
	}
}
bool QuadNode::contains(ParticleData &particle)
{
	int p_x = particle.xy.x;
	int p_y = particle.xy.y;
	return	p_x >= this->topLeft.x &&
			p_y <= this->topLeft.y &&
			p_x <= this->botRight.x &&
			p_y >= this->botRight.y;
}

void QuadNode::insert(ParticleData &newParticle)
{
	//check if it contains this 


	if (!this->contains(newParticle))
	{
		return; //doesn't contain this point
	}

	if (this->numParticles > 1)
	{
		//find quadrant
		//recurse on quadrant

		//check if subdivided already
		if (this->nodeArr[0]->contains(newParticle)) 
		{
			this->nodeArr[0]->insert(newParticle);
		}
		else if (this->nodeArr[1]->contains(newParticle)) 
		{
			this->nodeArr[1]->insert(newParticle);

		}
		else if (this->nodeArr[2]->contains(newParticle)) 
		{
			this->nodeArr[2]->insert(newParticle);

		}
		else if (this->nodeArr[3]->contains(newParticle)) 
		{
			this->nodeArr[3]->insert(newParticle);
		}
		else
		{
			std::cout << "Error not contained within this quadrant." << std::endl;
		}
	}
	else if (this->numParticles == 1)
	{
		if (this->nodeArr.size() == 0)
		{
			this->subdivide();
		}
		/*
		if (this->particle->xy.x == newParticle.xy.x &&
			this->particle->xy.y == newParticle.xy.y)
		{

		}
		//check if exactly the same
			//do soemthing
		*/
		
		//find quadrant of original
			//insert on that
			//remove pointer
		if (this->nodeArr[0]->contains( *( this -> particle) ))
		{
			this->nodeArr[0]->insert(*particle);
			this->particle = nullptr;
		}
		else if (this->nodeArr[1]->contains( *(this->particle)))
		{
			this->nodeArr[1]->insert(*particle);
			this->particle = nullptr;
		}
		else if (this->nodeArr[2]->contains( *(this->particle)))
		{
			this->nodeArr[2]->insert(*particle);
			this->particle = nullptr;
		}
		else if (this->nodeArr[3]->contains( *(this->particle)))
		{
			this->nodeArr[3]->insert(*particle);
			this->particle = nullptr;
		}
		else
		{
			std::cout << "Error not contained within this quadrant." << std::endl;
		}

			//find quadrant of new
			//insert on new
		if (this->nodeArr[0]->contains(newParticle))
		{
			this->nodeArr[0]->insert(newParticle);
		}
		else if (this->nodeArr[1]->contains(newParticle))
		{
			this->nodeArr[1]->insert(newParticle);

		}
		else if (this->nodeArr[2]->contains(newParticle))
		{
			this->nodeArr[2]->insert(newParticle);

		}
		else if (this->nodeArr[3]->contains(newParticle))
		{
			this->nodeArr[3]->insert(newParticle);
		}
		else
		{
			std::cout << "Error not contained within this quadrant." << std::endl;
		}
	}
	else if (this->numParticles == 0)
	{
		this->particle = &newParticle;
	}
	numParticles++;


}

Vector2D QuadNode::getVector(int n)
{
	switch (n)
	{
	case 0:
		return topLeft;
		break;
	case 1:
		return topRight;
		break;
	case 2:
		return botLeft;
		break;
	case 3:
		return botRight;
	}
}


void QuadNode::subdivide()
{
	int width = abs(topLeft.x) + abs(topRight.x);
	int height = abs(topLeft.y) + abs(botLeft.y);


	//NE
	Vector2D new_min = Vector2D(topRight.x - width / 2.0, topRight.y);
	Vector2D new_max = Vector2D(topRight.x, topRight.y - height / 2.0);
	nodeArr.push_back(  new QuadNode(new_min, new_max, this) ) ;

	//NW
	new_min = topLeft;
	new_max = Vector2D( (topLeft.x + (width)/2.0 ),(topLeft.y - height/2.0 ) ) ;
	nodeArr.push_back(new QuadNode(new_min, new_max, this));

	//SE
	new_min = Vector2D(botLeft.x, botLeft.y + height / 2);
	new_max = Vector2D(botLeft.x + width / 2, botLeft.y);
	nodeArr.push_back(new QuadNode(new_min, new_max, this));

	//SW
	new_min = Vector2D(botRight.x - width / 2, botRight.y + height / 2);
	new_max = botRight;
	nodeArr.push_back(new QuadNode(new_min, new_max, this));


	this->divided = true;


}

