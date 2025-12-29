#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QTimer>
#include <vector>
#include <string>


using namespace std;
class Engine
{
    public:
    Engine(const string& filepath);
    ~Engine();

    vector<pair<int,double>> full_on_pathfinder(const int& start_vector, const int& end_vector) const;
    pair<double,double> getPointCoordinates(const int& nodeID) const;
    vector<int> reference_map_IDs_getter();
    vector<int> return_neighbors(const int& vector_ID) const;

    private:
        struct EngineData;
        unique_ptr<EngineData> m_data;
};

#endif //ASTAR_ENGINE_H