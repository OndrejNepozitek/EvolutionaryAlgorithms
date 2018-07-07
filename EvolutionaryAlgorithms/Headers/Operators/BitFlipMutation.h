#ifndef BITFLIPMUTATION_H
#define BITFLIPMUTATION_H
#include <random>

namespace ea
{
	/**
	* \brief Class implementing the Bit flip mutation
	* \tparam TPopulation Type of the population
	*/
	template<typename TPopulation>
	class BitFlipMutation
	{
		std::uniform_real_distribution<> real_dist_;
		double bit_mutation_probability_;

	public:
		/**
		 * \brief 
		 * \param bit_mutation_probability Probability of flipping a bit
		 */
		explicit BitFlipMutation(const double bit_mutation_probability) : bit_mutation_probability_(bit_mutation_probability)
		{
			if (bit_mutation_probability < 0 || bit_mutation_probability > 1)
				throw std::invalid_argument("mutation_probability must be from the interval [0,1]");

			real_dist_ = std::uniform_real_distribution<>(0, 1);
		}
		
		/**
		* \brief For each bit in every individual, we check if it should be flipped and if so, we flip it.
		* \param population The population that is changed by the operator.
		*/
		void operator()(TPopulation& population) 
		{
			for (auto&& individual : population)
			{
				for (auto&& element : individual)
				{
					if (real_dist_(rng_gen()) < bit_mutation_probability_)
					{
						element = !element;
					}
				}
			}
		}
	};
}
#endif // BITFLIPMUTATION_H
