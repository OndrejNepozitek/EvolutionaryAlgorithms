#pragma once
#include <functional>
#include <vector>
#include <algorithm>

namespace ea
{
	template<typename TPopulation>
	class EvolutionaryAlgorithm
	{
		using TOperator = std::function<void(TPopulation &)>;
		using TSelector = std::function<void(TPopulation &, TPopulation &, int)>;
		using TFitness = std::function<void(TPopulation &)>;

		std::vector<TOperator> operators_;
		std::vector<TSelector> mating_selectors_;
		std::vector<TSelector> natural_selectors_;
		TFitness fitness_;

		bool elitism_enabled_ = false;
		double elitism_percentage_ = 0;

	public:
		void add_operator(TOperator op);

		void add_mating_selector(TSelector selector);

		void add_natural_selector(TSelector selector);

		void set_fitness(TFitness fitness);

		void set_elitism(double percentage);

		TPopulation evolve(TPopulation population);
	};

	template <typename TPopulation>
	void EvolutionaryAlgorithm<TPopulation>::add_operator(TOperator op)
	{
		operators_.push_back(op);
	}

	template <typename TPopulation>
	void EvolutionaryAlgorithm<TPopulation>::add_mating_selector(TSelector selector)
	{
		mating_selectors_.push_back(selector);
	}

	template <typename TPopulation>
	void EvolutionaryAlgorithm<TPopulation>::add_natural_selector(TSelector selector)
	{
		natural_selectors_.push_back(selector);
	}

	template <typename TPopulation>
	void EvolutionaryAlgorithm<TPopulation>::set_fitness(TFitness fitness)
	{
		this->fitness_ = fitness;
	}

	template <typename TPopulation>
	void EvolutionaryAlgorithm<TPopulation>::set_elitism(const double percentage)
	{
		if (percentage > 1 || percentage < 0)
		{
			throw std::invalid_argument("Percentage must be less than 1 and greater than 0");
		}

		elitism_enabled_ = true;
		elitism_percentage_ = percentage;
	}

	template <typename TPopulation>
	TPopulation EvolutionaryAlgorithm<TPopulation>::evolve(TPopulation population)
	{
		TPopulation new_pop{};

		for (auto&& mating_selector : mating_selectors_)
		{
			mating_selector(population, new_pop, population.size());
		}

		for (auto&& op : operators_)
		{
			op(new_pop);
		}

		fitness_(new_pop);

		auto temp = std::move(new_pop);
		auto select_count = population.size();
		new_pop = TPopulation{};

		if (elitism_enabled_)
		{
			std::sort(population.begin(), population.end(), [](auto i1, auto i2) { return i1.fitness > i2.fitness; });
			const int elite_count = std::ceil(population.size() * elitism_percentage_);

			for (auto i = 0; i < elite_count; ++i)
			{
				new_pop.push_back(population[i]);
			}

			select_count -= elite_count;
		}

		if (!natural_selectors_.empty())
		{
			for (auto&& natural_selector : natural_selectors_)
			{
				natural_selector(temp, new_pop, select_count);
			}
		}
		else
		{
			for (auto i = 0; i < select_count; ++i)
			{
				new_pop.push_back(temp[i]);
			}
		}

		return new_pop;
	}
}
