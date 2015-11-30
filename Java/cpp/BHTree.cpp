#include "BHTree.hpp"

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
}


// Adds body to BH Tree
void BHTree::insert(Body* b) {

    // if this node does not contain a body, put the new body b here
    if (noBody) {
        body.copyFrom(b);
        noBody = false;
        return;
    }

    // internal node
    if (! isExternal()) {
        // update the center-of-mass and total mass
        body.plus(b);

        // recursively insert Body b into the appropriate octant
        putBody(b);
    }

    // external node
    else {
        // subdivide the region further by creating four children
        TNW = new BHTree(oct.TNW());
        BNW = new BHTree(oct.BNW());
        TNE = new BHTree(oct.TNE());
        BNE = new BHTree(oct.BNE());
        TSE = new BHTree(oct.TSE());
        BSE = new BHTree(oct.BSE());
        TSW = new BHTree(oct.TSW());
        BSW = new BHTree(oct.BSW());


        // recursively insert both this body and Body b into the appropriate octant
        putBody(&body);
        putBody(b);

        // update the center-of-mass and total mass
        body.plus(b);
    }
}

// Inserts a body into the appropriate octant.
void BHTree::putBody(Body* b) {
    if (b->in(oct.TNW()))
        TNW->insert(b);
    else if (b->in(oct.BNW()))
        BNW->insert(b);
    else if (b->in(oct.TNE()))
        TNE->insert(b);
    else if (b->in(oct.BNE()))
        BNE->insert(b);
    else if (b->in(oct.TSE()))
        TSE->insert(b);
    else if (b->in(oct.BSE()))
        BSE->insert(b);
    else if (b->in(oct.TSW()))
        TSW->insert(b);
    else if (b->in(oct.BSW()))
        BSW->insert(b);
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
/*
public String toString() {
    if (isExternal())
        return " " + body + "\n";
    else
        return "*" + body + "\n" + TNW + BNW + TNE + BNE + TSW + BSW + TSE + BSE;
}
*/
