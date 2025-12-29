#include <QApplication>
#include "ui/mainwindow.h"
#include "modules/engine/engine.h"

import file_reader_surgeon;
import pathfinder_engine;
import GEOJson_exporter;
import pathfinder_engine_bidirectional;


using namespace std;

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    Engine main_engine("../graph_dc_area.2022-03-11.txt");
    MainWindow w(&main_engine);
    w.show();
    return a.exec();


    /*
    unordered_map<int, Vertex_struct> map;
    unordered_map<int, Node_Score> search_state_map;

    map_initializer("../graph_dc_area.2022-03-11.txt",map);

    main_finder(237003,2,search_state_map, map);

    GEOJson_exporter(2, search_state_map, map);
    */
    /*
    unordered_map<int, Vertex_struct> forward_map;
    unordered_map<int, Node_Score> forward_search_state_map;
    unordered_map<int, Vertex_struct> backward_map;
    unordered_map<int, Node_Score> backward_search_state_map;

    map_initializer_bidirectional("../graph_dc_area.2022-03-11.txt",forward_map,backward_map);

    main_finder_bidirectional(237003,2,forward_search_state_map, backward_search_state_map, forward_map, backward_map);
    */
}
