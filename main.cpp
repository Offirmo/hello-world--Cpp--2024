#include <iostream>
#include <cassert>
#include <cmath>
#include <functional>
#include <sstream>
#include <exception>

/////////////////////////////////////////////////
namespace Ruler {

	struct Ruler {
		std::string name{"??"};
		std::string title{"??"};
		int rule_begin{-10000};
		int rule_end{-10000};
	};

	std::string to_string(const Ruler &ruler) {
		std::ostringstream result;
		result << "[Ruler]{"
		       << ruler.title << " "
		       << ruler.name << ": "
		       << ruler.rule_begin << " - "
		       << ruler.rule_end << "}";

		return result.str();
	}

	void print(const Ruler &ruler) {
		std::cout << to_string(ruler) << std::endl;
	}

	int compare(const Ruler &a, const Ruler &b) {
		return a.rule_begin - b.rule_begin;
	}

	int compare_year(const int year, const Ruler &r) {
		if (year < r.rule_begin)
			return -1;
		if (year > r.rule_end)
			return 1;

		return 0;
	}
}

/////////////////////////////////////////////////
namespace Tree {
	enum class BinaryTreeDirection {
		left, right
	};
	enum class RBTreeColor {
		red, black
	};

	template<typename P>
	struct BinaryTreeNode {
		P payload{};

		BinaryTreeNode *left{};
		BinaryTreeNode *right{};

		// optional
		BinaryTreeNode *parent{};

		// meta
		RBTreeColor color{RBTreeColor::red}; // red-black tree
		int balance_factor{0}; // AVL tree
	};

	// some algos require a constant pointer to tree
	// where they can change the root (ex. due to rotation)
	template<typename P>
	struct Tree {
		BinaryTreeNode<P> *root{};
	};

	enum DepthFirstTreeTraversalOrder {
		pre, in, post
	};

	template<typename P>
	void traverse_depth_first(
		const BinaryTreeNode<P> *node,
		std::function<void(const BinaryTreeNode<P> *node, int depth)> callback,
		const DepthFirstTreeTraversalOrder order,
		int depth = 0 // helper for display, for convenience only
	) {
		if (!node)
			return;

		if (order == pre) callback(node, depth);
		traverse_depth_first(node->left, callback, order, depth + 1);
		if (order == in) callback(node, depth);
		traverse_depth_first(node->right, callback, order, depth + 1);
		if (order == post) callback(node, depth);
	}

	/* TODO
template <typename P>
void traverse_breadth_first(const BinaryTreeNode<P>* node, std::function<void(const BinaryTreeNode<P>* node, int depth)> callback, const DepthFirstTreeTraversalOrder order, int depth = 0) {
  if (!node)
	  return;

  xxx
}*/

	// TODO not found?
	template<typename P, typename T = P>
	P search(const BinaryTreeNode<P> *node, const T target, std::function<int(const T a, const P b)> compare) {
		const P payload = node->payload;
		const auto comp = compare(target, payload);
		if (comp == 0)
			return payload;

		if (comp < 0)
			return search(node->left, target, compare);

		return search(node->right, target, compare);
	}

	template<typename P>
	BinaryTreeNode<P>* get_child(BinaryTreeNode<P> &node, BinaryTreeDirection dir) {
		return (dir == BinaryTreeDirection::right)
		       ? node.right
		       : node.left;
	}

	template<typename P>
	BinaryTreeNode<P>* get_uncle(BinaryTreeNode<P> &node) {
		const auto parent = node.parent;
		if (!parent)
			return nullptr;

		return (parent->left == node)
		       ? parent->right
		       : parent->left;
	}

