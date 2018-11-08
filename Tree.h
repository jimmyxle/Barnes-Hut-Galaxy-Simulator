#pragma once
#include "Vector.h"
#include "ParticleData.h"
#include <vector>



class QuadNode
{
public:
	enum Quadrant
	{
		NE,NW,SE,SW,NONE
	};
	QuadNode();
	QuadNode(const Vector2D &min, const Vector2D &max, 
		Quadrant quad, QuadNode* _parent , int level_given);
	~QuadNode();

	void reset(const Vector2D &min, const Vector2D &max);

	int buildTree(std::vector<ParticleData*> &arr, int NUMBER_PARTICLES);
	int insert( ParticleData &newParticle);

	void subdivide();
	bool contains(ParticleData &particle);

	void computeMassDistribution();	
	void computeMassDistribution_iterative();

	void calcForce(ParticleData& _particle, Vector2D &forces);
	Vector2D calcForceTree( ParticleData& _particle);


	Vector2D calcAcceleration(ParticleData& _particle1, ParticleData& _particle2); 

	// variables
	std::vector<QuadNode*> nodeArr; //4 children, 0 NE, 1 NW, 2SE, 3 SW
	Vector2D getVector(int n);


private:

	ParticleData* particle;
	Vector2D topLeft;
	Vector2D center;
	Vector2D botRight;


	Vector2D COM; //center of mass
	double totalMass;

	QuadNode* parent;
	bool divided; 
	int numParticles;
	int numSubdivisions;
	Quadrant QUADRANT;

	int level;
	bool COM_calculated;

};

