#include "m2.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <cmath>
#include <X11/keysym.h>
#include "m3.h"
#include "m4.h"
using namespace std;

//****************************ALL CODE STARTS HERE****************************//



extern vector<LatLon> ld_intersection_position; //Stored all intersection positions
extern vector<LatLon> ld_point_of_interest_posiion; //Stored all point of interest positions
extern vector<vector<LatLon>> ld_feature_position; //Stored all feature positions
extern unsigned IntersectionID; //Total number of intersection
extern unsigned StreetID; //Total number of streets
extern unsigned SegmentID; //Total number of street segments
extern unsigned PointOfInterestID; //Total number of points of interest
extern unsigned FeatureID; //Total number of features
extern t_transform_coordinates trans_coord; //Extracted from graphic.cpp which contains all information of coordinates
extern vector<vector<LatLon>> ld_minor_street; //Total curve points positions on different kinds streets.
extern vector<vector<LatLon>> ld_major_street;
extern vector<vector<LatLon>> ld_expressway;
extern vector<vector<LatLon>> ld_primary;
extern vector<vector<LatLon>> oneway; // Only the oneway street curve points positions
extern vector<pair<long double, unsigned>> feature_area_id; //A vector storing all closed features' area and id
extern vector<vector<unsigned>> ld_segment_from_street_ids_vector; //For drawing street names
extern vector<string> intersection_names; //All intersection names
extern multimap<unsigned, unsigned> ld_segment_from_street_id;
extern bool nomatch;
vector<unsigned> intersection_ID_vector;
bool show_feature_name = true;
vector<unsigned> best_path; // The shortest path from m3 functions
vector<unsigned> show_path_input;
vector<string> best_path_text_form;
bool again = false; // Partial street name check
bool again_POI = false; //Partial POI name check

//A structure used to store information of the object last clicked
struct lastObjectClicked {
    LatLon position;
    string name;
};

lastObjectClicked lastIntersectionClicked; //To store last intersection clicked in order to restore it when click somewhere else
lastObjectClicked lastPointOfInterestClicked; //To store last point of interest clicked in order to restore it when click somewhere else
//Restore only when it is 1
bool lastClickIntersection = 0;
bool lastClickPointOfInterest = 0;
bool show_intersections = 0;
bool show_POI = 0;
bool show_FF = 0; //FF stands for fast food
bool show_B = 0; //B stands for bank
bool show_H = 0; //H stands for help
bool show_MF = 0; // MF stands for find path by mouse click
bool show_OW = 0; //OW stands for one way
bool find_func = 0;
bool show_FPPOI=0;

bool mouse_flag = 0; // Flag for the click on screen to find the shortest path
int name_count = 0;
vector<unsigned> path_name_input;

unsigned closeset_POI_ID;

extern int zoom_count; // Add zoom times count in the graphics.cpp (in functions: handle zoom in, handle zoom out, zoom fit )
string mapName; //Set the map name as global variable to use it in draw city name function

extern vector<unsigned> feature_Unknown;
extern vector<unsigned> feature_Park;
extern vector<unsigned> feature_Beach;
extern vector<unsigned> feature_Lake;
extern vector<unsigned> feature_River;
extern vector<unsigned> feature_Island;
extern vector<unsigned> feature_Shoreline;
extern vector<unsigned> feature_Building;
extern vector<unsigned> feature_Greenspace;
extern vector<unsigned> feature_Golfcourse;
extern vector<unsigned> feature_Stream;
extern double minLat, maxLat, minLon, maxLon;


bool display_search_box=false;
vector<char> input_string_from_textbox;
vector<char> string_need_handling;
int char_in_command=0;

color_types color_indicies[] = {
    LIGHTGREY,
    DARKGREY,
    WHITE,
    BLACK,
    BLUE,
    GREEN,
    YELLOW,
    CYAN,
    RED,
    DARKGREEN,
    MAGENTA
};

//Used to load map without recompiling

bool loadMap() {
    //reset all flags and be ;2hnvy for next map
    lastClickIntersection = 0;
    lastClickPointOfInterest = 0;
    show_intersections = 0;
    show_POI = 0;
    show_FF = 0;
    show_B = 0;
    show_H = 0;
    show_MF = 0;
    show_OW = 0;
    zoom_count = 0;
    find_func = 0;
    show_FPPOI=0;
    cout << "Input a city name here" << endl;
    getline(cin, mapName); //Get name from user

    if (mapName == "Toronto") {
        load_map("/cad2/ece297s/public/maps/toronto.streets.bin");
        return 1;
    }
    if (mapName == "Cairo") {
        load_map("/cad2/ece297s/public/maps/cairo_egypt.streets.bin");
        return 1;
    }
    if (mapName == "Moscow") {
        load_map("/cad2/ece297s/public/maps/moscow.streets.bin");
        return 1;
    }
    if (mapName == "Newyork") {
        load_map("/cad2/ece297s/public/maps/newyork.streets.bin");
        return 1;
    }
    if (mapName == "Saint Helena") {
        load_map("/cad2/ece297s/public/maps/saint_helena.streets.bin");
        return 1;
    }
    if (mapName == "Hamilton") {
        load_map("/cad2/ece297s/public/maps/hamilton_canada.streets.bin");
        return 1;
    }
    if (mapName == "London") {
        load_map("/cad2/ece297s/public/maps/london_england.streets.bin");
        return 1;
    }
    return 0;

}

void draw_map() {
    t_color myBackgroud(240, 237, 229);
    init_graphics("Graphics", myBackgroud);
    set_visible_world(minLon, minLat, maxLon, maxLat); //Same as the boundary of the city
    create_button("Window", "PathInter", act_on_FindPath_func);
    create_button("PathInter", "PathPOI", act_on_FindPathPOI_func);
    create_button("PathPOI", "Intersections", show_all_intersections);

    //The following two show intersections and POIs
    create_button("Intersections", "POI", show_all_points_of_interest);
    create_button("POI", "One Way", show_oneway);

    //The button show one way streets. 
    create_button("One Way", "Fast Food", show_fast_food);

    //The following two show fast food and bank
    create_button("Fast Food", "Bank", show_bank);
    create_button("Bank", "ClickFindPath", show_mouse_find);
    create_button("ClickFindPath", "Search box", seach_box_display);
    create_button("Search box", "Help", show_help);

    set_keypress_input(true);

    event_loop(act_on_button_press, NULL, act_on_key_press, drawscreen);
    clear_find_vector();
    display_search_box=false;
    show_FPPOI=0;
    input_string_from_textbox.clear();
    string_need_handling.clear();
    char_in_command=0;
    close_graphics();
}

void drawscreen() {
    clearscreen();
    //The order is to draw features first, then streets and then intersections and points of interest

    //Draw features
    draw_all_features();
    //Draw streets
    draw_street();
    //Draw one way street after click the button
    if (show_OW)
        draw_oneway();
    //Draw street name
    draw_street_name();

    //Draw intersections and points of interest
    if (show_intersections)
        draw_intersections();
    if (show_POI)
        draw_points_of_interest();

    //Draw fast food and bank

    if (show_FF)
        draw_fast_food();
    if (show_B)
        draw_bank();
    draw_feature_name();
    if (show_MF)
        draw_best_path(show_path_input[0], show_path_input[1], best_path);
    if (display_search_box){
        draw_search_box();
        draw_text_in_the_box();
    }
    if (show_FPPOI)
        draw_best_pathPOI(show_path_input[0], show_path_input[1], best_path,closeset_POI_ID);
    
    //draw intersection specified by streets names
    if (find_func)
        draw_intersection_from_street_name();
    if (show_H)
        draw_help();


           
}

//Used to set radius according to zooming scale

double setRadius() {
    //Just an arbitrarily set criteria
    //As user zooms in the radius of points of interest and intersection get smaller
    double radius;
    if ((trans_coord.init_ytop - trans_coord.init_ybot) > 15 * (trans_coord.ytop - trans_coord.ybot))
        radius = 0.0000010;
    else if ((trans_coord.init_ytop - trans_coord.init_ybot) > 4 * (trans_coord.ytop - trans_coord.ybot))
        radius = 0.0000025;
    else if ((trans_coord.init_ytop - trans_coord.init_ybot) > 1.5 * (trans_coord.ytop - trans_coord.ybot))
        radius = 0.000015;
    else
        radius = 0.000025;

    return radius;
}

void draw_intersections() {

    double radius = setRadius();
    //Draw intersections
    setcolor(247, 244, 158); //light yellow
    for (unsigned i = 0; i < IntersectionID; i++) {
        //Only draw things within the visible area
        if (ld_intersection_position[i].lon <= trans_coord.xright && ld_intersection_position[i].lon >= trans_coord.xleft
                && ld_intersection_position[i].lat <= trans_coord.ytop && ld_intersection_position[i].lat >= trans_coord.ybot)
            fillarc(ld_intersection_position[i].lon, ld_intersection_position[i].lat, radius, 0, 360);
    }

    if (lastClickIntersection) {
        //Prevent information of highlighted intersection from fading away
        setcolor(BLUE);
        fillarc(lastIntersectionClicked.position.lon, lastIntersectionClicked.position.lat, radius, 0, 360);
        setcolor(RED);
        setfontsize(10);
        drawtext(lastIntersectionClicked.position.lon, lastIntersectionClicked.position.lat + 3 * radius, lastIntersectionClicked.name, 0.01, 0.01);
    }
}

void draw_points_of_interest() {
    double radius = setRadius();
    //Draw points of interest
    setcolor(252, 131, 161); //pink
    for (unsigned i = 0; i < PointOfInterestID; i++) {
        //Only draw things within the visible area
        if (ld_point_of_interest_posiion[i].lon <= trans_coord.xright && ld_point_of_interest_posiion[i].lon >= trans_coord.xleft
                && ld_point_of_interest_posiion[i].lat <= trans_coord.ytop && ld_point_of_interest_posiion[i].lat >= trans_coord.ybot)
            fillarc(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat, radius, 0, 360);
    }

    if (lastClickPointOfInterest) {
        //Prevent information of highlighted intersection from fading away
        setcolor(BLUE);
        fillarc(lastPointOfInterestClicked.position.lon, lastPointOfInterestClicked.position.lat, radius, 0, 360);
        setcolor(RED);
        setfontsize(10);
        drawtext(lastPointOfInterestClicked.position.lon, lastPointOfInterestClicked.position.lat + 3 * radius, lastPointOfInterestClicked.name, 0.01, 0.01);
    }
}

