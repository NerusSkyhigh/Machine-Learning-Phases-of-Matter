#include<stdio.h>

#include <time.h>
#include <stdlib.h>

#include<stdbool.h> 

#define sgn(x)( x>=0 ? +1 : -1)

/*
 * Generates the initial configuration.
 *  Should it be random?
 */
void init(bool *lattice, const int L)
{
    double r;
    for(int row=0; row<L; row++)
    {
        for(int col; col<L; col++)
        {
            r = rand()/RAND_MAX;
            lattice[row*L + col] = sgn(r-0.5);
        }
    }
}

/*
 * Copies the lattice into an other lattice
 */
void copy(bool *dest_lattice, const bool *src_lattice, const int L)
{
    for(int row=0; row<L; row++)
    {
        for(int col; col<L; col++)
        {
            dest_lattice[row*L + col] = src_lattice[row*L + col];
        }
    }
}

/*
 * Computes the energy in units of J
 *      (J is a constant so I can
 *          multiply for it after)
 */
int computeEnergy(const int *lattice, int L)
{
    int E = 0;
    int idx,   // index of the current lattice site,
        idx_r, // of its nearest neighbourd on the right
        idx_b; // and the one below
    for(int row=0; row<L; row++)
    {
        for(int col; col<L; col++) // No PBC
        {
            idx = row*L + col;
            idx_r = (col == L-1) ? row*L : row*L + col+1;
            idx_b = (row == L-1) ? col   : (row+1)*L + col;

            E += lattice[idx]*lattice[idx_r];
            E += lattice[idx]*lattice[idx_b];
        }
    }

    return E;
}


/*
 * Flips the designeted number of flips.
 * Beware that this function does not exclude
 * multiple flips of the same bit
 */
int propose_configuration(int *lattice, int n_flips, int L, int E)
{
    int row, col,
        idx_l, idx_a, idx_r, idx_b; //Lefct, above, right, below


    for(int n=0; n<n_flips; n++)
    {
        row = rand()/RAND_MAX * L;
        col = rand()/RAND_MAX * L;

        lattice[row*L+col] = -1*lattice[row*L+col];

        idx_l = (col == 0)   ?   row*L+(L-1):     row*L + col-1;
        idx_a = (row == 0)   ? (L-1)*L+col  : (row-1)*L + col;
        idx_r = (col == L-1) ?   row*L      :     row*L + col+1;
        idx_b = (row == L-1) ?         col  : (row+1)*L + col;

        E +=  2*lattice[row*L+col] * (  lattice[idx_l] +
                                        lattice[idx_a] +
                                        lattice[idx_r] +
                                        lattice[idx_b]);
    }
    return E;
}


/*
 * Perform a Monte Carlo step
 * BEWARE that the input vector is changed!
 */
int montecarlo_step(int *lattice, int L, int E, double T)
{
    int target; // Lattice site to flip
    bool **lattice_prop = (bool**) calloc(L*L, sizeof(bool*));

    copy(lattice_prop, lattice, L);
    // NUMBER OF FLIPS TO BE DETERMINED
    // !!!
    E_prop = propose_configuration(lattice_prop, 1, L, E);

    // Acceptation step
    double w = exp(- (E_prop-E)/T)
    double r = rand()/RAND_MAX;
    if(r <= w) {
        // Accepted
        lattice = lattice_prop;
        return 1;
    } else {
        return 0; // Rejected
    }

}

void printLattice(const int *lattice, const int size)
{
    for(int row=0; row<size; row++)
    {
        for(int col; col<size; col++)
        {
            if( lattice[row*size+col] == 1) {
                printf("%c", (char)254u);
            } else {
                printf("  ");
            }
        }
        printf("\n")
    }
}

/*
 * The aim of this program is to generate Ising Configurtions
 * on a lattice
 */
int main(void)
{
    int L, E;
    double T;

    // Parse command line arguments
    for (int i = 0; i < argc; i++) {
        if( strcmp(argv[i], "-L")==0 &&  i+1<argc ) {
            L = atoi(argv[i+1]); // Side of the lattice
        } else if( strcmp(argv[i], "-T")==0 &&  i+1<argc ) {
            T = atoi(argv[i+1]); // Temperature
        }
    }


    bool **lattice = (bool**) calloc(L*L, sizeof(bool*));
    
    srand(time(NULL));

    for(int i = 0; i< 100; i++) {
        // Generate random initial configuration
        init(lattice, L);

        E = computeEnergy(lattice, L);

        montecarlo_step(lattice, L, E, T)

        printLattice(lattice);
        printf("\n\n");
    }

    return 0;
}

