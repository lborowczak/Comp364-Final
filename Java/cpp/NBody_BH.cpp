#include <iostream>
#include <chrono> //time
#include <random> //random
#include "NBody_BH.hpp"


using namespace std;

    int main (){
        srand(time(NULL)); //seed RNG
        const double dt = 0.1; //default time step, 0.1 seconds
        int nparts = 10000; // default particle count, 100 particles
        int num_steps = 1000; // default simulation steps, 100 steps
        double side = 1000.0;

        uniform_real_distribution<double> randDouble(0, 1);
        default_random_engine re;

        Body* bodies[nparts];// = new Body[nparts];
        for (int i = 0; i < nparts; i++){
            double x = randDouble(re) * side;
            double y = randDouble(re) * side;
            double z = randDouble(re) * side;
            double vx = randDouble(re) * 10.0;
            double vy = randDouble(re) * 10.0;
            double vz = randDouble(re) * 10.0;
            double mass = randDouble(re) * 1000000.0;
            bodies[i] = new Body(x, y, z, vx, vy, vz, mass);
        }

        auto t1 = chrono::system_clock::now();

        for (double t = 0.0; t < num_steps*dt; t += dt){
            Oct* oct = new Oct(0, 0, 0, side); // 1000x1000x1000 region of space
            BHTree* bh = new BHTree(oct); // New empty BH Tree

            // Build BH tree
            for (int i = 0; i < nparts; i++){
                if (bodies[i]->in(oct)){
                    bh->insert(bodies[i]);
                }
            }

            // update forces and positions
            for (int i = 0; i < nparts; i++){
               bodies[i]->resetForce();
               bh->updateForce(bodies[i]);
               bodies[i]->update(dt);
            }
        }

        auto t2 = chrono::system_clock::now();
        chrono::duration<double> t_elapsed = t2 - t1;

        //avg_time = t_elapsed/num_steps;
        //cout << "Average time = " << avg_time << " (ms) per step with " << nparts << " elements over " << num_steps << " steps." << endl;
    }