//Following two functions are only used to restore initial graphics

void re_drawscreen() {
    clearscreen();
    //The order is to draw features first, then streets and then intersections and points of interest
    //Draw features
    draw_all_features();
    //Draw streets
    draw_street();
    //Draw one way street after click the botton
    if (show_OW)
        draw_oneway();
    //Draw street name at last to show on the top of the street
    draw_street_name();

    //Draw intersections and points of interest
    re_draw_intersections_and_points_of_interest();

    //Prevent from being removed if POI button is clicked
    if (show_FF)
        draw_fast_food();
    if (show_B)
        draw_bank();
    draw_feature_name();
    if (show_MF)
        mouse_find();
    if (display_search_box){
        draw_search_box();
        draw_text_in_the_box();
    }
    if (show_FPPOI)
        draw_best_pathPOI(show_path_input[0], show_path_input[1], best_path,closeset_POI_ID);
    
    if (find_func)
        draw_intersection_from_street_name();
    if(show_H)
        draw_help();
}

void re_draw_intersections_and_points_of_interest() {
    double radius = setRadius();

    //Draw intersections
    if (show_intersections) {
        setcolor(247, 244, 158); //light yellow
        for (unsigned i = 0; i < IntersectionID; i++) {
            //Only draw things within the visible area
            if (ld_intersection_position[i].lon <= trans_coord.xright && ld_intersection_position[i].lon >= trans_coord.xleft
                    && ld_intersection_position[i].lat <= trans_coord.ytop && ld_intersection_position[i].lat >= trans_coord.ybot)
                fillarc(ld_intersection_position[i].lon, ld_intersection_position[i].lat, radius, 0, 360);
        }
    }

    //Draw points of interest
    if (show_POI) {
        setcolor(252, 131, 161); //pink
        for (unsigned i = 0; i < PointOfInterestID; i++) {
            //Only draw things within the visible area
            if (ld_point_of_interest_posiion[i].lon <= trans_coord.xright && ld_point_of_interest_posiion[i].lon >= trans_coord.xleft
                    && ld_point_of_interest_posiion[i].lat <= trans_coord.ytop && ld_point_of_interest_posiion[i].lat >= trans_coord.ybot)
                fillarc(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat, radius, 0, 360);
        }
    }
}

void draw_all_features() {
    double radius = setRadius();
    //draw features
    draw_features(radius);

}

void draw_closed_feature(vector<LatLon> & feature_points_vector, unsigned feature_tail_index) {
    double sumx = 0;
    double sumy = 0;

    t_point polygon_to_draw[feature_tail_index + 1];

    for (unsigned i = 0; i <= feature_tail_index; i++) {
        polygon_to_draw[i].x = feature_points_vector[i].lon;
        sumx += polygon_to_draw[i].x;
        polygon_to_draw[i].y = feature_points_vector[i].lat;
        sumy += polygon_to_draw[i].y;
    }

    fillpoly(polygon_to_draw, (feature_tail_index + 1));

}

void draw_open_feature(vector<LatLon> & feature_points_vector, unsigned feature_tail_index) {
    setlinewidth(1);
    for (unsigned i = 0; i < feature_tail_index; i++) {
        drawline(feature_points_vector[i].lon, feature_points_vector[i].lat, feature_points_vector[i + 1].lon, feature_points_vector[i + 1].lat);
    }
}

void draw_street() {
    //draw streets
    if (zoom_count >= -4) {
        //Draw minor road at first and express way at last since it will be covered.
        //minor
        if (zoom_count > 3) { //Dynamically draw all kinds of streets.
            setcolor(209, 209, 206); //grey
            setlinewidth(1);
            if (zoom_count >= 6) { //Dynamically control the street color
                setcolor(238, 201, 86); //orange
                setlinewidth(zoom_count - 5); //Dynamically control the line width
            }
            for (unsigned i = 0; i < ld_minor_street.size(); i++) {
                vector<LatLon> temp = ld_minor_street[i]; // get curve point position vector on each segment
                for (unsigned j = 0; j < temp.size() - 1; j++) //get curve point positions
                    //Only draw things within the visible area
                    if ((ld_minor_street[i][j].lon <= trans_coord.xright && ld_minor_street[i][j].lon >= trans_coord.xleft
                            && ld_minor_street[i][j].lat <= trans_coord.ytop && ld_minor_street[i][j].lat >= trans_coord.ybot)
                            || (ld_minor_street[i][j + 1].lon <= trans_coord.xright && ld_minor_street[i][j + 1].lon >= trans_coord.xleft
                            && ld_minor_street[i][j + 1].lat <= trans_coord.ytop && ld_minor_street[i][j + 1].lat >= trans_coord.ybot))
                        drawline(ld_minor_street[i][j].lon, ld_minor_street[i][j].lat, ld_minor_street[i][j + 1].lon, ld_minor_street[i][j + 1].lat);
            }
        }
        //major
        if (zoom_count >= 3) {
            setcolor(WHITE);
            setlinewidth(zoom_count - 2);
            for (unsigned i = 0; i < ld_major_street.size(); i++) {
                vector<LatLon> temp = ld_major_street[i];
                for (unsigned j = 0; j < temp.size() - 1; j++)
                    //Only draw things within the visible area
                    if ((ld_major_street[i][j].lon <= trans_coord.xright && ld_major_street[i][j].lon >= trans_coord.xleft
                            && ld_major_street[i][j].lat <= trans_coord.ytop && ld_major_street[i][j].lat >= trans_coord.ybot)
                            || (ld_major_street[i][j + 1].lon <= trans_coord.xright && ld_major_street[i][j + 1].lon >= trans_coord.xleft
                            && ld_major_street[i][j + 1].lat <= trans_coord.ytop && ld_major_street[i][j + 1].lat >= trans_coord.ybot))
                        drawline(ld_major_street[i][j].lon, ld_major_street[i][j].lat, ld_major_street[i][j + 1].lon, ld_major_street[i][j + 1].lat);
            }
        }
        //primary
        if (zoom_count > 1) {
            setcolor(255, 225, 104); // orange
            setlinewidth(zoom_count);
            for (unsigned i = 0; i < ld_primary.size(); i++) {
                vector<LatLon> temp = ld_primary[i];
                for (unsigned j = 0; j < temp.size() - 1; j++)
                    //Only draw things within the visible area
                    if ((ld_primary[i][j].lon <= trans_coord.xright && ld_primary[i][j].lon >= trans_coord.xleft
                            && ld_primary[i][j].lat <= trans_coord.ytop && ld_primary[i][j].lat >= trans_coord.ybot)
                            || (ld_primary[i][j + 1].lon <= trans_coord.xright && ld_primary[i][j + 1].lon >= trans_coord.xleft
                            && ld_primary[i][j + 1].lat <= trans_coord.ytop && ld_primary[i][j + 1].lat >= trans_coord.ybot))
                        drawline(ld_primary[i][j].lon, ld_primary[i][j].lat, ld_primary[i][j + 1].lon, ld_primary[i][j + 1].lat);
            }
        }
        //expressway
        setcolor(250, 158, 37); // dark orange
        setlinewidth(4 + zoom_count);
        for (unsigned i = 0; i < ld_expressway.size(); i++) {
            vector<LatLon> temp = ld_expressway[i];
            for (unsigned j = 0; j < temp.size() - 1; j++)
                //Only draw things within the visible area
                if ((ld_expressway[i][j].lon <= trans_coord.xright && ld_expressway[i][j].lon >= trans_coord.xleft
                        && ld_expressway[i][j].lat <= trans_coord.ytop && ld_expressway[i][j].lat >= trans_coord.ybot)
                        || (ld_expressway[i][j + 1].lon <= trans_coord.xright && ld_expressway[i][j + 1].lon >= trans_coord.xleft
                        && ld_expressway[i][j + 1].lat <= trans_coord.ytop && ld_expressway[i][j + 1].lat >= trans_coord.ybot))
                    drawline(ld_expressway[i][j].lon, ld_expressway[i][j].lat, ld_expressway[i][j + 1].lon, ld_expressway[i][j + 1].lat);
        }

    }
}

