module;
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

export module file_reader_surgeon;

using namespace std;


struct Neighbor_struct
{
    int neighbor_ID = 0;
    double neighbor_distance = 0;
};

export struct Vertex_struct
{
    double longitude = 0;
    double latitude = 0;
    vector<Neighbor_struct> neighbors;
};


size_t next_comma_pos(const string& current_string)
{
    return current_string.find(',');
}

string next_part_comma(const string& current_string, size_t& current_pos)
{
    const size_t temp_pos = current_pos;
    current_pos = next_comma_pos(current_string.substr(current_pos,current_string.size()))+temp_pos+1;
    return current_string.substr(temp_pos, current_pos-2);
}

void vertex_map_decomposer(const string& line, unordered_map<int, Vertex_struct>&vertex_map)
{
    if (line[0]=='V')
    {
        // INITIALIZING VERTICES AND ADDING THEIR COORDINATES
        size_t current_str_pos = next_comma_pos(line)+1;
        const int vertex_ID = stoi(next_part_comma(line, current_str_pos));
        //cout << vertex_ID << endl;
        const double vertex_longitude = stod(next_part_comma(line,current_str_pos));
        //cout << setprecision(10) << vertex_longitude << endl;
        const double vertex_latitude = stod(next_part_comma(line,current_str_pos));
        //cout << setprecision(10) << vertex_latitude << endl;

        vertex_map[vertex_ID].latitude=vertex_latitude;
        vertex_map[vertex_ID].longitude=vertex_longitude;
    }
    if (line[0]=='E')
    {
        // ADDING NEIGHBORS INTO THE MAIN VERTEX MAP
        size_t current_str_pos = next_comma_pos(line)+1;
        const int source_ID = stoi(next_part_comma(line, current_str_pos));
        //cout << source_ID << endl;
        const int dest_ID= stoi(next_part_comma(line,current_str_pos));
        //cout << dest_ID << endl;
        const double edge_length = stod(next_part_comma(line,current_str_pos));
        //cout << setprecision(10) << edge_length << endl;

        vertex_map[source_ID].neighbors.push_back({dest_ID,edge_length});
    }
}

void vertex_map_decomposer_bidirectional(const string& line, unordered_map<int, Vertex_struct>&forward_map, unordered_map<int, Vertex_struct>&backward_map)
{
    if (line[0]=='V')
    {
        // INITIALIZING VERTICES AND ADDING THEIR COORDINATES
        size_t current_str_pos = next_comma_pos(line)+1;
        const int vertex_ID = stoi(next_part_comma(line, current_str_pos));
        //cout << vertex_ID << endl;
        const double vertex_longitude = stod(next_part_comma(line,current_str_pos));
        //cout << setprecision(10) << vertex_longitude << endl;
        const double vertex_latitude = stod(next_part_comma(line,current_str_pos));
        //cout << setprecision(10) << vertex_latitude << endl;

        forward_map[vertex_ID].latitude=vertex_latitude;
        forward_map[vertex_ID].longitude=vertex_longitude;
        backward_map[vertex_ID].latitude=vertex_latitude;
        backward_map[vertex_ID].longitude=vertex_longitude;
    }
    if (line[0]=='E')
    {
        // ADDING NEIGHBORS INTO THE MAIN VERTEX MAP
        size_t current_str_pos = next_comma_pos(line)+1;
        const int source_ID = stoi(next_part_comma(line, current_str_pos));
        //cout << source_ID << endl;
        const int dest_ID= stoi(next_part_comma(line,current_str_pos));
        //cout << dest_ID << endl;
        const double edge_length = stod(next_part_comma(line,current_str_pos));
        //cout << setprecision(10) << edge_length << endl;

        forward_map[source_ID].neighbors.push_back({dest_ID,edge_length});
        backward_map[dest_ID].neighbors.push_back({source_ID,edge_length});
    }
}

void line_iterator(const string& filepath,unordered_map<int, Vertex_struct>&vertex_map)
{
    ifstream graph_file(filepath);
    string current_line;
    while (getline(graph_file,current_line))
    {
        vertex_map_decomposer(current_line, vertex_map);
    }
}

void line_iterator_bidirectional(const string& filepath, unordered_map<int, Vertex_struct>&forward_map, unordered_map<int, Vertex_struct>&backward_map)
{
    ifstream graph_file(filepath);
    string current_line;
    while (getline(graph_file,current_line))
    {
        vertex_map_decomposer_bidirectional(current_line, forward_map,backward_map);
    }
}

export void map_initializer(const string& filepath, unordered_map<int, Vertex_struct>& node_map)
{
    line_iterator(filepath, node_map);
    cout << "Finished build map_initializer" << endl;
}

export void map_initializer_bidirectional(const string& filepath, unordered_map<int, Vertex_struct>& forward_map, unordered_map<int, Vertex_struct>& backward_map)
{
    line_iterator_bidirectional(filepath, forward_map,backward_map);
    cout << "Finished build map_initializer_bidirectional" << endl;
}