#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define N_FLIPS_PER_STEP 1
#define HOT_INIT 0

/*
 * Generates the initial configuration.
 */
void init(int *lattice, const int L)
{
    double r;
    r = 1.0*rand() / RAND_MAX;
    for(int row=0; row<L; row++) {
        for(int col=0; col<L; col++) {
            #if HOT_INIT == 1
                // HOT init, all sites are random
                r = 1.0*rand() / RAND_MAX; // Returns 0 or 1
                lattice[row*L + col] = (r>0.5 ? 1 : -1);
            #else
                // COLD init, the same state for all the spins
                lattice[row*L + col] = (r>0.5 ? 1 : -1);
            # endif
        }
    }
}


/*
 * Copies the lattice into an other lattice
 */
void copy(int *dest_lattice, const int *src_lattice, const int L) {
    for(int row=0; row<L; row++) {
        for(int col=0; col<L; col++) {
            dest_lattice[row*L + col] = src_lattice[row*L + col];
        }
    }
}


/*
 * Computes the energy in units of J
 */
int computeEnergy(const int *lattice, int L)
{
    int E = 0;
    int idx,   // index of the current lattice site,
        idx_r, // of its nearest neighbor on the right
        idx_b; // and the one below
    for(int row=0; row<L; row++) {
        for(int col=0; col<L; col++) {
            idx = row*L + col;
            // Periodic Boundary Conditions
            idx_r = (col == L-1) ? row*L : row*L + col+1;
            idx_b = (row == L-1) ? col   : (row+1)*L + col;

            E += -1*lattice[idx]*lattice[idx_r];
            E += -1*lattice[idx]*lattice[idx_b];
        }
    }

    return E;
}


/*
 * Flips the designated number of flips.
 * Beware that this function does not exclude
 * multiple flips of the same bit
 */
int propose_configuration(int *lattice, int n_flips, int L, int E) {
    int row, col,
        idx_l, idx_a, idx_r, idx_b; //Left, above, right, below

    for(int n=0; n<n_flips; n++) {
        row = rand()%L;
        col = rand()%L;

        lattice[row*L+col] = -1*lattice[row*L+col];

        idx_l = (col == 0)   ?   row*L+(L-1):     row*L + col-1;
        idx_a = (row == 0)   ? (L-1)*L+col  : (row-1)*L + col;
        idx_r = (col == L-1) ?   row*L      :     row*L + col+1;
        idx_b = (row == L-1) ?         col  : (row+1)*L + col;

        E +=  -2*lattice[row*L+col] * ( lattice[idx_l] +
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
int montecarlo_step(int *lattice, int L, int E, double T) {
    int E_prop; // Energy of the proposed configuration
    int *lattice_prop = (int*) calloc(L*L, sizeof(int*));

    copy(lattice_prop, lattice, L);
    E_prop = propose_configuration(lattice_prop, N_FLIPS_PER_STEP, L, E);

    // Acceptation step
    double w = exp(- (E_prop-E)/T);
    double r = 1.0*rand()/RAND_MAX;

    if(r <= w) {        
        copy(lattice, lattice_prop, L);
        free(lattice_prop);
        return 1; // Accepted
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
void printLattice(const int *lattice, const int L) {
    for(int row=0; row<L; row++) {
        for(int col=0; col<L; col++) {
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
 * Save Ising lattice to file in CSV format
 * The columns are, in order:
 *      Lattice size, Temperature, Lattice Energy, lattice sites
 */
void save(FILE *fptr, int L, double T, int E, int *lattice) {
    fprintf(fptr,"%d, %lf, %d", L, T, E);
    for(int row=0; row<L; row++) {
        for(int col=0; col<L; col++) {
            fprintf(fptr, ", %d", lattice[row*L+col]);
        }
    }
    fprintf(fptr, "\n");
}


/*
 * Alternative to srand(time(NULL)) which keeps returning the same value
 * due to very close callings
 * https://stackoverflow.com/questions/7617587/is-there-an-alternative-to-using-time-to-seed-a-random-number-generation
 */
unsigned long long rdtsc() {
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}


/*
 * This program aims to generate Ising configurations
 * on a lattice
 */
int main(int argc, char *argv[]) {
    int L, E, NUMBER_OF_CONFIGURATIONS, THERMALISATION_STEPS, INITIAL_THERMALISATION_STEPS;
    double T;
    FILE *fptr;

    THERMALISATION_STEPS = 100;
    INITIAL_THERMALISATION_STEPS = 100;

    // Parse command line arguments
    for (int i = 0; i < argc; i++) {
        if( strcmp(argv[i], "--lattice-side")==0  &&  i+1<argc ) {
            L = atoi(argv[i+1]); // Side of the lattice
        } else if( strcmp(argv[i], "--number-of-configurations")==0 &&  i+1<argc ) {
            NUMBER_OF_CONFIGURATIONS = atof(argv[i+1]); // Steps
        } else if( strcmp(argv[i], "--thermalisation-steps")==0 &&  i+1<argc ) {
            THERMALISATION_STEPS = atof(argv[i+1]); // Steps
        } else if( strcmp(argv[i], "--temperature")==0 &&  i+1<argc ) {
            T = atof(argv[i+1]); // Temperature
        } else if( strcmp(argv[i], "--initial-thermalisation-steps")==0 &&  i+1<argc ) {
            INITIAL_THERMALISATION_STEPS = atof(argv[i+1]); // Steps
        } else if( strcmp(argv[i], "--output-file")==0 &&  i+1<argc ) {
            fptr = fopen(argv[i+1], "a+");
            if(fptr == NULL) {
                printf("Can not open file %s\n", argv[i+1]);
                exit(1);
            }
        }
    }

    printf("################### SIMULATION INFO ###################\n");
    printf("LATTICE SIDE: %d\n", L);
    printf("TEMPERATURE: %f\n", T);
    printf("NUMBER OF CONFIGURATIONS: %d\n", NUMBER_OF_CONFIGURATIONS);
    printf("THERMALISATION_STEPS: %d\n", THERMALISATION_STEPS);
    printf("INITIAL_THERMALISATION_STEPS: %d\n", THERMALISATION_STEPS);
    printf("#######################################################\n\n");

    int *lattice = (int*) calloc(L*L, sizeof(int));

    srand(rdtsc());

    init(lattice, L);

    int acc, steps;

    E = computeEnergy(lattice, L);
    for(steps=0; steps<INITIAL_THERMALISATION_STEPS; steps++) {
        acc+=montecarlo_step(lattice, L, E, T);
    }

    for(int i = 0; i<NUMBER_OF_CONFIGURATIONS; i++) {
        E = computeEnergy(lattice, L);
        acc = 0;
        steps = 0;

        while(steps<THERMALISATION_STEPS) {
            steps++;
            acc+=montecarlo_step(lattice, L, E, T);
        }

        montecarlo_step(lattice, L, E, T);
        E = computeEnergy(lattice, L);
        save(fptr, L, T, E, lattice);

        printf("montecarlo_step: acceptation_rate=%d/%d=%f\t Energy=%d\t Temperature=%f\n", acc, steps, (double) acc/steps, E, T);
    }

    fclose(fptr);
    return 0;
}