void draw_street_name() {
    if (zoom_count > 2) { //Dynamically show out all kinds of street names.

        setcolor(BLACK);
        setfontsize(10);
        for (auto iter = ld_segment_from_street_ids_vector.begin(); iter != ld_segment_from_street_ids_vector.end(); iter++) {
            LatLon lastP;
            bool compare = 0;
            for (auto iterS = iter->begin(); iterS != iter->end(); iterS++) {
                if ((getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat >= trans_coord.ybot)

                        && (getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat >= trans_coord.ybot)

                        && getStreetSegmentInfo(*iterS).speedLimit >= 90) {

                    unsigned temp = getStreetSegmentInfo(*iterS).curvePointCount;
                    float xc, yc; // Calculate the center point on a segment and show the name on this point
                    float dx, dy;
                    if (temp == 0) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat);
                    } else if (temp == 1) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getStreetSegmentCurvePoint(*iterS, 0).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getStreetSegmentCurvePoint(*iterS, 0).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getStreetSegmentCurvePoint(*iterS, 0).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getStreetSegmentCurvePoint(*iterS, 0).lat);
                    } else {
                        xc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon + getStreetSegmentCurvePoint(*iterS, temp / 2).lon) / 2;
                        yc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat + getStreetSegmentCurvePoint(*iterS, temp / 2).lat) / 2;
                        dx = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon - getStreetSegmentCurvePoint(*iterS, temp / 2).lon);
                        dy = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat - getStreetSegmentCurvePoint(*iterS, temp / 2).lat);
                    }
                    if (!compare) {
                        lastP.lat = yc;
                        lastP.lon = xc;
                        compare = 1;
                        int deg = atan(dy / dx) / DEG_TO_RAD;
                        settextrotation(deg);
                        if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        settextrotation(0);
                    } else {
                        LatLon point1(yc, xc);
                        if (find_distance_between_two_point(point1, lastP) >= 0.000005 / zoom_count / zoom_count) {
                            lastP.lat = yc;
                            lastP.lon = xc;
                            int deg = atan(dy / dx) / DEG_TO_RAD;
                            settextrotation(deg);
                            if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                            else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                            settextrotation(0);
                        }
                    }

                }

            }
        }

    }
    if (zoom_count > 6) {

        setcolor(BLACK);
        setfontsize(zoom_count); //Dynamically control the text front size
        for (auto iter = ld_segment_from_street_ids_vector.begin(); iter != ld_segment_from_street_ids_vector.end(); iter++) {
            LatLon lastP;
            bool compare = 0;
            for (auto iterS = iter->begin(); iterS != iter->end(); iterS++) {
                if ((getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat >= trans_coord.ybot)

                        && (getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat >= trans_coord.ybot)

                        && getStreetSegmentInfo(*iterS).speedLimit < 90 && getStreetSegmentInfo(*iterS).speedLimit > 50) {

                    unsigned temp = getStreetSegmentInfo(*iterS).curvePointCount;
                    float xc, yc; // Calculate the center point on a segment and show the name on this point
                    float dy, dx;
                    if (temp == 0) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat);
                    } else if (temp == 1) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getStreetSegmentCurvePoint(*iterS, 0).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getStreetSegmentCurvePoint(*iterS, 0).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getStreetSegmentCurvePoint(*iterS, 0).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getStreetSegmentCurvePoint(*iterS, 0).lat);
                    } else {
                        xc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon + getStreetSegmentCurvePoint(*iterS, temp / 2).lon) / 2;
                        yc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat + getStreetSegmentCurvePoint(*iterS, temp / 2).lat) / 2;
                        dx = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon - getStreetSegmentCurvePoint(*iterS, temp / 2).lon);
                        dy = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat - getStreetSegmentCurvePoint(*iterS, temp / 2).lat);
                    }
                    if (!compare) {
                        lastP.lat = yc;
                        lastP.lon = xc;
                        compare = 1;
                        int deg = atan(dy / dx) / DEG_TO_RAD;
                        settextrotation(deg);
                        if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        settextrotation(0);
                    } else {
                        LatLon point1(yc, xc);
                        if (find_distance_between_two_point(point1, lastP) >= 0.0000001 / zoom_count / zoom_count) {
                            lastP.lat = yc;
                            lastP.lon = xc;
                            int deg = atan(dy / dx) / DEG_TO_RAD;
                            settextrotation(deg);
                            if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.04, 0.04);
                            else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.04, 0.04);
                            settextrotation(0);
                        }
                    }
                }
            }

        }
    }
    if (zoom_count > 8) {

        setcolor(96, 99, 107);
        setfontsize(zoom_count);
        for (auto iter = ld_segment_from_street_ids_vector.begin(); iter != ld_segment_from_street_ids_vector.end(); iter++) {
            LatLon lastP;
            bool compare = 0;
            for (auto iterS = iter->begin(); iterS != iter->end(); iterS++) {
                if ((getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat >= trans_coord.ybot)

                        && (getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat >= trans_coord.ybot)

                        && getStreetSegmentInfo(*iterS).speedLimit <= 50 && getStreetSegmentInfo(*iterS).speedLimit > 40) {

                    unsigned temp = getStreetSegmentInfo(*iterS).curvePointCount;
                    float xc, yc; // Calculate the center point on a segment and show the name on this point
                    float dx, dy;
                    if (temp == 0) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat);
                    } else if (temp == 1) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getStreetSegmentCurvePoint(*iterS, 0).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getStreetSegmentCurvePoint(*iterS, 0).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getStreetSegmentCurvePoint(*iterS, 0).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getStreetSegmentCurvePoint(*iterS, 0).lat);
                    } else {
                        xc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon + getStreetSegmentCurvePoint(*iterS, temp / 2).lon) / 2;
                        yc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat + getStreetSegmentCurvePoint(*iterS, temp / 2).lat) / 2;
                        dx = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon - getStreetSegmentCurvePoint(*iterS, temp / 2).lon);
                        dy = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat - getStreetSegmentCurvePoint(*iterS, temp / 2).lat);
                    }
                    if (!compare) {
                        lastP.lat = yc;
                        lastP.lon = xc;
                        compare = 1;
                        int deg = atan(dy / dx) / DEG_TO_RAD;
                        settextrotation(deg);
                        if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        settextrotation(0);
                    } else {
                        LatLon point1(yc, xc);
                        if (find_distance_between_two_point(point1, lastP) >= 0.00000005 / zoom_count / zoom_count) {
                            lastP.lat = yc;
                            lastP.lon = xc;
                            int deg = atan(dy / dx) / DEG_TO_RAD;
                            settextrotation(deg);
                            if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.04, 0.04);
                            else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.04, 0.04);
                            settextrotation(0);
                        }
                    }
                }

            }
        }
    }
    if (zoom_count > 9) {
        setcolor(96, 99, 107);
        setfontsize(zoom_count - 2);
        for (auto iter = ld_segment_from_street_ids_vector.begin(); iter != ld_segment_from_street_ids_vector.end(); iter++) {
            LatLon lastP;
            bool compare = 0;
            for (auto iterS = iter->begin(); iterS != iter->end(); iterS++) {
                if ((getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat >= trans_coord.ybot)

                        && (getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon <= trans_coord.xright && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon >= trans_coord.xleft
                        && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat <= trans_coord.ytop && getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat >= trans_coord.ybot)

                        && getStreetSegmentInfo(*iterS).speedLimit <= 40) {

                    unsigned temp = getStreetSegmentInfo(*iterS).curvePointCount;
                    float xc, yc; // Calculate the center point on a segment and show the name on this point
                    float dx, dy;
                    if (temp == 0) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getIntersectionPosition(getStreetSegmentInfo(*iterS).to).lat);
                    } else if (temp == 1) {
                        xc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon + getStreetSegmentCurvePoint(*iterS, 0).lon) / 2;
                        yc = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat + getStreetSegmentCurvePoint(*iterS, 0).lat) / 2;
                        dx = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lon - getStreetSegmentCurvePoint(*iterS, 0).lon);
                        dy = (getIntersectionPosition(getStreetSegmentInfo(*iterS).from).lat - getStreetSegmentCurvePoint(*iterS, 0).lat);
                    } else {
                        xc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon + getStreetSegmentCurvePoint(*iterS, temp / 2).lon) / 2;
                        yc = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat + getStreetSegmentCurvePoint(*iterS, temp / 2).lat) / 2;
                        dx = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lon - getStreetSegmentCurvePoint(*iterS, temp / 2).lon);
                        dy = (getStreetSegmentCurvePoint(*iterS, temp / 2 - 1).lat - getStreetSegmentCurvePoint(*iterS, temp / 2).lat);
                    }
                    if (!compare) {
                        lastP.lat = yc;
                        lastP.lon = xc;
                        compare = 1;
                        int deg = atan(dy / dx) / DEG_TO_RAD;
                        settextrotation(deg);
                        if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                            drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.02, 0.02);
                        settextrotation(0);
                    } else {
                        LatLon point1(yc, xc);
                        if (find_distance_between_two_point(point1, lastP) >= 4 / zoom_count / zoom_count) {
                            lastP.lat = yc;
                            lastP.lon = xc;
                            int deg = atan(dy / dx) / DEG_TO_RAD;
                            settextrotation(deg);
                            if (getStreetName(getStreetSegmentInfo(*iterS).streetID) != "<unknown>") // Since the <unknown> streets are too much, ignore them on the small scale.                                                    
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.04, 0.04);
                            else if (zoom_count > 12)//Only zoom scale greater than 12 times, the unknown street will show out.
                                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(*iterS).streetID), 0.04, 0.04);
                            settextrotation(0);
                        }
                    }
                }

            }
        }
    }

}

void draw_oneway() {
    setlinewidth(2);
    for (unsigned i = 0; i < oneway.size(); i++) {

        vector<LatLon> temp = oneway[i];
        unsigned count = temp.size();
        if (count == 2) { //This segment doesn't have curve point. Calculate the center point.
            float x = (oneway[i][0].lon + oneway[i][1].lon) / 2;
            float y = (oneway[i][0].lat + oneway[i][1].lat) / 2;
            setcolor(BLUE);
            drawline(oneway[i][0].lon, oneway[i][0].lat, x, y);
            setcolor(GREEN);
            drawline(x, y, oneway[i][1].lon, oneway[i][1].lat);
        } else { //Use the center curve point to separate the segment into two parts. "From" part in BLUE, "TO" part in GREEN.
            setcolor(BLUE);
            for (unsigned j = 0; j < count / 2; j++)
                //Only draw things within the visible area
                if ((oneway[i][j].lon <= trans_coord.xright && oneway[i][j].lon >= trans_coord.xleft
                        && oneway[i][j].lat <= trans_coord.ytop && oneway[i][j].lat >= trans_coord.ybot)
                        || (oneway[i][j + 1].lon <= trans_coord.xright && oneway[i][j + 1].lon >= trans_coord.xleft
                        && oneway[i][j + 1].lat <= trans_coord.ytop && oneway[i][j + 1].lat >= trans_coord.ybot))
                    drawline(oneway[i][j].lon, oneway[i][j].lat, oneway[i][j + 1].lon, oneway[i][j + 1].lat);
            setcolor(GREEN);
            for (unsigned j = count / 2; j < count - 1; j++)
                //Only draw things within the visible area
                if ((oneway[i][j].lon <= trans_coord.xright && oneway[i][j].lon >= trans_coord.xleft
                        && oneway[i][j].lat <= trans_coord.ytop && oneway[i][j].lat >= trans_coord.ybot)
                        || (oneway[i][j + 1].lon <= trans_coord.xright && oneway[i][j + 1].lon >= trans_coord.xleft
                        && oneway[i][j + 1].lat <= trans_coord.ytop && oneway[i][j + 1].lat >= trans_coord.ybot))
                    drawline(oneway[i][j].lon, oneway[i][j].lat, oneway[i][j + 1].lon, oneway[i][j + 1].lat);
        }
    }
}
//Make the map be able to interact with mouse button press

