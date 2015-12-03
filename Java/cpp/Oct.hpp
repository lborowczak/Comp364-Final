#ifndef OCT_HPP_INCLUDED
#define OCT_HPP_INCLUDED

class Oct
{
private:
    double xmid;
    double ymid;
    double zmid;
    double length;
// xmid, ymid, zmid gives middle coords of the octant
// length is the side length of an octant


public:
    Oct();
    Oct(double xmid, double ymid, double zmid, double length);
    double getLength();
    bool contains(double x, double y, double z);
    Oct* TNW();
    Oct* BNW();
    Oct* TNE();
    Oct* BNE();
    Oct* TSW();
    Oct* BSW();
    Oct* TSE();
    Oct* BSE();
    void copyFrom(Oct* o);
};

#endif








