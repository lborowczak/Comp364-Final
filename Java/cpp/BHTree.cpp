#include "BHTree.hpp"
//#include <cmath>
#include <algorithm>

// Constructor: Creates new BHTree with no bodies
BHTree::BHTree(Oct* o) {
    oct.copyFrom(o);
    noBody = true;
    TNW = nullptr;
    BNW = nullptr;
    TNE = nullptr;
    BNE = nullptr;
    TSW = nullptr;
    BSW = nullptr;
    TSE = nullptr;
    BSE = nullptr;
    omp_init_lock(&treeLocked);
}


// Adds body to BH Tree
void BHTree::insert(Body* b) {

    // if this node does not contain a body, put the new body b here
    if (insertIfFree(b)){
        #pragma omp critical
        unlock();
        return;
    }

    // internal node
    if (! isExternal()) {
        // update the center-of-mass and total mass
        body.plus(b);

        //Body is now updated, no need to keep the tree locked
        #pragma omp critical
        unlock();

        // recursively insert Body b into the appropriate octant
        putBody(b);
    }

    // external node
    else {

        // update the center-of-mass and total mass
        body.plus(b);

        //unlock this tree because we are not going to be adding anything to it
        #pragma omp critical
        unlock();

        // subdivide the region further by creating eight children
        Oct* tmpTNW = oct.TNW();
        Oct* tmpBNW = oct.BNW();
        Oct* tmpTNE = oct.TNE();
        Oct* tmpBNE = oct.BNE();
        Oct* tmpTSE = oct.TSE();
        Oct* tmpBSE = oct.BSE();
        Oct* tmpTSW = oct.TSW();
        Oct* tmpBSW = oct.BSW();
        TNW = new BHTree(tmpTNW);
        BNW = new BHTree(tmpBNW);
        TNE = new BHTree(tmpTNE);
        BNE = new BHTree(tmpBNE);
        TSE = new BHTree(tmpTSE);
        BSE = new BHTree(tmpBSE);
        TSW = new BHTree(tmpTSW);
        BSW = new BHTree(tmpBSW);
        delete tmpTNW;
        delete tmpBNW;
        delete tmpTNE;
        delete tmpBNE;
        delete tmpTSE;
        delete tmpBSE;
        delete tmpTSW;
        delete tmpBSW;

        // recursively insert both this body and Body b into the appropriate octant
        putBody(&body);
        putBody(b);


    }
}

// Inserts a body into the appropriate octant.
void BHTree::putBody(Body* b) {

    Oct* tmpTNW = oct.TNW();
    Oct* tmpBNW = oct.BNW();
    Oct* tmpTNE = oct.TNE();
    Oct* tmpBNE = oct.BNE();
    Oct* tmpTSE = oct.TSE();
    Oct* tmpBSE = oct.BSE();
    Oct* tmpTSW = oct.TSW();
    Oct* tmpBSW = oct.BSW();

    if (b->in(tmpTNW))
    {
        #pragma omp critical
        TNW->lock();
        TNW->insert(b);
        //TNW->unlock();
    }

    else if (b->in(tmpBNW))
    {
        #pragma omp critical
        BNW->lock();
        BNW->insert(b);
        //BNW->unlock();
    }

    else if (b->in(tmpTNE))
    {
        #pragma omp critical
        TNE->lock();
        TNE->insert(b);
        //TNE->unlock();
    }

    else if (b->in(tmpBNE))
    {
        #pragma omp critical
        BNE->lock();
        BNE->insert(b);
        //BNE->unlock();
    }

    else if (b->in(tmpTSE))
    {
        #pragma omp critical
        TSE->lock();
        TSE->insert(b);
        //TSE->unlock();
    }

    else if (b->in(tmpBSE))
    {
        #pragma omp critical
        BSE->lock();
        BSE->insert(b);
        //BSE->unlock();
    }

    else if (b->in(tmpTSW))
    {
        #pragma omp critical
        TSW->lock();
        TSW->insert(b);
        //TSW->unlock();
    }

    else if (b->in(tmpBSW))
    {
        #pragma omp critical
        BSW->lock();
        BSW->insert(b);
        //BSW->unlock();
    }

    delete tmpTNW;
    delete tmpBNW;
    delete tmpTNE;
    delete tmpBNE;
    delete tmpTSE;
    delete tmpBSE;
    delete tmpTSW;
    delete tmpBSW;
}


// returns true if the node is external
bool BHTree::isExternal() {
    // a node is external iff all eight children are null
    return (TNW == nullptr && BNW == nullptr && TNE == nullptr && BNE == nullptr &&
            TSW == nullptr && BSW == nullptr && TSE == nullptr && BSE == nullptr);
}


/**
 * Approximates the net force acting on Body b from all bodies
 * in the invoking Barnes-Hut tree, and updates b's force accordingly.
 */
void BHTree::updateForce(Body* b) {

    if (noBody || b == &body)
        return;

    // if the current node is external, update net force acting on b
    if (isExternal())
        b->addForce(&body);

    // for internal nodes
    else {

        // width of region represented by internal node
        double s = oct.getLength();

        // distance between Body b and this node's center-of-mass
        double d = body.distanceTo(b);

        // compare ratio (s / d) to threshold value Theta
        if ((s / d) < Theta)
            b->addForce(&body);   // b is far away

        // recurse on each of current node's children
        else {
            TNW->updateForce(b);
            BNW->updateForce(b);
            TNE->updateForce(b);
            BNE->updateForce(b);
            TSW->updateForce(b);
            BSW->updateForce(b);
            TSE->updateForce(b);
            BSE->updateForce(b);
        }
    }
}
void BHTree::search(Body* b[], int n){
    double vmin = 1e10;
    double vmax = 0;
    double ave = 0;
    #pragma omp parallel for reduction (+: ave) reduction (max: vmax) reduction (min: vmin)
    for (int i = 0; i < n; i++){
        double vmag = b[i]->vx * b[i]->vx + b[i]->vy * b[i]->vy + b[i]->vz * b[i]->vz;
        vmag = std::sqrt(vmag);
        vmax = std::max(vmax, vmag);
        vmin = std::min(vmin, vmag);
        ave += vmag;
    }
    ave = ave / n;
    printf("Min/Max/Ave Velocity = %e, %e, %e\n", vmin, vmax, ave);
}

bool BHTree::insertIfFree(Body *b){
    if (noBody) {
        body.copyFrom(b);
        noBody = false;
        return true;
    }
    else {
        return false;
    }

}

void BHTree::lock(){
    omp_set_lock(&treeLocked);
    return;
}
void BHTree::unlock(){
omp_unset_lock(&treeLocked);
}

BHTree::~BHTree(){
    delete TNW;
    delete BNW;
    delete TNE;
    delete BNE;
    delete TSW;
    delete BSW;
    delete TSE;
    delete BSE;
    omp_destroy_lock(&treeLocked);
}