	// https://en.wikipedia.org/wiki/Tree_rotation
	template<typename P>
	void rotate(BinaryTreeNode<P> &node, BinaryTreeDirection dir, Tree<P>* tree = nullptr) {
		auto root = &node;
		auto grand_parent = root->parent;

		BinaryTreeNode<P>* pivot;

		if (dir == BinaryTreeDirection::right) {
			pivot = root->left;
			if (!pivot) throw std::invalid_argument("node to rotate has no pivot!");
			root->left = pivot->right;
			if (root->left) root->left->parent = root;
			pivot->right = root;
		}
		else {
			pivot = root->right;
			if (!pivot) throw std::invalid_argument("node to rotate has no pivot!");
			root->right = pivot->left;
			if (root->right) root->right->parent = root;
			pivot->left = root;
		}
		root->parent = pivot;

		pivot->parent = grand_parent;
		if (grand_parent) {
			if (grand_parent->left == root)
				grand_parent->left = pivot;
			else
				grand_parent->right = pivot;
		}

		if (tree && tree->root == root)
			tree->root = pivot;
	}
/*
	template<typename P>
	void RBinsert(Tree<P> &tree, BinaryTreeNode<P> *parent, BinaryTreeNode<P> *node, BinaryTreeDirection dir) {
		BinaryTreeNode<P> *grand_parent{};
		BinaryTreeNode<P> *uncle{};

		node->parent = parent;
		if (!parent) {
			tree.root = node;
			return;
		}

		if (dir == BinaryTreeDirection::right) {
			parent->right = node;
		} else {
			parent->left = node;
		}

		// 1. Every node is either red or black.
		// 2. All NULL nodes are considered black.
		// 3. A red node does not have a red child.
		// 4. Every path from a given node to any of its descendant NULL nodes goes through the same number of black nodes.
		// 5. (Conclusion) If a node N has exactly one child, it must be a red child, because if it were black, its NIL descendants would sit at a different black depth than N's NIL child, violating requirement 4.

		do {
			if (parent->color == RBTreeColor::black) {
				// i1
				// all good
				return;
			}

			// parent is red
			if (!grand_parent) {
				goto i4;
			}


		}
		const auto comp = Ruler::compare(*parent->payload, ruler);
		assert(comp != 0);

		if (comp < 0) {
			parent->right = add_ruler_01(parent->right, ruler);
		} else {
			parent->left = add_ruler_01(parent->left, ruler);
		}

		return parent;
	}
*/
}

/////////////////////////////////////////////////

using NodeRulerPayload = const Ruler::Ruler *;
using NodeRuler = Tree::BinaryTreeNode<NodeRulerPayload>;

std::string node_to_string(const NodeRuler *node, bool recursive = false, int depth = 0) {
	std::ostringstream result;

	if (!node) {
		result << "⚫️NULL";
	} else {
		const auto ruler = node->payload;

		/*if (recursive && (node->left || node->right)) {
			std::string indent(depth * 4, ' ');
			result << std::endl << indent << "↱ L " << node_to_string(node->left, recursive, depth + 1);
		}*/

		result << ((node->color == Tree::RBTreeColor::red) ? "🔴" : "⚫️");
		result << Ruler::to_string(*ruler);

		if (recursive && (node->left || node->right)) {
			std::string indent(depth * 4, ' ');
			result << std::endl << indent << "↱ L " << node_to_string(node->left, recursive, depth + 1);
			result << std::endl << indent << "↳ R " << node_to_string(node->right, recursive, depth + 1);
		}
	}

	return result.str();
}

void print_tree(const NodeRuler *node) {
	std::cout << node_to_string(node, true) << std::endl;
}

void print_node(const NodeRuler *node, int depth = 0) {
	std::cout << node_to_string(node, false, depth) << std::endl;
}

NodeRuler *add_ruler(NodeRuler *node, const Ruler::Ruler &ruler) {
	// v1
	// build a binary search tree
	// ensure strict total order
	// BUT does not balance
	// tree quality is dependent on the order of insertion
	// and can lead to degeneracy

	if (!node)
		return new NodeRuler{.payload=&ruler};

	const auto comp = Ruler::compare(*node->payload, ruler);
	assert(comp != 0);

	if (comp < 0) {
		node->right = add_ruler(node->right, ruler);
	} else {
		node->left = add_ruler(node->left, ruler);
	}

	return node;
}


/*
NodeRuler* add_ruler(NodeRuler* parent, const Ruler::Ruler &ruler) {
	// red-black

	const auto new_node = new NodeRuler{.payload{&ruler},.parent{parent},.is_red{true}};

	if (!parent)
		return new_node;

	const auto comp = Ruler::compare(*parent->payload, ruler);
	assert(comp != 0);

	if (comp < 0) {
		parent->right = add_ruler_01(parent->right, ruler);
	}
	else {
		parent->left = add_ruler_01(parent->left, ruler);
	}

	return parent;
}*/

/////////////////////////////////////////////////

