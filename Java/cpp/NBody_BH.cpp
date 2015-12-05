#include <iostream>
#include <chrono> //time
#include <random> //random
#include "NBody_BH.hpp"


using namespace std;

        int main (int argc, char* argv[]){
        srand(time(NULL)); //seed RNG
        const double dt = 0.001; //default time step, 0.1 seconds
        int nparts = 100; // default particle count, 100 particles
        // define number of particles
        if (argc > 1){
           if (isdigit(*argv[1])){
              nparts = atoi( argv[1]);
           }
        }
        int num_steps = 100; // default simulation steps, 100 steps
        if (argc > 2){
           if (isdigit(*argv[2])){
              num_steps = atoi(argv[2]);
           }
        }
        double side = 2.0;

        uniform_real_distribution<double> randDouble(0, 1);
        default_random_engine re;

        Body* bodies[nparts];// = new Body[nparts];
        for (int i = 0; i < nparts; i++){
            double x = side * (randDouble(re) - 0.5);
            double y = side * (randDouble(re) - 0.5);
            double z = side * (randDouble(re) - 0.5);
            double vx = 0;
            double vy = 0;
            double vz = 0;
            double mass = randDouble(re) + std::numeric_limits<double>::epsilon();
            bodies[i] = new Body(x, y, z, vx, vy, vz, mass);
        }

        auto t1 = chrono::system_clock::now();

        for (int step = 0; step < num_steps; step++){
            Oct* oct = new Oct(0, 0, 0, side); // 1000x1000x1000 region of space
            //printf("Side length: %e  \n", side);
            BHTree* bh = new BHTree(oct); // New empty BH Tree

            // Build BH tree
            for (int i = 0; i < nparts; i++){
                if (bodies[i]->in(oct)){
                    bh->insert(bodies[i]);
                }
            }

            // update forces
            for (int i = 0; i < nparts; i++){
               bodies[i]->resetForce();
               bh->updateForce(bodies[i]);
            }

            // update positions and velocities and side length
            #pragma omp parallel for reduction (max: side)
            for (int i = 0; i < nparts; i++){
               bodies[i]->update(dt);
               side = std::max(side, 2*std::abs(bodies[i]->rx));
               side = std::max(side, 2*std::abs(bodies[i]->ry));
               side = std::max(side, 2*std::abs(bodies[i]->rz));
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

        chrono::duration<double> avg_time = 1000*t_elapsed/num_steps;
        //printf("Average time = %f (ms) per step with %d elements over %d steps\n", avg_time*1000, nparts, num_steps);
        cout << "Average time = " << fixed << avg_time.count() << " (ms) per step with " << nparts << " elements over " << num_steps << " steps." << endl;
        cout << "Total time (ms): " <<fixed << t_elapsed.count()*1000 << endl;
    }
