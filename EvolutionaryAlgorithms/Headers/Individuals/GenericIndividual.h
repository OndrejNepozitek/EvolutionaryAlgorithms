#ifndef GENERICINDIVIDUAL_H
#define GENERICINDIVIDUAL_H

namespace ea
{
	/**
	 * \brief Data structure for a generic individual. Programmers can use almost any available standard container.
	 * \tparam TContainer Type of the used container
	 * \tparam TFitness Type of the fitness value
	 */
	template<typename TContainer, typename TFitness>
	class GenericIndividual
	{
		TContainer container_;

	public:
		using element_type = typename TContainer::value_type;
		using fitness_type = TFitness;
		using container_type = TContainer;

		TFitness fitness{};

		/**
		 * \brief Construct the individual with a given container.
		 * \param container 
		 */
		explicit GenericIndividual(TContainer && container) : container_(std::move(container)) {}

		/**
		 * \brief Operatror[] on the container.
		 * \param index 
		 * \return 
		 */
		element_type & operator[](const size_t index)
		{
			return container_[index];
		}

		/**
		 * \brief Size of the container
		 * \return 
		 */
		size_t size() const
		{
			return container_.size();
		}


		/**
		 * \brief Gets the begin iterator of the container
		 * \return 
		 */
		typename TContainer::iterator begin()
		{
			return std::begin(container_);
		}

		/**
		 * \brief Gets the end iterator of the container.
		 * \return 
		 */
		typename TContainer::iterator end()
		{
			return std::end(container_);
		}
	};

}
#endif // GENERICINDIVIDUAL_H