void act_on_button_press(float x, float y, t_event_buttonPressed button_info) {
    //When users click somewhere else, redraw screen to original one
    if (lastClickIntersection || lastClickPointOfInterest) {
        re_drawscreen();
    }
    act_on_intersection_press(x, y, button_info);
    act_on_point_of_interest_press(x, y, button_info);
}

//When users click an intersection

void act_on_intersection_press(float x, float y, t_event_buttonPressed button_info) {
    //The info will be used to determine if the click is valid
    double radius = setRadius();
    

    for (unsigned i = 0; i < IntersectionID; i++) {
        //When users left click an intersection
        if (button_info.button == 1 && (x - ld_intersection_position[i].lon)*(x - ld_intersection_position[i].lon)
                + (y - ld_intersection_position[i].lat)*(y - ld_intersection_position[i].lat) < radius * radius) {
            setcolor(BLUE);
            fillarc(ld_intersection_position[i].lon, ld_intersection_position[i].lat, radius, 0, 360);

            //Set flag and load information
            lastClickIntersection = 1;
            lastIntersectionClicked.position = ld_intersection_position[i];
            lastIntersectionClicked.name = getIntersectionName(i);

            if (mouse_flag == 1) {
                best_path.clear();
                show_path_input.clear();
                path_name_input.push_back(i);
                
                //  name_count++;
                if (path_name_input.size() == 2) {
                    best_path = find_path_between_intersections(path_name_input[0], path_name_input[1]);
                    draw_best_path(path_name_input[0], path_name_input[1], best_path);
                    show_path_input = path_name_input;
                    path_name_input.clear();
                    show_MF = 1;
                    name_count = 0;
                    return;
                }else
                    show_MF = 0;
                return;
            }


            //Show on graphics
            setcolor(RED);
            setfontsize(10);
            drawtext(ld_intersection_position[i].lon, ld_intersection_position[i].lat + 3 * radius, getIntersectionName(i), 0.01, 0.01);

            //Print information
            cout << endl;
            cout << "Intersection name:" << endl;
            cout << " " << getIntersectionName(i) << endl;
            cout << "Intersection position:" << endl;

            cout << " ";
            cout.precision(6);


            if (ld_intersection_position[i].lat >= 0) {
                cout << ld_intersection_position[i].lat << "° " << "N" << endl;
            } else {
                cout << -ld_intersection_position[i].lat << "° " << "S" << endl;
            }

            cout << " ";
            cout.precision(6);
            if (ld_intersection_position[i].lon >= 0) {
                cout << ld_intersection_position[i].lon << "° " << "E" << endl;
            } else {
                cout << -ld_intersection_position[i].lon << "° " << "W" << endl;
            }
            cout << i << endl;

            break;
        } else {
            lastClickIntersection = 0;
        }
    }
}

//When users click a point of interest

void act_on_point_of_interest_press(float x, float y, t_event_buttonPressed button_info) {
    //The info will be used to determine if the click is valid
    double radius = setRadius();

    for (unsigned i = 0; i < PointOfInterestID; i++) {
        //When users left click an intersection
        if (button_info.button == 1 && (x - ld_point_of_interest_posiion[i].lon)*(x - ld_point_of_interest_posiion[i].lon)
                + (y - ld_point_of_interest_posiion[i].lat)*(y - ld_point_of_interest_posiion[i].lat) < radius * radius) {
            setcolor(BLUE);
            fillarc(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat, radius, 0, 360);

            //Set the flag and load information
            lastClickPointOfInterest = 1;
            lastPointOfInterestClicked.position = ld_point_of_interest_posiion[i];
            lastPointOfInterestClicked.name = getPointOfInterestName(i);

            //Show on graphics
            setcolor(RED);
            setfontsize(10);
            drawtext(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat + 3 * radius, getPointOfInterestName(i), 0.01, 0.01);

            //Print information
            cout << endl;
            cout << "POI name:" << endl;
            cout << " " << getPointOfInterestName(i) << endl;
            cout << "POI position:" << endl;

            cout << " ";
            cout.precision(6);


            if (ld_intersection_position[i].lat >= 0) {
                cout << ld_point_of_interest_posiion[i].lat << "° " << "N" << endl;
            } else {
                cout << -ld_point_of_interest_posiion[i].lat << "° " << "S" << endl;
            }

            cout << " ";
            cout.precision(6);
            if (ld_intersection_position[i].lon >= 0) {
                cout << ld_point_of_interest_posiion[i].lon << "° " << "E" << endl;
            } else {
                cout << -ld_point_of_interest_posiion[i].lon << "° " << "W" << endl;
            }
            cout << i << endl;

            break;
        } else {
            lastClickPointOfInterest = 0;
        }
    }
}

void draw_features(double radius) {
    //Sort the vector based on area
    sort(feature_area_id.begin(), feature_area_id.end());

    //To draw closed features from one with the largest area to the one with the smallest
    for (auto iter = feature_area_id.rbegin(); iter != feature_area_id.rend(); iter++) {

        draw_feature_Unknown(iter->second);
        draw_feature_Beach(iter->second);
        draw_feature_Park(iter->second);
        draw_feature_Lake(iter->second);
        draw_feature_Island(iter->second);
        draw_feature_Shoreline(iter->second);
        draw_feature_Building(iter->second);
        draw_feature_Golfcourse(iter->second);
        draw_feature_Greenspace(iter->second);
        draw_feature_River(iter->second);
        draw_feature_Stream(iter->second);
    }

    //Then draw open features
    for (unsigned i = 0; i < FeatureID; i++) {
        int pointCount = getFeaturePointCount((unsigned) i);
        //Check if it is open
        if (getFeaturePoint((unsigned) i, 0).lat != getFeaturePoint((unsigned) i, pointCount - 1).lat ||
                getFeaturePoint((unsigned) i, 0).lon != getFeaturePoint((unsigned) i, pointCount - 1).lon) {
            draw_feature_Unknown((unsigned) i);
            draw_feature_Beach((unsigned) i);
            draw_feature_Park((unsigned) i);
            draw_feature_Lake((unsigned) i);
            draw_feature_Island((unsigned) i);
            draw_feature_Shoreline((unsigned) i);
            draw_feature_Building((unsigned) i);
            draw_feature_Golfcourse((unsigned) i);
            draw_feature_Greenspace((unsigned) i);
            draw_feature_River((unsigned) i);
            draw_feature_Stream((unsigned) i);
        }
    }

}

//Find button
void act_on_FindPath_func(void (*drawscreen_ptr) (void)){
    
    if (show_MF){
        show_MF=0;
        re_drawscreen();
        return;
    }
    find_func=1;
    cout<<"Please enter the start point: "<<endl;
    unsigned start_node, end_node;
    show_path_input.clear();
    intersection_ID_vector.clear();
    
    re_drawscreen();
   
    act_on_Find_func();
    drawscreen();
    
    int vector_size = intersection_ID_vector.size();
    if(vector_size ==0){
        drawscreen_ptr();
        return;
    }
    
    else if(vector_size>1){
        cout<<"Please choose an intersection from above"<<endl;
        int index_wanted;
        string temp;
        getline(cin, temp);
        index_wanted=string_to_int(temp);
        while(index_wanted<0||index_wanted>=vector_size){
            cout<<"no such intersection, please try again"<<endl;
            temp.clear();
            getline(cin, temp);
            index_wanted=string_to_int(temp);
        }
        
        start_node=intersection_ID_vector[index_wanted];
    }
    
    else start_node=intersection_ID_vector[0];
    
    intersection_ID_vector.clear();
    
    cout<<"Please enter the end point: "<<endl;
    act_on_Find_func();
    drawscreen();
    
    vector_size = intersection_ID_vector.size();
    if(vector_size ==0){
        drawscreen_ptr();
        return;
    }
    
    else if(vector_size>1){
        cout<<"Please choose an intersection from above"<<endl;
        int index_wanted;
        string temp;
        getline(cin, temp);
        index_wanted=string_to_int(temp);
        while(index_wanted<0||index_wanted>=vector_size){
            
            getline(cin, temp);
            index_wanted=string_to_int(temp);
            if (index_wanted<0||index_wanted>=vector_size){
                cout<<"no such intersection, please try again"<<endl;
            }
        }
        
        end_node=intersection_ID_vector[index_wanted];
    }
    
    else end_node=intersection_ID_vector[0];
    
    best_path=find_path_between_intersections(start_node, end_node);
    
    find_func=0;
    show_path_input.clear();
    show_path_input.push_back(start_node);
    show_path_input.push_back(end_node);
    show_MF=!show_MF;
    draw_best_path(start_node, end_node, best_path);
    re_drawscreen();

}

