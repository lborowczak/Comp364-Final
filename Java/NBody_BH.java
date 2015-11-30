import java.util.Random;

public class NBody_BH{
    public static void main(String [] args){
        Random rand = new Random();
        final double dt = 0.1; //default time step, 0.1 seconds
        int nparts = 10000; // default particle count, 100 particles
        int num_steps = 1000; // default simulation steps, 100 steps
        double side = 1000.0;

        Body [] bodies = new Body[nparts];
        for (int i = 0; i < nparts; i++){
            double x = rand.nextDouble() * side;
            double y = rand.nextDouble() * side;
            double z = rand.nextDouble() * side;
            double vx = rand.nextDouble() * 10.0;
            double vy = rand.nextDouble() * 10.0;
            double vz = rand.nextDouble() * 10.0;
            double mass = rand.nextDouble() * 1000000.0;
            bodies[i] = new Body(x, y, z, vx, vy, vz, mass);
        }

        double t1 = System.currentTimeMillis();

        for (double t = 0.0; t < num_steps*dt; t += dt){
            Oct oct = new Oct(0, 0, 0, side); // 1000x1000x1000 region of space
            BHTree bh = new BHTree(oct); // New empty BH Tree

            // Build BH tree
            for (int i = 0; i < nparts; i++){
                if (bodies[i].in(oct)){
                    bh.insert(bodies[i]);
                }
            }

            // update forces and positions
            for (int i = 0; i < nparts; i++){
               bodies[i].resetForce();
               bh.updateForce(bodies[i]);
               bodies[i].update(dt);
            }
        }
        double t2 = System.currentTimeMillis();
        double t_elapsed = t2 - t1;

        double avg_time = t_elapsed/num_steps;
        System.out.println("Average time = " + avg_time + " (ms) per step with " + nparts + " elements over " + num_steps + " steps.");
    }
}
