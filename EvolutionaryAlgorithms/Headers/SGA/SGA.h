#ifndef SGA_H
#define SGA_H
#include <vector>
#include <array>
#include "EvolutionaryAlgorithm.h"
#include "Operators/BitFlipMutation.h"
#include "Operators/OnePtXOver.h"
#include "Selectors/RouletteWheelSelector.h"
#include "Individuals/GenericIndividual.h"
#include "Utils/Initializer.h"
#include "Utils/Settings.h"

namespace sga
{
	struct SGASettings : ea::Settings
	{
		double bit_mutation_probability = 0.2; // Probability of mutating a bit
		double crossover_probability = 0.1; // Probability of crossover
	};

	template<typename TIndividual>
	std::string print_individual(TIndividual & individual)
	{
		std::string s = "<";

		for (size_t i = 0; i < individual.size(); ++i)
		{
			s += std::to_string(individual[i]);
		}

		s += ">";

		return s;
	}

	template<size_t Size>
	void solve_sga(const SGASettings & settings)
	{
		using individual_type = ea::GenericIndividual<std::array<bool, Size>, int>;
		using population_type = std::vector<individual_type>;

		ea::EvolutionaryAlgorithm<population_type> ea{};

		ea.add_operator(ea::BitFlipMutation<population_type>{settings.bit_mutation_probability});
		ea.add_operator(ea::OnePtXOver<population_type>{settings.crossover_probability});
		ea.add_mating_selector(ea::RouletteWheelSelector<population_type>{});
		ea.set_fitness([](auto & pop) -> void {
			for (auto && individual : pop)
			{
				auto sum = 0;

				for (auto && element : individual)
				{
					if (element)
					{
						++sum;
					}
				}

				individual.fitness = sum;
			}
		});

		ea.set_elitism(settings.elitism);
		ea.set_output_frequency(settings.output_frequency);
		ea.set_individual_to_string(print_individual<individual_type>);
		ea.set_objective([&](auto & ind) { return Size - ind.fitness; });

		ea.evolve_batch(settings.generations, settings.number_of_runs, [&]() { return ea::init_pop_uniform_bool<population_type>(settings.pop_size, Size); });
	}
}
#endif // SGA_H
