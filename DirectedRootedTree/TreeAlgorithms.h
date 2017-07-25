#ifndef TREEALGORITHMS_H
#define TREEALGORITHMS_H

#include <vector>

#include "DirectedRootedTree.h"

namespace tree_algorithms
{

template <typename T>
std::vector<T> top_leaves(const DirectedRootedTree<T>& tree,
                          std::vector<typename DirectedRootedTree<T>::TreeNode*>* top_nodes = nullptr);

// TODO: make tree a const reference by implementing const DirectedRootedTree<T>::Iterator
template <typename T>
std::vector<T> bottom_leaves(DirectedRootedTree<T>& tree
                             /*!< Non-const because const Iterator is not implemented yet */,
                             std::vector<typename DirectedRootedTree<T>::TreeNode*>* bottom_nodes = nullptr);

template <typename T>
using parallel_sequencing_t = std::vector< std::vector<T> >;

// TODO: Copy tree instead of changing the original one
template <typename T>
parallel_sequencing_t<T> lower_parallel_sequencing(DirectedRootedTree<T>& tree);

// TODO: Copy tree instead of changing the original one
template <typename T>
parallel_sequencing_t<T> upper_parallel_sequencing(DirectedRootedTree<T>& tree);

}

#include "TreeAlgorithmsImpl.h"

#endif // TREEALGORITHMS_H
