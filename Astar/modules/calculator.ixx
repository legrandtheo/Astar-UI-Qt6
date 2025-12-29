module;
#include <iostream>
#include <cmath>
#include <numbers>

export module calculator;

import file_reader_surgeon;

using namespace std;

constexpr double earth_radius = 6371e3;


double degree_to_radiant(const double& degree_value)
{
    return degree_value*numbers::pi/180;
}

double calculate_heuristic_brut(const double& lat1, const double& lon1, const double& lat2, const double& lon2)
{
    const double haversin_theta = pow(sin(degree_to_radiant(lat1-lat2)/2),2) + cos(degree_to_radiant(lat1))*cos(degree_to_radiant(lat2))*pow(sin(degree_to_radiant(lon1-lon2)/2),2);
    const double theta = 2*atan2(sqrt(haversin_theta), sqrt(1-haversin_theta));
    return earth_radius*theta;
}

export double calculate_heuristic(const Vertex_struct& first_vertex, const Vertex_struct& second_vertex)
{
    return calculate_heuristic_brut(first_vertex.latitude, first_vertex.longitude, second_vertex.latitude, second_vertex.longitude);
    //return 0;
}


