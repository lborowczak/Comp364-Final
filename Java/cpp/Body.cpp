
#include "Body.hpp"

#include <math.h>

using namespace std;

Body::Body() {
    this->rx    = -1;
    this->ry    = -1;
    this->rz    = -1;
    this->vx    = -1;
    this->vy    = -1;
    this->vz    = -1;
    this->mass  = -1;
}

Body::Body(double rx, double ry, double rz, double vx, double vy, double vz, double mass) {
    this->rx    = rx;
    this->ry    = ry;
    this->rz    = rz;
    this->vx    = vx;
    this->vy    = vy;
    this->vz    = vz;
    this->mass  = mass;
}

// updates at the next time step
void Body::update(double dt) {
    vx += dt * fx / mass;
    vy += dt * fy / mass;
    vz += dt * fz / mass;
    rx += dt * vx;
    ry += dt * vy;
    rz += dt * vz;
}

// returns distance between two 'bodies' (which
// could be multiple bodies in BH approximation)
double Body::distanceTo(Body* b) {
    double dx = rx - b->rx;
    double dy = ry - b->ry;
    double dz = rz - b->rz;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

void Body::resetForce() {
    fx = 0.0;
    fy = 0.0;
    fz = 0.0;
}

// adds the forces together
void Body::addForce(Body* b) {
    double dx = b->rx - rx;
    double dy = b->ry - ry;
    double dz = b->rz - rz;
    double dist = sqrt(dx*dx + dy*dy + dz*dz + 1E-10);
    double F = (G * mass * b->mass) / (dist*dist);
    fx += F * dx / dist;
    fy += F * dy / dist;
    fz += F * dz / dist;
}
// checks if it's in the octant
bool Body::in(Oct* o) {
    return o->contains(rx, ry, rz);
}

// center of mass calculation
//TODO check if this is the correct formula
void Body::plus(Body* b) {
    Body* a = this;

    totalMass = a->mass + b->mass;
    a->rx = (a->rx * a->mass + b->rx * b->mass) / totalMass; //[(location * mass) + (location * mass)] / totalMass
    a->ry = (a->ry * a->mass + b->ry * b->mass) / totalMass;
    a->rz = (a->rz * a->mass + b->rz * b->mass) / totalMass;
    a->mass = totalMass;
    //works: ((((((2*7)+(3*5))/12) * 12) + (4*19))/31) == ((2*7)+(3*5)+(4*19))/31
}

void Body::copyFrom (Body* b){
    rx = b->rx;
    ry = b->ry;
    rz = b->rz;
    vx = b->vx;
    vy = b->vy;
    vz = b->vz;
    mass = b->mass;
}

