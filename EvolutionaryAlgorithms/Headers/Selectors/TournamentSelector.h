#ifndef TOURNAMENTSELECTOR_H
#define TOURNAMENTSELECTOR_H
#include <random>

namespace ea
{
	/**
	* \brief Class implementing the Tournament selection
	* \tparam TPopulation Type of the population
	* \remarks https://en.wikipedia.org/wiki/Tournament_selection
	*/
	template<typename TPopulation>
	class TournamentSelector
	{
		using t_individual = typename TPopulation::value_type;
		using t_fitness = typename t_individual::fitness_type;

		std::uniform_real_distribution<> real_distribution_;

	public:
		/**
		* \brief Uses the Tournament selection to select a given number of individuals
		* \param from The source of individuals to be selected.
		* \param to The destination of selected individuals.
		* \param count How many individuals should be selected.
		*/
		void operator()(const TPopulation& from, TPopulation& to, const size_t count)
		{
			const std::uniform_int_distribution<int> int_distribution(0, from.size() - 1);

			for (auto i = 0; i < count; ++i)
			{
				auto i1 = from[int_distribution(ea::rng_gen())];
				auto i2 = from[int_distribution(ea::rng_gen())];

				if (i1.fitness > i2.fitness && real_distribution_(ea::rng_gen()) < 0.8)
				{
					to.push_back(i1);
				}
				else
				{
					to.push_back(i2);
				}
			}
		}
	};
}
#endif // TOURNAMENTSELECTOR_H
