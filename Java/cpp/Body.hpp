#ifndef BODY_HPP_INCLUDED
#define BODY_HPP_INCLUDED

#include "Oct.hpp"

class Body
{
public:
    const double G = 6.67e-11;
    double rx, ry, rz;       // position
    double vx, vy, vz;       // velocity
    double fx, fy, fz;       // force
    double mass;             // mass

    Body();
    Body(double rx, double ry, double rz, double vx, double vy, double vz, double mass);
    void update(double dt);
    double distanceTo(Body* b);
    void resetForce();
    void addForce(Body* b);
    bool in(Oct* o);
    void plus(Body* b);
    void copyFrom(Body* b);
};

#endif
