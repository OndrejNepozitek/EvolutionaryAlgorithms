#ifndef BINPACKINGFITNESS_H
#define BINPACKINGFITNESS_H
#include <vector>
#include <limits>   

namespace bin_packing
{
	/**
	 * \brief Fitness function for the bin packing problem. 
	 *		  The value is computed to be 1 / ((the difference between the biggest and the smallest bin) + 1).
	 * \tparam TPopulation 
	 */
	template<typename TPopulation>
	class BinPackingFitness
	{
		using individual_type = typename TPopulation::value_type;
		const std::vector<int> & weights_;
		size_t bins_count_;

	public:
		/**
		 * \brief 
		 * \param weights Weights of individual items.
		 * \param bins_count Number of bins.
		 */
		explicit BinPackingFitness(const std::vector<int> & weights, const size_t bins_count) : weights_(weights), bins_count_(bins_count)
		{
			/* empty */
		}

		/**
		 * \brief Sets the fitness of all individuals.
		 * \param population 
		 */
		void operator()(TPopulation & population)
		{
			for (auto && individual : population)
			{
				operator()(individual);
			}
		}

		/**
		 * \brief Sets the fitness of a given individual.
		 * \param individual 
		 */
		void operator()(individual_type & individual)
		{
			auto min = (std::numeric_limits<int>::max)();
			auto max = (std::numeric_limits<int>::min)();

			for (auto && bin_weight : get_bin_weights(individual))
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

			const auto fitness = 1.0 / ((max - min) + 1);
			individual.fitness = fitness;
		}

		/**
		 * \brief Computes weights of individual bins.
		 * \param individual 
		 * \return 
		 */
		std::vector<int> get_bin_weights(individual_type & individual)
		{
			std::vector<int> weights(bins_count_, 0);

			for (size_t i = 0; i < individual.size(); ++i)
			{
				weights[individual[i]] += weights_[i];
			}

			return weights;
		}
	};

}
#endif // BINPACKINGFITNESS_H
