#ifndef SETTINGS_H
#define SETTINGS_H

namespace ea
{
	struct Settings
	{
		size_t generations = 1000; // Number of generations
		size_t pop_size = 100; // Size of the population
		double elitism = 0; // The percentage of the best individuals to keep
		size_t output_frequency = 1; // How often do we want to show the progress
		size_t number_of_runs = 1; // How many times do we want to run the algorithm
	};
}
#endif // SETTINGS_H
