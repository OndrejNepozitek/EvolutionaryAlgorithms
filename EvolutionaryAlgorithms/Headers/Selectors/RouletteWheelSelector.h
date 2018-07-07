#ifndef ROULETTEWHEELSELECTOR_H
#define ROULETTEWHEELSELECTOR_H
#include <random>

namespace ea
{
	/**
	 * \brief Class implementing the Roulette wheel selection
	 * \tparam TPopulation Type of the population
	 * \remarks https://en.wikipedia.org/wiki/Fitness_proportionate_selection
	 */
	template<typename TPopulation>
	class RouletteWheelSelector
	{
		using individual_type = typename TPopulation::value_type;
		using fitness_type = typename individual_type::fitness_type;

		std::uniform_real_distribution<> real_distribution_;

	public:

		/**
		 * \brief Uses the Roulette wheel selection to select a given number of individuals
		 * \param from The source of individuals to be selected.
		 * \param to The destination of selected individuals.
		 * \param count How many individuals should be selected.
		 * \remarks Fitness of all individuals must be non-negative.
		 */
		void operator()(const TPopulation & from, TPopulation & to, const size_t count)
		{
			fitness_type fitness_sum = 0;

			for (auto&& individual : from)
			{
				if (individual.fitness < 0)
				{
					throw std::logic_error{"Fitness of all individuals must be non-negative."};
				}

				fitness_sum += individual.fitness;
			}

			auto counter = 0;
			while (counter < count)
			{
				auto random = real_distribution_(rng_gen()) * fitness_sum;
				fitness_type sum = 0;

				for (auto&& individual : from)
				{
					sum += individual.fitness;

					if (random <= sum)
					{
						to.push_back(individual);
						++counter;
						break;
					}
				}
			}
		}
	};
}
#endif // ROULETTEWHEELSELECTOR_H
