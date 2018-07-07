#ifndef EVOLUTIONARYALGORITHM_H
#define EVOLUTIONARYALGORITHM_H
#include <functional>
#include <vector>
#include <algorithm>

namespace ea
{
	/**
	 * \brief Customizable evolutionary algorithm
	 * \tparam TPopulation Type of the population.
	 */
	template<typename TPopulation>
	class EvolutionaryAlgorithm
	{
		// Helper typedefs
		using operator_type = std::function<void(TPopulation &)>;
		using selector_type = std::function<void(const TPopulation &, TPopulation &, size_t)>;
		using fitness_fnc_type = std::function<void(TPopulation &)>;
		using individual_type = typename TPopulation::value_type;
		using fitness_value_type = typename individual_type::fitness_type;
		using objective_fnc_type = std::function<fitness_value_type(individual_type &)>;
		using population_initializer_type = std::function<TPopulation()>;
		using individual_to_string_type = std::function<std::string(individual_type&)>;

		std::vector<operator_type> operators_;
		std::vector<selector_type> mating_selectors_;
		std::vector<selector_type> natural_selectors_;
		fitness_fnc_type fitness_;
		objective_fnc_type objective_fnc_;
		individual_to_string_type individual_to_string_;

		size_t output_frequency_ = 1;

		bool elitism_enabled_ = false;
		double elitism_percentage_ = 0;

		/**
		 * \brief Finds the best individual.
		 * \param population 
		 * \return 
		 */
		individual_type & get_best_individual(TPopulation & population)
		{
			return *(std::max_element(population.begin(), population.end(), [](auto i1, auto i2) { return i1.fitness < i2.fitness; }));
		}

		/**
		 * \brief Helper function that computes how many individuals should we select in idividual selectors
		 * \param selector_order 
		 * \param total_selectors 
		 * \param elements_to_select 
		 * \return 
		 */
		size_t how_many_should_select(const size_t selector_order, const size_t total_selectors, const size_t elements_to_select) const
		{
			const auto each = static_cast<int>(std::floor(elements_to_select / static_cast<double>(total_selectors)));

			if (selector_order < total_selectors - 1)
			{
				return each;
			}

			return elements_to_select - (total_selectors - 1) * each;
		}

	public:

		/**
		 * \brief Registers a given operator. 
		 * \remarks If more than one operator is registered, they will be called in the order of registration.
		 * \param op 
		 */
		void add_operator(operator_type op)
		{
			operators_.push_back(op);
		}

		/**
		 * \brief Registers a given mating selector.
		 * \remarks If more than one mating selector is registered, each selector will be used to select a portion of the total population.
		 * \param selector 
		 */
		void add_mating_selector(selector_type selector)
		{
			mating_selectors_.push_back(selector);
		}

		/**
		* \brief Registers a given natural selector.
		* \remarks If more than one natural selector is registered, each selector will be used to select a portion of the total population.
		* \param selector
		*/
		void add_natural_selector(selector_type selector)
		{
			natural_selectors_.push_back(selector);
		}

		/**
		 * \brief Sets the fitness function.
		 * \param fitness 
		 */
		void set_fitness(fitness_fnc_type fitness)
		{
			this->fitness_ = fitness;
		}

		/**
		 * \brief Configures elitism.
		 * \param percentage What percentage of the best individuals should be kept.
		 */
		void set_elitism(const double percentage)
		{
			if (percentage > 1 || percentage < 0)
			{
				throw std::invalid_argument("Percentage must be less than 1 and greater than 0");
			}

			elitism_enabled_ =  percentage != 0;
			elitism_percentage_ = percentage;
		}

		/**
		 * \brief Sets te objective function.
		 * \param objective 
		 */
		void set_objective(objective_fnc_type objective)
		{
			objective_fnc_ = objective;
		}

		/**
		 * \brief Sets a function that is used to convert the best individual to string.
		 * \param individual_to_string 
		 */
		void set_individual_to_string(individual_to_string_type individual_to_string)
		{
			individual_to_string_ = individual_to_string;
		}

		/**
		 * \brief Configures how often (in terms of iterations) should be progress shown.
		 * \param frequency 
		 */
		void set_output_frequency(const size_t frequency)
		{
			output_frequency_ = frequency;
		}

