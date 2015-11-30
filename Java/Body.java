
public class Body {

    // gravitational constant
    private static final double G = 6.67e-11;

    private double rx, ry, rz;       // position
    private double vx, vy, vz;       // velocity
    private double fx, fy, fz;       // force
    private double mass;             // mass

    public Body(double rx, double ry, double rz, double vx, double vy, double vz, double mass) {
        this.rx    = rx;
        this.ry    = ry;
        this.rz    = rz;
        this.vx    = vx;
        this.vy    = vy;
        this.vz    = vz;
        this.mass  = mass;
    }

    // updates at the next time step
    public void update(double dt) {
        vx += dt * fx / mass;
        vy += dt * fy / mass;
        vz += dt * fz / mass;
        rx += dt * vx;
        ry += dt * vy;
        rz += dt * vz;
    }

    // returns distance between two 'bodies' (which
    // could be multiple bodies in BH approximation)
    public double distanceTo(Body b) {
        double dx = rx - b.rx;
        double dy = ry - b.ry;
        return Math.sqrt(dx*dx + dy*dy);
    }

    public void resetForce() {
        fx = 0.0;
        fy = 0.0;
        fz = 0.0;
    }

    // adds the forces together
    public void addForce(Body b) {
        Body a = this;
        double EPS = 3E4;      // softening parameter
        double dx = b.rx - a.rx;
        double dy = b.ry - a.ry;
        double dz = b.rz - a.rz;
        double dist = Math.sqrt(dx*dx + dy*dy + dz*dz);
        double F = (G * a.mass * b.mass) / (dist*dist + EPS*EPS);
        a.fx += F * dx / dist;
        a.fy += F * dy / dist;
        a.fz += F * dz / dist;
    }
    // Nice representation of a body
    public String toString() {
        return String.format("%10.3E %10.3E %10.3E %10.3E %10.3E %10.3E %10.3E", rx, ry, rz, vx, vy, vz, mass);
    }
    // checks if it's in the octant
    public boolean in(Oct o) {
        return o.contains(this.rx, this.ry, this.rz);
    }

    // center of mass calculation
    public Body plus(Body b) {
        Body a = this;

        double m = a.mass + b.mass;
        double x = (a.rx * a.mass + b.rx * b.mass) / m;
        double y = (a.ry * a.mass + b.ry * b.mass) / m;
        double z = (a.rz * a.mass + b.rz * b.mass) / m;

        return new Body(x, y, z, a.vx, b.vx, a.vy, m);
    }
}
