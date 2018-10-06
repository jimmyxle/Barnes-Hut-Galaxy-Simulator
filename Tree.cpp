#include "Tree.h"
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include "Galaxy.h"

/*
static/global variables go here 
*/

const double _THETA = 0.9; //close to 1.0, determines # calcs
const double G_CONST = 6.674 * pow(10.0, -11.0);
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

QuadNode::QuadNode(const Vector2D &min, const Vector2D &max, Quadrant quad, QuadNode* _parent)
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


}
QuadNode::~QuadNode()
{
	if (nodeArr.size() != 0)
	{
		for (int i = 0; i < 4; i++)
		{

			delete  nodeArr[i];
			//nodeArr[i] = nullptr;
			
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
		p_x <= this->botRight.x && 
		p_y >= this->botRight.y &&
		p_y <= this->topLeft.y)
	{
//		std::cout << "found it ! " << std::endl;
		return true;
	}
	else
	{
		/*
		std::cout << p_x << "," << p_y << std::endl;
		std::cout << " not found in (" <<
			this->topLeft.x << " , " << this->topLeft.y << ") and (" <<
			this->botRight.x << " , " << this->botRight.y << ")\n";
			*/
		return false;

	}
}


void QuadNode::insert(ParticleData &newParticle)
{

	if (!this->contains( newParticle ))
	{
		std::stringstream ss;
		ss << "doesn't contain this particle";
		throw std::runtime_error(ss.str());
	}
	


	if (this->numParticles > 1)
	{
		//std::cout << ">1 "  << std::endl;

		//find quadrant
		//recurse on quadrant

		//check if subdivided already
	//	std::cout <<"alpha: "<< nodeArr.size()<<"\n";

		if (nodeArr.size() == 0 && divided == false)
		{
			this->subdivide();
		}
//		newParticle.printParticle();
	//	std::cout << "\n";


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

			std::cerr << "a: Particle added to renegades.\n";
			newParticle.printParticle();
			std::cout << "\n";
			Galaxy::renegades.push_back(&(newParticle));
				//pushback(newParticle);
			//this->parent->insert(*(newParticle).renegadeHandler());
		}
	}
	else if (this->numParticles == 1)
	{
	//	std::cout << "beta: " << nodeArr.size() << "\n";


		if (this->nodeArr.size() == 0 && this->divided == false)
		{
	//		std::cout << "split" << std::endl;
			this->subdivide();
		}
		
		
		if ((particle)->xy->x == newParticle.xy->x &&
			this->particle->xy->y == newParticle.xy->y)
		{
		//handle renegades ?			
			if(newParticle.xy->x <=0)
				newParticle.xy->x += 0.01;
			else
				newParticle.xy->x -= 0.01;


			if (newParticle.xy->y <= 0)
				newParticle.xy->y += 0.01;
			else
				newParticle.xy->y -= 0.01;

		}
		



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


			std::cerr << "b: Particle added to renegades.\n";
			newParticle.printParticle();
			std::cout << "\n";
			Galaxy::renegades.push_back(&(newParticle));


		}
		//std::cout << "Handle new particle =" << (&newParticle)->xy->x << std::endl;

			//find quadrant of new
			//insert on new
	//	newParticle.printParticle();
//		std::cout << "\n";

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

			std::cerr << "c: Particle added to renegades.\n";
			newParticle.printParticle();
			std::cout << "\n";
			Galaxy::renegades.push_back(&(newParticle));


		}
	}
	else if (this->numParticles == 0)
	{
	//	std::cout << "gamma: " << nodeArr.size() << "\n";

		//std::cout << " == 0 " << std::endl;
//		newParticle.printParticle();
//		std::cout << "\n";
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
	/*
	std::cout << "subdivide begins" << std::endl;

	double width = abs(topLeft.x) + abs(botRight.x);
	double height = abs(topLeft.y) + abs(botRight.y);

	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: "<< height << std::endl;

	Vector2D temp_topLeft = topLeft;
	Vector2D temp_botRight = botRight;
	*/


	//NE
	Vector2D new_min = Vector2D(center.x, topLeft.y );
	Vector2D new_max = Vector2D(botRight.x, center.y);
	nodeArr.push_back(  new QuadNode(new_min, new_max, NE , this) ) ;

	//NW
	new_min = topLeft;
	new_max = center;
	nodeArr.push_back(new QuadNode(new_min, new_max,NW,  this));

	//SE

	new_min = center;
	new_max = botRight;
	nodeArr.push_back(new QuadNode(new_min, new_max, SE,this));

	//SW
	new_min = Vector2D(topLeft.x, center.y);
	new_max = Vector2D(center.x , botRight.y);
	nodeArr.push_back(new QuadNode(new_min, new_max, SW, this));




	this->divided = true;
	this->numSubdivisions++;
	/*
	std::cout << "subdivisions: "<<numSubdivisions << std::endl;
	std::cout << "subdivide ends" << std::endl;
	*/
	
}

