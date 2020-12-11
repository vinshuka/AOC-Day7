#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <stack>

//Day7 Part 1
//Given a list of bag rules, where the rules are the type of bag and what bags it can contain, you have a shiny gold bag and you need to find the number of
//bags that will eventually contain a shiny gold bag.
//Example:
//Given the following rules
//light red bags contain 1 bright white bag, 2 muted yellow bags.
//dark orange bags contain 3 bright white bags, 4 muted yellow bags.
//bright white bags contain 1 shiny gold bag.
//muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
//shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
//dark olive bags contain 3 faded blue bags, 4 dotted black bags.
//vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
//faded blue bags contain no other bags.
//dotted black bags contain no other bags.
//The result would be 4 because:
//A bright white bag, which can hold your shiny gold bag directly.
//A muted yellow bag, which can hold your shiny gold bag directly, plus some other bags.
//A dark orange bag, which can hold bright whiteand muted yellow bags, either of which could then hold your shiny gold bag.
//A light red bag, which can hold bright whiteand muted yellow bags, either of which could then hold your shiny gold bag.

//For this first we need to organize our rules, a map can be used for this, the type of bag is the key and the bags it contains are the values, we can
//store them in a vector, so map<string, vector<string>>. First we will build a function to parse each line into our map. Once we have the map, we can
//start with our target bag, shiny gold, we will check for which bags can contain it directly, then we can check which bags can contain those bags 
//and so on, we will add each bag to a list if that list does not already contain that bag, the length of the list will be our answer.

//Okay so we've made a small mistake, we've made the bag the parent and the bags it contain the children but what we actually want is the child and the
//bags that are its parents, this can be fixed easily in our rule parsing

//Day7 Part 2
//Given the rules how many other bags are required to be inside your shiny gold bag.
//Example:
//Shiny gold bags contain 1 dark olive and 2 vibrant plum
//dark olive bags contain 3 faded blue and 4 dotted black
//vibrant plum bags contain 5 faded blue and 6 dotted black
//faded blue contain no other bags
//dotted black contain no other bags
//in total the shiny gold bag will contain 32 other bags in total

//For this problem we'll need to create a new data structure where our bags are in the parent children format, the math needed for the calculation is as 
//follows, bag#1 + (bag#1 * total bags) and so on.
//Using the example above:
//shiny gold have 1 dark olive, so the equation is currently 1 +
//dark olive have 3 faded blue and 4 dotted black which totals 7, so the equation is 1 + (1*7) +
//faded blue and black do not contain any bags so we can stop here and go to the next bag
//shiny gold have 2 vibrant plum, so the equation is now 1 + (1*7) + 2 +
//vibrant plum have 5 faded blue and 6 dotted black, so the equation is now 1 + (1*7) + 2 + (2*11)
//faded blue and dotted black contain no bags
//the resulting calculation is 1 + (1*7) + 2 + (2*11) = 32


std::vector<std::string> test =
{
	"light red bags contain 1 bright white bag, 2 muted yellow bags.",
	"dark orange bags contain 3 bright white bags, 4 muted yellow bags.",
	"bright white bags contain 1 shiny gold bag.",
	"muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.",
	"shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.",
	"dark olive bags contain 3 faded blue bags, 4 dotted black bags.",
	"vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.",
	"faded blue bags contain no other bags.",
	"dotted black bags contain no other bags."
};


std::vector<std::string> getPuzzleInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("puzzle-input.txt");
	int count = 0;
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}

std::vector<std::string> split(const std::string s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (getline(ss, item, delim))
	{
		tokens.push_back(item);
	}

	return tokens;
}

std::pair<std::string, std::vector<std::string>> splitRule(std::string rule)
{
	std::vector<std::string> parts = split(rule, ' ');

	std::string parent = parts[0] + " " + parts[1];

	std::vector<std::string> children;

	
	return std::pair<std::string, std::vector<std::string>>(parent, children);
}

