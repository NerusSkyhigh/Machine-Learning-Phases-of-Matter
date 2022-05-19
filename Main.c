#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define N_FLIPS_PER_STEP 1
#define HOT_INIT 0

// steps: number of steps since last valid configuration
// acc: number of accepted configurations
#define TERMALISATION_CONDITION (steps<100 || (steps < 10000 && abs(acc-0.5) > 0.1 ) )

/*
 * Generates the initial configuration.
 *  Should it be random?
 */
void init(int *lattice, const int L)
{
    int r;
    r = rand() % 2;
    for(int row=0; row<L; row++)
    {
        for(int col=0; col<L; col++)
        {
            #if HOT_MODE == 1
                // HOT init, all site are random
                r = rand() % 2; // Returns 0 or 1
                printf("%d", r ? 1 : -1);
                lattice[row*L + col] = (r ? 1 : -1);
            #else
                // COLD init, same state for all the spins
                lattice[row*L + col] = (r ? 1 : -1);
            # endif
        }
        //printf("\n");
    }
}

/*
 * Copies the lattice into an other lattice
 */
void copy(int *dest_lattice, const int *src_lattice, const int L)
{
    for(int row=0; row<L; row++)
    {
        for(int col=0; col<L; col++)
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
        for(int col=0; col<L; col++)
        {
            // Periodic Boundary Conditions
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
        row = rand()%L;
        col = rand()%L;
        //printf("row=%d\t col=%d\n", row, col);

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
    int E_prop; // Energy of the proposed configuration
    int *lattice_prop = (int*) calloc(L*L, sizeof(int*));

    copy(lattice_prop, lattice, L);
    E_prop = propose_configuration(lattice_prop, N_FLIPS_PER_STEP, L, E);

    // Acceptation step
    double w = exp(- (E_prop-E)/T);
    double r = 1.0*rand()/RAND_MAX;
    //printf("E_prop=%d\t w=%f\t r=%f\t T=%f\n",E_prop, w, r, T);

    if(r <= w) {
        // Accepted
        copy(lattice, lattice_prop, L);
        free(lattice_prop);
        return 1;
    } else {
        free(lattice_prop);
        return 0; // Rejected
    }

}

/*
 * Prints the lattice.
 *      spin=1 --> +
 *      spin=0 --> O
 */
void printLattice(const int *lattice, const int L)
{
    for(int row=0; row<L; row++)
    {
        for(int col=0; col<L; col++)
        {
            if( lattice[row*L+col] == 1) {
                printf("+");
            } else {
                printf("O");
            }
        }
        printf("\n");
    }
}

/*
 * Save ising lattice to file in CSV format
 * The columns are, in order,
 *      Lattice size, Temperature, Lattice Energy, lattice sites
 */
void save(FILE *fptr, int L, double T, int E, int *lattice)
{
    fprintf(fptr,"%d, %lf, %d", L, T, E);
    for(int row=0; row<L; row++) {
        for(int col=0; col<L; col++) {
            fprintf(fptr, ", %d", lattice[row*L+col]);
        }
    }
    fprintf(fptr, "\n");
}


/*
 * The aim of this program is to generate Ising Configurtions
 * on a lattice
 */
int main(int argc, char *argv[])
{
    int L, E, N_MARKOV_STEPS;
    double T;
    FILE *fptr;

    // Parse command line arguments
    for (int i = 0; i < argc; i++) {
        if( strcmp(argv[i], "-L")==0 &&  i+1<argc ) {
            L = atoi(argv[i+1]); // Side of the lattice
        } else if( strcmp(argv[i], "-ms")==0 &&  i+1<argc ) {
            N_MARKOV_STEPS = atof(argv[i+1]); // Steps
        } else if( strcmp(argv[i], "-T")==0 &&  i+1<argc ) {
            T = atof(argv[i+1]); // Temperature
        } else if( strcmp(argv[i], "-o")==0 &&  i+1<argc ) {
            fptr = fopen(argv[i+1], "a+");
            if(fptr == NULL) {
                printf("Can not open file %s\n", argv[i+1]);
                exit(1);
            }
        }
    }
    
    int *lattice = (int*) calloc(L*L, sizeof(int));

    srand(time(NULL));

    printf("init:\n");
    init(lattice, L);


    int acc, steps;
    for(int i = 0; i<N_MARKOV_STEPS; i++) {

        //printf("computeEnergy: ");
        E = computeEnergy(lattice, L);
        //printf("%d\n", E);

        acc = 0;
        steps = 0;
        while( TERMALISATION_CONDITION ) {
            steps++;
            acc+=montecarlo_step(lattice, L, E, T);
            //printf("%f\t", (double) acc/steps );
        }

        montecarlo_step(lattice, L, E, T);
        E = computeEnergy(lattice, L);
        save(fptr, L, T, E, lattice);

        printf("montecarlo_step: acc=%d/%d=%f\t E=%d\t T=%f\n", acc, steps, (double) acc/steps, E, T);

        /*
            printf("printLattice:\n");
            printLattice(lattice, L);
            printf("\n----------------------------------\n");
         */
    }

    fclose(fptr);
    return 0;
}