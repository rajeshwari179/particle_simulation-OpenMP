#include "common.h"
#include <omp.h>
#include <cmath>
# include <set>
# include <vector>
# include <iostream>
# include <unordered_map>

std::vector<std::set<int>> cell_map;

double cell_size = cutoff;
int ROW;
omp_lock_t* locks; 

int cell_num(double x, double y, double size, double cell_size){
    double col = x / cell_size;
    double row = y / cell_size;
    double temp = ceil(size / cell_size);
    ROW = (int)temp;
    int col_num = (int)col;
    int row_num = (int)row;
    return col_num + row_num * ROW;
}



// Apply the force from neighbor to particle
void apply_force(particle_t& particle, particle_t& neighbor) {
    // Calculate Distance
    double dx = neighbor.x - particle.x;
    double dy = neighbor.y - particle.y;
    double r2 = dx * dx + dy * dy;

    // Check if the two particles should interact
    if (r2 > cutoff * cutoff)
        return;

    r2 = fmax(r2, min_r * min_r);
    double r = sqrt(r2);

    // Very simple short-range repulsive force
    double coef = (1 - cutoff / r) / r2 / mass;
    particle.ax += coef * dx;
    particle.ay += coef * dy;
}

// Integrate the ODE
void move(particle_t& p, double size) {
    // Slightly simplified Velocity Verlet integration
    // Conserves energy better than explicit Euler method
    p.vx += p.ax * dt;
    p.vy += p.ay * dt;
    p.x += p.vx * dt;
    p.y += p.vy * dt;

    // Bounce from walls
    while (p.x < 0 || p.x > size) {
        p.x = p.x < 0 ? -p.x : 2 * size - p.x;
        p.vx = -p.vx;
    }

    while (p.y < 0 || p.y > size) {
        p.y = p.y < 0 ? -p.y : 2 * size - p.y;
        p.vy = -p.vy;
    }
}
void cleanup_simulation() {
    delete[] locks;
}



void init_simulation(particle_t* parts, int num_parts, double size) {
    double temp = ceil(size / cell_size);
    ROW = (int)temp;
    int cell;
    cell_map.resize(ROW*ROW);
    for (int i = 0; i < num_parts; ++i) {
        int cell = cell_num(parts[i].x, parts[i].y, size, cell_size);
        cell_map[cell].insert(i);
    }
    locks = new omp_lock_t[ROW*ROW];
    for (int i = 0; i < ROW*ROW; ++i) {
        omp_init_lock(&locks[i]);
    }
	// You can use this space to initialize static, global data objects
    // that you may need. This function will be called once before the
    // algorithm begins. Do not do any particle simulation here
}
bool edge_check(int row, int col){
    if(row < 0 || row >= ROW || col < 0 || col >= ROW){
        return false;
    }
    return true;
}


void simulate_one_step(particle_t* parts, int num_parts, double size) {
    // // Compute Forces
    // for (int i = 0; i < num_parts; ++i) {
    //     parts[i].ax = parts[i].ay = 0;
    //     for (int j = 0; j < num_parts; ++j) {
    //         apply_force(parts[i], parts[j]);
    //     }
    // }

    #pragma omp parallel for
    for (int i=0; i<ROW;++i){
        for(int j=0; j<ROW; ++j){
            int g = j + i*ROW;
            for (const int& k : cell_map[g]){
                parts[k].ax = parts[k].ay = 0;

                for (int l = i-1; l <= i+1; ++l){
                    for (int m = j-1; m <= j+1; ++m){
                        if (edge_check(l, m)){
                            int h = m + l*ROW;
                            for (const int& t : cell_map[h]){
                                apply_force(parts[k], parts[t]);
                            }
                        }
                    }
                }
            }
        }

        }
    


    // #pragma omp critical
    // Move Particles
    #pragma omp parallel for
    for (int i = 0; i < num_parts; ++i) {
        
        int temp1 = cell_num(parts[i].x, parts[i].y, size, cell_size);

        move(parts[i], size);

        int x = cell_num(parts[i].x, parts[i].y, size, cell_size);

        if(temp1 != x)
        {
            omp_set_lock(&locks[temp1]);
            cell_map[temp1].erase(i);
            omp_unset_lock(&locks[temp1]);
             omp_set_lock(&locks[x]);
            cell_map[x].insert(i);
            omp_unset_lock(&locks[x]);
        }
        
    }
  
}
