#ifndef TREENODEIMPL_H
#define TREENODEIMPL_H

#include "DirectedRootedTree.h"

template <typename T>
const T& DirectedRootedTree<T>::TreeNode::value() const
{
    return m_value;
}

template <typename T>
T& DirectedRootedTree<T>::TreeNode::value()
{
    return m_value;
}

template <typename T>
const typename DirectedRootedTree<T>::TreeNode* DirectedRootedTree<T>::TreeNode::parent() const
{
    return m_parent;
}

template <typename T>
const typename DirectedRootedTree<T>::node_children_t& DirectedRootedTree<T>::TreeNode::children() const
{
    return m_children;
}

template <typename T>
DirectedRootedTree<T>::TreeNode::TreeNode(const T& value, TreeNode* parent)
    : m_value(value),
      m_parent(parent)
{

}

template <typename T>
DirectedRootedTree<T>::TreeNode::TreeNode(T&& value, TreeNode* parent)
    : m_value(std::move(value)),
      m_parent(parent)
{

}

template <typename T>
void DirectedRootedTree<T>::TreeNode::add_child(std::unique_ptr<TreeNode>&& child)
{
    m_children.emplace_back(std::move(child));
}

#endif // TREENODEIMPL_H
