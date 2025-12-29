module;

#include <unordered_map>
#include <iostream>
#include <queue>
#include <numeric>

export module pathfinder_engine;

import calculator;
import file_reader_surgeon;

using namespace std;


export struct Node_Score
{
    double g_score{numeric_limits<double>::infinity()};
    double f_score{numeric_limits<double>::infinity()};
    int parent_ID{-1};
    bool visited{false};

};


using priority_queue_type = std::pair<double,int>;

vector<pair<int,double>> parkour_and_distance_printer(const int& start_vertex_ID, const int& end_vertex_ID,const unordered_map<int, Node_Score>& search_state_map)
{
    int current_parent_vertex_ID = end_vertex_ID;

    vector<pair<int,double>> parcoured_vertices;
    while (search_state_map.at(current_parent_vertex_ID).parent_ID != -1)
    {
        parcoured_vertices.emplace_back(current_parent_vertex_ID,search_state_map.at(current_parent_vertex_ID).g_score);
        //cout << current_parent_vertex_ID << " : Vertex ID || Current True Distance : " << search_state_map.at(current_parent_vertex_ID).g_score << endl;
        current_parent_vertex_ID=search_state_map.at(current_parent_vertex_ID).parent_ID;
    }
    parcoured_vertices.emplace_back(start_vertex_ID,search_state_map.at(start_vertex_ID).g_score);
    reverse(parcoured_vertices.begin(),parcoured_vertices.end());

    for (const auto&[vertex_ID, distance_traveled] : parcoured_vertices)
    {
        //cout << " UNI Visiting this node : " << vertex_ID << " || Distance traveled since start : " << distance_traveled << endl;
    }
    return parcoured_vertices;
}

void neighbor_handler(unordered_map<int, Node_Score>& search_state_map, const unordered_map<int, Vertex_struct>& reference_map, priority_queue<priority_queue_type, vector<priority_queue_type>, greater<>>& open_list, const int& current_vertex_ID, const int& neighbor_vertex_ID, const int& end_vertex_ID, const double& accepted_g)
{
    search_state_map[neighbor_vertex_ID].parent_ID=current_vertex_ID;
    search_state_map[neighbor_vertex_ID].g_score=accepted_g;
    search_state_map[neighbor_vertex_ID].f_score=accepted_g+calculate_heuristic(reference_map.at(neighbor_vertex_ID),reference_map.at(end_vertex_ID));
    open_list.emplace(search_state_map[neighbor_vertex_ID].f_score,neighbor_vertex_ID);
}

void start_vertex_init(unordered_map<int, Node_Score>& search_state_map, const unordered_map<int, Vertex_struct>& reference_map, priority_queue<priority_queue_type,vector<priority_queue_type>,greater<>>& open_list, const int& start_vertex_ID, const int& end_vertex_ID)
{
    search_state_map[start_vertex_ID].g_score=0;
    search_state_map[start_vertex_ID].f_score=search_state_map[start_vertex_ID].g_score+calculate_heuristic(reference_map.at(start_vertex_ID),reference_map.at(end_vertex_ID));
    open_list.emplace(search_state_map[start_vertex_ID].f_score, start_vertex_ID);
}

export void main_finder(const int& start_vertex_ID, const int& end_vertex_ID,  unordered_map<int, Node_Score>& search_state_map, const unordered_map<int, Vertex_struct>& reference_map, vector<pair<int, double>>& target_vector)
{
    /*
    cout << "TESTING HEURISTIC VALUES : " << endl;
    cout << calculate_heuristic(reference_map.at(54047),reference_map.at(58720)) << endl;
    cout << "\n " << endl;
    */

    //CHECK IF BOTH VECTORS ARE INSIDE THE MAP.
    if (!(reference_map.contains(start_vertex_ID) & reference_map.contains(end_vertex_ID)))
    {
        cout << "Did not find : " << start_vertex_ID << " And : " << end_vertex_ID << " In the map" << endl;
        return;
    }
    //IF TRUE WE CONTINUE AND START THE ACTUAL ALGORITHM
    //unordered_map<int, Node_Score> search_state_map;
    priority_queue<priority_queue_type,vector<priority_queue_type>,greater<>> open_list;

    start_vertex_init(search_state_map,reference_map,open_list,start_vertex_ID,end_vertex_ID);
    int other_count = 0;
    while (!open_list.empty())
    {
        const int current_Vertex_ID = open_list.top().second;
        open_list.pop();
        other_count++;
        if (current_Vertex_ID == end_vertex_ID)
        {
            //cout << "Found NODE" << endl;
            break;
        }
        if (search_state_map.at(current_Vertex_ID).visited)
        {
            continue;
        }
        search_state_map.at(current_Vertex_ID).visited = true;

        for (const auto&[neighbor_ID, neighbor_distance] : reference_map.at(current_Vertex_ID).neighbors)
        {
            if (const double tentative_g = search_state_map.at(current_Vertex_ID).g_score+neighbor_distance;
                search_state_map[neighbor_ID].g_score>tentative_g)
            {
                neighbor_handler(search_state_map,reference_map,open_list,current_Vertex_ID,neighbor_ID,end_vertex_ID,tentative_g);
            }
        }
    }
    //cout << other_count << endl;
    target_vector = parkour_and_distance_printer(start_vertex_ID, end_vertex_ID,search_state_map);
}
