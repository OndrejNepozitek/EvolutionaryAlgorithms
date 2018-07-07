#ifndef ONEPTXOVER_H
#define ONEPTXOVER_H
#include <random>
#include "../Utils/Utils.h"

namespace ea
{
	/**
	* \brief Class implementing the One point crossover operator
	* \tparam TPopulation Type of the population
	* \remarks https://en.wikipedia.org/wiki/Crossover_(genetic_algorithm)
	*/
	template<typename TPopulation>
	class OnePtXOver
	{
		double probability_;
		std::uniform_real_distribution<> real_dist_;

	public:
		/**
		 * \brief 
		 * \param probability The probability of doing the crossover for each pair of individuals
		 */
		explicit OnePtXOver(const double probability) : probability_(probability)
		{	
			real_dist_ = std::uniform_real_distribution<>(0, 1);
		}

		/**
		 * \brief For each consecutive pair of individuals rolls a dice to decide if we should do the crossover.
		 *		  If so, we pick a random position in the individuls and switch all data beyond that position.
		 * \param population The population that is changed by the operator.
		 */
		void operator()(TPopulation& population)
		{
			auto individual_size = population[0].size();
			const std::uniform_int_distribution<int> int_distribution(0, individual_size - 1);

			for (size_t i = 0; i < population.size() / 2; ++i)
			{
				auto p1 = population[2 * i];
				auto p2 = population[2 * i + 1];

				if (real_dist_(rng_gen()) < probability_)
				{
					const auto position = int_distribution(rng_gen());

					for (size_t pos = position; pos < individual_size; ++pos)
					{
						auto temp = p1[pos];
						p1[pos] = p2[pos];
						p2[pos] = temp;
					}
				}
			}
		}
	};
}
#endif // ONEPTXOVER_H
