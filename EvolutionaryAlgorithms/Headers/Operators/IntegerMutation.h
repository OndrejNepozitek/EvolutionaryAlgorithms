#ifndef INTEGERMUTATION_H
#define INTEGERMUTATION_H
#include <cstdint>
#include <random>
#include "Utils/Utils.h"

namespace ea
{
	/**
	* \brief Class implementing the Uniform integer mutation
	* \tparam TPopulation Type of the population
	*/
	template<typename TPopulation>
	class IntegerMutation
	{
		int_least32_t from_;
		int_least32_t to_;
		double mutation_probability_;
		double gene_change_probability_;

		std::uniform_real_distribution<> real_dist_;
		std::uniform_int_distribution<> int_dist_;

	public:
		/**
		 * \brief 
		 * \param from The lower bound of the mutation. Inclusive.
		 * \param to The upper bound of the mutation. Inclusive.
		 * \param mutation_probability The probability of mutating an individual.
		 * \param gene_change_probability The probability of mutating an individual gene value.
		 */
		IntegerMutation(const int_least32_t from, const int_least32_t to, const double mutation_probability, const double gene_change_probability)
			: from_(from), to_(to), mutation_probability_(mutation_probability), gene_change_probability_(gene_change_probability)
		{
			if (from > to)
				throw std::invalid_argument("from must be less than or equal to to");

			if (mutation_probability < 0 || mutation_probability > 1)
				throw std::invalid_argument("mutation_probability must be from the interval [0,1]");

			if (gene_change_probability < 0 || gene_change_probability > 1)
				throw std::invalid_argument("gene_change_probability must be from the interval [0,1]");

			real_dist_ = std::uniform_real_distribution<>(0, 1);
			int_dist_ = std::uniform_int_distribution<>(from, to);
		}

		/**
		* \brief For each individual, we check if it should be mutated and if so, repeat the process for every gene value, mutating chosen ones.
		* \param population The population that is changed by the operator.
		*/
		void operator()(TPopulation& population)
		{
			for (auto && individual : population)
			{
				if (real_dist_(rng_gen()) < mutation_probability_)
				{
					for (auto && element : individual)
					{
						if (real_dist_(rng_gen()) < gene_change_probability_)
						{
							element = int_dist_(rng_gen());
						}
					}
				}
			}
		}
	};
}
#endif // INTEGERMUTATION_H
