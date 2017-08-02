#include <QString>
#include <QtTest>

#include <vector>
#include <string>
#include <sstream>

#include "DirectedRootedTree.h"
#include "TreeSerialization.h"

class TreeSerializationTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void write_empty_tree_data();
    void write_empty_tree();

    void write_flat_tree_data();
    void write_flat_tree();

    void write_balanced_tree_data();
    void write_balanced_tree();

    void write_unbalanced_tree_data();
    void write_unbalanced_tree();

    void read_empty_tree_data();
    void read_empty_tree();

    void read_flat_tree_data();
    void read_flat_tree();

    void read_balanced_tree_data();
    void read_balanced_tree();

    void read_unbalanced_tree_data();
    void read_unbalanced_tree();

private:
    template <typename T>
    DirectedRootedTree<T> build_empty_tree(const std::vector<T>& data,
                                           std::string& serialized);

    template <typename T>
    DirectedRootedTree<T> build_flat_tree(const std::vector<T>& data,
                                          std::string& serialized);

    template <typename T>
    DirectedRootedTree<T> build_balanced_tree(const std::vector<T>& data,
                                              std::string& serialized);

    template <typename T>
    DirectedRootedTree<T> build_unbalanced_tree(const std::vector<T>& data,
                                                std::string& serialized);
};

void TreeSerializationTest::write_empty_tree_data()
{
    QTest::addColumn< std::vector<int> >("data");

    QTest::newRow("0") << std::vector<int>({0});
    QTest::newRow("1") << std::vector<int>({1});
}

void TreeSerializationTest::write_empty_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_empty_tree(data, serialized);

    std::stringstream stream;
    TreeSerialization::write_tree(stream, tree);
    QCOMPARE(serialized, stream.str());
}

void TreeSerializationTest::write_flat_tree_data()
{
    QTest::addColumn< std::vector<int> >("data");

    QTest::newRow("0") << std::vector<int>({0, 1, 2});
    QTest::newRow("1") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7});
    QTest::newRow("2") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
}

void TreeSerializationTest::write_flat_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_flat_tree(data, serialized);

    std::stringstream stream;
    TreeSerialization::write_tree(stream, tree);
    QCOMPARE(serialized, stream.str());
}

void TreeSerializationTest::write_balanced_tree_data()
{
    QTest::addColumn< std::vector<int> >("data");

    QTest::newRow("0") << std::vector<int>({0, 1, 2});
    QTest::newRow("1") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7});
    QTest::newRow("2") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    QTest::newRow("3") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    QTest::newRow("4") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
}

void TreeSerializationTest::write_balanced_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_balanced_tree(data, serialized);

    std::stringstream stream;
    TreeSerialization::write_tree(stream, tree);
    QCOMPARE(serialized, stream.str());
}

void TreeSerializationTest::write_unbalanced_tree_data()
{
    QTest::addColumn< std::vector<int> >("data");

    QTest::newRow("0") << std::vector<int>({0, 1, 2});
    QTest::newRow("1") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7});
    QTest::newRow("2") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    QTest::newRow("3") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    QTest::newRow("4") << std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
}

void TreeSerializationTest::write_unbalanced_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_unbalanced_tree(data, serialized);

    std::stringstream stream;
    TreeSerialization::write_tree(stream, tree);
    QCOMPARE(serialized, stream.str());
}

void TreeSerializationTest::read_empty_tree_data()
{
    write_empty_tree_data();
}

void TreeSerializationTest::read_empty_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_empty_tree(data, serialized);

    std::stringstream stream(serialized);
    DirectedRootedTree<int> read_tree = TreeSerialization::read_tree<int>(stream);
    QVERIFY(tree == read_tree);
}

void TreeSerializationTest::read_flat_tree_data()
{
    write_flat_tree_data();
}

void TreeSerializationTest::read_flat_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_flat_tree(data, serialized);

    std::stringstream stream(serialized);
    DirectedRootedTree<int> read_tree = TreeSerialization::read_tree<int>(stream);
    QVERIFY(tree == read_tree);
}

void TreeSerializationTest::read_balanced_tree_data()
{
    write_balanced_tree_data();
}

