#include <cmath>
#include <cstdio>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

typedef struct node
{
	int val;
	node* left;
	node* right;
	int ht;
};

int height(node* root)
{
	if (root != nullptr)
		return root->ht;
	return -1;
}

node* LeftRotate(node* root)
{
	node* new_root = root->right;
	node* new_left = new_root->left;

	new_root->left = root;
	root->right = new_left;

	root->ht = std::max(height(root->left), height(root->right)) + 1;
	new_root->ht = std::max(height(new_root->left), height(new_root->right)) + 1;

	return new_root;
}

node* RightRotate(node* root)
{
	node* new_root = root->left;
	node* new_right = new_root->right;

	new_root->right = root;
	root->left = new_right;

	root->ht = std::max(height(root->left), height(root->right)) + 1;
	new_root->ht = std::max(height(new_root->left), height(new_root->right)) + 1;

	return new_root;
}

int GetBalance(node* root)
{
	if (root == nullptr)
		return 0;
	return (height(root->left) - height(root->right));
}

node* Insert(node* root, int val)
{
	if (root == nullptr)
	{
		node* new_node = new node();
		new_node->val = val;
		return new_node;
	}

	if (root->val > val)
		root->left = Insert(root->left, val);
	else if (root->val < val)
		root->right = Insert(root->right, val);
	else
		return root;

	root->ht = 1 + std::max(height(root->left), height(root->right));

	int balance = GetBalance(root);
	if (balance == 2) //left imbalance
	{
		int left_balance = GetBalance(root->left);
		if (left_balance == -1)
		{
			root->left = LeftRotate(root->left);
			return RightRotate(root);
		}
		if (left_balance == 1)
			return RightRotate(root);
	}
	if (balance == -2) //right imbalance
	{
		int right_balance = GetBalance(root->right);
		if (right_balance == 1)
		{
			root->right = RightRotate(root->right);
			return LeftRotate(root);
		}
		if (right_balance == -1)
			return LeftRotate(root);
	}

	return root;
}