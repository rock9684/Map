#pragma once //protects against multiple inclusions of this header file

#include <string>
#include <vector>
#include "StreetsDatabaseAPI.h"
using namespace std;
//use these defines if you need earth radius or conversion from degrees to radians
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define DEG_TO_RAD 0.017453292519943295769236907684886


void load_intersection_segment_count();
void load_point_of_interest_position();
void load_segment_from_street_ids();
void load_intersection_from_street_ids();
void load_segment_from_street_ids_vector();
void load_intersection_from_street_vector();
void load_street_name();
void load_intersection_segment();
void load_intersection_position();
void load_Intersection_ID_from_Street_ID();
void load_street_segment_classes();
void load_feature_area_id();
long double polygonArea(double *X, double *Y, int points);
void load_feature_type_distribution();
void ld_m4();
void load_segment_info();
//function to load map streets.bin file
bool load_map(std::string map_name);

//close the loaded map
void close_map();

//function to return street id(s) for a street name
//return a 0-length vector if no street with this name exists.
std::vector<unsigned> find_street_ids_from_name(std::string street_name);

//function to return the street segments for a given intersection 
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id);

//function to return street names at an intersection (include duplicate street names in returned vector)
std::vector<std::string> find_intersection_street_names(unsigned intersection_id);

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2);

//find all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id);

//for a given street, return all the street segments
std::vector<unsigned> find_street_street_segments(unsigned street_id);

//for a given street, find all the intersections
std::vector<unsigned> find_all_street_intersections(unsigned street_id);

//function to return all intersection ids for two intersecting streets
//this function will typically return one intersection id between two street names
//but duplicate street names are allowed, so more than 1 intersection id may exist for 2 street names
std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1, std::string street_name2);

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2);

//find the length of a given street segment
double find_street_segment_length(unsigned street_segment_id);

//find the length of a whole street
double find_street_length(unsigned street_id);

//find the travel time to drive a street segment (time(minutes) = distance(km)/speed_limit(km/hr) * 60
double find_street_segment_travel_time(unsigned street_segment_id);

//find the nearest point of interest to a given position
unsigned find_closest_point_of_interest(LatLon my_position);

//find the nearest intersection (by ID) to a given position
unsigned find_closest_intersection(LatLon my_position);


//A class used to convert LatLon to Cartesian coordinate
class x_y_coord {
public:   
    double x1;
    double y1;
    double x2;
    double y2;


    x_y_coord();
    x_y_coord(LatLon point1, LatLon point2);

    
};

//helper function for find_intersection_ids_from_street_names()
void find_intersection_ids_from_street_IDs(unsigned ID_for_name_1, unsigned ID_for_name_2, vector<unsigned> &in_put_vector);

float find_distance_between_two_point(LatLon point1, LatLon point2);
