module;
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>


export module GEOJson_exporter;

import file_reader_surgeon;
import pathfinder_engine;


using namespace std;


export void GEOJson_exporter(const int& end_Vertex_ID, const unordered_map<int, Node_Score>& search_state_map, const unordered_map<int, Vertex_struct>& reference_map)
{
    ofstream GEOJson_file("../exported_data/GEOJson_data.json");
    if (!GEOJson_file.is_open())
    {
        cout << "Could not open the file" << endl;
        return;
    }

    //FILE HEADER
    GEOJson_file << "{ \n   \"type\": \"FeatureCollection\", \n   \"features\": [\n   {\n      \"type\": \"Feature\",\n      \"properties\": {\n      \"stroke\": \"#ff0000\",\n      \"name\": \"Mon Super Chemin A*\"\n      },\n      \"geometry\": {\n       \"type\": \"LineString\", \n       \"coordinates\": [" << endl;
    //ITERATING OVER EACH LINE FOR COORDINATES
    int current_ID = end_Vertex_ID;
    while (search_state_map.at(current_ID).parent_ID!=-1)
    {
        //cout << "Longitude : " << reference_map.at(current_ID).longitude << " || Latitude : " << reference_map.at(current_ID).latitude << endl;
        GEOJson_file << setprecision(12) << "          [ " << reference_map.at(current_ID).longitude << ", " << reference_map.at(current_ID).latitude << " ]," << endl;
        current_ID = search_state_map.at(current_ID).parent_ID;
    }
    //LAST LINE
    GEOJson_file << setprecision(12) << "          [ " << reference_map.at(current_ID).longitude << ", " << reference_map.at(current_ID).latitude << " ]" << endl;
    //FILE FOOTER
    GEOJson_file << "        ]\n      }\n    }\n  ]\n}"<<endl;


    GEOJson_file.close();
}


