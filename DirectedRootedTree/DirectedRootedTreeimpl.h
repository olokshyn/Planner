#ifndef DIRECTEDROOTEDTREEIMPL_H
#define DIRECTEDROOTEDTREEIMPL_H

#include "DirectedRootedTree.h"

#include <iterator>
#include <algorithm>
#include <stdexcept>

template <typename T>
bool DirectedRootedTree<T>::equal(DirectedRootedTree<T>& left,
                                  DirectedRootedTree<T>& right)
{
    if (left.size() != right.size())
    {
        return false;
    }
    return std::equal(left.begin(), left.end(), right.begin());
}

template <typename T>
DirectedRootedTree<T>::DirectedRootedTree(const T& value)
    : m_root(new TreeNode(value, nullptr)),
      m_size(1)
{

}

template <typename T>
DirectedRootedTree<T>::DirectedRootedTree(T&& root_value)
    : m_root(new TreeNode(std::move(root_value), nullptr)),
      m_size(1)
{

}

template <typename T>
const typename DirectedRootedTree<T>::TreeNode* DirectedRootedTree<T>::root() const
{
    return m_root.get();
}

template <typename T>
typename DirectedRootedTree<T>::TreeNode* DirectedRootedTree<T>::root()
{
    return m_root.get();
}

template <typename T>
typename DirectedRootedTree<T>::Iterator DirectedRootedTree<T>::begin()
{
    return Iterator(m_root.get());
}

template <typename T>
typename DirectedRootedTree<T>::Iterator DirectedRootedTree<T>::end()
{
    return Iterator(nullptr);
}

template <typename T>
typename DirectedRootedTree<T>::TreeNode* DirectedRootedTree<T>::add_child(TreeNode* parent, const T& value)
{
    TreeNode* child = parent->add_child(std::unique_ptr<TreeNode>(new TreeNode(value, parent)));
    ++m_size;
    return child;
}

template <typename T>
typename DirectedRootedTree<T>::TreeNode* DirectedRootedTree<T>::add_child(TreeNode* parent, T&& value)
{
    TreeNode* child = parent->add_child(std::unique_ptr<TreeNode>(new TreeNode(std::move(value), parent)));
    ++m_size;
    return child;
}

template <typename T>
void DirectedRootedTree<T>::remove_node(TreeNode* node)
{
    if (node == m_root.get())
    {
        throw std::logic_error("Root node cannot be removed");
    }
    TreeNode* parent = node->m_parent;

    typename node_children_t::iterator iter = std::find_if(parent->m_children.begin(),
                                                           parent->m_children.end(),
                                                           [node](const std::unique_ptr<TreeNode>& child) -> bool
    {
        return child.get() == node;
    });
    if (iter == parent->m_children.end())
    {
        throw std::runtime_error("Inconsistent tree: node is not in the children list of it`s parent");
    }
    std::unique_ptr<TreeNode> node_obj = std::move(*iter);  // Make node a scoped object

    for (std::unique_ptr<TreeNode>& child : node->m_children)
    {
        child->m_parent = parent;
    }

    iter = parent->m_children.insert(iter,
                                     std::make_move_iterator(node->m_children.begin()),
                                     std::make_move_iterator(node->m_children.end()));
    iter += node->m_children.size();
    parent->m_children.erase(iter);

    --m_size;
    if (m_size == 0)
    {
        throw std::runtime_error("Inconsistent tree: size is invalid");
    }
}

template <typename T>
size_t DirectedRootedTree<T>::size() const
{
    return m_size;
}

#endif // DIRECTEDROOTEDTREEIMPL_H
