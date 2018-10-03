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
	QuadNode(const Vector2D &min, const Vector2D &max, Quadrant quad, QuadNode* _parent = nullptr);
	~QuadNode();

	void reset(const Vector2D &min, const Vector2D &max);


	void insert( ParticleData &newParticle);

	void subdivide();
	bool contains(ParticleData &particle);

	void computeMassDistribution();
	Vector2D calcForce(ParticleData& _particle);
	Vector2D calcForceTree( ParticleData& _particle);
	Vector2D calcAcceleration(ParticleData& _particle1, ParticleData& _particle2); //new particle acting on this

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
	// int mass;
	// Vector2D centerMass;
	// Vector2D center; //not yet

	//subdivded use for force calculation. true if too close for approx

};