void act_on_Find_func() {
    if (!find_func) {
        
        re_drawscreen();
        return;
    } else
        find_func = 1;
    std::string street_name1, street_name2;
    cout << "Please enter street name 1: " << endl;
    getline(cin, street_name1);
    // Partial name input check
    bool partial_test1 = test_partial_name(street_name1);
 
    do {
        if (!partial_test1) { // Check the input name is valid
            cout << "The input is not a correct street name. Please enter again: " << endl;
            getline(cin, street_name1);
            partial_test1 = test_partial_name(street_name1);

        }
        else {
            while (again == true) { // Check the partial input name 
                cout << "Please choose street name 1: " << endl;
                getline(cin, street_name1);
                partial_test1 = test_partial_name(street_name1);
                if (partial_test1 == false)
                    break;
            }
        }
        if ((!again)&&partial_test1) break;
    } while (1);
    
    again = false;
    
    cout << "Please enter street name 2: " << endl;
    getline(cin, street_name2);
    
    bool partial_test2 = test_partial_name(street_name2);

    do {
        if (!partial_test2) {
            cout << "The input is not a correct street name. Please enter again: " << endl;
            getline(cin, street_name2);
            partial_test2 = test_partial_name(street_name2);

        }
        else {
            while (again == true) {
                cout << "Please choose street name 2: " << endl;
                getline(cin, street_name2);
                partial_test2 = test_partial_name(street_name2);
                if (partial_test2 == false)
                    break;
            }
        }

        if ((!again)&&partial_test2) break;
    } while (1);
    
    again = false;

    intersection_ID_vector = find_intersection_ids_from_street_names(street_name1, street_name2);

    int vector_size = intersection_ID_vector.size();

    if (vector_size == 0)
        cout << "There is no intersection for these streets. " << endl;//segmentation fault here
    sort(intersection_ID_vector.begin(), intersection_ID_vector.end());

    for (int i = 0; i < vector_size; i++) {

        LatLon this_position = getIntersectionPosition(intersection_ID_vector[i]);
        cout << i << ": " << endl;
        cout << "Intersection name:" << endl;
        cout << " " << getIntersectionName(intersection_ID_vector[i]) << endl;
        cout << "Intersection position:" << endl;

        cout << " ";
        if (this_position.lat >= 0)
            cout << this_position.lat << "° " << "N" << endl;
        else
            cout << -this_position.lat << "° " << "S" << endl;

        cout << " ";
        if (this_position.lon >= 0)
            cout << this_position.lon << "° " << "E" << endl;
        else
            cout << -this_position.lon << "° " << "W" << endl << endl;
    }
}

void draw_intersection_from_street_name() {
    double radius=setRadius()/1.5;
    
    int vector_size = intersection_ID_vector.size();
    if (vector_size == 0)
        return;

    //light yellow
    for (int i = 0; i < vector_size; i++) {
        setcolor(247, 244, 158);
        LatLon this_position = getIntersectionPosition(intersection_ID_vector[i]);
        fillarc(this_position.lon, this_position.lat, radius * 5, 0, 360);
        char index[200];

        sprintf(index, "%d", i);
        setcolor(BLACK);
        setfontsize(16);
        drawtext(this_position.lon, this_position.lat, index, radius * 20, radius * 20);
    }
}

//Show all intersections

void show_all_intersections(void (*drawscreen_ptr) (void)) {
    if (show_intersections) {
        show_intersections = 0;
        lastClickIntersection = 0;
        re_drawscreen();
    } else
        show_intersections = 1;
    //Draw intersections
    draw_intersections();

    drawscreen_ptr();
}

//Show all points of interest

void show_all_points_of_interest(void (*drawscreen_ptr) (void)) {
    if (show_POI) {
        show_POI = 0;
        re_drawscreen();
        lastClickPointOfInterest = 0;
    } else
        show_POI = 1;
    //Draw points of interest
    draw_points_of_interest();

    drawscreen_ptr();
}

void show_oneway(void(*drawscreen_ptr)(void)) {

    if (show_OW) {
        show_OW = 0;
        re_drawscreen();
    } else
        show_OW = 1;
    draw_oneway();
    cout << "On any segment, the only legal direction is from BLUE to GREEN" << endl;
    cout << "At any intersection, only moving between different colors is legal" << endl;

    drawscreen_ptr();
}
//To show all fast food

void show_fast_food(void (*drawscreen_ptr) (void)) {
    if (show_FF) {
        //Means the user clicked the button again so clear everything
        show_FF = 0;
        re_drawscreen();
    } else
        show_FF = 1;
    //Draw fast food
    draw_fast_food();


    drawscreen_ptr();
}

void draw_fast_food() {
    double radius = setRadius();
    for (unsigned i = 0; i < PointOfInterestID; i++) {
        if (getPointOfInterestType(i) == "fast_food") {
            setcolor(13, 241, 245); //cyan
            fillarc(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat, radius, 0, 360);
            //Show on graphics
            setcolor(RED);
            setfontsize(10);
            drawtext(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat + 3 * radius, getPointOfInterestName(i), 0.00003, 0.01);
        }
    }
}

//To draw bank

void draw_bank() {
    double radius = setRadius();
    for (unsigned i = 0; i < PointOfInterestID; i++) {
        if (getPointOfInterestType(i) == "bank") {
            setcolor(210, 88, 232); //purple
            fillarc(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat, radius, 0, 360);
            //Show on graphics
            setcolor(RED);
            setfontsize(10);
            drawtext(ld_point_of_interest_posiion[i].lon, ld_point_of_interest_posiion[i].lat + 3 * radius, getPointOfInterestName(i), 0.00001, 0.01);
        }
    }

}
//Show all bank

void show_bank(void (*drawscreen_ptr) (void)) {
    if (show_B) {
        //Means the user clicked the button again so clear everything
        show_B = 0;
        re_drawscreen();
    } else
        show_B = 1;
    //Draw points of interest
    draw_bank();


    drawscreen_ptr();
}

void draw_help() {
    
    // Cut the screen area into 30x30 pieces, calculate each piece length
    double height = (trans_coord.ytop - trans_coord.ybot) / 30;
    double width = (trans_coord.xleft - trans_coord.xright) / 30;
    double halfx = (trans_coord.xleft - trans_coord.xright) / 2;
    double halfy = (trans_coord.ytop - trans_coord.ybot) / 2;
    // Calculate the central point on the screen
    double centerx = trans_coord.xright + halfx;
    double centery = trans_coord.ytop - halfy;
    setcolor(198, 238, 245); // kind blue
    fillrect(trans_coord.xright + 2 * width, trans_coord.ytop - 2 * height, trans_coord.xleft - 2 * width, trans_coord.ybot + 2 * height);
    string find = "FIND: Enter two streets name in terminal, the intersection will pop on the screen and correspoding information will show in terminal.";
    string find_path_intern1 = "PathInter: a. Get 2 intersections by input 4 street names in terminal, the street will show up on the screen.";
    string find_path_intern2 = "b. The RED poind stands for staring point and the BLUE point stands for ending point. The detailed travel guide will show in the left side of the screen. ";
    string find_path_POI1 = "PathPOI: a. Get 1 intersections by input 2 street names, and input 1 point of interest name. The street will show up on the screen.";
    string find_path_POI2 = "b. The RED poind stands for staring point, the BLUE point stands for ending point and the purple point stands for POI ";
    string find_path_POI3 = "c. The detailed travel guide will show in the left side of the screen.";
    string Intersection = "Intersection: Show up all intersections on the screen.";
    string POI = "POI: Show up all point of interest on the screen.";
    string oneway1 = "One Way: a. On any segment, the only legal direction is from BLUE to GREEN.";
    string oneway2 = "b. At any intersection, only moving between different colors is legal.";
    string Fast_Food = "Fast Food: Auto search all fast food on the screen.";
    string bank = "Bank: Auto search all bank on the screen.";
    string click_find_path1 = "ClickFindPath: a.Click two intersections on the screen, the shortest travel time street will show up on the screen.";
    string click_find_path2 = "b. The detailed travel guide will show in the left side of the screen.";
    string search_box = "Search Box: Enter Street name, intersections to get the path.";
    string close = "Before click new button, please click again to stop the old one. ";
    
    setcolor(ORANGE);
    setfontsize(12);
    drawtext(centerx, centery + 12 * height, close, 0.2, 0.2);
    setcolor(BLACK);
    setfontsize(10);
    drawtext(centerx, centery + 11 * height, find, 0.2, 0.2);
    drawtext(centerx, centery + 10 * height, find_path_intern1, 0.02, 0.02);
    drawtext(centerx, centery + 8 * height, find_path_intern2, 0.02, 0.02);
    drawtext(centerx, centery + 6 * height, find_path_POI1, 0.02, 0.02);
    drawtext(centerx, centery + 5 * height, find_path_POI2, 0.02, 0.02);
    drawtext(centerx, centery + 4 * height, find_path_POI3, 0.02, 0.02);
    drawtext(centerx, centery + 2 * height, Intersection, 0.02, 0.02);
    drawtext(centerx, centery, POI, 0.02, 0.02);
    drawtext(centerx, centery - 2 * height, oneway1, 0.02, 0.02);
    drawtext(centerx, centery - 3 * height, oneway2, 0.02, 0.02);
    drawtext(centerx, centery - 5 * height, Fast_Food, 0.02, 0.02);
    drawtext(centerx, centery - 7 * height, bank, 0.02, 0.02);
    drawtext(centerx, centery - 9 * height, click_find_path1, 0.02, 0.02);
    drawtext(centerx, centery - 10 * height, click_find_path2, 0.02, 0.02);
    drawtext(centerx, centery - 12 * height, search_box, 0.02, 0.02);
}

void show_help(void (*drawscreen_ptr) (void)) {
    if (show_H) {
        //Means the user clicked the button again so clear everything
        show_H = 0;
        re_drawscreen();
    } else
        show_H = 1;
    //Draw points of interest
    draw_help();
    drawscreen_ptr();
}

// Set flag for click on the screen to find the path
void mouse_find() {
    mouse_flag = 1;
}

void show_mouse_find(void (*drawscreen_ptr) (void)) {
    //Means the user clicked the button again so clear everything
    if (!show_MF) {
        path_name_input.clear();
        best_path.clear();
        mouse_flag = 1;
    } else {
        mouse_flag = 0;
        show_MF = 0;
        re_drawscreen();
    }
    //Draw points of interest
    drawscreen_ptr();
}

void clear_find_vector() {
    int vector_size = intersection_ID_vector.size();
    if (vector_size == 0)
        return;

    for (int i = 0; i < vector_size; i++) {
        intersection_ID_vector.erase(intersection_ID_vector.begin());
    }
}

