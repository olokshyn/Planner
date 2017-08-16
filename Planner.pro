#-------------------------------------------------
#
# Project created by olokshyn on 2017-07-17 16:48:11
#
#-------------------------------------------------

TEMPLATE = subdirs
SUBDIRS = DirectedRootedTree \
    Test_DirectedRootedTree \
    TreeSerialization \
    Test_TreeSerialization \
    SequencingModel \
    SequencingModelTest

Test_DirectedRootedTree.depends = DirectedRootedTree
Test_TreeSerialization.depends = TreeSerialization
SequencingModelTest.depends = SequencingModel
