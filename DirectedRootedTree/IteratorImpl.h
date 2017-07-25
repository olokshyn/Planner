#ifndef ITERATORIMPL_H
#define ITERATORIMPL_H

#include "DirectedRootedTree.h"

#include <stdexcept>

template <typename T>
DirectedRootedTree<T>::Iterator::Iterator(TreeNode *current_node)
    : m_current_node(current_node)
{
    m_viewed_nodes.insert(m_current_node);
}

template <typename T>
typename DirectedRootedTree<T>::Iterator& DirectedRootedTree<T>::Iterator::operator++()
{
    if (!m_current_node)
    {
        throw std::out_of_range("Iterator does not point to any element");
    }
    const TreeNode* parent_node = m_current_node;
    do
    {
        if (go_to_unviewed_child(parent_node))
        {
            return *this;
        }
        parent_node = parent_node->parent();
    } while (parent_node != nullptr);
    m_current_node = nullptr;
    m_viewed_nodes.clear();
    return *this;
}

template <typename T>
typename DirectedRootedTree<T>::Iterator DirectedRootedTree<T>::Iterator::operator++(int)
{
    Iterator temp(*this);
    operator++();
    return temp;
}

template <typename T>
T& DirectedRootedTree<T>::Iterator::operator*()
{
    if (!m_current_node)
    {
        throw std::out_of_range("Iterator does not point to any element");
    }
    return m_current_node->value();
}

template <typename T>
T* DirectedRootedTree<T>::Iterator::operator->()
{
    if (!m_current_node)
    {
        throw std::out_of_range("Iterator does not point to any element");
    }
    return &m_current_node->value();
}

template <typename T>
typename DirectedRootedTree<T>::TreeNode* DirectedRootedTree<T>::Iterator::current_node()
{
    return m_current_node;
}

template <typename T>
bool DirectedRootedTree<T>::Iterator::go_to_unviewed_child(const TreeNode *parent)
{
    const node_children_t& children = parent->children();
    for (const std::unique_ptr<TreeNode>& child : children)
    {
        if (m_viewed_nodes.find(child.get()) == m_viewed_nodes.end())
        {
            m_current_node = child.get();
            m_viewed_nodes.insert(m_current_node);
            return true;
        }
    }
    return false;
}

#endif // ITERATORIMPL_H