void draw_feature_Unknown(unsigned id) {
    if (getFeatureType(id) != Unknown)
        return;
    setcolor(231, 231, 255); //light purpel

    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Park(unsigned id) {
    if (getFeatureType(id) != Park)
        return;
    setcolor(202, 223, 170); //green



    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Beach(unsigned id) {
    if (getFeatureType(id) != Beach)
        return;
    setcolor(250, 242, 199); //yellow


    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Lake(unsigned id) {
    if (getFeatureType(id) != Lake)
        return;
    setcolor(160, 195, 255); // blue


    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_River(unsigned id) {
    if (getFeatureType(id) != River)
        return;
    setcolor(137, 183, 255); //light blue

    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Island(unsigned id) {
    if (getFeatureType(id) != Island)
        return;
    setcolor(231, 231, 255); //light purpel


    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Shoreline(unsigned id) {
    if (getFeatureType(id) != Shoreline)
        return;
    setcolor(67, 147, 255); //blue

    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Building(unsigned id) {
    if (getFeatureType(id) != Building)
        return;
    setcolor(220, 220, 220); // grey


    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Greenspace(unsigned id) {
    if (getFeatureType(id) != Greenspace)
        return;
    setcolor(194, 218, 158); //green


    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Golfcourse(unsigned id) {
    if (getFeatureType(id) != Golfcourse)
        return;
    setcolor(187, 211, 187); //light green

    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void draw_feature_Stream(unsigned id) {
    if (getFeatureType(id) != Stream)
        return;
    setcolor(137, 183, 255); //light blue

    //Only draw things within the visible area
    bool draw = 0;
    for (unsigned j = 0; j < ld_feature_position[id].size(); j++) {
        if (ld_feature_position[id][j].lon <= trans_coord.xright && ld_feature_position[id][j].lon >= trans_coord.xleft
                && ld_feature_position[id][j].lat <= trans_coord.ytop && ld_feature_position[id][j].lat >= trans_coord.ybot)
            draw = 1;
    }
    if (draw) {
        vector<LatLon> feature_points_vector = ld_feature_position[id];
        unsigned feature_tail_index = getFeaturePointCount(id) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature(feature_points_vector, feature_tail_index);
        else
            draw_open_feature(feature_points_vector, feature_tail_index);
    }

}

void act_on_FN_button(void (*drawscreen_ptr) (void)) {
    drawscreen_ptr();
}

void draw_feature_name() {
    if(zoom_count > 5){
    for (unsigned i = 0; i < FeatureID; i++) {
        vector<LatLon> feature_points_vector = ld_feature_position[i];
        unsigned feature_tail_index = getFeaturePointCount(i) - 1;
        if (feature_points_vector[0].lat == feature_points_vector[feature_tail_index].lat &&
                feature_points_vector[0].lon == feature_points_vector[feature_tail_index].lon)
            draw_closed_feature_name(feature_points_vector, feature_tail_index, i);
        else
            draw_open_feature_name(feature_points_vector, feature_tail_index, i);
    }
    }
    return;
}

void draw_closed_feature_name(vector<LatLon> & feature_points_vector, unsigned feature_tail_index, unsigned feature_id) {

    double sumx = 0;
    double sumy = 0;

    for (unsigned i = 0; i <= feature_tail_index; i++) {

        sumx += feature_points_vector[i].lon;
        sumy += feature_points_vector[i].lat;
    }

    double avex = sumx / (feature_tail_index + 1);
    double avey = sumy / (feature_tail_index + 1);

    string this_name = getFeatureName(feature_id);

    if (!show_feature_name || this_name == "<noname>")
        return;

    setcolor(BLACK);
    setfontsize(7);
    drawtext(avex, avey, this_name,
            0.000008
            //avex-feature_points_vector[0].lon, 
            , 0.005
            //avey-feature_points_vector[0].lat
            );

}

void draw_open_feature_name(vector<LatLon> & feature_points_vector, unsigned feature_tail_index, unsigned feature_id) {
    string this_name = getFeatureName(feature_id);

    if (!show_feature_name || this_name == "<noname>")
        return;

    setcolor(BLACK);
    setfontsize(7);
    drawtext(feature_points_vector[0].lon, feature_points_vector[0].lat, this_name, 0.000008, 0.005);
}

void draw_best_path(unsigned start, unsigned end, vector<unsigned> path) {

    for (unsigned i = 0; i < path.size(); i++) {
        vector<LatLon> street_segment;
        StreetSegmentInfo temp = getStreetSegmentInfo(path[i]);
        //Create curve point position vector for each segment
        street_segment.push_back(getIntersectionPosition(temp.from));
        for (unsigned j = 0; j < temp.curvePointCount; j++) {
            LatLon temp2 = getStreetSegmentCurvePoint(path[i], j);
            street_segment.push_back(temp2);
        }
        street_segment.push_back(getIntersectionPosition(temp.to));
        for (unsigned i = 0; i < street_segment.size() - 1; i++) {
            setcolor(BLUE);
            setlinewidth(3);
            drawline(street_segment[i].lon, street_segment[i].lat, street_segment[i + 1].lon, street_segment[i + 1].lat);
        }
    }

    // Get the start and end point position
    LatLon start_position = getIntersectionPosition(start);
    LatLon end_position = getIntersectionPosition(end);
    
    double radius1 = 3*setRadius();
    double radius2 = setRadius();
    // calculate the triangle for the start and end point position
    t_point start_t[3];
    start_t[0].x = start_position.lon;
    start_t[0].y = start_position.lat;
    start_t[1].x = start_position.lon-radius1/tan(48);
    start_t[1].y = start_position.lat+radius1;
    start_t[2].x = start_position.lon+radius1/tan(48);
    start_t[2].y = start_position.lat+radius1;
    t_point end_t[3];
    end_t[0].x = end_position.lon;
    end_t[0].y = end_position.lat;
    end_t[1].x = end_position.lon-radius1/tan(48);
    end_t[1].y = end_position.lat+radius1;
    end_t[2].x = end_position.lon+radius1/tan(48);
    end_t[2].y = end_position.lat+radius1;
    setcolor(247,88,76); //kind red
    fillpoly(start_t, 3);
    fillarc(start_position.lon, start_position.lat+4.5*setRadius(), radius1, 0, 360);
    setcolor(0,176,255); //kind blue
    fillpoly(end_t, 3);
    fillarc(end_position.lon, end_position.lat+4.5*setRadius(), radius1, 0, 360);
    setcolor(BLACK);
    fillarc(start_position.lon, start_position.lat+4.5*setRadius(), radius2, 0, 360);
    fillarc(end_position.lon, end_position.lat+4.5*setRadius(), radius2, 0, 360);

    string start_name = getIntersectionName(start);
    string end_name = getIntersectionName(end);
    setcolor(RED);
    drawtext(start_position.lon, start_position.lat-2*setRadius(), start_name, 0.02,0.02);
    drawtext(end_position.lon, end_position.lat-2*setRadius(), end_name, 0.02,0.02);
    print_path(path, start, end);
}

void seach_box_display(void(*drawscreen_ptr)(void)){
    display_search_box = !display_search_box;
    if(!display_search_box){
        input_string_from_textbox.clear();
        char_in_command=0;
        drawscreen_ptr();
        return;
    }
    draw_search_box();
    draw_text_in_the_box();
}

void draw_search_box(){
    t_bound_box this_scale = get_visible_world();
    float this_height= this_scale.get_height();
    float this_width=this_scale.get_width();
    float this_xcenter=this_scale.get_xcenter();
    float this_ycenter=this_scale.get_ycenter();
    
    float box_buttom_left_x=this_xcenter-(this_width/8);
    float box_buttom_left_y=this_ycenter-(this_height/14);
    float box_top_right_x=this_xcenter+(this_width/8);
    float box_top_right_y=this_ycenter+(this_height/14);
    
    
    setcolor(WHITE);
    fillrect(box_buttom_left_x, box_buttom_left_y,box_top_right_x, box_top_right_y );
}

void act_on_key_press (char c, int keysym){
    if (!display_search_box)
        return;
    
    textbox_input_parser(c, keysym);
    
        cout<<c<<keysym<<endl;
        draw_search_box();
    draw_text_in_the_box();
    
}

void draw_text_in_the_box(){
    t_bound_box this_scale = get_visible_world();
    float this_height= this_scale.get_height();
    float this_width=this_scale.get_width();
    float this_xcenter=this_scale.get_xcenter();
    float this_ycenter=this_scale.get_ycenter();
    

    
    
        setcolor(BLACK);
        setfontsize(30);
        drawtext(this_xcenter, this_ycenter, vector_of_char_to_string(input_string_from_textbox), this_width/4, this_height/14);
        
    
}

void textbox_input_parser(char c, int keysym){
    if (keysym==65507||keysym==65505||keysym==65509||keysym==65289||keysym==65515||keysym==65513||keysym==65514||
            keysym==65516||keysym==65383||keysym==65508||keysym==65506||keysym==65362||keysym==65364||keysym==65361||
            keysym==65363||keysym==65360||keysym==65367)
        return;
    
    else if (keysym==65421||keysym==65293){
        string_need_handling.clear();
        string_need_handling=input_string_from_textbox;
        input_string_from_textbox.clear();
        char_in_command=0;
    }//as "Enter" pressed
    
    else if (keysym==65288||keysym==65535){
        if (char_in_command<=0||char_in_command>input_string_from_textbox.size())
            return;
        else {
            input_string_from_textbox.erase(input_string_from_textbox.begin()+char_in_command-1);
            char_in_command--;
        }
    }//as Delete or Backspace pressed
    
    
    
    else {
        input_string_from_textbox.insert(input_string_from_textbox.begin()+char_in_command, c);
        char_in_command++;
        cout<<input_string_from_textbox.size()<<endl;
    
    }
    
}

string vector_of_char_to_string(vector<char> the_vector){
    
    string Empty_sttring;
    if (the_vector.size()<=0)
        return Empty_sttring;
    std::string returning_string(the_vector.begin(), the_vector.end());
    
    
    
    return returning_string;
    
}

bool test_partial_name(string input){
    vector<string> output;
    bool print;
    for (unsigned i = 0; i < StreetID; i++) {
        string temp = getStreetName(i);
        size_t found = temp.find(input);
        if(found == 0 && find(output.begin(), output.end(), temp)== output.end()){
            if(temp == input){
                cout<<"This is a correct street name."<<endl;
                cout<<"--------------------------------------------------"<<endl;
                print = false;
                again = false;
                return true;
            }
            else{
                output.push_back(temp);
                print = true;
            }
        }
    }
    if(output.size() == 0){
        return false;
    }
    else if(print == true){
        for(unsigned i = 0; i<output.size(); i++){
            cout<<output[i]<<endl;
        }
        cout<<"--------------------------------------------------"<<endl;
        again = true;
        return true;
    }
}

int string_to_int(string input_string){
    int size=input_string.size();
    int output_int=0;
    for(int i; i<size; i++){
        int p=0;
        char_to_int(input_string[i], p);
        output_int=output_int*10+p;
    }
    
    return output_int;
}
bool char_to_int(char & input_char, int & i){
    if (input_char=='0')
        i=0;
    else if (input_char=='1')
        i=1;
    else if (input_char=='2')
        i=2;
    else if (input_char=='3')
        i=3;
    else if (input_char=='4')
        i=4;
    else if (input_char=='5')
        i=5;
    else if (input_char=='6')
        i=6;
    else if (input_char=='7')
        i=7;
    else if (input_char=='8')
        i=8;
    else if (input_char=='9')
        i=9;
    else {
        i = -1;
        return false;
    }
    return true;
    
}

void print_path( vector<unsigned> segment_vector, unsigned intersect_id_start, unsigned intersect_id_end){
    int segment_number=segment_vector.size();
    unsigned prev_node=intersect_id_start;
    unsigned next_node, current_node;

    // Create a travel guide vector
    vector<string> turn_guide;
    string open = "The travel guide";
    turn_guide.push_back(open);
    
    // convert double to string, only keep 2 decimals
    double travel_time = compute_path_travel_time(segment_vector);
    int time_to_int = travel_time*100;
    travel_time=time_to_int/100.0;
    ostringstream strs;
    strs << travel_time;
    string str = strs.str();
    
    string time = "The estimate travel time is " +str+ " min(s).";
    turn_guide.push_back(time);
    string start_point = "Start at " + getIntersectionName(intersect_id_start);
    turn_guide.push_back(start_point);
    string start_seg = "Take " + getStreetName(getStreetSegmentInfo(segment_vector[0]).streetID);
    turn_guide.push_back(start_seg);
    
    
    int dis_between_turns=0;
    
    for(int i=0; i<segment_number-1; i++){
        StreetSegmentInfo segment_before=getStreetSegmentInfo(segment_vector[i]);
        StreetSegmentInfo segment_after=getStreetSegmentInfo(segment_vector[i+1]);
        
        dis_between_turns+=int(find_street_segment_length(segment_vector[i]));
        
        t_point point_before, point_current, point_after;

        unsigned possible_node_1=segment_before.from;
        unsigned possible_node_2=segment_before.to;
        
        if (prev_node==possible_node_1){
            current_node=possible_node_2;
            if (segment_before.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i], segment_before.curvePointCount-1).lon, getStreetSegmentCurvePoint(segment_vector[i], segment_before.curvePointCount-1).lat);
                point_before=temp;
            }
            else {
                t_point temp(getIntersectionPosition(prev_node).lon, getIntersectionPosition(prev_node).lat);
                point_before=temp;
            }
        }
        else{
            current_node=possible_node_1;
            if (segment_before.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i], 0).lon, getStreetSegmentCurvePoint(segment_vector[i], 0).lat);
                point_before=temp;
            }
            else {
                t_point temp(getIntersectionPosition(prev_node).lon, getIntersectionPosition(prev_node).lat);
                point_before=temp;
            }
            
        }
        unsigned possible_node_3=segment_after.from;
        unsigned possible_node_4=segment_after.to;
        
        if (current_node==possible_node_4){
            next_node=possible_node_3;
            if (segment_after.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i+1], segment_after.curvePointCount-1).lon, getStreetSegmentCurvePoint(segment_vector[i+1], segment_after.curvePointCount-1).lat);
                point_after=temp;
            }
            else {
                t_point temp(getIntersectionPosition(next_node).lon, getIntersectionPosition(next_node).lat);
                point_after=temp;
            }
        }
        else{
            next_node=possible_node_4;
            if (segment_after.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i+1], 0).lon, getStreetSegmentCurvePoint(segment_vector[i+1], 0).lat);
                point_after=temp;
            }
            else {
                t_point temp(getIntersectionPosition(next_node).lon, getIntersectionPosition(next_node).lat);
                point_after=temp;
            }
        }
        
        t_point temp(getIntersectionPosition(current_node).lon, getIntersectionPosition(current_node).lat);
        point_current=temp;
        
        if (segment_before.streetID!=segment_after.streetID){
            string turn0, turn1,turn2;
            turn0 = "After " + to_string(dis_between_turns) + " meters, ";
            bool direction=left_or_right( point_before,point_current,point_after);
            if (!direction){
                
                turn1 = "at intersection " + getIntersectionName(current_node);
                turn2 = "Turn left to " + getStreetName(segment_after.streetID);
            }
            else {
                turn1 = "At intersection " + getIntersectionName(current_node);
                turn2= "Turn right to " + getStreetName(segment_after.streetID);
            }  
            turn_guide.push_back(turn0);
            turn_guide.push_back(turn1);
            turn_guide.push_back(turn2);
            dis_between_turns=0;
            // Draw each turning point name
        setfontsize(10);
        setcolor(255,97,31);
        drawtext(getIntersectionPosition(current_node).lon, getIntersectionPosition(current_node).lat, getIntersectionName(current_node), 0.02, 0.02);
        
        }
        prev_node=current_node;
    }
    string end1 = "You will reach";
    string end2 = getIntersectionName(intersect_id_end) + " in " + to_string(dis_between_turns) + " meters, ";
    turn_guide.push_back(end1);
    turn_guide.push_back(end2);
    // Show travel guide information on screen   
    int count = turn_guide.size();
    if(count > 1) {
        double height = (trans_coord.ytop - trans_coord.ybot) / 30;
        double width = (trans_coord.xleft - trans_coord.xright) / 3;
        setcolor(238, 173, 255); // kind blue
        // count for draw space on screen
        fillrect(trans_coord.xleft - width, trans_coord.ytop, trans_coord.xleft, trans_coord.ytop-(count+6)*height);
        setcolor(BLACK);
        setfontsize(20);
        drawtext(trans_coord.xleft - width/2, trans_coord.ytop - 3*height, turn_guide[0], 0.2, 0.2);
        setfontsize(15);
        drawtext(trans_coord.xleft - width/2, trans_coord.ytop - 5*height, turn_guide[1], 0.2, 0.2);
        setfontsize(10);
        for(int i = 2; i <count; i++){
            drawtext(trans_coord.xleft - width/2, trans_coord.ytop - (5+i)*height, turn_guide[i], 0.2, 0.2);
        }
    }
}

