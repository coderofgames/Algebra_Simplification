
#include "SimpleScript.h"
#include "Utils.h"

/*=================================================================
Genetic Programming 
=================================================================*/

class fitness_sample
{
public:
	vector<double> terminal_values;
	double training_sample;
};

class ASTPopulation : public ASTContainer
{
public:
	ASTPopulation()
	{
		//	ast = new AST();
		//	ast->CreateRoot();

	}
	~ASTPopulation()
	{

	}

	struct terminal_rep
	{
		double value;
		string id;
		bool is_numeric;
	};

	/*void AddTerminal(string term, bool numeric)
	{
	terminal_rep rep;
	rep.id = term;
	rep.is_numeric = numeric;
	terminals.push_back(rep);
	}

	void SetTerminalValue(string term, double value)
	{
	for (int u = 0; u < terminals.size(); u++)
	{
	terminals[u].value = value;
	}
	}*/

	void SetPopulationSize(int size)
	{
		population_size = size;
	}

	void generatePopulation()
	{
		for (int i = 0; i < population_size; i++)
		{
			AST *ast = new AST();
			ast->SetParentObject(this);
			ast->CreateRoot();
			//ast->AddTerminal("x", false);
			//ast->AddTerminal("y", false);
			//ast->AddTerminal("a", false);
			//ast->AddTerminal("3.13", true);
			/*for (int j = 0; j < terminals.size(); j++)
			{
			ast->AddTerminal(terminals[j].id, terminals[j].is_numeric);
			}*/
			ast->CreateTree(ast->root);

			population.push_back(ast);
			ast->PrintTree(ast->root);
			population_values.push_back(0.0);
			/*
			ast->SetTerminalValue("x", 9.8);
			ast->SetTerminalValue("y", 4.5);
			ast->SetTerminalValue("a", 1);
			//			double root_value = ast->root->value();

			cout << "value of calculation with x= " << 9.8 << " and y= " << 4.5 << " and a=" << 1 << "  :==" << root_value;
			delete ast;
			*/
			//cout << endl;
		}
	}

	double current_best_sample = 10000000.0;

	void ComputeValues(int index)
	{
		for (int i = 0; i < population.size(); i++)
		{
			double this_value = population[i]->root->value();
			if (this_value == INFINITY || this_value == -INFINITY){
				population_values[i] += 1000;
				continue;
			}
			population_values[i] += (this_value - terminal_fitness_values[index].training_sample);
			//cout << "This Value: " << this_value << "        ";
			//double compare_val = abs(population_values[i]);
			//if (compare_val < current_best_sample )current_best_sample = compare_val;
		}
	}
	int the_best = 0;
	double current_best_score = 0.0;
	void ComputeFitness()
	{
		double percentage_scale = 1 / current_best_sample;

		for (int i = 0; i < population.size(); i++)
		{
			//population_values[i]
			//population[i]->PrintTree(population[i]->root);
			if (population_values[i] == 0)population[i]->fitness = 100000000;
			else
			{
				double average_difference = population_values[i] / terminal_fitness_values.size();

				double abs_difference = (average_difference*average_difference);
				//double abs_difference_squared = abs_difference * abs_difference;
				//population_values[i] = abs_difference_squared;
				if (abs_difference < 0.00001)
				{
					//if (population[i]->fitness == 0.0)
					//	population[i]->fitness = 1.0;

					population[i]->fitness = 10000;
				}


				if (abs_difference >= 100000000 || abs_difference <= -INFINITY)
				{
					population[i]->fitness = 0;
				}
				else population[i]->fitness = 1 / abs_difference;
			}

			if ((population[i]->fitness != population[i]->fitness))population[i]->fitness = 1;

			sumFitness += population[i]->fitness;
			//cout << "SumFitness: " << sumFitness << " ";
			population[i]->swapped = false;
			if (population[i]->fitness > current_best_score)
			{
				current_best_score = population[i]->fitness;
				the_best = i;
			}
			//cout << "fitness: " << abs_difference_squared << "        ";

			population_values[i] = 0.0f;
		}
	}

	void compute_sample_values()
	{
		the_best = 0;
		current_best_sample = 10000000.0;
		current_best_score = 0.0f;
		for (int i = 0; i < terminal_fitness_values.size(); i++)
		{
			for (int j = 0; j < terminal_fitness_values[i].terminal_values.size(); j++)
			{
				SetTerminalValue(this->terminals[j]->sym, terminal_fitness_values[i].terminal_values[j]);
			}
			ComputeValues(i);

			//			cout << endl;

		}

		ComputeFitness();

		//		sumFitness /= terminal_fitness_values.size();
	}

