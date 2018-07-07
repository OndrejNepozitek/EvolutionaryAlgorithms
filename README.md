# EvolutionaryAlgorithms

A very simple framework for evolutionary algorithms created for my advanced C++ course (2017/2018).

## Table of contents
- [User documentation](#user-documentation)
- [Programmer documentation](#programmer-documentation)

## User documentation

### Installation

Compile the source code in Visual Studio (should probably also work with gcc, but tested only on Windows). Run the application. It has no command line arguments. The `main` method currently contains a showcase of two problems that are solved with evolutionary algorithms.

---

### Features
**Implemented operators**:
- Bit flip mutation (`BitFlipMutation` class)
- Integer mutation (`IntegerMutation` class)
- One point crossover (`OnePtXOver` class)

**Implemented selectors**:
- Rouletter wheel selector (`RouletteWheelSelector` class)
- Tournament selector (`TournamentSelector` class)

**`EvolutionaryAlgorithm` class**:
- Elitism
- Batch evolving - running the algorithm multiple times to compare individual runs
- Different objective and fitness function - the fitness function is used in the evolution process while the objective function is used when displaying results

**Utilities**:
- Easy population initialization (`init_pop_uniform_int` and `init_pop_uniform_bool`)

---

### Examples

We demonstrate how to solve 2 problems using evolutionary algorithm. The first is a Simple genetic algorithm (SGA) where the goal is to evolve a vector that consists only of ones. The second is Bin packing where we have a set of items with different weights and the goal is to divide these items to multiple bins, while minimizing the difference between the smallest and the biggest bin.

We use these two problems to demonstrate basic features of the framework.

---

### Creating a new evolutionary algorithm

#### 1) Population and individuals representation
The first step is to decide how will we represent our individuals and the population. 

For individuals, we provide the `GenericIndividual<typename TContainer, typename TFitness>` class. With this class, we can easily represent any individual that consists of multiple elements. For example, we can use `TContainer = std::vector<int>` to create an individual that consists of multiple integer values, or `TContainer = std::array<bool, 100>` to create an individual that consists of 100 bool values. `TContainer` can be almost any standard container.

For the population, we should be able to use any standard container that provides the `value_type` typedef, allows adding elements (`push_back`) and contains a parameterless constructor. However, we tested only `std::vector<T>` as it is convenient for the majority of scenarios.

#### 2) Compose the algorithm
First, we have to create an instance of the `EvolutionaryAlgorithm` class. This class is parametrized with the type of the population.

To get a simple evolutionary algorithm, we should:
- Add at least one operator (see implemented operators above)
- Add at least one mating selector (see implemented selectors above)
- (optional) Add at least one natural selector (see implemented selectors above)
- Set the fitness function

#### 3) Run the algorithm
The `EvolutionaryAlgorithm` class provides 3 ways to run the algorithm:

- Handle the evolution manually with the `evolve(TPopulation population)` method
- Run the evolution once with the `evolve(TPopulation population, const size_t generations)` method
- Run the evolution multiple times with the `evolve_batch(size_t generations, size_t runs, population_initializer_type population_initializer)` method

For the last way, we must be able to tell the evolutionary algorithm how to create the initial population for each run. For this purpose, we provide the `init_pop_uniform_int` and the `init_pop_uniform_bool` functions.

---

### How the `EvolutionaryAlgorithm` class works
1) Use mating selectors to select a new population
2) Apply operators one by one
3) Use the fitness function to update the fitness value of all individuals in the population
4) If elitism is enabled, pick the best individuals from the parent generation to form the base of the offspring population
5) Use natural selectors to fill in the rest of the offspring population

---

## Programmer documentation

### Creating operators

Operator is anything that can be assigned to `std::function<void(TPopulation &)>` where `TPopulation` is the type of population to operate on.

### Creating selectors

Selector is anything that can be assigned to `std::function<void(const TPopulation &, TPopulation &, size_t)>` where `TPopulation` is the type of population.

Semantics of the parameters:
- `const TPopulation &` - the source population from which we want to select individuals
- `TPopulation &` - the destination population to which we add selected individuals
- `size_t` - how many individuals should we select

### Creating fitness functions

Fitness function is anything that can be assigned to `std::function<void(TPopulation &)>` where `TPopulation` is the type of population. The goal of the fitness function is to update the `fitness` property of each individual in the population.
