#include <QString>
#include <QtTest>

#include "DirectedRootedTree.h"
#include "TreeAlgorithms.h"

#define ASSERT_THROWS(EXPR, EXCEPTION, FAIL_MSG) \
    try \
    { \
        EXPR; \
        QFAIL(FAIL_MSG); \
    } \
    catch (const EXCEPTION&) \
    { \
    } \
    catch (const std::exception&) \
    { \
        QFAIL("Caught invalid exception"); \
    }

class DirectedRootedTreeTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void create_data();
    void create();

    void add_one_node_data();
    void add_one_node();

    void add_several_nodes_data();
    void add_several_nodes();

    void add_nodes_on_two_layers_data();
    void add_nodes_on_two_layers();

    void iterator_prefix_inc_data();
    void iterator_prefix_inc();

    void iterator_postfix_inc_data();
    void iterator_postfix_inc();

    void equal_data();
    void equal();

    void not_equal_data();
    void not_equal();

    void remove_node();

    void algo_top_leaves();
    void algo_bottom_leaves();

    void algo_lower_parallel_sequencing();
    void algo_upper_parallel_sequencing();

private:
    DirectedRootedTree<int> build_multilayer_tree(std::vector<int>& nodes_values_depth_first) const;

    template <typename T>
    bool is_tree_consistent(const DirectedRootedTree<T>& tree) const;

    template <typename T>
    bool is_node_consistent(const typename DirectedRootedTree<T>::TreeNode* node) const;
};

void DirectedRootedTreeTest::create_data()
{
    QTest::addColumn<float>("value");

    QTest::newRow("0") << 0.0f;
    QTest::newRow("1") << 1.0f;
    QTest::newRow("2") << -1.0f;
    QTest::newRow("3") << 2.3f;
    QTest::newRow("4") << -2.3f;
}

void DirectedRootedTreeTest::create()
{
    QFETCH(float, value);

    DirectedRootedTree<float> tree(value);
    QCOMPARE(tree.size(), 1ul);

    const DirectedRootedTree<float>::TreeNode* root = tree.root();
    QCOMPARE(root->value(), value);

    const DirectedRootedTree<float>::node_children_t& children = root->children();
    QCOMPARE(children.size(), 0ul);
}

void DirectedRootedTreeTest::add_one_node_data()
{
    QTest::addColumn<int>("rootValue");
    QTest::addColumn<int>("value");

    QTest::newRow("0") << 0 << 0;
    QTest::newRow("1") << -1 << 1;
    QTest::newRow("2") << 1 << -1;
}

void DirectedRootedTreeTest::add_one_node()
{
    QFETCH(int, rootValue);
    QFETCH(int, value);

    DirectedRootedTree<int> tree(rootValue);
    DirectedRootedTree<int>::TreeNode* root = tree.root();
    tree.add_child(root, value);
    QCOMPARE(tree.size(), 2ul);

    const DirectedRootedTree<int>::node_children_t& children = root->children();
    QCOMPARE(children.size(), 1ul);
    QCOMPARE(children[0]->value(), value);
}

void DirectedRootedTreeTest::add_several_nodes_data()
{
    QTest::addColumn<bool>("value1");
    QTest::addColumn<bool>("value2");
    QTest::addColumn<bool>("value3");

    QTest::newRow("0") << false << false << false;
    QTest::newRow("1") << false << false << true;
    QTest::newRow("2") << false << true << false;
    QTest::newRow("3") << false << true << true;
    QTest::newRow("4") << true << false << false;
    QTest::newRow("5") << true << false << true;
    QTest::newRow("6") << true << true << false;
    QTest::newRow("7") << true << true << true;
}

void DirectedRootedTreeTest::add_several_nodes()
{
    QFETCH(bool, value1);
    QFETCH(bool, value2);
    QFETCH(bool, value3);

    DirectedRootedTree<bool> tree(true);
    DirectedRootedTree<bool>::TreeNode* root = tree.root();
    tree.add_child(root, value1);
    tree.add_child(root, value2);
    tree.add_child(root, value3);
    QCOMPARE(tree.size(), 4ul);

    const DirectedRootedTree<bool>::node_children_t& children = root->children();
    QCOMPARE(children.size(), 3ul);
    QCOMPARE(children[0]->value(), value1);
    QCOMPARE(children[1]->value(), value2);
    QCOMPARE(children[2]->value(), value3);
}

