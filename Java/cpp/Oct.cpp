#include "Oct.hpp"

Oct::Oct() {
    this->xmid = -1;
    this->ymid = -1;
    this->zmid = -1;
    this->length = -1;
}

Oct::Oct(double xmid, double ymid, double zmid, double length) {
    this->xmid = xmid;
    this->ymid = ymid;
    this->zmid = zmid;
    this->length = length;
}

// returns side length of octant
double Oct::getLength() {
    return length;
}

// checks if body is in the octant
bool Oct::contains(double x, double y, double z) {
    double halfLen = length / 2.0;
    return (x <= xmid + halfLen &&
            x >= xmid - halfLen &&
            y <= ymid + halfLen &&
            y >= ymid - halfLen &&
            z <= zmid + halfLen &&
            z >= zmid - halfLen);
}

/*
 * BH quadtrees usually use NW, SW, NE, SE for the directions
 * We'll do something similar for the octrees: We'll keep the
 * above directions for the 2D parts, and then for the z-comp
 * we'll add T or B for top and bottom.
 */

Oct* Oct::TNW() {
    double x = xmid - length / 4.0;
    double y = ymid + length / 4.0;
    double z = zmid + length / 4.0;
    double len = length / 2.0;
    Oct* TNW = new Oct(x, y, z, len);
    return TNW;
}

Oct* Oct::BNW() {
    double x = xmid - length / 4.0;
    double y = xmid + length / 4.0;
    double z = xmid - length / 4.0;
    double len = length / 2.0;
    Oct* BNW = new Oct(x, y, z, len);
    return BNW;
}


Oct* Oct::TNE() {
    double x = xmid + length / 4.0;
    double y = ymid + length / 4.0;
    double z = ymid + length / 4.0;
    double len = length / 2.0;
    Oct* TNE = new Oct(x, y, z, len);
    return TNE;
}

Oct* Oct::BNE() {
    double x = xmid + length / 4.0;
    double y = ymid + length / 4.0;
    double z = zmid + length / 4.0;
    double len = length / 2.0;
    Oct* BNE = new Oct(x, y, z, len);
    return BNE;
}


Oct* Oct::TSW() {
    double x = xmid - length / 4.0;
    double y = ymid - length / 4.0;
    double z = zmid + length / 4.0;
    double len = length / 2.0;
    Oct* TSW = new Oct(x, y, z, len);
    return TSW;
}

Oct* Oct::BSW() {
    double x = xmid - length / 4.0;
    double y = ymid - length / 4.0;
    double z = zmid - length / 4.0;
    double len = length / 2.0;
    Oct* BSW = new Oct(x, y, z, len);
    return BSW;
}


Oct* Oct::TSE() {
    double x = xmid + length / 4.0;
    double y = ymid - length / 4.0;
    double z = zmid + length / 4.0;
    double len = length / 2.0;
    Oct* TSE = new Oct(x, y, z, len);
    return TSE;
}
Oct* Oct::BSE() {
    double x = xmid + length / 4.0;
    double y = ymid - length / 4.0;
    double z = zmid - length / 4.0;
    double len = length / 2.0;
    Oct* BSE = new Oct(x, y, z, len);
    return BSE;
}

/*
public String toString() {
    String ret = "\n";
    for (int row = 0; row < length; row++) {
        for (int col = 0; col < length; col++) {
            if (row == 0 || col == 0 || row == length - 1 || col == length - 1)
                ret += "*";
            else
                ret += " ";
        }
        ret += "\n";
    }
    return ret;
}
*/

void Oct::copyFrom(Oct* o){
    this->xmid = o->xmid;
    this->ymid = o->ymid;
    this->zmid = o->zmid;
    this->length = o->length;
}
