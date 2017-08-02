#ifndef TREESERIALIZATION_H
#define TREESERIALIZATION_H

#include <iostream>
#include <unordered_map>

#include "DirectedRootedTree.h"

namespace TreeSerialization
{

// TODO: use const reference
// TODO: change function signature to return std::ostream
template <typename T>
void write_tree(std::ostream& stream,
                DirectedRootedTree<T>& tree
                /*!< Non-const reference because there is no const Iterator in DirectedRootedTree */);

// TODO: change function signature to return std::istream
template <typename T>
DirectedRootedTree<T> read_tree(std::istream& stream);

}

template <typename T>
void TreeSerialization::write_tree(std::ostream& stream, DirectedRootedTree<T>& tree)
{
    stream.exceptions(stream.exceptions() | std::ios_base::failbit | std::ios_base::badbit);

    typename DirectedRootedTree<T>::Iterator iter = tree.begin();

    size_t current_node_index = 0;
    std::unordered_map<typename DirectedRootedTree<T>::TreeNode*, size_t> nodes_indexes(tree.size());
    nodes_indexes[nullptr] = 0;

    while (iter != tree.end())
    {
        stream << nodes_indexes[iter.current_node()->parent()] << " " << *iter << std::endl;
        nodes_indexes[iter++.current_node()] = current_node_index++;
    }
}

template <typename T>
DirectedRootedTree<T> TreeSerialization::read_tree(std::istream& stream)
{
    DirectedRootedTree<T> tree;
    typename DirectedRootedTree<T>::TreeNode* current_parent_node = nullptr;

    std::unordered_map<size_t, typename DirectedRootedTree<T>::TreeNode*> indexed_nodes;

    size_t current_node_index = 0;
    size_t parent_node_index;
    T value;
    while (stream >> parent_node_index >> value)
    {
        if (!current_parent_node)
        {
            tree.root()->value() = value;
            current_parent_node = tree.root();
        }
        else
        {
            while (current_parent_node != indexed_nodes[parent_node_index])
            {
                current_parent_node = current_parent_node->parent();
            }
            if (!current_parent_node)
            {
                throw std::runtime_error("Inconsistent tree: specified parent not found");
            }
            current_parent_node = tree.add_child(current_parent_node, value);
        }
        indexed_nodes[current_node_index++] = current_parent_node;
    }
    return tree;
}

#endif // TREESERIALIZATION_H
