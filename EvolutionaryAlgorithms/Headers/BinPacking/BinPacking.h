#ifndef BINPACKING_H
#define BINPACKING_H

#include <string>
#include <fstream>
#include <vector>
#include "EvolutionaryAlgorithm.h"
#include "Operators/IntegerMutation.h"
#include "Operators/OnePtXOver.h"
#include "BinPacking/BinPackingFitness.h"
#include "Individuals/GenericIndividual.h"
#include "Selectors/TournamentSelector.h"
#include "Utils/Initializer.h"
#include "Utils/Settings.h"

namespace bin_packing
{
	struct BinPackingSettings : ea::Settings
	{
		double mutation_probability = 0.1; // Probability of mutation
		double gene_change_probability = 0.1; // Probability of changing a gene
		double crossover_probability = 0.1; // Probability of crossover
		size_t bins_count = 5; // Number of bins
	};

	/**
	 * \brief Loads the weight of items from a given file.
	 * \param path 
	 * \return 
	 */
	inline std::vector<int> load_weights(const std::string & path)
	{
		std::ifstream myfile(path);
		std::string line;
		std::vector<int> bins;

		while (getline(myfile, line))
		{
			bins.push_back(std::stoi(line));
		}

		return bins;
	}

	/**
	 * \brief Pretty prints given weights.
	 * \param weights 
	 * \return 
	 */
	inline std::string weights_to_string(const std::vector<int> & weights)
	{
		std::string s = "<";

		for (size_t i = 0; i < weights.size(); ++i)
		{
			s += std::to_string(weights[i]);

			if (i != weights.size() - 1)
			{
				s += ", ";
			}
		}

		s += ">";

		return s;
	}

	/**
	 * \brief Computes the objective value.
	 * \param weights 
	 * \return 
	 */
	inline double compute_objective(const std::vector<int> & weights)
	{
		auto min = (std::numeric_limits<int>::max)();
		auto max = (std::numeric_limits<int>::min)();

		for (auto && bin_weight : weights)
		{
			if (bin_weight > max)
			{
				max = bin_weight;
			}

			if (bin_weight < min)
			{
				min = bin_weight;
			}
		}

		return max - min;
	}

	/**
	 * \brief Tries to solve the bin packing problem.
	 * \param path Path to weights of items
	 * \param settings
	 */
	inline void solve_bin_packing(const std::string & path, const BinPackingSettings & settings)
	{
		using individual_type = ea::GenericIndividual<std::vector<int>, double>;
		using population_type = std::vector<individual_type>;

		if (settings.bins_count < 2)
			throw std::invalid_argument{ "The number of bins should be at least two." };

		const auto weights = load_weights(path);
		ea::EvolutionaryAlgorithm<population_type> ea;

		BinPackingFitness<population_type> fitness(weights, settings.bins_count);

		ea.add_operator(ea::OnePtXOver<population_type>(settings.crossover_probability));
		ea.add_operator(ea::IntegerMutation<population_type>(0, settings.bins_count - 1, settings.mutation_probability, settings.gene_change_probability));
		ea.add_mating_selector(ea::TournamentSelector<population_type>());
		ea.add_natural_selector(ea::TournamentSelector<population_type>());
		ea.set_elitism(settings.elitism);
		ea.set_fitness(fitness);

		ea.set_objective([&](auto & ind) { return compute_objective(fitness.get_bin_weights(ind)); });
		ea.set_individual_to_string([&](individual_type & ind) { return weights_to_string(fitness.get_bin_weights(ind)); });
		ea.set_output_frequency(settings.output_frequency);

		ea.evolve_batch(settings.generations, settings.number_of_runs, [&]() { return ea::init_pop_uniform_int<population_type>(0, settings.bins_count - 1, settings.pop_size, weights.size()); });
	}
}

#endif // BINPACKING_H