void TreeSerializationTest::read_balanced_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_balanced_tree(data, serialized);

    std::stringstream stream(serialized);
    DirectedRootedTree<int> read_tree = TreeSerialization::read_tree<int>(stream);
    QVERIFY(tree == read_tree);
}

void TreeSerializationTest::read_unbalanced_tree_data()
{
    write_unbalanced_tree_data();
}

void TreeSerializationTest::read_unbalanced_tree()
{
    QFETCH(std::vector<int>, data);

    std::string serialized;
    DirectedRootedTree<int> tree = build_unbalanced_tree(data, serialized);

    std::stringstream stream(serialized);
    DirectedRootedTree<int> read_tree = TreeSerialization::read_tree<int>(stream);
    QVERIFY(tree == read_tree);
}

template <typename T>
DirectedRootedTree<T>
TreeSerializationTest::build_empty_tree(const std::vector<T>& data,
                                        std::string& serialized)
{
    if (data.size() != 1ul)
    {
        throw std::runtime_error("data must contain only one value");
    }
    return build_flat_tree(data, serialized);
}

template <typename T>
DirectedRootedTree<T>
TreeSerializationTest::build_flat_tree(const std::vector<T>& data,
                                       std::string& serialized)
{
    if (data.empty())
    {
        throw std::runtime_error("data must not be empty");
    }

    DirectedRootedTree<T> tree(data.front());
    std::stringstream stream;
    stream << static_cast<size_t>(0) << " " << data.front() << std::endl;

    for (size_t i = 1; i != data.size(); ++i)
    {
        tree.add_child(tree.root(), data[i]);
        stream << static_cast<size_t>(0) << " " << data[i] << std::endl;
    }
    serialized = stream.str();
    return tree;
}

template <typename T>
DirectedRootedTree<T>
TreeSerializationTest::build_balanced_tree(const std::vector<T>& data,
                                           std::string& serialized)
{
    if (data.empty())
    {
        throw std::runtime_error("data must not be empty");
    }

    DirectedRootedTree<T> tree(data.front());
    std::stringstream stream;
    stream << static_cast<size_t>(0) << " " << data.front() << std::endl;

    for (size_t i = 1; i < data.size(); i += 2)
    {
        typename DirectedRootedTree<T>::TreeNode* node = nullptr;
        node = tree.add_child(tree.root(), data[i]);
        stream << static_cast<size_t>(0) << " " << data[i] << std::endl;
        if (i < data.size() - 1)
        {
            tree.add_child(node, data[i + 1]);
            stream << i << " " << data[i + 1] << std::endl;
        }
    }
    serialized = stream.str();
    return tree;
}

template <typename T>
DirectedRootedTree<T>
TreeSerializationTest::build_unbalanced_tree(const std::vector<T>& data,
                                             std::string& serialized)
{
    if (data.size() < 3)
    {
        throw std::runtime_error("data must contain at least 3 elements");
    }

    DirectedRootedTree<T> tree(data.front());
    std::stringstream stream;
    stream << static_cast<size_t>(0) << " " << data.front() << std::endl;

    for (size_t i = 1; i != data.size() / 3; ++i)
    {
        tree.add_child(tree.root(), data[i]);
        stream << static_cast<size_t>(0) << " " << data[i] << std::endl;
    }

    typename DirectedRootedTree<T>::TreeNode* node = tree.add_child(tree.root(), data[data.size() / 3]);
    stream << static_cast<size_t>(0) << " " << data[data.size() / 3] << std::endl;
    for (size_t i = data.size() / 3 + 1; i != 2 * data.size() / 3; ++i)
    {
        node = tree.add_child(node, data[i]);
        stream << i - 1 << " " << data[i] << std::endl;
    }

    for (size_t i = 2 * data.size() / 3; i != data.size(); ++i)
    {
        tree.add_child(tree.root(), data[i]);
        stream << static_cast<size_t>(0) << " " << data[i] << std::endl;
    }

    serialized = stream.str();
    return tree;
}

QTEST_APPLESS_MAIN(TreeSerializationTest)

#include "TreeSerializationTest.moc"
