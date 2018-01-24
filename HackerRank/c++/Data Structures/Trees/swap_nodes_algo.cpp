#include <cmath>
#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

struct node
{
	int data;
	node* left;
	node* right;
};

void InorderTraversal(node* root)
{
	if (root != nullptr)
	{
		InorderTraversal(root->left);
		std::cout << root->data << " ";
		InorderTraversal(root->right);
	}
}

void SwapNodes(node* root, int nodeLevel)
{
	int current_level = 1;
	std::queue<node*> current_queue;
	current_queue.push(root);
	while (!current_queue.empty())
	{
		std::queue<node*> next_queue;
		while (!current_queue.empty())
		{
			node* current_node = current_queue.front();
			current_queue.pop();

			if (current_level % nodeLevel == 0)
			{
				node* prev_left = current_node->left;
				current_node->left = current_node->right;
				current_node->right = prev_left;
			}

			if (current_node->left != nullptr)
				next_queue.push(current_node->left);
			if (current_node->right != nullptr)
				next_queue.push(current_node->right);
		}
		current_queue = next_queue;
		current_level++;
	}
}

int SwapNodesAlgo()
{
	std::queue<node*> node_queue;
	int num_nodes, left_node, right_node, num_swaps;
	node* top = new node();
	top->data = 1;
	node_queue.push(top);

	std::cin >> num_nodes;
	for (int i = 0; i < num_nodes; i++)
	{
		std::cin >> left_node >> right_node;
		node* current = node_queue.front();
		node_queue.pop();
		if (left_node > 0)
		{
			current->left = new node();
			current->left->data = left_node;
			node_queue.push(current->left);
		}
		else
			current->left = nullptr;

		if (right_node > 0)
		{
			current->right = new node();
			current->right->data = right_node;
			node_queue.push(current->right);
		}
		else
			current->right = nullptr;
	}

	std::cin >> num_swaps;
	std::vector<int> swap_levels;
	for (int i = 0; i < num_swaps; i++)
	{
		int node_level;
		std::cin >> node_level;
		swap_levels.push_back(node_level);
	}

	for (int i = 0; i < num_swaps; i++)
	{
		SwapNodes(top, swap_levels[i]);
		InorderTraversal(top);
		std::cout << std::endl;
	}

	return 0;
}