void DirectedRootedTreeTest::add_nodes_on_two_layers_data()
{
    QTest::addColumn<int>("a1");
    QTest::addColumn<int>("a2");
    QTest::addColumn<int>("a3");
    QTest::addColumn<int>("a11");
    QTest::addColumn<int>("a12");
    QTest::addColumn<int>("a21");
    QTest::addColumn<int>("a31");
    QTest::addColumn<int>("a32");
    QTest::addColumn<int>("a33");

    QTest::newRow("0") << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    QTest::newRow("1") << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1;
    QTest::newRow("2") << 1 << 0 << 1 << 0 << 1 << 0 << 1 << 0 << 1;
}

void DirectedRootedTreeTest::add_nodes_on_two_layers()
{
    QFETCH(int, a1);
    QFETCH(int, a2);
    QFETCH(int, a3);
    QFETCH(int, a11);
    QFETCH(int, a12);
    QFETCH(int, a21);
    QFETCH(int, a31);
    QFETCH(int, a32);
    QFETCH(int, a33);

    DirectedRootedTree<int> tree(0);
    DirectedRootedTree<int>::TreeNode* root = tree.root();
    tree.add_child(root, a1);
    tree.add_child(root, a2);
    tree.add_child(root, a3);

    const DirectedRootedTree<int>::node_children_t& children = root->children();
    QCOMPARE(children.size(), 3ul);
    QCOMPARE(children[0]->value(), a1);
    QCOMPARE(children[1]->value(), a2);
    QCOMPARE(children[2]->value(), a3);

    DirectedRootedTree<int>::TreeNode* a1_node = children[0].get();
    tree.add_child(a1_node, a11);
    tree.add_child(a1_node, a12);

    DirectedRootedTree<int>::TreeNode* a2_node = children[1].get();
    tree.add_child(a2_node, a21);

    DirectedRootedTree<int>::TreeNode* a3_node = children[2].get();
    tree.add_child(a3_node, a31);
    tree.add_child(a3_node, a32);
    tree.add_child(a3_node, a33);

    QCOMPARE(tree.size(), 10ul);

    const DirectedRootedTree<int>::node_children_t& a1_children = a1_node->children();
    QCOMPARE(a1_children.size(), 2ul);
    QCOMPARE(a1_children[0]->value(), a11);
    QCOMPARE(a1_children[1]->value(), a12);

    const DirectedRootedTree<int>::node_children_t& a2_children = a2_node->children();
    QCOMPARE(a2_children.size(), 1ul);
    QCOMPARE(a2_children[0]->value(), a21);

    const DirectedRootedTree<int>::node_children_t& a3_children = a3_node->children();
    QCOMPARE(a3_children.size(), 3ul);
    QCOMPARE(a3_children[0]->value(), a31);
    QCOMPARE(a3_children[1]->value(), a32);
    QCOMPARE(a3_children[2]->value(), a33);
}

void DirectedRootedTreeTest::iterator_prefix_inc_data()
{
    QTest::addColumn<int>("inc_count");

    for (int i = 0; i != 22; ++i)
    {
        QTest::newRow(std::to_string(i).c_str()) << i;
    }
}

void DirectedRootedTreeTest::iterator_prefix_inc()
{
    QFETCH(int, inc_count);

    std::vector<int> nodes_values;
    DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

    DirectedRootedTree<int>::Iterator iter = tree.begin();
    QCOMPARE(*iter, nodes_values[0]);
    for (int i = 1; i <= inc_count; ++i)
    {
        if (static_cast<size_t>(i) < nodes_values.size())
        {
            QCOMPARE(*++iter, nodes_values[i]);
        }
        else if (static_cast<size_t>(i) == nodes_values.size())
        {
            ++iter;
            ASSERT_THROWS(*iter, std::out_of_range,
                          "out_of_range exception must be thrown on accessing out of range interator")

            QCOMPARE(iter, tree.end());
        }
        else
        {
            ASSERT_THROWS(++iter, std::out_of_range,
                          "out_of_range exception must be thrown on incrementing out of range interator");
        }
    }
}

