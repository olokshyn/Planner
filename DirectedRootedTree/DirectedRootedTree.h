#ifndef DIRECTEDROOTEDTREE_H
#define DIRECTEDROOTEDTREE_H

#include <iterator>
#include <vector>
#include <memory>
#include <unordered_set>

template <typename T>
class DirectedRootedTree
{
    // TODO: Make this class copyable if T is copyable or clonable if T is movable and clonable but not copyable

    // TODO: make parameters const references
    friend bool operator==(DirectedRootedTree& left, DirectedRootedTree& right)
    {
        return equal(left, right);
    }

    // TODO: make parameters const references
    friend bool operator!=(DirectedRootedTree& left, DirectedRootedTree& right)
    {
        return !equal(left, right);
    }

public:
    class TreeNode;

    typedef std::vector< std::unique_ptr<TreeNode> > node_children_t;

    class TreeNode
    {
        friend class DirectedRootedTree;

    public:
        const T& value() const;
        T& value();

        const TreeNode* parent() const;
        TreeNode* parent();

        const node_children_t& children() const;

    private:
        explicit TreeNode(const T& value, TreeNode* parent = nullptr);
        explicit TreeNode(T&& value, TreeNode* parent = nullptr);

        TreeNode* add_child(std::unique_ptr<TreeNode>&& child);

    private:
        T m_value;
        TreeNode* m_parent;
        node_children_t m_children;
    };

    class Iterator : public std::iterator<std::forward_iterator_tag, T, size_t, T*, T&>
    {
        // TODO: add const Iterator

        friend bool operator==(const Iterator& left, const Iterator& right)
        {
            return left.m_current_node == right.m_current_node;
        }

        friend bool operator!=(const Iterator& left, const Iterator& right)
        {
            return left.m_current_node != right.m_current_node;
        }

    public:
        explicit Iterator(TreeNode* current_node = nullptr);

        Iterator& operator++();
        Iterator operator++(int);

        T& operator*();
        T* operator->();

        TreeNode* current_node();

    private:
        bool go_to_unviewed_child(const TreeNode* parent);

    private:
        TreeNode* m_current_node;
        std::unordered_set<TreeNode*> m_viewed_nodes;
    };

public:
    // TODO: make parameters const references
    static bool equal(DirectedRootedTree& left, DirectedRootedTree& right);

public:
    explicit DirectedRootedTree(const T& value);
    explicit DirectedRootedTree(T&& root_value = T());

    const TreeNode* root() const;
    TreeNode* root();

    Iterator begin();
    Iterator end();

    TreeNode* add_child(TreeNode* parent, const T& value);
    TreeNode* add_child(TreeNode* parent, T&& value);
    void remove_node(TreeNode* node);

    size_t size() const;

private:
    std::unique_ptr<TreeNode> m_root;
    size_t m_size;
};


#include "TreeNodeImpl.h"
#include "IteratorImpl.h"
#include "DirectedRootedTreeimpl.h"


#endif // DIRECTEDROOTEDTREE_H
