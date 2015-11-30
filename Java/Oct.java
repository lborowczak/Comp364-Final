
public class Oct {

    private double xmid;
    private double ymid;
    private double zmid;
    private double length;

    // xmid, ymid, zmid gives middle coords of the octant
    // length is the side length of an octant
    public Oct(double xmid, double ymid, double zmid, double length) {
        this.xmid = xmid;
        this.ymid = ymid;
        this.zmid = zmid;
        this.length = length;
    }

    // returns side length of octant
    public double length() {
        return length;
    }

    // checks if body is in the octant
    public boolean contains(double x, double y, double z) {
        double halfLen = this.length / 2.0;
        return (x <= this.xmid + halfLen &&
                x >= this.xmid - halfLen &&
                y <= this.ymid + halfLen &&
                y >= this.ymid - halfLen &&
                z <= this.zmid + halfLen &&
                z >= this.zmid - halfLen);
    }

    /*
     * BH quadtrees usually use NW, SW, NE, SE for the directions
     * We'll do something similar for the octrees: We'll keep the
     * above directions for the 2D parts, and then for the z-comp
     * we'll add T or B for top and bottom.
     */

    public Oct TNW() {
        double x = this.xmid - this.length / 4.0;
        double y = this.ymid + this.length / 4.0;
        double z = this.zmid + this.length / 4.0;
        double len = this.length / 2.0;
        Oct TNW = new Oct(x, y, z, len);
        return TNW;
    }

    public Oct BNW() {
        double x = this.xmid - this.length / 4.0;
        double y = this.xmid + this.length / 4.0;
        double z = this.xmid - this.length / 4.0;
        double len = this.length / 2.0;
        Oct BNW = new Oct(x, y, z, len);
        return BNW;
    }


    public Oct TNE() {
        double x = this.xmid + this.length / 4.0;
        double y = this.ymid + this.length / 4.0;
        double z = this.ymid + this.length / 4.0;
        double len = this.length / 2.0;
        Oct TNE = new Oct(x, y, z, len);
        return TNE;
    }

    public Oct BNE() {
        double x = this.xmid + this.length / 4.0;
        double y = this.ymid + this.length / 4.0;
        double z = this.zmid + this.length / 4.0;
        double len = this.length / 2.0;
        Oct BNE = new Oct(x, y, z, len);
        return BNE;
    }


    public Oct TSW() {
        double x = this.xmid - this.length / 4.0;
        double y = this.ymid - this.length / 4.0;
        double z = this.zmid + this.length / 4.0;
        double len = this.length / 2.0;
        Oct TSW = new Oct(x, y, z, len);
        return TSW;
    }

    public Oct BSW() {
        double x = this.xmid - this.length / 4.0;
        double y = this.ymid - this.length / 4.0;
        double z = this.zmid - this.length / 4.0;
        double len = this.length / 2.0;
        Oct BSW = new Oct(x, y, z, len);
        return BSW;
    }


    public Oct TSE() {
        double x = this.xmid + this.length / 4.0;
        double y = this.ymid - this.length / 4.0;
        double z = this.zmid + this.length / 4.0;
        double len = this.length / 2.0;
        Oct TSE = new Oct(x, y, z, len);
        return TSE;
    }
    public Oct BSE() {
        double x = this.xmid + this.length / 4.0;
        double y = this.ymid - this.length / 4.0;
        double z = this.zmid - this.length / 4.0;
        double len = this.length / 2.0;
        Oct BSE = new Oct(x, y, z, len);
        return BSE;
    }


    public String toString() {
        String ret = "\n";
        for (int row = 0; row < this.length; row++) {
            for (int col = 0; col < this.length; col++) {
                if (row == 0 || col == 0 || row == this.length - 1 || col == this.length - 1)
                    ret += "*";
                else
                    ret += " ";
            }
            ret += "\n";
        }
        return ret;
    }
}