bool left_or_right(t_point point_before,t_point point_current,t_point point_after){
    double original_x=(point_current.x-point_before.x)*1000000;
    double original_y=(point_current.y-point_before.y)*1000000;
    
    double next_x=(point_after.x-point_current.x)*1000000;
    
    double next_y=(point_after.y-point_current.y)*1000000;
    
    double transfer_x=0.0-original_y;
    double transfer_y=original_x;
    
    double product=transfer_x*next_x+transfer_y*next_y;
    
    if (product>0) return false;//for left
    else return true;//for right
    
    
    
}

void act_on_FindPathPOI_func(void (*drawscreen_ptr) (void)){
    
    show_FPPOI=!show_FPPOI;
    if (!show_FPPOI){
        best_path.clear();
        drawscreen();
        return;
    }
    
    find_func=1;
    cout<<"Please enter the start point: "<<endl;
    unsigned start_node, end_node;
    show_path_input.clear();
    intersection_ID_vector.clear();
 
    act_on_Find_func();
    
    int vector_size = intersection_ID_vector.size();
    if(vector_size ==0){
        drawscreen_ptr();
        return;
    }
    
    else if(vector_size>1){
        cout<<"Please choose an intersection from above"<<endl;
        int index_wanted;
        string temp;
        getline(cin, temp);
        index_wanted=string_to_int(temp);
        while(index_wanted<0||index_wanted>=vector_size){
            cout<<"no such intersection, please try again"<<endl;
            temp.clear();
            getline(cin, temp);
            index_wanted=string_to_int(temp);
        }
        
        start_node=intersection_ID_vector[index_wanted];
    }
    
    else start_node=intersection_ID_vector[0];
    
    intersection_ID_vector.clear();
    
    cout<<"Please enter the end POI: "<<endl;
    string POI_wanted;
    getline(cin, POI_wanted);
    
    bool partial_test1 = test_partial_name_for_POI(POI_wanted);
 
    do {
        if (!partial_test1) { // Check the input name is valid
            cout << "The input is not a correct POI name. Please enter again: " << endl;
            getline(cin, POI_wanted);
            partial_test1 = test_partial_name_for_POI(POI_wanted);

        }
        else {
            while (again_POI == true) { // Check the partial input name 
                cout << "Please choose POI name: " << endl;
                getline(cin, POI_wanted);
                partial_test1 = test_partial_name_for_POI(POI_wanted);
                if (partial_test1 == false)
                    break;
            }
        }
        if ((!again_POI)&&partial_test1) break;
    } while (1);
    
    again_POI = false;
    
    best_path=find_path_to_point_of_interest (start_node, POI_wanted);
    
    if (best_path.size()==0){
        cout<<"No such path"<<endl;
        show_path_input.clear();
        find_func=0;
        show_FPPOI=!show_FPPOI;
        drawscreen();
        return;
    }
    
    
    int final_index=best_path.size()-1;
    int before_final_index=final_index-1;
    
    if (final_index==0)
        before_final_index=final_index;
    
    
    unsigned final_from=getStreetSegmentInfo(best_path[final_index]).from;
    unsigned final_to=getStreetSegmentInfo(best_path[final_index]).to;
    unsigned before_from=getStreetSegmentInfo(best_path[before_final_index]).from;
    unsigned before_to=getStreetSegmentInfo(best_path[before_final_index]).to;
    
    if(before_to==final_from)
        end_node=final_to;
    else if(before_from==final_to)
        end_node=final_from;
    else if(before_to==final_to)
        end_node=final_from;
    else end_node=final_to;
    
    if (final_index==0){
        if (final_from==start_node)
            end_node=final_to;
        else end_node=final_from;
    }
    
    closeset_POI_ID=get_closest_POI_to_a_given_Intersection_from_given_POI_name(POI_wanted, end_node);
    
    find_func=0;
    show_path_input.clear();
    show_path_input.push_back(start_node);
    show_path_input.push_back(end_node);
    
    draw_best_pathPOI(start_node, end_node, best_path,closeset_POI_ID);
        re_drawscreen();

}

