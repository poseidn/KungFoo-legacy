#pragma once

//#include <boost/tr_container/ptr_vector.hpp>
#include <list>
#include <vector>

class ParticleSystemVisual;

typedef std::list<ParticleSystemVisual *> ParticeSystemVisualList;

typedef std::vector<ParticleSystemVisual *> ParticeSystemVisualVectorAdd;
typedef std::vector<ParticleSystemVisual *> ParticeSystemVisualVector;

