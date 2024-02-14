#include <iostream>
#include <cassert>
#include <cmath>
#include <functional>

enum Color {
	 red, white, blue
};

namespace Ruler {

	 struct State {
		  std::string name{"??"};
		  std::string title{"??"};
		  int rule_begin{-10000};
		  int rule_end{-10000};
	 };

	 void print(const State &state) {
		 std::cout << "[Ruler]{"
		           << state.title << " "
		           << state.name << ": "
		           << state.rule_begin << " - "
		           << state.rule_end << "}" << std::endl;
	 }

	 int compare(const State &a, const State&b) {
		 return a.rule_begin - b.rule_begin;
	 }
}




template <typename P>
struct Node {
	 P payload{};
	 Node *left{};
	 Node *right{};
};

enum DepthFirstTreeTraversalOrder {
	 pre, in, post
};

template <typename P>
void traverse(Node<P>* node, std::function<void(Node<P>* node, int depth)> callback, const DepthFirstTreeTraversalOrder order, int depth = 0) {
	if (!node)
		return;

	if (order == pre) callback(node, depth);
	traverse(node->left, callback, order, depth + 1);
	if (order == in) callback(node, depth);
	traverse(node->right, callback, order, depth + 1);
	if (order == post) callback(node, depth);
}



void print_node(const Node<Ruler::State> &node, int depth = 0) {

	const auto ruler = node.payload;
	Ruler::print(ruler);
/*
	if (node.left) {
		print_node(*node.left, depth + 1);
	}
	if (node.right) {
		print_node(*node.right, depth + 1);
	}*/
}

Node<Ruler::State>* add_ruler(Node<Ruler::State>* node, const Ruler::State &ruler) {
	if (!node)
		return new Node<Ruler::State>{.payload{ruler}};

	const auto comp = Ruler::compare(node->payload, ruler);
	assert(comp != 0);

	if (comp < 0) {
		node->left = add_ruler(node->left, ruler);
	}
	else {
		node->right = add_ruler(node->right, ruler);
	}

	return node;
}

int main() {
	std::cout << "Hello, world!" << std::endl;

	const Ruler::State CLOVIS_I{   .title{"king"}, .name{"Clovis I"},    .rule_begin=509, .rule_end=511};
	const Ruler::State THEUDERIC_I{.title{"king"}, .name{"Theuderic I"}, .rule_begin=511, .rule_end=534};
	const Ruler::State CHARLES_II{ .title{"king"}, .name{"Charles II"},  .rule_begin=843, .rule_end=877};
	const Ruler::State LOUIS_II{   .title{"king"}, .name{"Louis II"},    .rule_begin=877, .rule_end=879};
	const Ruler::State LOUIS_III{  .title{"king"}, .name{"Louis III"},   .rule_begin=879, .rule_end=882};

	//print_ruler(CLOVIS_I);
	//print_ruler(THEUDERIC_I);
	//print_ruler(CHARLES_II);


	Node<Ruler::State>* tree = nullptr;
	// intentionally insert them in alphabetical order, not reign order
	tree = add_ruler(tree, CHARLES_II);
	tree = add_ruler(tree, CLOVIS_I);
	tree = add_ruler(tree, LOUIS_II);
	tree = add_ruler(tree, LOUIS_III);
	tree = add_ruler(tree, THEUDERIC_I);

	std::cout << "pre" << std::endl;
	traverse<Ruler::State>(tree, [](Node<Ruler::State>* node, int depth) { print_node(*node); }, pre);
	std::cout << "in" << std::endl;
	traverse<Ruler::State>(tree, [](Node<Ruler::State>* node, int depth) { print_node(*node); }, in);
	std::cout << "post" << std::endl;
	traverse<Ruler::State>(tree, [](Node<Ruler::State>* node, int depth) { print_node(*node); }, post);
	//print_node(*tree);
	//print_ruler(tree.payload);
	//const Ruler *ruler = get_ruler_during_year(521);
	//print_ruler(*ruler);

	return 0;
}