void draw_best_pathPOI(unsigned start, unsigned end, vector<unsigned> path, unsigned POI_ID){
    draw_best_path(start, end, path);
    
    
    LatLon POI_position=getPointOfInterestPosition(POI_ID);
    double radius1 = 3*setRadius();
    double radius2 = setRadius();
    t_point POI_t[3];
    POI_t[0].x = POI_position.lon;
    POI_t[0].y = POI_position.lat;
    POI_t[1].x = POI_position.lon-radius1/tan(48);
    POI_t[1].y = POI_position.lat+radius1;
    POI_t[2].x = POI_position.lon+radius1/tan(48);
    POI_t[2].y = POI_position.lat+radius1;
    
    setcolor(171,39,192); //purple
    fillpoly(POI_t, 3);
    fillarc(POI_position.lon, POI_position.lat+4.5*setRadius(), radius1, 0, 360);
    setcolor(BLACK);
    fillarc(POI_position.lon, POI_position.lat+4.5*setRadius(), radius2, 0, 360);
    setcolor(RED);
    drawtext(POI_position.lon, POI_position.lat-2*setRadius(), getPointOfInterestName(POI_ID), 0.02,0.02);
    
    print_pathPOI(path, start, end, POI_ID);
    
}


unsigned get_closest_POI_to_a_given_Intersection_from_given_POI_name(string POI_name, unsigned given_intersection_id){
    vector<unsigned> POI_with_same_name;
    for (unsigned i=0; i<PointOfInterestID; i++){
        if (POI_name==getPointOfInterestName(i))
            POI_with_same_name.push_back(i);
        
    }
    
    
    LatLon given_intersection=getIntersectionPosition(given_intersection_id);
    int j=POI_with_same_name.size();
    unsigned returning_POI_index_in_vector=0;
    double distance=find_distance_between_two_points(given_intersection, getPointOfInterestPosition(POI_with_same_name[0]));
    
    for (int i=0; i<j; i++){
        double newDis=find_distance_between_two_points(given_intersection, getPointOfInterestPosition(POI_with_same_name[i]));
        if (newDis<distance){
            distance=newDis;
            returning_POI_index_in_vector=i;
        }
    }
    
    
    unsigned returning_POI_ID=POI_with_same_name[returning_POI_index_in_vector];
    return returning_POI_ID;
    
}


void print_pathPOI( vector<unsigned> segment_vector, unsigned intersect_id_start, unsigned intersect_id_end, unsigned POI_ID){
    
    int segment_number=segment_vector.size();
    unsigned prev_node=intersect_id_start;
    unsigned next_node, current_node;

    // Create a travel guide vector
    vector<string> turn_guide;
    string open = "The travel guide";
    turn_guide.push_back(open);
    
    // convert double to string, only keep 2 decimals
    double travel_time = compute_path_travel_time(segment_vector);
    int time_to_int = travel_time*100;
    travel_time=time_to_int/100.0;
    ostringstream strs;
    strs << travel_time;
    string str = strs.str();
    
    string time = "The estimate travel time is " +str+ " min(s).";
    turn_guide.push_back(time);
    string start_point = "Start at " + getIntersectionName(intersect_id_start);
    turn_guide.push_back(start_point);
    string start_seg = "Take " + getStreetName(getStreetSegmentInfo(segment_vector[0]).streetID);
    turn_guide.push_back(start_seg);
    
    
    int dis_between_turns=0;
    
    for(int i=0; i<segment_number-1; i++){
        StreetSegmentInfo segment_before=getStreetSegmentInfo(segment_vector[i]);
        StreetSegmentInfo segment_after=getStreetSegmentInfo(segment_vector[i+1]);
        
        dis_between_turns+=int(find_street_segment_length(segment_vector[i]));
        
        t_point point_before, point_current, point_after;

        unsigned possible_node_1=segment_before.from;
        unsigned possible_node_2=segment_before.to;
        
        if (prev_node==possible_node_1){
            current_node=possible_node_2;
            if (segment_before.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i], segment_before.curvePointCount-1).lon, getStreetSegmentCurvePoint(segment_vector[i], segment_before.curvePointCount-1).lat);
                point_before=temp;
            }
            else {
                t_point temp(getIntersectionPosition(prev_node).lon, getIntersectionPosition(prev_node).lat);
                point_before=temp;
            }
        }
        else{
            current_node=possible_node_1;
            if (segment_before.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i], 0).lon, getStreetSegmentCurvePoint(segment_vector[i], 0).lat);
                point_before=temp;
            }
            else {
                t_point temp(getIntersectionPosition(prev_node).lon, getIntersectionPosition(prev_node).lat);
                point_before=temp;
            }
            
        }
        unsigned possible_node_3=segment_after.from;
        unsigned possible_node_4=segment_after.to;
        
        if (current_node==possible_node_4){
            next_node=possible_node_3;
            if (segment_after.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i+1], segment_after.curvePointCount-1).lon, getStreetSegmentCurvePoint(segment_vector[i+1], segment_after.curvePointCount-1).lat);
                point_after=temp;
            }
            else {
                t_point temp(getIntersectionPosition(next_node).lon, getIntersectionPosition(next_node).lat);
                point_after=temp;
            }
        }
        else{
            next_node=possible_node_4;
            if (segment_after.curvePointCount!=0){
                t_point temp(getStreetSegmentCurvePoint(segment_vector[i+1], 0).lon, getStreetSegmentCurvePoint(segment_vector[i+1], 0).lat);
                point_after=temp;
            }
            else {
                t_point temp(getIntersectionPosition(next_node).lon, getIntersectionPosition(next_node).lat);
                point_after=temp;
            }
        }
        
        t_point temp(getIntersectionPosition(current_node).lon, getIntersectionPosition(current_node).lat);
        point_current=temp;
        
        if (segment_before.streetID!=segment_after.streetID){
            string turn0, turn1,turn2;
            turn0 = "After " + to_string(dis_between_turns) + " meters, ";
            bool direction=left_or_right( point_before,point_current,point_after);
            if (!direction){
                
                turn1 = "at intersection " + getIntersectionName(current_node);
                turn2 = "Turn left to " + getStreetName(segment_after.streetID);
            }
            else {
                turn1 = "At intersection " + getIntersectionName(current_node);
                turn2= "Turn right to " + getStreetName(segment_after.streetID);
            }  
            turn_guide.push_back(turn0);
            turn_guide.push_back(turn1);
            turn_guide.push_back(turn2);
            dis_between_turns=0;
            // Draw each turning point name
        setfontsize(10);
        setcolor(255,97,31);
        drawtext(getIntersectionPosition(current_node).lon, getIntersectionPosition(current_node).lat, getIntersectionName(current_node), 0.02, 0.02);
        
        }
        prev_node=current_node;
    }
    string end1 = "You will reach";
    string end2 = getIntersectionName(intersect_id_end) + " in " + to_string(dis_between_turns) + " meters, ";
    string end3="The "+getPointOfInterestName(POI_ID)+" is around there.";
    turn_guide.push_back(end1);
    turn_guide.push_back(end2);
    turn_guide.push_back(end3);

    // Show travel guide information on screen   
    int count = turn_guide.size();
    if(count > 1) {
        double height = (trans_coord.ytop - trans_coord.ybot) / 30;
        double width = (trans_coord.xleft - trans_coord.xright) / 3;
        setcolor(238, 173, 255); // kind blue
        // count for draw space on screen
        fillrect(trans_coord.xleft - width, trans_coord.ytop, trans_coord.xleft, trans_coord.ytop-(count+6)*height);
        setcolor(BLACK);
        setfontsize(20);
        drawtext(trans_coord.xleft - width/2, trans_coord.ytop - 3*height, turn_guide[0], 0.2, 0.2);
        setfontsize(15);
        drawtext(trans_coord.xleft - width/2, trans_coord.ytop - 5*height, turn_guide[1], 0.2, 0.2);
        setfontsize(10);
        for(int i = 2; i <count; i++){
            drawtext(trans_coord.xleft - width/2, trans_coord.ytop - (5+i)*height, turn_guide[i], 0.2, 0.2);
        }
    }
}

bool test_partial_name_for_POI(string input){
    vector<string> output;
    bool print;
    for (unsigned i = 0; i <  PointOfInterestID; i++) {
        string temp = getPointOfInterestName(i);
        size_t found = temp.find(input);
        if(found == 0 && find(output.begin(), output.end(), temp)== output.end()){
            if(temp == input){
                cout<<"This is a correct POI name."<<endl;
                cout<<"--------------------------------------------------"<<endl;
                print = false;
                again_POI = false;
                return true;
            }
            else{
                output.push_back(temp);
                print = true;
            }
        }
    }
    if(output.size() == 0){
        return false;
    }
    else if(print == true){
        for(unsigned i = 0; i<output.size(); i++){
            cout<<output[i]<<endl;
        }
        cout<<"--------------------------------------------------"<<endl;
        again_POI = true;
        return true;
    }
}