void DirectedRootedTreeTest::iterator_postfix_inc_data()
{
    QTest::addColumn<int>("inc_count");

    for (int i = 0; i != 22; ++i)
    {
        QTest::newRow(std::to_string(i).c_str()) << i;
    }
}

void DirectedRootedTreeTest::iterator_postfix_inc()
{
    QFETCH(int, inc_count);

    std::vector<int> nodes_values;
    DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

    DirectedRootedTree<int>::Iterator iter = tree.begin();
    for (int i = 0; i != inc_count; ++i)
    {
        if (static_cast<size_t>(i) < nodes_values.size())
        {
            QCOMPARE(*iter++, nodes_values[i]);
        }
        else if (static_cast<size_t>(i) == nodes_values.size())
        {
            ASSERT_THROWS(iter++, std::out_of_range,
                          "out_of_range exception must be thrown on incrementing out of range interator");
            ASSERT_THROWS(*iter, std::out_of_range,
                          "out_of_range exception must be thrown on accessing out of range interator")

            QCOMPARE(iter, tree.end());
        }
    }
}

void DirectedRootedTreeTest::equal_data()
{
    QTest::addColumn< std::vector<int> >("left_tree_data");
    QTest::addColumn< std::vector<int> >("right_tree_data");

    QTest::newRow("0") << std::vector<int>() << std::vector<int>();
    QTest::newRow("1") << std::vector<int>({1}) << std::vector<int>({1});
    QTest::newRow("2") << std::vector<int>({2, 2}) << std::vector<int>({2, 2});
    QTest::newRow("3") << std::vector<int>({3, 3, 3}) << std::vector<int>({3, 3, 3});
    QTest::newRow("4") << std::vector<int>({4, 4, 4, 4}) << std::vector<int>({4, 4, 4, 4});
}

void DirectedRootedTreeTest::equal()
{
    QFETCH(std::vector<int>, left_tree_data);
    QFETCH(std::vector<int>, right_tree_data);

    if (left_tree_data != right_tree_data)
    {
        QFAIL("data vectors must be equal");
    }

    DirectedRootedTree<int> left_tree;
    DirectedRootedTree<int> right_tree;

    if (!left_tree_data.empty())
    {
        left_tree.root()->value() = left_tree_data.front();
    }
    if (!right_tree_data.empty())
    {
        right_tree.root()->value() = right_tree_data.front();
    }

    for (size_t i = 1; i < left_tree_data.size(); ++i)
    {
        left_tree.add_child(left_tree.root(), left_tree_data[i]);
        right_tree.add_child(right_tree.root(), right_tree_data[i]);
    }

    QVERIFY(left_tree == right_tree);
}

void DirectedRootedTreeTest::not_equal_data()
{
    QTest::addColumn< std::vector<int> >("left_tree_data");
    QTest::addColumn< std::vector<int> >("right_tree_data");

    QTest::newRow("0") << std::vector<int>() << std::vector<int>({1});
    QTest::newRow("1") << std::vector<int>({1}) << std::vector<int>({2});
    QTest::newRow("2") << std::vector<int>({2, 2}) << std::vector<int>({2, 2, 2});
    QTest::newRow("3") << std::vector<int>({3, 4, 3}) << std::vector<int>({3, 3});
    QTest::newRow("4") << std::vector<int>({4, 4, 4, 4, 5}) << std::vector<int>({4, 4, 4, 4});
}

