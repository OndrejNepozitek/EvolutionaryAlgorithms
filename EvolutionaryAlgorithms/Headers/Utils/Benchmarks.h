#ifndef BENCHMARKS_H
#define BENCHMARKS_H
#include "TimeMeasuring.h"
#include <iostream>

namespace ea
{
	/**
	 * \brief Runs a benchmark of the algorithm.
	 * \tparam TEvolutionaryAlgorithm 
	 * \tparam TPopulation 
	 * \param ea 
	 * \param population 
	 * \param generation_count 
	 * \return 
	 */
	template<typename TEvolutionaryAlgorithm, typename TPopulation>
	unsigned long long benchmark(TEvolutionaryAlgorithm ea, TPopulation population, const int generation_count)
	{
		const auto time_start = GetTimeMs64();

		for (auto i = 0; i < generation_count; ++i)
		{
			population = ea.evolve(population);
		}

		const auto time_end = GetTimeMs64();
		const auto time_total_ms = time_end - time_start;

		return time_total_ms;
	}

	/**
	 * \brief Runs the benchamrk of the algorithm and outputs the results.
	 * \tparam TEvolutionaryAlgorithm 
	 * \tparam TPopulation 
	 * \param ea 
	 * \param population 
	 * \param generation_count 
	 */
	template<typename TEvolutionaryAlgorithm, typename TPopulation>
	void benchmark_and_output(TEvolutionaryAlgorithm ea, TPopulation population, const int generation_count)
	{
		auto time_total_ms = benchmark(ea, population, generation_count);

		std::cout << "Generations: " << generation_count << ", total ms: " << time_total_ms << ", ms per generation: " << (time_total_ms / static_cast<double>(generation_count)) << std::endl;
	}
}
#endif // BENCHMARKS_H
