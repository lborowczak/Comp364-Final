#ifndef BHTREE_HPP_INCLUDED
#define BHTREE_HPP_INCLUDED

#include "Oct.hpp"
#include "Body.hpp"
#include <omp.h>

class BHTree
{

public:
    BHTree(Oct* o);
    void insert(Body* b);
    void updateForce(Body* b);
    void search(Body* b[], int n);
    void lock();
    void unlock();
    ~BHTree();
private:

    const double Theta = 0.5;

    Body body;     // body or aggregate body stored in this node
    bool noBody;
    Oct oct;       // cubic region that the tree represents
    BHTree* TNW;    // tree representing top northwest octant
    BHTree* BNW;    // tree representing bottom northwest octant
    BHTree* TNE;    // tree representing top northeast octant
    BHTree* BNE;    // tree representing bottom northeast octant
    BHTree* TSW;    // tree representing top southwest octant
    BHTree* BSW;    // tree representing bottom southwest octant
    BHTree* TSE;    // tree representing top southeast octant
    BHTree* BSE;    // tree representing bot southeast octant
    void putBody(Body* b);
    bool isExternal();
    bool insertIfFree(Body* b);
    omp_lock_t treeLocked;
};

#endif
