#include <iostream>
#include <chrono> //time
#include <random> //random
#include "NBody_BH.hpp"


using namespace std;

    int main (){
        srand(time(NULL)); //seed RNG
        const double dt = 0.1; //default time step, 0.1 seconds
        int nparts = 10000; // default particle count, 100 particles
        int num_steps = 100; // default simulation steps, 100 steps
        double side = 1000.0;

        uniform_real_distribution<double> randDouble(0, 1);
        default_random_engine re;

        Body* bodies[nparts];// = new Body[nparts];
        for (int i = 0; i < nparts; i++){
            double x = randDouble(re) * side;
            double y = randDouble(re) * side;
            double z = randDouble(re) * side;
            double vx = 0;
            double vy = 0;
            double vz = 0;
            double mass = randDouble(re) * 1e18;
            bodies[i] = new Body(x, y, z, vx, vy, vz, mass);
        }

        auto t1 = chrono::system_clock::now();

        for (int step; step < num_steps; step++){
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
            // search sometimes
            if (step % 10 == 0){
               bh->search(bodies, nparts);
            }
        delete oct;
        delete bh;
        }
        for (int i = 0; i < nparts; i++){
            delete bodies[i];
        }
        auto t2 = chrono::system_clock::now();
        chrono::duration<double> t_elapsed = t2 - t1;

        std::chrono::duration<double> avg_time = t_elapsed/num_steps;
        printf("Average time = %f (ms) per step with %d elements over %d steps\n", avg_time*1000, nparts, num_steps);
        //cout << "Average time = " << avg_time << " (ms) per step with " << nparts << " elements over " << num_steps << " steps." << endl;
    }