std::map<std::string, std::vector<std::string>> plantTree(std::vector<std::string> input)
{
	std::map<std::string, std::vector<std::string>> tree;
	for (auto i : input)
	{

		std::vector<std::string> parts = split(i, ' ');

		std::string parent = parts[0] + " " + parts[1];


		for (int i = 0; i < parts.size(); i++)
		{

			if (std::all_of(parts[i].begin(), parts[i].end(), ::isdigit))
			{
				std::string child = parts[i + 1] + " " + parts[i + 2];

				std::map<std::string, std::vector<std::string> >::const_iterator it = tree.find(child);

				if (it != tree.end())
				{
					std::vector<std::string> s = it->second;
					if (std::find(s.begin(), s.end(), parent) == s.end())
					{
						tree.at(child).push_back(parent);
					}
				}
				else
				{
					std::vector<std::string> parents = { parent };
					tree.insert(std::pair<std::string, std::vector<std::string>>(child, parents));
				}
			}
		}

	}
	return tree;
}

std::map<std::string, std::map<std::string, int>> growTree(std::vector<std::string> input)
{
	std::map<std::string, std::map<std::string, int>> tree;
	for (auto i : input)
	{

		std::vector<std::string> parts = split(i, ' ');

		std::string parent = parts[0] + " " + parts[1];


		for (int i = 0; i < parts.size(); i++)
		{

			if (std::all_of(parts[i].begin(), parts[i].end(), ::isdigit))
			{
				std::string child = parts[i + 1] + " " + parts[i + 2];

				int number = std::stoi(parts[i]);

				if (tree.find(parent) != tree.end())
				{
					tree.at(parent).insert(std::pair<std::string, int>(child, number));
				}
				else
				{
					std::map<std::string, int> inner;
					inner.insert(std::pair<std::string, int>(child, number));
					tree.insert(std::pair<std::string, std::map<std::string, int>>(parent, inner));
				}
			}
		}

	}
	return tree;
}

void dfs(std::map<std::string, std::vector<std::string>> tree, std::string start)
{
	std::vector<std::string> answer;
	std::stack<std::string> stack;
	stack.push(start);
	std::set<std::string> visited;

	while (stack.size() > 0)
	{
		std::string item = stack.top();
		stack.pop();

		if (visited.find(item) != visited.end())
			continue;
		
		visited.insert(item);

		if (tree.find(item) != tree.end())
		{
			std::vector<std::string> parents = tree.at(item);
			for (auto p : parents)
			{
				stack.push(p);
			}
		}
		else
		{
			answer.push_back(item);
		}
	}
	for (auto a : answer)
	{
		//std::cout << a << '\n';
	}

	//std::cout << visited.size() - 1 << '\n';
}

void dfs2(std::map<std::string, std::map<std::string, int>> tree)
{
	int answer = 0;
	std::stack<std::pair<std::string, int>> stack;
	std::pair<std::string, int> start = std::pair<std::string, int>("shiny gold", 1);
	stack.push(start);

	while (stack.size() > 0)
	{
		std::pair<std::string, int> item = stack.top();
		stack.pop();
		std::string bag = item.first;
		int amount = item.second;

		answer += amount;

		if (tree.find(bag) != tree.end())
		{
			for (auto c : tree.at(bag))
			{
				std::pair<std::string, int> item = std::pair<std::string, int>(c.first, (c.second * amount));
				stack.push(item);
			}
		}
	}

	std::cout << (answer - 1) << '\n';
}


int main()
{
	std::vector<std::string> input = getPuzzleInput();
	std::map<std::string, std::vector<std::string>> tree = plantTree(input);

	//std::map<std::string, std::vector<std::string>> tree = plantTree(test);

	std::map<std::string, std::map<std::string, int>> tree2 = growTree(input);

	dfs2(tree2);

	//for (auto t : tree)
	//{
	//	std::cout << "Child: " << t.first << '\n';

	//	std::cout << "Parents: " << '\n';

	//	for (auto c : t.second)
	//	{
	//		std::cout << c << '\n';
	//	}

	//	std::cout << '\n';
	//}

	//for (auto t : tree2)
	//{
	//	std::cout << "Parent: " << t.first << '\n';

	//	std::cout << "Children: " << '\n';

	//	for (auto c : t.second)
	//	{
	//		std::cout << c.first << ":" << c.second << '\n';
	//	}
	//	std::cout << '\n';
	//}


	//if (tree2.find("faded blue") == tree2.end())
	//{
	//	std::cout << "return 0" << '\n';
	//}

	//dfs(tree, "shiny gold");

	return 0;
}