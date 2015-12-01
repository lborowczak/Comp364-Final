import java.util.Random;

public class BHTree {

    // threshold value
    private final double Theta = 0.5;

    private Body body;     // body or aggregate body stored in this node
    private Oct oct;       // cubic region that the tree represents
    private BHTree TNW;    // tree representing top northwest octant
    private BHTree BNW;    // tree representing bottom northwest octant
    private BHTree TNE;    // tree representing top northeast octant
    private BHTree BNE;    // tree representing bottom northeast octant
    private BHTree TSW;    // tree representing top southwest octant
    private BHTree BSW;    // tree representing bottom southwest octant
    private BHTree TSE;    // tree representing top southeast octant
    private BHTree BSE;    // tree representing bot southeast octant

    // Constructor: Creates new BHTree with no bodies
    public BHTree(Oct o) {
        this.oct = o;
        this.body = null;
        this.TNW = null;
        this.BNW = null;
        this.TNE = null;
        this.BNE = null;
        this.TSW = null;
        this.BSW = null;
        this.TSE = null;
        this.BSE = null;
    }


    // Adds body to BH Tree
    public void insert(Body b) {

        // if this node does not contain a body, put the new body b here
        if (body == null) {
            body = b;
            return;
        }

        // internal node
        if (! isExternal()) {
            // update the center-of-mass and total mass
            body = body.plus(b);

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
            putBody(this.body);
            putBody(b);

            // update the center-of-mass and total mass
            body = body.plus(b);
        }
    }

    // Inserts a body into the appropriate octant.
    private void putBody(Body b) {
        if (b.in(oct.TNW()))
            TNW.insert(b);
        else if (b.in(oct.BNW()))
            BNW.insert(b);
        else if (b.in(oct.TNE()))
            TNE.insert(b);
        else if (b.in(oct.BNE()))
            BNE.insert(b);
        else if (b.in(oct.TSE()))
            TSE.insert(b);
        else if (b.in(oct.BSE()))
            BSE.insert(b);
        else if (b.in(oct.TSW()))
            TSW.insert(b);
        else if (b.in(oct.BSW()))
            BSW.insert(b);
    }


    // returns true if the node is external
    private boolean isExternal() {
        // a node is external iff all eight children are null
        return (TNW == null && BNW == null && TNE == null && BNE == null &&
                TSW == null && BSW == null && TSE == null && BSE == null);
    }


    /**
     * Approximates the net force acting on Body b from all bodies
     * in the invoking Barnes-Hut tree, and updates b's force accordingly.
     */
    public void updateForce(Body b) {

        if (body == null || b.equals(body))
            return;

        // if the current node is external, update net force acting on b
        if (isExternal())
            b.addForce(body);

        // for internal nodes
        else {

            // width of region represented by internal node
            double s = oct.length();

            // distance between Body b and this node's center-of-mass
            double d = body.distanceTo(b);

            // compare ratio (s / d) to threshold value Theta
            if ((s / d) < Theta)
                b.addForce(body);   // b is far away

            // recurse on each of current node's children
            else {
                TNW.updateForce(b);
                BNW.updateForce(b);
                TNE.updateForce(b);
                BNE.updateForce(b);
                TSW.updateForce(b);
                BSW.updateForce(b);
                TSE.updateForce(b);
                BSE.updateForce(b);
            }
        }
    }
    public void search(Body [] b, int n){
        double vmin = 1e10;
        double vmax = 0;
        double ave = 0;

        for (int i = 0; i < n; i++){
            double vmag = b[i].vx * b[i].vx + b[i].vy * b[i].vy + b[i].vz * b[i].vz;
            vmag = Math.sqrt(vmag);
            vmax = Math.max(vmax, vmag);
            vmin = Math.min(vmin, vmag);
            ave += vmag;
        }
        ave = ave / n;
        System.out.println("Min/Max/Ave Velocity: " + vmin + ", " + vmax + ", " + ave);
    }    

    public String toString() {
        if (isExternal())
            return " " + body + "\n";
        else
            return "*" + body + "\n" + TNW + BNW + TNE + BNE + TSW + BSW + TSE + BSE;
    }
}
