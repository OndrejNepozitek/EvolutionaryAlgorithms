#ifndef INITIALIZER_H
#define INITIALIZER_H

namespace container_creator
{
	/**
	 * \brief Helper class for initializing containers
	 * \tparam TContainer Type of a container that should be created
	 */
	template<typename TContainer>
	struct ContainerCreator
	{
		/**
		 * \brief Creates a container with a given number of elements.
		 * \param length
		 * \return New container with a given length
		 */
		static TContainer create_container(const size_t length)
		{
			return TContainer(length);
		}
	};


	/**
	 * \brief Partial specialization of ContainerCreator to handle arrays.
	 * \tparam TValue Type of elements in the array
	 * \tparam Size Size of the array
	 */
	template<typename TValue, size_t Size>
	struct ContainerCreator<std::array<TValue, Size>>
	{
		
		/**
		 * \brief Creates an array
		 * \return 
		 */
		static std::array<TValue, Size> create_container(const size_t)
		{
			return std::array<TValue, Size>();
		}
	};
}

namespace ea
{
	/**
	 * \brief Creates a random initial population with individuals consisting of uniformly distributed integers.
	 * \tparam TPopulation Type of the population that will be created
	 * \param min The lower bound of random integers. Inclusive. 
	 * \param max The upper bound of random integers. Inclusive.
	 * \param individuals_count How many individual do we want in the population.
	 * \param individual_size How many elements should every individual contain.
	 * \return Random initial population.
	 */
	template<typename TPopulation>
	TPopulation init_pop_uniform_int(const int min, const int max, const size_t individuals_count, const size_t individual_size)
	{
		// TPopulation must contain a value_type typedef
		using individual_type = typename TPopulation::value_type;

		// individual_type must contain a container_type typedef
		using container_type = typename individual_type::container_type;

		TPopulation pop{};
		const std::uniform_int_distribution<> int_distribution(min, max);

		for (size_t i = 0; i < individuals_count; ++i)
		{
			// Create container
			container_type vec = container_creator::ContainerCreator<container_type>::create_container(individual_size);

			// Fill the container with random numbers from a given range
			for (size_t j = 0; j < individual_size; ++j)
			{
				vec[j] = int_distribution(ea::rng_gen());
			}

			// Create the individual by moving the container
			pop.emplace_back(std::move(vec));
		}

		return pop;
	}

	/**
	* \brief Creates a random initial population with individuals consisting of uniformly distributed booleans.
	* \tparam TPopulation Type of the population that will be created
	* \param individuals_count How many individual do we want in the population.
	* \param individual_size How many elements should every individual contain.
	* \return Random initial population.
	*/
	template<typename TPopulation>
	TPopulation init_pop_uniform_bool(const size_t individuals_count, const size_t individual_size)
	{
		return init_pop_uniform_int<TPopulation>(0, 1, individuals_count, individual_size);
	}
}

#endif // INITIALIZER_H