void DirectedRootedTreeTest::not_equal()
{
    QFETCH(std::vector<int>, left_tree_data);
    QFETCH(std::vector<int>, right_tree_data);

    if (left_tree_data == right_tree_data)
    {
        QFAIL("data vectors must not be equal");
    }

    DirectedRootedTree<int> left_tree;
    DirectedRootedTree<int> right_tree;

    if (!left_tree_data.empty())
    {
        left_tree.root()->value() = left_tree_data.front();
    }
    if (!right_tree_data.empty())
    {
        right_tree.root()->value() = right_tree_data.front();
    }

    for (size_t i = 1; i < left_tree_data.size(); ++i)
    {
        left_tree.add_child(left_tree.root(), left_tree_data[i]);
    }

    for (size_t i = 1; i < right_tree_data.size(); ++i)
    {
        right_tree.add_child(right_tree.root(), right_tree_data[i]);
    }

    QVERIFY(left_tree != right_tree);
}

void DirectedRootedTreeTest::remove_node()
{
    std::vector<int> nodes_values;
    DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

    tree.remove_node(tree.root()->children()[1].get());
    QVERIFY(is_tree_consistent(tree));
    QCOMPARE(tree.root()->children().size(), 4ul);

    nodes_values.erase(nodes_values.begin() + 4);
    QCOMPARE(tree.size(), nodes_values.size());
    QVERIFY(std::equal(tree.begin(), tree.end(), nodes_values.begin()));

    tree.remove_node(tree.root()->children()[3].get());
    QVERIFY(is_tree_consistent(tree));
    QCOMPARE(tree.root()->children().size(), 6ul);
    nodes_values.erase(nodes_values.begin() + 9);
    QCOMPARE(tree.size(), nodes_values.size());
    QVERIFY(std::equal(tree.begin(), tree.end(), nodes_values.begin()));
}

void DirectedRootedTreeTest::algo_top_leaves()
{
    {
        std::vector<int> nodes_values;
        DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

        std::vector<int> top_leaves_actual = tree_algorithms::top_leaves(tree);
        std::vector<int> top_leaves_expected = { nodes_values[1], nodes_values[4], nodes_values[10] };
        QCOMPARE(top_leaves_actual, top_leaves_expected);
    }

    {
        std::vector<int> nodes_values;
        DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);
        std::vector<DirectedRootedTree<int>::TreeNode*> top_nodes;

        std::vector<int> top_leaves_actual = tree_algorithms::top_leaves(tree, &top_nodes);
        std::vector<int> top_leaves_expected = { nodes_values[1], nodes_values[4], nodes_values[10] };
        QCOMPARE(top_leaves_actual, top_leaves_expected);

        QCOMPARE(top_nodes.size(), top_leaves_expected.size());
        QVERIFY(std::equal(top_nodes.begin(), top_nodes.end(), top_leaves_expected.begin(),
                           [](DirectedRootedTree<int>::TreeNode* node, int leaf) -> bool {
                    return node->value() == leaf;
                }));
    }
}

void DirectedRootedTreeTest::algo_bottom_leaves()
{
    {
        std::vector<int> nodes_values;
        DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

        std::vector<int> bottom_leaves_actual = tree_algorithms::bottom_leaves(tree);
        std::vector<int> bottom_leaves_expected = { nodes_values[3], nodes_values[6], nodes_values[8],
                                                    nodes_values[9], nodes_values[12], nodes_values[14],
                                                    nodes_values[15], nodes_values[17], nodes_values[18],
                                                    nodes_values[19] };
        QCOMPARE(bottom_leaves_actual, bottom_leaves_expected);
    }

    {
        std::vector<int> nodes_values;
        DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);
        std::vector<DirectedRootedTree<int>::TreeNode*> bottom_nodes;

        std::vector<int> bottom_leaves_actual = tree_algorithms::bottom_leaves(tree, &bottom_nodes);
        std::vector<int> bottom_leaves_expected = { nodes_values[3], nodes_values[6], nodes_values[8],
                                                    nodes_values[9], nodes_values[12], nodes_values[14],
                                                    nodes_values[15], nodes_values[17], nodes_values[18],
                                                    nodes_values[19] };
        QCOMPARE(bottom_leaves_actual, bottom_leaves_expected);

        QCOMPARE(bottom_nodes.size(), bottom_leaves_expected.size());
        QVERIFY(std::equal(bottom_nodes.begin(), bottom_nodes.end(), bottom_leaves_expected.begin(),
                           [](DirectedRootedTree<int>::TreeNode* node, int leaf) -> bool {
                    return node->value() == leaf;
                }));
    }
}

