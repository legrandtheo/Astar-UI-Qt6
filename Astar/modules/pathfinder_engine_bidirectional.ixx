module;

#include <unordered_map>
#include <iostream>
#include <queue>
#include <numeric>

export module pathfinder_engine_bidirectional;

import file_reader_surgeon;
import calculator;
import pathfinder_engine;

using namespace std;

using priority_queue_type = std::pair<double,int>;


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

void parkour_and_distance_printer_bidirectional(const int& start_vertex_ID, const int& end_vertex_ID, const int& meeting_vertex_ID, const unordered_map<int, Node_Score>& forward_state_map, const unordered_map<int, Node_Score>& backward_state_map)
{
    int current_parent_vertex_ID = meeting_vertex_ID;
    //double meeting_g_score = forward_state_map.at(current_parent_vertex_ID).g_score;
    vector<pair<int,double>> parcoured_forward_vertices;
    vector<pair<int,double>> parcoured_backward_vertices;

    while (forward_state_map.at(current_parent_vertex_ID).parent_ID != -1)
    {
        parcoured_forward_vertices.emplace_back(current_parent_vertex_ID,forward_state_map.at(current_parent_vertex_ID).g_score);
        //cout << current_parent_vertex_ID << " : Vertex ID || Current True Distance : " << forward_state_map.at(current_parent_vertex_ID).g_score << endl;
        current_parent_vertex_ID=forward_state_map.at(current_parent_vertex_ID).parent_ID;
    }
    parcoured_forward_vertices.emplace_back(start_vertex_ID,forward_state_map.at(start_vertex_ID).g_score);

    current_parent_vertex_ID = meeting_vertex_ID;
    const double max_forward_distance = forward_state_map.at(current_parent_vertex_ID).g_score;
    const double max_backward_distance = backward_state_map.at(current_parent_vertex_ID).g_score;
    while (backward_state_map.at(current_parent_vertex_ID).parent_ID != -1)
    {
        parcoured_backward_vertices.emplace_back(current_parent_vertex_ID,abs(backward_state_map.at(current_parent_vertex_ID).g_score-max_backward_distance)+max_forward_distance);
        //cout << current_parent_vertex_ID << " : Vertex ID || Current True Distance : " << backward_state_map.at(current_parent_vertex_ID).g_score << endl;
        current_parent_vertex_ID=backward_state_map.at(current_parent_vertex_ID).parent_ID;
    }
    parcoured_backward_vertices.emplace_back(end_vertex_ID,abs(backward_state_map.at(end_vertex_ID).g_score-max_backward_distance)+max_forward_distance);

    reverse(parcoured_forward_vertices.begin(),parcoured_forward_vertices.end());
    parcoured_forward_vertices.pop_back();

    vector<pair<int,double>> final_parcoured_vertices = parcoured_forward_vertices;
    final_parcoured_vertices.insert(final_parcoured_vertices.end(),parcoured_backward_vertices.begin(),parcoured_backward_vertices.end());

    for (auto&[vertex_ID,distance_traveled]:final_parcoured_vertices)
    {
        cout << "BI Visiting this node : " << vertex_ID << " || Distance traveled since start : " << distance_traveled << endl;
    }
}

