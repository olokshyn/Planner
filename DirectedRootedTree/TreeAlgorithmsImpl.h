#ifndef TREEALGORITHMSIMPL_H
#define TREEALGORITHMSIMPL_H

#include "TreeAlgorithms.h"

#include <algorithm>
#include <iterator>
#include <list>

template <typename T>
std::vector<T> tree_algorithms::top_leaves(const DirectedRootedTree<T>& tree,
                                           std::vector<typename DirectedRootedTree<T>::TreeNode*>* top_nodes)
{
    std::vector<T> leaves;

    const typename DirectedRootedTree<T>::node_children_t& children = tree.root()->children();
    leaves.reserve(children.size());
    std::transform(children.begin(), children.end(), std::back_inserter(leaves),
                   [](const std::unique_ptr<typename DirectedRootedTree<T>::TreeNode>& child) -> T {
        return child->value();
    });

    if (top_nodes)
    {
        top_nodes->reserve(children.size());
        std::transform(children.begin(), children.end(), std::back_inserter(*top_nodes),
                       [](const std::unique_ptr<typename DirectedRootedTree<T>::TreeNode>& child) -> typename DirectedRootedTree<T>::TreeNode* {
            return child.get();
        });
    }

    return leaves;
}

template <typename T>
std::vector<T> tree_algorithms::bottom_leaves(DirectedRootedTree<T>& tree,
                                              std::vector<typename DirectedRootedTree<T>::TreeNode*>* bottom_nodes)
{
    std::list<T> leaves;
    std::list<typename DirectedRootedTree<T>::TreeNode*> nodes;

    typename DirectedRootedTree<T>::Iterator iter = tree.begin();
    while (iter != tree.end())
    {
        if (iter.current_node()->children().empty())
        {
            leaves.push_back(*iter);
            if (bottom_nodes)
            {
                nodes.push_back(iter.current_node());
            }
        }
        ++iter;
    }
    if (bottom_nodes)
    {
        bottom_nodes->assign(nodes.begin(), nodes.end());
    }
    return std::vector<T>(leaves.begin(), leaves.end());
}

template <typename T>
tree_algorithms::parallel_sequencing_t<T> tree_algorithms::lower_parallel_sequencing(DirectedRootedTree<T>& tree)
{
    parallel_sequencing_t<T> parallel_sequencing;

    while (!tree.root()->children().empty())
    {
        std::vector<typename DirectedRootedTree<T>::TreeNode*> top_nodes;
        parallel_sequencing.push_back(top_leaves(tree, &top_nodes));

        for (typename DirectedRootedTree<T>::TreeNode* child : top_nodes)
        {
            tree.remove_node(child);
        }
    }

    return parallel_sequencing;
}

template <typename T>
tree_algorithms::parallel_sequencing_t<T> tree_algorithms::upper_parallel_sequencing(DirectedRootedTree<T>& tree)
{
    parallel_sequencing_t<T> parallel_sequencing;

    while (!tree.root()->children().empty())
    {
        std::vector<typename DirectedRootedTree<T>::TreeNode*> bottom_nodes;
        parallel_sequencing.push_back(bottom_leaves(tree, &bottom_nodes));

        for (typename DirectedRootedTree<T>::TreeNode* child : bottom_nodes)
        {
            tree.remove_node(child);
        }
    }

    return parallel_sequencing;
}

#endif // TREEALGORITHMSIMPL_H