void QuadNode::computeMassDistribution()
{
	// std::cout << "num of particles: " << numParticles << std::endl;
	if (numParticles == 1)
	{
		COM.x = particle->xy->x;
		COM.y = particle->xy->y;

		totalMass = particle->mState;
		// std::cout << "COM is (" << COM.x << "," << COM.y << ")" << std::endl;
	}
	else
	{
		totalMass = 0;
		COM.x = 0;
		COM.y = 0;

		for (std::vector<QuadNode*>::iterator it = nodeArr.begin(); it != nodeArr.end(); 
			it++)
		{
			if ( (*it) )
			{

				(*it)->computeMassDistribution();

				this->totalMass += (*it)->totalMass;

				this->COM.x += (*it)->totalMass*(*it)->COM.x;
				this->COM.y += (*it)->totalMass*(*it)->COM.y;
			}
		}
		COM.x /= totalMass;
		COM.y /= totalMass;
	}
}


Vector2D QuadNode::calcForce(ParticleData& _particle)
{
	Vector2D force1 = this->calcForceTree(_particle);

	//include osmething for renegade stuff

	if (int s = Galaxy::renegades.size())
	{
		for (int i = 0; i<s; ++i)
		{
			Vector2D force4 = calcAcceleration(_particle, *Galaxy::renegades[i] );
			force1.x += force4.x;
			force1.y += force4.y;
		}
	}
	 return force1;
}


Vector2D QuadNode::calcForceTree(ParticleData& _particle)
{
	Vector2D force2;

	if (numParticles == 1)
	{
		force2 =  calcAcceleration(*particle, _particle);
		//add debug stuff but not needed here yet
	}
	else
	{
		//there should be a check here 
		double x1 = COM.x;
		double y1 = COM.y;
		double x2 = _particle.xy->x;
		double y2 = _particle.xy->y;
		double mass1 = this->totalMass;
		double mass2 = _particle.mState;

		double r = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		double d = botRight.x - topLeft.x;

		double theta = d / r;
		if (theta <= _THETA)
		{
			double k = G_CONST * ( (mass1) / (r*r*r));

			force2.x += k*(x1 - x2);
			force2.y += k*(y1 - y2);
/*			double k = G_CONST * (mass1 / (r * r * r));
			force2.x += k* (x2 - x1);
			force2.y += k* (y2 - y1);*/

			//add number of calc for debug
		}
		else
		{
			// subdivided = true
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


//new particle acting on this 
Vector2D QuadNode::calcAcceleration(ParticleData& _particle1, ParticleData& _particle2)
{
	Vector2D force3;
	const double &x1 = _particle1.xy->x;
	const double &y1 = _particle1.xy->y;
	const double &x2 = _particle2.xy->x;
	const double &y2 = _particle2.xy->y;
	const double &mass1 = _particle1.mState;
	const double &mass2 = _particle2.mState;

	double r = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

	if (x1 == x2 && y1 == y2)
	{
		return force3 = Vector2D();
	}
	else if (r > 0)
	{
		double k = G_CONST * ((mass2) / (r*r*r));

		force3.x += k * (x2 - x1);
		force3.y += k * (y2 - y1);

	}
	else
	{
		//not possible if two particles are too close together
		force3.x = force3.y = 0;
	}
	return force3;
}
/*

Function MainApp::CalcForce
for all particles
force = RootNode.CalculateForceFromTree(particle)
end for
end

Function force = TreeNode::CalculateForce(targetParticle)
force = 0

if number of particle equals 1
force = Gravitational force between targetParticle and particle
else
r = distance from nodes center of mass to targetParticle
d = height of the node
if (d/r < θ)
force = Gravitational force between targetParticle and node
else
for all child nodes n
force += n.CalculateForce(particle)
end for
end if
end
end

*/

void QuadNode::buildTree(std::vector<ParticleData*> &arr, int NUMBER_PARTICLES)
{
	if (parent != nullptr)
	{
		std::cout << "only the root can call this." << std::endl;

		return;
	}

	this->reset(topLeft, botRight);

	for (int i = 0; i < NUMBER_PARTICLES; i++)
	{
		//std::cout << "particle#: " << i << "\n";
		this->insert(*arr[i]);
	}
}

void QuadNode::reset(const Vector2D &min, const Vector2D &max )
{
	if (parent != nullptr)
	{
		std::cout << "not root" << std::endl;
		return;
	}

	if (nodeArr.size() != 0 )
	{

		for (int i = 0; i < 4; i++)
		{

			delete nodeArr[i];
			nodeArr[i] = nullptr;

		}
		nodeArr.clear();


		topLeft = min;
		botRight = max;

		center = Vector2D((min.x + (max.x - min.x) / 2.0), (min.y + (max.y - min.y) / 2.0), 0, 0);
		numParticles = 0;
		totalMass = 0;
		COM.x = 0;
		COM.y = 0;
		divided = false;
	}


}