export void main_finder_bidirectional(const int& start_vertex_ID, const int& end_vertex_ID, unordered_map<int, Node_Score>& forward_search_state_map, unordered_map<int, Node_Score>& backward_search_state_map, const unordered_map<int, Vertex_struct>& forward_reference_map, const unordered_map<int, Vertex_struct>& backward_reference_map)
{

    //CHECK IF BOTH VECTORS ARE INSIDE THE MAP.
    if (!(forward_reference_map.contains(start_vertex_ID) & forward_reference_map.contains(end_vertex_ID)))
    {
        cout << "Did not find : " << start_vertex_ID << " And : " << end_vertex_ID << " In the map" << endl;
        return;
    }
    //IF TRUE WE CONTINUE AND START THE ACTUAL ALGORITHM
    //unordered_map<int, Node_Score> search_state_map;
    priority_queue<priority_queue_type,vector<priority_queue_type>,greater<>> forward_open_list;
    priority_queue<priority_queue_type,vector<priority_queue_type>,greater<>> backward_open_list;

    start_vertex_init(forward_search_state_map,forward_reference_map,forward_open_list,start_vertex_ID, end_vertex_ID);
    start_vertex_init(backward_search_state_map,backward_reference_map,backward_open_list,end_vertex_ID,start_vertex_ID);
    int count = 0;


    double best_path_cost = numeric_limits<double>::infinity();
    int meeting_vertex_ID = -1;
    while (!forward_open_list.empty() && !backward_open_list.empty())
    {
        if (min(forward_open_list.top().first,backward_open_list.top().first) >= best_path_cost)
        {
            cout << "Optimal path found and guaranted : " << endl;
            break;
        }

        const int forward_current_vertex_ID = forward_open_list.top().second;
        forward_open_list.pop();
        const int backward_current_vertex_ID = backward_open_list.top().second;
        backward_open_list.pop();

        //cout << "Visiting this node now (FrontWise) : " << forward_current_vertex_ID << endl;
        //cout << "Visiting this node now (BackWise) : " << backward_current_vertex_ID << endl;
        count +=2;


        //FORWARD

        if (!forward_search_state_map.at(forward_current_vertex_ID).visited)
        {
            if (backward_search_state_map.contains(forward_current_vertex_ID) && backward_search_state_map.at(forward_current_vertex_ID).visited)
            {
                if (const double current_total_cost = forward_search_state_map.at(forward_current_vertex_ID).g_score + backward_search_state_map.at(forward_current_vertex_ID).g_score; current_total_cost < best_path_cost)
                {
                    best_path_cost = current_total_cost;
                    meeting_vertex_ID = forward_current_vertex_ID;
                    cout << "New candidate path found via Forward at " << meeting_vertex_ID << " Cost : " << best_path_cost << endl;
                }
            }
            forward_search_state_map.at(forward_current_vertex_ID).visited = true;

            for (const auto&[neighbor_ID, neighbor_distance] : forward_reference_map.at(forward_current_vertex_ID).neighbors)
            {
                if (const double tentative_g = forward_search_state_map.at(forward_current_vertex_ID).g_score+neighbor_distance;
                    forward_search_state_map[neighbor_ID].g_score>tentative_g)
                {
                    neighbor_handler(forward_search_state_map,forward_reference_map,forward_open_list,forward_current_vertex_ID,neighbor_ID,end_vertex_ID,tentative_g);
                }
            }
        }

        //BACKWARD

        if (!backward_search_state_map.at(backward_current_vertex_ID).visited)
        {
            if (forward_search_state_map.contains(backward_current_vertex_ID) && forward_search_state_map.at(backward_current_vertex_ID).visited)
            {
                if (const double current_total_cost = forward_search_state_map.at(backward_current_vertex_ID).g_score + backward_search_state_map.at(backward_current_vertex_ID).g_score; current_total_cost < best_path_cost)
                {
                    best_path_cost = current_total_cost;
                    meeting_vertex_ID = backward_current_vertex_ID;
                    cout << "New candidate path found via Backward at " << meeting_vertex_ID << " Cost : " << best_path_cost << endl;
                }

            }

            backward_search_state_map.at(backward_current_vertex_ID).visited = true;

            for (const auto&[neighbor_ID, neighbor_distance] : backward_reference_map.at(backward_current_vertex_ID).neighbors)
            {
                if (const double tentative_g = backward_search_state_map.at(backward_current_vertex_ID).g_score+neighbor_distance;
                    backward_search_state_map[neighbor_ID].g_score>tentative_g)
                {
                    neighbor_handler(backward_search_state_map,backward_reference_map,backward_open_list,backward_current_vertex_ID,neighbor_ID,start_vertex_ID,tentative_g);
                }
            }
        }
    }

    //cout << meeting_vertex_ID << endl;
    cout << count << endl;

    if (meeting_vertex_ID!=1)
    {
        parkour_and_distance_printer_bidirectional(start_vertex_ID, end_vertex_ID, meeting_vertex_ID, forward_search_state_map, backward_search_state_map);
        //cout << best_path_cost << endl;
    }
    else
    {
        cout << "No path have been found unfortunately." << endl;
    }
}