int main() {
	std::cout << "Hello, world!" << std::endl;

	const Ruler::Ruler CLOVIS_I{.title{"king"}, .name{"Clovis I"}, .rule_begin=509, .rule_end=511};
	const Ruler::Ruler VARIOUS_FRANKS{.title{"kings"}, .name{"frankish"}, .rule_begin=511, .rule_end=675};
	//const Ruler::Ruler CHLODOMER{     .title{"king"}, .name{"Childebert I"},.rule_begin=511, .rule_end=558};
	const Ruler::Ruler CHARLES_II{.title{"king"}, .name{"Charles II"}, .rule_begin=843, .rule_end=877};
	const Ruler::Ruler LOUIS_II{.title{"king"}, .name{"Louis II"}, .rule_begin=877, .rule_end=879};
	//const Ruler::Ruler LOUIS_III{     .title{"king"}, .name{"Louis III"}, .rule_begin=879, .rule_end=882};
	const Ruler::Ruler CARLOMAN_II{.title{"king"}, .name{"Carloman II"}, .rule_begin=879, .rule_end=884};
	const Ruler::Ruler CHARLES_IIIa{.title{"king"}, .name{"Charles III the fat"}, .rule_begin=884, .rule_end=887};
	const Ruler::Ruler ODO{.title{"king"}, .name{"Odo"}, .rule_begin=888, .rule_end=898};
	const Ruler::Ruler CHARLES_IIIb{.title{"king"}, .name{"Charles III the simple"}, .rule_begin=898, .rule_end=922};
	const Ruler::Ruler ROBERT_I{.title{"king"}, .name{"Robert I"}, .rule_begin=922, .rule_end=923};
	const Ruler::Ruler RUDOLPH{.title{"king"}, .name{"Rudolph"}, .rule_begin=923, .rule_end=936};

	//print_ruler(CLOVIS_I);
	//print_ruler(THEUDERIC_I);
	//print_ruler(CHARLES_II);


	NodeRuler *root_node = nullptr;
	if (true) {
		// insert in reign order = degenerate root_node (if not self-balancing)
		root_node = add_ruler(root_node, CLOVIS_I);
		root_node = add_ruler(root_node, VARIOUS_FRANKS);
		root_node = add_ruler(root_node, CHARLES_II);
		root_node = add_ruler(root_node, LOUIS_II);
		root_node = add_ruler(root_node, CARLOMAN_II);
		root_node = add_ruler(root_node, CHARLES_IIIa);
		root_node = add_ruler(root_node, ODO);
		root_node = add_ruler(root_node, CHARLES_IIIb);
		root_node = add_ruler(root_node, ROBERT_I);
		root_node = add_ruler(root_node, RUDOLPH);
	} else {
		// insert in alphabetical order, not reign order
		root_node = add_ruler(root_node, CARLOMAN_II);
		root_node = add_ruler(root_node, CHARLES_II);
		root_node = add_ruler(root_node, CHARLES_IIIa);
		root_node = add_ruler(root_node, CHARLES_IIIb);
		root_node = add_ruler(root_node, CLOVIS_I);
		root_node = add_ruler(root_node, LOUIS_II);
		root_node = add_ruler(root_node, ODO);
		root_node = add_ruler(root_node, ROBERT_I);
		root_node = add_ruler(root_node, RUDOLPH);
		root_node = add_ruler(root_node, VARIOUS_FRANKS);
	}

	print_tree(root_node);

	const auto year = 921;
	const auto ruler = search<NodeRulerPayload, int>(root_node, year, [](auto year, auto ruler) -> int {
		return compare_year(year, *ruler);
	});
	std::cout << "SEARCH: for year=" << year << " found ruler: " << Ruler::to_string(*ruler) << std::endl;

	Tree::Tree<NodeRulerPayload> tree{.root=root_node};
	Tree::rotate<NodeRulerPayload>(*tree.root, Tree::BinaryTreeDirection::left, &tree);
	Tree::rotate<NodeRulerPayload>(*tree.root, Tree::BinaryTreeDirection::left, &tree);
	print_tree(tree.root);

	/*std::cout << "pre" << std::endl;
	traverse_depth_first<NodeRulerPayload>(root_node, [](const NodeRuler *node, int depth) { print_node(*node); }, pre);
	std::cout << "in" << std::endl;
	traverse<NodeRulerPayload>(root_node, [](BinaryTreeNode<Ruler::Ruler>* node, int depth) { print_node(*node); }, in);
	std::cout << "post" << std::endl;
	traverse_depth_first<NodeRulerPayload>(root_node, [](const NodeRuler* node, int depth) {
		 std::string indent(depth, '.');
		 std::cout << indent << ((!node) ? "NULL" : node_to_string(*node)) << std::endl;
	}, post);*/
	//print_node(*root_node);
	//print_ruler(root_node.payload);
	//const Ruler *ruler = get_ruler_during_year(521);
	//print_ruler(*ruler);

	return 0;
}
