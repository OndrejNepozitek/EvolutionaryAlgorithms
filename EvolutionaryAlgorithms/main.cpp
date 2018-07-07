#include <iostream>
#include "Headers/BinPacking/BinPacking.h"
#include "Headers/SGA/SGA.h"

int main()
{
	std::cout << "Simple genetic algorithm:" << std::endl << std::endl;

	{
		// Simple genetic algoritm - the goal is to evolve an individual with all ones
		// The objective is set to be the number of positions that are not yet set to one.
		// The goal is to get the objective value to be equal to zero.
		sga::SGASettings settings;

		settings.output_frequency = 100;
		settings.number_of_runs = 10;
		settings.bit_mutation_probability = 0.25;
		settings.crossover_probability = 0.6;
		settings.elitism = 0.01;

		sga::solve_sga<70>(settings);
	}

	std::cout << std::endl << std::endl << "Bin packing:" << std::endl << std::endl;

	{
		// Bin packing - the goal is to divide items with different weights to the specified number of bins, 
		// minimizing the difference between the smallest and the biggest bin.
		// The objective is set to be the difference between the smallest and the biggest bin.
		// The goal is to get the objective value to be equal to zero.
		bin_packing::BinPackingSettings settings;

		settings.mutation_probability = 0.6;
		settings.crossover_probability = 0.2;
		settings.gene_change_probability = 0.001;
		settings.output_frequency = 500;
		settings.number_of_runs = 5;
		settings.bins_count = 7;
		settings.generations = 2500;
		settings.elitism = 0.05;

		bin_packing::solve_bin_packing("packingInput-easier.txt", settings);
	}

	return 0;
}