		/**
		 * \brief Does one iteration of the evolutionary algorithm.
		 * \param population 
		 * \return 
		 */
		TPopulation evolve(const TPopulation & population)
		{
			TPopulation new_pop{};

			// Apply mating selectors
			for (size_t i = 0; i < mating_selectors_.size(); ++i)
			{
				mating_selectors_[i](population, new_pop, how_many_should_select(i, mating_selectors_.size(), population.size()));
			}

			// Apply operators
			for (auto&& op : operators_)
			{
				op(new_pop);
			}

			// Apply the fitness function
			fitness_(new_pop);

			auto temp = std::move(new_pop);
			auto select_count = population.size();
			new_pop = TPopulation{};

			// Apply elitism if enabled
			if (elitism_enabled_)
			{
				auto population_copy = population;
				std::sort(population_copy.begin(), population_copy.end(), [](auto i1, auto i2) { return i1.fitness > i2.fitness; });
				const auto elite_count = static_cast<int>(std::ceil(population_copy.size() * elitism_percentage_));

				for (auto i = 0; i < elite_count; ++i)
				{
					new_pop.push_back(population_copy[i]);
				}

				select_count -= elite_count;
			}

			// Apply natural selectors if we have any
			if (!natural_selectors_.empty())
			{
				for (size_t i = 0; i < natural_selectors_.size(); ++i)
				{
					natural_selectors_[i](temp, new_pop, how_many_should_select(i, natural_selectors_.size(), select_count));
				}
			}
			// Or just pick the best individuals if there is no natural selector
			else
			{
				for (size_t i = 0; i < select_count; ++i)
				{
					new_pop.push_back(temp[i]);
				}
			}

			return new_pop;
		}

		/**
		 * \brief Evolve the population for a given number of generations
		 * \param population The initial population
		 * \param generations How many generations should we evolve
		 * \return The best individual
		 */
		individual_type evolve(TPopulation population, const size_t generations)
		{
			for (size_t i = 0; i < generations; ++i)
			{
				population = evolve(population);

				// Output progress
				if (output_frequency_ != 0 && i % output_frequency_ == 0)
				{
					auto & best = get_best_individual(population);
					auto objective = objective_fnc_ ? objective_fnc_(best) : best.fitness;

					std::cout << "gen: " << i << "; obj: " << objective;
					
					if (individual_to_string_)
					{
						std::cout << "; best ind.: " << individual_to_string_(best);
					}
					
					std::cout << std::endl;
				}
			}

			// Output total results
			auto & total_best = get_best_individual(population);
			auto total_objective = objective_fnc_ ? objective_fnc_(total_best) : total_best.fitness;
			std::cout << "<< FINISHED >> best obj: " << total_objective;

			if (individual_to_string_)
			{
				std::cout << "; best ind.: " << individual_to_string_(total_best);
			}

			std::cout << std::endl;

			// Return the best individual
			return total_best;
		}

		/**
		 * \brief Does multiple runs of the algorithm.
		 * \param generations How many generations should we evolve in each run
		 * \param runs How many runs
		 * \param population_initializer Function that creates the initial population for each run
		 * \return The best individual of each run
		 */
		std::vector<individual_type> evolve_batch(const size_t generations, const size_t runs, population_initializer_type population_initializer)
		{
			std::vector<individual_type> results{};

			for (size_t i = 0; i < runs; ++i)
			{
				auto initial_pop = population_initializer();
				auto best_individual = evolve(initial_pop, generations);
				results.push_back(best_individual);
			}

			std::cout << "<<< Results >>>" << std::endl;

			for (size_t i = 0; i < runs; ++i)
			{
				auto & best = results[i];
				auto best_objective = objective_fnc_ ? objective_fnc_(best) : best.fitness;
				std::cout << "<< Run " << i << " >> best obj: " << best_objective;

				if (individual_to_string_)
				{
					std::cout << "; best ind.: " << individual_to_string_(best);
				}

				std::cout << std::endl;
			}

			return results;
		}
	};
}
#endif // EVOLUTIONARYALGORITHM_H