void DirectedRootedTreeTest::algo_lower_parallel_sequencing()
{
    std::vector<int> nodes_values;
    DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

    tree_algorithms::parallel_sequencing_t<int> parallel_sequencing_actual = tree_algorithms::lower_parallel_sequencing(tree);
    tree_algorithms::parallel_sequencing_t<int> parallel_sequencing_expected = {
        { nodes_values[1], nodes_values[4], nodes_values[10] },
        { nodes_values[2], nodes_values[5], nodes_values[7], nodes_values[11], nodes_values[13], nodes_values[16] },
        { nodes_values[3], nodes_values[6], nodes_values[8], nodes_values[9], nodes_values[12],
          nodes_values[14], nodes_values[15], nodes_values[17], nodes_values[18], nodes_values[19] }
    };
    QCOMPARE(parallel_sequencing_actual, parallel_sequencing_expected);
}

void DirectedRootedTreeTest::algo_upper_parallel_sequencing()
{
    // TODO: create the test tree for which lower and upper parallel sequencings will be different

    std::vector<int> nodes_values;
    DirectedRootedTree<int> tree = build_multilayer_tree(nodes_values);

    tree_algorithms::parallel_sequencing_t<int> parallel_sequencing_actual = tree_algorithms::upper_parallel_sequencing(tree);
    tree_algorithms::parallel_sequencing_t<int> parallel_sequencing_expected = {
        { nodes_values[3], nodes_values[6], nodes_values[8], nodes_values[9], nodes_values[12],
          nodes_values[14], nodes_values[15], nodes_values[17], nodes_values[18], nodes_values[19] },
        { nodes_values[2], nodes_values[5], nodes_values[7], nodes_values[11], nodes_values[13], nodes_values[16] },
        { nodes_values[1], nodes_values[4], nodes_values[10] }
    };
    QCOMPARE(parallel_sequencing_actual, parallel_sequencing_expected);
}

DirectedRootedTree<int> DirectedRootedTreeTest::build_multilayer_tree(std::vector<int>& nodes_values_depth_first) const
{
    DirectedRootedTree<int> tree;
    for (int i = 1; i <= 3; ++i)
    {
        tree.add_child(tree.root(), i);
    }
    for (const auto& child : tree.root()->children())
    {
        for (int i = 1; i <= child->value(); ++i)
        {
            tree.add_child(child.get(), i);
        }
        for (const auto& grandson : child->children())
        {
            for (int i = 0; i != grandson->value(); ++i)
            {
                tree.add_child(grandson.get(), 0);
            }
        }
    }
    nodes_values_depth_first = { 0,
        1,
            1,
                0,
        2,
            1,
                0,
            2,
                0,
                0,
        3,
            1,
                0,
            2,
                0,
                0,
            3,
                0,
                0,
                0

    };
    if (tree.size() != nodes_values_depth_first.size())
    {
        throw std::runtime_error("Invalid tree");
    }
    return tree;
}

template <typename T>
bool DirectedRootedTreeTest::is_tree_consistent(const DirectedRootedTree<T>& tree) const
{
    auto root = tree.root();
    if (!root || root->parent())
    {
        return false;
    }

    for (const auto& child : root->children())
    {
        if (!is_node_consistent<T>(child.get()))
        {
            return false;
        }
    }
    return true;
}

template <typename T>
bool DirectedRootedTreeTest::is_node_consistent(const typename DirectedRootedTree<T>::TreeNode* node) const
{
    for (const auto& child : node->children())
    {
        if (child->parent() != node)
        {
            return false;
        }
        if (!is_node_consistent<T>(child.get()))
        {
            return false;
        }
    }
    return true;
}

QTEST_APPLESS_MAIN(DirectedRootedTreeTest)

#include "DirectedRootedTreeTest.moc"
