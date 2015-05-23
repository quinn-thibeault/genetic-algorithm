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

char* random_genome(); //Defines a random genome of the length specified in GENOMESIZE macro
void init_population(struct organism pop[]); //generates a population of POPSIZE with random genomes

void calc_fitness(struct organism pop[]); //determines the fitness of all organisms in population by finding the difference between all of the characters
void sort_by_fitness(struct organism pop[]); //bubble sorts the population so the most fit are in the front
void print_most_fit(struct organism pop[]); //prints the first organism in the population - assumes the population has been sorted

void regen_population(struct organism pop[], struct organism buffer[]); //regenerates the population, preserving the first two and crossing half of the remaining to form the rest of the population
void mutate(struct organism *org); //mutates one character in organism genome
void swap(struct organism **pop, struct organism **buffer); //swaps the population and buffer pointers

int main(void)
{	
	srand(time(NULL));
	
	struct organism population[POPSIZE], buffer[POPSIZE];
	
	struct organism *p_pop = population;
	struct organism *p_buf = buffer;

	init_population(p_pop);
	
	
	for(int i=0;i<ITER; ++i){
		calc_fitness(p_pop);
		sort_by_fitness(p_pop);
		//print_most_fit(p_pop);
		
		if(p_pop->fitness == 0){
			printf("Number of generations: %d\n", i);
			break;
		}

		regen_population(p_pop, p_buf);
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

char* random_genome(void)
{
	char* genome = malloc(sizeof(char) * GENOMESIZE+1);
	
	for(int i=0;i<GENOMESIZE;++i){
		genome[i] = (rand() % 26) + 97;
	}
	genome[GENOMESIZE] = '\0';
	
	return genome;
}

void calc_fitness(struct organism pop[])
{
	char* target = TARGET;
	unsigned int fitness;

	for(int i=0;i<POPSIZE;++i){
		fitness = 0;
		
		for(int j=0;j<GENOMESIZE;++j){
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

void init_population(struct organism* pop)
{
	for(int i=0;i<POPSIZE;++i){
		pop[i].genome = random_genome();
	}
}

void mutate(struct organism *org)
{
	int r = rand() % GENOMESIZE;
	char c = (rand() % 26) + 97;
	org->genome[r] = c;
}

void regen_population(struct organism pop[], struct organism buffer[])
{
	buffer[0] = pop[0];
	buffer[1] = pop[1];

	for(int i=2;i<POPSIZE;++i){
		struct organism new_org;
		new_org.genome = malloc(sizeof(char) * GENOMESIZE+1);
		
		int p1 = rand() % (POPSIZE / 2);
		int p2 = rand() % (POPSIZE / 2);
		int r = rand() % GENOMESIZE;

		for(int j=0;j<r;j++){
			new_org.genome[j] = pop[p1].genome[j];
		}

		for(int l=r;l<GENOMESIZE;++l){
			new_org.genome[l] = pop[p2].genome[l];
		}

		new_org.genome[GENOMESIZE] = '\0';
		if(r < MUT) mutate(&new_org);
	
		buffer[i] = new_org;
	}
}
