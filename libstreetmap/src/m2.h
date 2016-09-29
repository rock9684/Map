#include"m1.h"
#include"graphics.h"
#pragma once

vector<double> find_boundary_points_of_the_city();

bool loadMap();

void draw_map();

double setRadius(); //To set radius according to zoom scale
void drawscreen();
void re_drawscreen(); //Use to restore initial graphics
void draw_all_features();//draw all the feature
void draw_intersections();
void draw_points_of_interest();
void re_draw_intersections_and_points_of_interest();
//two types of feature to draw
void draw_closed_feature(vector<LatLon> & feature_points_vector, unsigned feature_tail_index);
void draw_open_feature(vector<LatLon> & feature_points_vector, unsigned feature_tail_index);
void draw_street();
void draw_street_name();

//Make the map able to interact with mouse button press
void act_on_button_press(float x, float y, t_event_buttonPressed button_info);

//When users click an intersection
void act_on_intersection_press(float x, float y, t_event_buttonPressed button_info);

//When users click a point of interest
void act_on_point_of_interest_press(float x, float y, t_event_buttonPressed button_info);
void draw_features(double radius);

//deploy Find function to find intersection from streets names
void draw_intersection_from_street_name();
void act_on_Find_func();
void act_on_FindPath_func(void (*drawscreen_ptr) (void));
void act_on_FindPathPOI_func(void (*drawscreen_ptr) (void));
int string_to_int(string input_string);
bool char_to_int(char & input_char, int & i);

//Show all intersections
void show_all_intersections(void (*drawscreen_ptr) (void));

//Show all points of interest
void show_all_points_of_interest(void (*drawscreen_ptr) (void));
void show_oneway(void(*drawscreen_ptr)(void));
void draw_oneway();
//To draw fast food
void draw_fast_food();
//Show all fast food
void show_fast_food(void (*drawscreen_ptr) (void));
//To draw bank
void draw_bank();
//Show all bank
void show_bank(void (*drawscreen_ptr) (void));
void clear_find_vector();

//here to draw feature
void draw_feature_Unknown(unsigned);
void draw_feature_Park(unsigned);
void draw_feature_Beach(unsigned);
void draw_feature_Lake(unsigned);
void draw_feature_River(unsigned);
void draw_feature_Island(unsigned);
void draw_feature_Shoreline(unsigned);
void draw_feature_Building(unsigned);
void draw_feature_Greenspace(unsigned);
void draw_feature_Golfcourse(unsigned);
void draw_feature_Stream(unsigned);

//show or hide all feature names
void act_on_FN_button(void (*drawscreen_ptr) (void));
void draw_feature_name();
void draw_closed_feature_name(vector<LatLon> & feature_points_vector, unsigned feature_tail_index, unsigned feature_id);
void draw_open_feature_name(vector<LatLon> & feature_points_vector, unsigned feature_tail_index, unsigned feature_id);

void draw_help();
void show_help(void (*drawscreen_ptr) (void));
void mouse_find();
void show_mouse_find(void (*drawscreen_ptr) (void));
void draw_best_path(unsigned start, unsigned end, vector<unsigned> path);
void draw_best_pathPOI(unsigned start, unsigned end, vector<unsigned> path, unsigned POI_ID);
void draw_find_street();
void show_find_street(void (*drawscreen_ptr) (void));
bool test_partial_name(string input);
bool test_partial_name_for_POI(string input);

void seach_box_display(void(*drawscreen_ptr)(void));
void draw_search_box();
void act_on_key_press (char c, int keysym);
void draw_text_in_the_box();
void textbox_input_parser(char c, int keysym);
string vector_of_char_to_string(vector<char> the_vector);
bool left_or_right(t_point point_before,t_point point_current,t_point point_after);
void print_path( vector<unsigned> segment_vector, unsigned intersect_id_start, unsigned intersect_id_end);
void print_pathPOI( vector<unsigned> segment_vector, unsigned intersect_id_start, unsigned intersect_id_end, unsigned POI_ID);
unsigned get_closest_POI_to_a_given_Intersection_from_given_POI_name(string POI_name, unsigned given_intersection_id);