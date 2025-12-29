#include "engine.h"
#include <unordered_map>
#include <iostream>
#include <QElapsedTimer>

import file_reader_surgeon;
import pathfinder_engine;

struct Engine::EngineData
{
    unordered_map<int, Vertex_struct> main_map;
};

Engine::Engine(const string& filepath)
{
    m_data = make_unique<EngineData>();

    QElapsedTimer main_timer;
    main_timer.start();

    map_initializer(filepath,m_data->main_map);

    qint64 time_passed = main_timer.elapsed();

    cout << "Engine created" << endl;
    cout << "Time elapsed in its creation : " << time_passed << endl;
}

Engine::~Engine() = default;

vector<pair<int, double>> Engine::full_on_pathfinder(const int& start_vector, const int& end_vector) const
{
    unordered_map<int, Node_Score> search_state_map;
    vector<pair<int, double>> target_vector;

    main_finder(start_vector,end_vector,search_state_map,m_data->main_map,target_vector);

    return target_vector;
}

pair<double,double> Engine::getPointCoordinates(const int& nodeID) const
{

    return make_pair(m_data->main_map.at(nodeID).longitude,m_data->main_map.at(nodeID).latitude);
}
vector<int> Engine::return_neighbors(const int& vector_ID) const
{
    vector<int> neighbors_ID_vector;
    for (auto &[neighbor_ID, distance_for_neighbor] : m_data->main_map.at(vector_ID).neighbors)
    {
        neighbors_ID_vector.push_back(neighbor_ID);
    }
    return neighbors_ID_vector;
}


vector<int> Engine::reference_map_IDs_getter()
{
    vector<int> ID_vector;
    for (auto kv : m_data->main_map)
    {
        ID_vector.push_back(kv.first);
    }
    return ID_vector;
}
