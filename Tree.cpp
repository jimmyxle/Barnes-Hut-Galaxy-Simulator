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
	numSubdivisions = 0;
	QUADRANT = NONE;

}

QuadNode::QuadNode(const Vector2D &min, const Vector2D &max, Quadrant quad, QuadNode* _parent)
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
	numSubdivisions = 0;
	QUADRANT = quad;


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
bool QuadNode::contains(ParticleData &_particle)
{
	double p_x = (&_particle)->xy->x;
	double p_y = (&_particle)->xy->y;
	
	std::cout << "Contains begins on ";

	std::cout << p_x << "," << p_y << std::endl;

	std::cout << "checked against (" <<
		this->topLeft.x << " , " << this->topLeft.y << ") and (" <<
		this->botRight.x << " , " << this->botRight.y << ")\n";
		
	return	p_x >= this->topLeft.x &&
			p_x <= this->botRight.x &&
			p_y >= this->botRight.y &&
			p_y <= this->topLeft.y;
			
}

void QuadNode::insert(ParticleData &newParticle)
{
	//check if it contains this 
	/*
	std::cout << "insert begins on new_particle"<<(&newParticle)->xy->x << std::endl;
	std::cout << "numParticles: " << numParticles << std::endl;
	
	if (!this->contains( newParticle ))
	{
		std::cout << "doesn't contain this particle" << std::endl;
		return; //doesn't contain this point
	}*/
	


	if (this->numParticles > 1)
	{
		//std::cout << ">1 "  << std::endl;

		//find quadrant
		//recurse on quadrant

		//check if subdivided already
		if (nodeArr.size() == 0 && divided == false)
		{
			this->subdivide();
		}
		newParticle.printParticle();
		std::cout << "\n";


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
			std::cout << "a: Error not contained within this quadrant." << std::endl;
		}
	}
	else if (this->numParticles == 1)
	{
		//std::cout << " == 1 " << std::endl;

		if (this->nodeArr.size() == 0 && this->divided == false)
		{
			std::cout << "split" << std::endl;
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
		//std::cout << "Handle old particle ::" << (particle)->xy->x << std::endl;
		//find quadrant of original
			//insert on that
			//remove pointer

		particle->printParticle(); 
		std::cout << "\n";


		if (this->nodeArr[0]->contains( *particle ))
		{
			//std::cout << "old in NE" << std::endl;
			this->nodeArr[0]->insert(*particle);
			this->particle = nullptr;
		}
		else if (this->nodeArr[1]->contains( *(this->particle) ))
		{
			//std::cout << "old in NW" << std::endl;

			this->nodeArr[1]->insert(*particle);
			this->particle = nullptr;

		}
		else if (this->nodeArr[2]->contains( *(this->particle)))
		{
			//std::cout << "old in SE" << std::endl;

			this->nodeArr[2]->insert(*particle);
			this->particle = nullptr;

		}
		else if (this->nodeArr[3]->contains( *(this->particle)))
		{
			//std::cout << "old in SW" << std::endl;

			this->nodeArr[3]->insert(*particle);
			this->particle = nullptr;

		}
		else
		{
			std::cout << "b: Error not contained within this quadrant." << std::endl;
		}
		//std::cout << "Handle new particle =" << (&newParticle)->xy->x << std::endl;

			//find quadrant of new
			//insert on new
		newParticle.printParticle();
		std::cout << "\n";

		if (this->nodeArr[0]->contains(newParticle))
		{
			//std::cout << "new in NE" << std::endl;

			this->nodeArr[0]->insert(newParticle);
		}
		else if (this->nodeArr[1]->contains(newParticle))
		{
			//std::cout << "new in NW" << std::endl;

			this->nodeArr[1]->insert(newParticle);

		}
		else if (this->nodeArr[2]->contains(newParticle))
		{
			//std::cout << "new in SE" << std::endl;

			this->nodeArr[2]->insert(newParticle);

		}
		else if (this->nodeArr[3]->contains(newParticle))
		{
			//std::cout << "new in SW" << std::endl;

			this->nodeArr[3]->insert(newParticle);
		}
		else
		{
			std::cout << "c: Error not contained within this quadrant." << std::endl;
		}
	}
	else if (this->numParticles == 0)
	{
		//std::cout << " == 0 " << std::endl;
		newParticle.printParticle();
		std::cout << "\n";
		this->particle = &newParticle;
	}
	numParticles++;
	//std::cout << "insert on new_particle" << (&newParticle)->xy->x << std::endl;



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
	std::cout << "subdivide begins" << std::endl;

	double width = abs(topLeft.x) + abs(topRight.x);
	double height = abs(topLeft.y) + abs(botLeft.y);

	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: "<< height << std::endl;

	Vector2D temp_topLeft = topLeft;
	Vector2D temp_botRight = botRight;


	//NE
	Vector2D new_min = Vector2D(temp_botRight.x - (width / 2.0), temp_topLeft.y );
	Vector2D new_max = Vector2D(temp_botRight.x, temp_botRight.y + (height / 2.0));
	nodeArr.push_back(  new QuadNode(new_min, new_max, NE , this) ) ;

	//NW
	new_min = temp_topLeft;
	new_max = Vector2D( (temp_topLeft.x + (width/2.0) ),(temp_topLeft.y - (height/2.0) ) ) ;
	nodeArr.push_back(new QuadNode(new_min, new_max,NW,  this));

	//SE

	new_min = Vector2D(temp_botRight.x - (width / 2.0), temp_botRight.y + (height / 2.0));
	new_max = temp_botRight;
	nodeArr.push_back(new QuadNode(new_min, new_max, SE,this));

	//SW
	new_min = Vector2D(temp_topLeft.x, temp_topLeft.y - height / 2.0);
	new_max = Vector2D(temp_topLeft.x + (width / 2.0), temp_botRight.y);
	nodeArr.push_back(new QuadNode(new_min, new_max, SW, this));




	this->divided = true;
	this->numSubdivisions++;
	
	std::cout << "subdivisions: "<<numSubdivisions << std::endl;
	std::cout << "subdivide ends" << std::endl;
	
}

