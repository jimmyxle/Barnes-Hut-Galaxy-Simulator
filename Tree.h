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
	/*
	bool isRoot() const;
	const Vector2D& getMin() const;
	const Vector2D& getMax() const;
	*/

	void insert( ParticleData &newParticle);

	void subdivide();
	bool contains(ParticleData &particle);
	/*
	Quadrant getQuadrant(int x, int y) const;
	QuadNode* createQuadeNode(const ParticleData &p) const; //why &p
	*/
	// variables
	std::vector<QuadNode*> nodeArr; //4 children, 0 NE, 1 NW, 2SE, 3 SW
	Vector2D getVector(int n);


private:

	ParticleData* particle;
	Vector2D topLeft;
	Vector2D center;
	Vector2D botRight;

	QuadNode* parent;
	bool divided; 
	int numParticles;
	int numSubdivisions;
	Quadrant QUADRANT;
	// int mass;
	// Vector2D centerMass;
	// Vector2D center; //not yet

};

