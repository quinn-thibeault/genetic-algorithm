#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define TARGET     "harryhan\0" //the target string for the algorithm to find
#define POPSIZE    100 //the population size
#define GENOMESIZE 8 //the size of the genome - must be the size of the target string -- TODO make this dynamic
#define ITER       100000 //Number of iterations to perform in the main loop
#define MUT	   0.25f * POPSIZE // mutation rate

struct organism //data carrying organism structure
{
	char* genome; //char pointer for string
	unsigned int fitness; //fitness - cannot be under zero hence unsigned
};

void random_genome(char* genome, int strlen); //Defines a random genome of the length specified in GENOMESIZE macro
void gen_random_population(struct organism pop[], int strlen); //generates a random population of POPSIZE with random genomes
void init_population(struct organism pop[], int strlen); //initializes structures with memory allocated for genomes. 

void calc_fitness(struct organism pop[], char* target, int strlen); //determines the fitness of all organisms in population by finding the difference between all of the characters
void sort_by_fitness(struct organism pop[]); //bubble sorts the population so the most fit are in the front
void print_most_fit(struct organism pop[]); //prints the first organism in the population - assumes the population has been sorted

void regen_population(struct organism pop[], struct organism buffer[], int strlen); //regenerates the population, preserving the first two and crossing half of the remaining to form the rest of the population
void mutate(struct organism *org, int strlen); //mutates one character in organism genome
void swap(struct organism **pop, struct organism **buffer); //swaps the population and buffer pointers

int main(void)
{	
	int popsize = 0, max_iter = 0;
	char target[128];
	char str_buffer[64];

	FILE *fp = fopen("conf", "r");
	fscanf(fp, "%s %d", &str_buffer, &popsize);
	fscanf(fp, "%s %s", &str_buffer, &target);
	fscanf(fp, "%s %d", &str_buffer, &max_iter);

	int target_length = strlen(target);

	srand(time(NULL));

	printf("Genetic Algorithm to find a user-specified target string\n");
	printf("Quinn Thibeault - 2015\n");
	printf("Target string: %s Length: %d\n", target, target_length);
	printf("Population Size: %d\n", popsize);
	printf("Maximum Iterations: %d\n\n", max_iter);
	
	struct organism population[POPSIZE], buffer[POPSIZE];
	
	struct organism *p_pop = population;
	struct organism *p_buf = buffer;

	init_population(p_pop, target_length);
	init_population(p_buf, target_length);

	gen_random_population(p_pop, target_length);
	
	
	for(int i=0;i<ITER; ++i){
		calc_fitness(p_pop, target, target_length);
		sort_by_fitness(p_pop);
		//print_most_fit(p_pop);
		
		if(p_pop->fitness == 0){
			printf("Number of generations: %d\n", i);
			break;
		}

		regen_population(p_pop, p_buf, target_length);
		swap(&p_pop, &p_buf);
	}
	
	print_most_fit(p_pop);	
	
	return 0;
}

void swap(struct organism **pop, struct organism **buffer)
{
	struct organism* temp = *pop;
	*pop = *buffer;
	*buffer = temp;
}

void print_most_fit(struct organism pop[])
{
	printf("Genome: %s Fitness: %d\n", pop[0].genome, pop[0].fitness);
}

void random_genome(char* genome, int strlen)
{	
	for(int i=0;i<strlen;++i){
		genome[i] = (rand() % 26) + 97;
	}
	genome[strlen] = '\0';
}

void calc_fitness(struct organism pop[], char* target, int strlen)
{
	unsigned int fitness;

	for(int i=0;i<POPSIZE;++i){
		fitness = 0;
		
		for(int j=0;j<strlen;++j){
			fitness += abs(target[j] - pop[i].genome[j]);
		}

		pop[i].fitness = fitness;
	}
}

void sort_by_fitness(struct organism pop[])
{
	for(int i=0;i<POPSIZE;++i){
		for(int j=0;j<POPSIZE-i-1;++j){
			if(pop[j].fitness > pop[j+1].fitness){
				struct organism temp = pop[j];
				pop[j] = pop[j+1];
				pop[j+1] = temp;
			}
		}
	}
}

void gen_random_population(struct organism pop[], int strlen)
{
	for(int i=0;i<POPSIZE;++i){
		random_genome(pop[i].genome, strlen);
	}
}

void init_population(struct organism* pop, int strlen)
{
	for(int i=0;i<POPSIZE;++i){
		pop[i].genome = malloc(sizeof(char) * strlen+1);
	}
}

void mutate(struct organism *org, int strlen)
{
	int r = rand() % strlen;
	char c = (rand() % 26) + 97;
	org->genome[r] = c;
}

void regen_population(struct organism pop[], struct organism buffer[], int strlen)
{
	buffer[0] = pop[0];
	buffer[1] = pop[1];

	for(int i=2;i<POPSIZE;++i){	
		int p1 = rand() % (POPSIZE / 2);
		int p2 = rand() % (POPSIZE / 2);
		int r = rand() % strlen;

		for(int j=0;j<r;j++){
			buffer[i].genome[j] = pop[p1].genome[j];
		}

		for(int l=r;l<strlen;++l){
			buffer[i].genome[l] = pop[p2].genome[l];
		}

		buffer[i].genome[strlen] = '\0';
		if(r < MUT) mutate(&buffer[i], strlen);
	}
}