	int SelectBreedOp()
	{
		int breed_op_sel = RandomInt(0, 10000);
		int cross_thresh = prob_cross_over * 10000;
		int mutate_thresh = prob_mutate * 10000;
		int clone_thresh = prob_clone * 10000;

		int threshold_1 = cross_thresh + mutate_thresh;


		if (breed_op_sel < cross_thresh)
		{
			// cross
			return 0;
		}
		else if (breed_op_sel >= cross_thresh && breed_op_sel < threshold_1)
		{
			// mutate
			return 1;
		}
		else if (breed_op_sel >= threshold_1)
		{
			// clone
			return 2;
		}

		return 0;
	}
	vector<AST*> old_population;
	void Breed()
	{

		for (int i = 0; i < population.size(); i++)
		{
			AST* new_tree = population[i]->CopyTree();
			old_population.push_back(new_tree);
		}


		for (int i = 0; i < population.size(); i++)
		{


			//if (old_population[i]->swapped)continue;

			int breed_op = SelectBreedOp();

			if (i == this->the_best)
			{
				breed_op = 2;
			}
			else if (population[i]->fitness == 0)
			{
				breed_op = 1;
			}


			switch (breed_op)
			{
			case 0:
			{
				int parent_one_selector = ChooseParent();

				//AST* parent_one = population[parent_one_selector]->CopyTree();

				int parent_two_selector = ChooseParent();

				//int parent_one_selector = RandomInt(0, population.size()-1);

				//int parent_two_selector = RandomInt(0, population.size() - 1);

				if (parent_one_selector == parent_two_selector)
				{
					if (parent_one_selector == population.size() - 1)
					{
						parent_two_selector = the_best;
					}
					else
					{
						parent_two_selector = parent_one_selector + 1;
					}
				}

				AST* newNode = this->DoSelectionCrossover(old_population, parent_one_selector, parent_two_selector);
				//this->DoSwapCrossover(parent_one_selector, parent_two_selector);
				delete population[i];
				population[i] = newNode;


				break;
			}
			case 1:
				population[i]->Mutate();

				break;
			case 2:

				// cloning .... lol
				break;

			}


			population_values[i] = 0.0f;
		}

		for (int i = 0; i < old_population.size(); i++)
		{
			delete old_population[i];


			population[i]->fitness = 0.0f;
		}

		old_population.clear();
		sumFitness = 0.0;
	}


	int ChooseParent()
	{
		int randSelector = (int)(RandomFloat(0, sumFitness) * 100);

		// Translate this number to the corresponding member**
		int memberID = 0;
		int partialSum = 0;

		for (int i = 0; i < population.size(); i++) {
			int n = (int)(old_population[i]->fitness * 100);
			if (partialSum < randSelector && partialSum + n >= randSelector)
			{
				return i;
			}
			partialSum += n;

		}

		int alternate = RandomInt(0, 99);

		return this->the_best;

	}


	void SetProbabilities(double cross_over, double mutate, double clone)
	{
		prob_cross_over = cross_over;
		prob_mutate = mutate;
		prob_clone = clone;

	}


	AST* DoSelectionCrossover(vector<AST*> &pool, unsigned int s1, unsigned int s2)
	{

		AST* first_tree = pool[s1]->CopyTree();
		AST* second_tree = pool[s2]->CopyTree();


		ASTNode* cross1 = first_tree->CrossPoint();
		ASTNode* cross2 = second_tree->CrossPoint();

		ASTNode* cross1_p = first_tree->current_crossover_point_parent;
		ASTNode* cross2_p = second_tree->current_crossover_point_parent;

		if (cross1_p == cross1)
		{
			//cout << "first tree swaps root" << endl;
			first_tree->root = (OperatorNode*)cross2;
		}
		else
		{
			//cout << "Performing cross" << endl;
			// is it the left or the right?
			if (cross1 == cross1_p->left)
			{
				cross1_p->left = cross2;
			}
			else
			{
				cross1_p->right = cross2;
			}
		}

		if (cross2_p == cross2)
		{
			//cout << "second tree swaps root" << endl;
			second_tree->root = (OperatorNode*)cross1;
		}
		else
		{
			//cout << "Performing cross" << endl;
			if (cross2 == cross2_p->left)
			{
				cross2_p->left = cross1;
			}
			else
			{
				cross2_p->right = cross1;
			}
		}

		///old_population[s1]->swapped = true;
		//old_population[s2]->swapped = true;
		int rand_selector = RandomInt(0, 100);
		if (rand_selector > 50)
			return first_tree;
		else return second_tree;
		//population[s1]->fitness = 0.0f;
		//population[s2]->fitness = 0.0f;
	}

	void DoSwapCrossover(unsigned int s1, unsigned int s2)
	{

		ASTNode* cross1 = population[s1]->CrossPoint();
		ASTNode* cross2 = population[s2]->CrossPoint();

		ASTNode* cross1_p = population[s1]->current_crossover_point_parent;
		ASTNode* cross2_p = population[s2]->current_crossover_point_parent;

		if (cross1_p == cross1)
		{
			//cout << "first tree swaps root" << endl;
			population[s1]->root = (OperatorNode*)cross2;
		}
		else
		{
			//cout << "Performing cross" << endl;
			// is it the left or the right?
			if (cross1 == cross1_p->left)
			{
				cross1_p->left = cross2;
			}
			else
			{
				cross1_p->right = cross2;
			}
		}

		if (cross2_p == cross2)
		{
			//cout << "second tree swaps root" << endl;
			population[s2]->root = (OperatorNode*)cross1;
		}
		else
		{
			//cout << "Performing cross" << endl;
			if (cross2 == cross2_p->left)
			{
				cross2_p->left = cross1;
			}
			else
			{
				cross2_p->right = cross1;
			}
		}

		population[s1]->swapped = true;
		population[s2]->swapped = true;

		//population[s1]->fitness = 0.0f;
		//population[s2]->fitness = 0.0f;
	}


	double sumFitness = 0.0;

	double prob_cross_over;
	double prob_mutate;
	double prob_clone;

	vector< fitness_sample > terminal_fitness_values;
	//vector<terminal_rep> terminals;
	int population_size = 0;


	vector<AST*> population;
	vector<AST*> copy_population;

	vector<double> population_values;
};