#pragma once
#include <vector>
#include <string>
#include "m1.h"
#include "m2.h"

#include <queue>
#include <map>
#include <unordered_map>


const double Vmax = 200; //km/hr

std::vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end);

double compute_path_travel_time(const std::vector<unsigned>& path);

std::vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, std::string point_of_interest_name);



class node{
public:
    unsigned fromNode;
    unsigned fromSegment;
    double travelTime;
    double projectedTime;
    node();
    node(unsigned, unsigned, unsigned, unsigned);
    node(unsigned, unsigned);
};

