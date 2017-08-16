#include <QApplication>
#include <QTableView>

#include "TreeAlgorithms.h"
#include "SequencingModel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    tree_algorithms::parallel_sequencing_t<std::string> lowerSequencing(4);
    lowerSequencing[0] = std::vector<std::string>({"First", "Second"});
    lowerSequencing[1] = std::vector<std::string>({"Third", "Fourth"});
    lowerSequencing[2] = std::vector<std::string>({"Fifth", "Sixth"});
    lowerSequencing[3] = std::vector<std::string>({"Seventh"});

    tree_algorithms::parallel_sequencing_t<std::string> upperSequencing(4);
    upperSequencing[0] = std::vector<std::string>({"First"});
    upperSequencing[1] = std::vector<std::string>({"Second"});
    upperSequencing[2] = std::vector<std::string>({"Third"});
    upperSequencing[3] = std::vector<std::string>({"Fourth", "Fifth", "Sixth", "Seventh"});

    QTableView view;
    SequencingModel model(lowerSequencing, upperSequencing);

    view.setModel(&model);
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.setDragEnabled(true);
    view.setAcceptDrops(true);
    view.setDropIndicatorShown(true);

    view.show();

    return a.exec();
}
