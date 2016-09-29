#include <vector>
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <algorithm>
#include "m3.h"
#include "m4.h"
#include "m1.h"
#include <signal.h>
#include <unistd.h>
#include "StreetsDatabaseAPI.h"
#include "algorithm"
#include <vector>
#include <string>
#include "m1.h"
#include <queue>
#include <map>
#include <unordered_map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <thread>
#include <assert.h>
typedef void (*sighandler_t)(int);
using namespace std;
extern vector<LatLon> ld_intersection_position;
extern vector<unsigned> intersection_segment_count;
extern vector<vector<unsigned>> ld_intersection_segment;
extern vector<StreetSegmentInfo> segmentInfo;
extern unsigned SegmentID;
extern unsigned IntersectionID;
//To terminate the function
bool termination = 0;
//Stored the cost between any pair of points 
double cost[1000][1000];
//Follow the sequence of all pick-up points, all drop-off points and all depots
vector<unsigned> input;

vector<unsigned> unduplicate;
vector<unsigned> unduplicate_deliveries;

vector<unsigned> finalPath;
vector<unsigned> finalPath2;
vector<unsigned> finalPath3;
vector<unsigned> finalPath4;
vector<unsigned> finalPath5;
vector<unsigned> finalPath6;
vector<unsigned> finalPath7;
vector<unsigned> finalPath8;

vector<unsigned> neighbor;
vector<unsigned> neighbor1;
vector<unsigned> neighbor2;
vector<unsigned> neighbor3;
vector<unsigned> neighbor4;
vector<unsigned> neighbor5;
vector<unsigned> neighbor6;
vector<unsigned> neighbor7;


//To find the solution as good as possible of the post-man problem within 30 seconds
//The algorithm used here is simulated annealing

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots) {
    finalPath.clear();
    finalPath2.clear();
    finalPath3.clear();
    finalPath4.clear();
    finalPath5.clear();
    finalPath6.clear();
    finalPath7.clear();
    finalPath8.clear();
    neighbor.clear();
    neighbor1.clear();
    neighbor2.clear();
    neighbor3.clear();
    neighbor4.clear();
    neighbor5.clear();
    neighbor6.clear();
    neighbor7.clear();

    signal(SIGALRM, terminate_and_return);
    alarm(27);
    unsigned N = deliveries.size();
    unsigned M = depots.size();
    unsigned number_of_points = N * 2 + M;
    //Temperature
    double T = 400;

    for (unsigned i = 0; i < number_of_points; i++) {
        if (i < N)
            input.push_back(deliveries[i].pickUp);
        else if (i < 2 * N)
            input.push_back(deliveries[i - N].dropOff);
        else
            input.push_back(depots[i - 2 * N]);
    }

    clear_duplicate(N, M);

    int NUM_THREADS;
    int length;
    if (unduplicate.size() <= 8) {
        NUM_THREADS = unduplicate.size();
        length = 1;
    } else {
        NUM_THREADS = 8;
        length = unduplicate.size() / 7;
    }
    time_t now;
    time_t end;
    time(&now);

    thread myThread[NUM_THREADS];

    for (int ithr = 0; ithr < NUM_THREADS; ithr++) {
        myThread[ithr] = thread(cost_helper, ithr, length);
    }

    for (int ithr = 0; ithr < NUM_THREADS; ithr++) {
        myThread[ithr].join();
    }

    time(&end);
    cout << (double) difftime(now, end) << "cost" << endl;
    //Call cost_function here to initialize 2-dimensional array cost
    //total_cost();
    /*for(unsigned i = 0; i < unduplicate.size(); i ++){
        for(unsigned j = 0; j < unduplicate.size(); j ++){
            cout << cost[i][j] << " ";
        }
        cout << endl;
    }*/
    thread first_thread[8];
    for (int i = 0; i < 8; i++) {
        first_thread[i] = thread(initialize, i, N, M, 0, 0);
    }
    for (int i = 0; i < 8; i++) {
        first_thread[i].join();
    }
    
    thread SA_thread[8];
    for (int i = 0; i < 8; i++) {
        if (i == 0)
            SA_thread[i] = thread(SA, ref(neighbor), N, M);
        if (i == 1)
            SA_thread[i] = thread(SA, ref(neighbor1), N, M);
        if (i == 2)
            SA_thread[i] = thread(SA, ref(neighbor2), N, M);
        if (i == 3)
            SA_thread[i] = thread(SA, ref(neighbor3), N, M);
        if (i == 4)
            SA_thread[i] = thread(SA, ref(neighbor4), N, M);
        if (i == 5)
            SA_thread[i] = thread(SA, ref(neighbor5), N, M);
        if (i == 6)
            SA_thread[i] = thread(SA, ref(neighbor6), N, M);
        if (i == 7)
            SA_thread[i] = thread(SA, ref(neighbor7), N, M);
    }
    for (int i = 0; i < 8; i++) {
        SA_thread[i].join();
    }

    //Retrieve the path in terms of segments

    clock_t c = clock();
    thread second_thread[8];
    for (int i = 0; i < 8; i++) {
        if (i == 0)
            second_thread[i] = thread(calculateFinalPath, neighbor, i);
        if (i == 1)
            second_thread[i] = thread(calculateFinalPath, neighbor1, i);
        if (i == 2)
            second_thread[i] = thread(calculateFinalPath, neighbor2, i);
        if (i == 3)
            second_thread[i] = thread(calculateFinalPath, neighbor3, i);
        if (i == 4)
            second_thread[i] = thread(calculateFinalPath, neighbor4, i);
        if (i == 5)
            second_thread[i] = thread(calculateFinalPath, neighbor5, i);
        if (i == 6)
            second_thread[i] = thread(calculateFinalPath, neighbor6, i);
        if (i == 7)
            second_thread[i] = thread(calculateFinalPath, neighbor7, i);
    }
    for (int i = 0; i < 8; i++)
        second_thread[i].join();
    c = clock() - c;
    cout << (double) c / CLOCKS_PER_SEC << "A*" << endl;
    double a1[8];
    a1[0] = compute_path_travel_time(finalPath);
    a1[1] = compute_path_travel_time(finalPath2);
    a1[2] = compute_path_travel_time(finalPath3);
    a1[3] = compute_path_travel_time(finalPath4);
    a1[4] = compute_path_travel_time(finalPath5);
    a1[5] = compute_path_travel_time(finalPath6);
    a1[6] = compute_path_travel_time(finalPath7);
    a1[7] = compute_path_travel_time(finalPath8);
    cout << a1[0] << endl;
    cout << a1[1] << endl;
    cout << a1[2] << endl;
    cout << a1[3] << endl;
    cout << a1[4] << endl;
    cout << a1[5] << endl;
    cout << a1[6] << endl;
    cout << a1[7] << endl;
    vector<double> compare;
    for (int i = 0; i < 8; i++) {
        compare.push_back(a1[i]);
    }
    sort(compare.begin(), compare.end());
    if (compare[0] == a1[1])
        finalPath = finalPath2;
    if (compare[0] == a1[2])
        finalPath = finalPath3;
    if (compare[0] == a1[3])
        finalPath = finalPath4;
    if (compare[0] == a1[4])
        finalPath = finalPath5;
    if (compare[0] == a1[5])
        finalPath = finalPath6;
    if (compare[0] == a1[6])
        finalPath = finalPath7;
    if (compare[0] == a1[7])
        finalPath = finalPath8;
    termination = 0;
    input.clear();

    return finalPath;
}

//Probability function
//T stands for temperature, which should be related to time already consumed
//The math expression used is p = e^((old_cost - new_cost)/T)

double p_func(double old_cost, double new_cost, double T) {
    double temp = (old_cost - new_cost) / T;
    double P;
    //Taylor series of e^temp
    P = 1 + temp / 1 + temp * temp / 2 + temp * temp * temp / 6 + temp * temp * temp * temp / 24 + temp * temp * temp * temp * temp / 120 +
            temp * temp * temp * temp * temp * temp / 720;
    return P;
}

void clear_duplicate(unsigned N, unsigned M) {
    unduplicate.clear();
    unduplicate_deliveries.clear();

    unduplicate = input;
    unduplicate_deliveries.insert(unduplicate_deliveries.begin(), input.begin(), input.begin() + 2 * N);

    sort(unduplicate.begin(), unduplicate.end());
    sort(unduplicate_deliveries.begin(), unduplicate_deliveries.end());

    for (auto iter = unduplicate_deliveries.begin(); iter != unduplicate_deliveries.end() - 1;) {
        if (*(iter + 1) == *iter)
            unduplicate_deliveries.erase(iter + 1);
        else
            iter++;
    }
    auto iter = unduplicate.begin();
    while (iter != unduplicate.end() - 1) {
        if (*(iter + 1) == *iter) {
            unduplicate.erase(iter + 1);
        } else
            iter++;
    }
}

void cost_helper(int index, int length) {
    ld_intersection_segment.resize(IntersectionID * 5);
    unsigned start = index*length;
    unsigned end = (index + 1) * length;
    if (index == 7)
        end = unduplicate.size();
    if (index == 8) {
        start = 0;
        end = unduplicate.size();
    }

    for (unsigned i = start; i < end; i++) {
        unsigned intersection = unduplicate[i];
        vector<unsigned> temp;
        if (find(unduplicate_deliveries.begin(), unduplicate_deliveries.end(), intersection) == unduplicate_deliveries.end())
            temp = unduplicate_deliveries;
        else
            temp = unduplicate;


        //Dijkstra starts here
        priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, greater<pair<double, unsigned>>> path;
        unordered_map<unsigned, node> openSet; //Nodes visited but not expanded
        unordered_map<unsigned, node> closedSet; //Nodes already expanded

        //Initialize all structures
        node start(intersection, 0);
        start.travelTime = 0;
        openSet.emplace(intersection, start);
        path.emplace(start.travelTime, intersection);

        //When open set is not empty
        while (!openSet.empty()) {
            //If already expanded, skip
            if (closedSet.find(path.top().second) != closedSet.end()) {
                path.pop();
                continue;
            }
            //Store useful data

            unsigned intersectID = path.top().second;
            node current = openSet.find(intersectID)->second;
            //Update all structures
            path.pop();
            openSet.erase(intersectID);
            closedSet.emplace(intersectID, current);

            //If found
            if (find(temp.begin(), temp.end(), intersectID) != temp.end()) {
                int j = distance(unduplicate.begin(), find(unduplicate.begin(), unduplicate.end(), intersectID));
                cost[i][j] = current.travelTime;
                temp.erase(find(temp.begin(), temp.end(), intersectID));
                while (find(temp.begin(), temp.end(), intersectID) != temp.end())
                    temp.erase(find(temp.begin(), temp.end(), intersectID));
            }
            //Expansion

            //cout << "start" << endl;
            //getIntersectionStreetSegmentCount(intersectID);
            //cout << "end" << endl;
            //cout.flush();
            for (unsigned i = 0; i < intersection_segment_count[intersectID]; i++) {
                vector<unsigned> a = ld_intersection_segment[intersectID];
                unsigned test = ld_intersection_segment[intersectID][i];
                StreetSegmentInfo info = segmentInfo[ld_intersection_segment[intersectID][i]];
                unsigned tempSegment = ld_intersection_segment[intersectID][i];
                //Chick path legality
                if (intersectID == info.from) {
                    unsigned next = info.to;
                    node temp(intersectID, tempSegment);
                    temp.travelTime += current.travelTime;
                    if (segmentInfo[current.fromSegment].streetID != segmentInfo[temp.fromSegment].streetID)
                        temp.travelTime += 0.25;
                    //If the target is already expanded or current time is not less than time stored in the target, skip
                    if (closedSet.find(next) != closedSet.end())
                        continue;
                    else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                        continue;
                    //The best till now, store
                    openSet.erase(next);
                    openSet.emplace(next, temp);
                    path.emplace(temp.travelTime, next);
                } else if (!info.oneWay) {
                    unsigned next = info.from;
                    node temp(intersectID, tempSegment);
                    temp.travelTime += current.travelTime;
                    if (segmentInfo[current.fromSegment].streetID != segmentInfo[temp.fromSegment].streetID)
                        temp.travelTime += 0.25;
                    if (closedSet.find(next) != closedSet.end())
                        continue;
                    else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                        continue;
                    openSet.erase(next);
                    openSet.emplace(next, temp);
                    path.emplace(temp.travelTime, next);
                }
            }
            //Check if finished
            if (temp.size() == 0)
                break;
        }
        //If some point(s) not found, assign -1 to corresponding position
        if (temp.size() != 0) {
            for (unsigned j = 0; j < temp.size(); j++) {
                int k = distance(unduplicate.begin(), find(unduplicate.begin(), unduplicate.end(), temp[j]));
                cost[i][k] = -1;
            }
        }
    }
    return;
}

//To set the flag when alarm signal is sent

void terminate_and_return(int dum) {
    termination = 1;
}

//To find cost of a sequence of intersection ids

double find_cost(vector<unsigned> _input) {
    double _cost = 0;
    for (unsigned i = 0; i < _input.size() - 1; i++) {
        int position1 = distance(unduplicate.begin(), find(unduplicate.begin(), unduplicate.end(), _input[i]));
        int position2 = distance(unduplicate.begin(), find(unduplicate.begin(), unduplicate.end(), _input[i + 1]));
        //If the path is illegal
        if (cost[position1][position2] < 0)
            return -1;
        //Accumulate the cost
        _cost += cost[position1][position2];
    }
    return _cost;
}


//To generate a new solution from a legal solution

vector<unsigned> newPath(vector<unsigned> oldPath, unsigned N, unsigned M, double &new_cost, double &old_cost) {
    unsigned size = oldPath.size();
    int position1;
    int position2;
    bool ok = 0;
    srand(time(NULL));
    position1 = rand() % (size - 2) + 1;
    position2 = rand() % (size - 2) + 1;
    while (position1 == position2) {
        position2 = rand() % (size - 2) + 1;
    }
    ok = 1;
    if (position2 < position1) {
        int temp = position1;
        position1 = position2;
        position2 = temp;
    }
    for (unsigned i = 0; i < N; i++) {
        if (input[i] == oldPath[position1] && find(oldPath.begin() + position1 + 1, oldPath.end() + position2 + 1, input[i + N]) != oldPath.end()) {
            ok = 0;
        }
    }
    for (unsigned i = N; i < 2 * N; i++) {
        if (input[i] == oldPath[position2] && find(oldPath.begin() + position1, oldPath.begin() + position2, input[i - N]) != oldPath.end()) {
            ok = 0;
        }
    }
    old_cost = find_cost({oldPath[0], oldPath[1]}) + find_cost({oldPath[position1 - 1], oldPath[position1]}) + find_cost({oldPath[position1], oldPath[position2]})
    + find_cost({oldPath[position2], oldPath[position2 + 1]});
    //Swap the two element
    if (ok) {
        unsigned temp = oldPath[position1];
        oldPath[position1] = oldPath[position2];
        oldPath[position2] = temp;
    }

    int dummy = rand() % M;
    if (find_cost({input[2 * N + dummy], oldPath[1]}) > 0)
    oldPath[0] = input[2 * N + dummy];

    new_cost = find_cost({oldPath[0], oldPath[1]}) + find_cost({oldPath[position1 - 1], oldPath[position1]}) + find_cost({oldPath[position1], oldPath[position2]})
    + find_cost({oldPath[position2], oldPath[position2 + 1]});

    return oldPath;
}

//To find the first solution

vector<unsigned> basicGreedy(unsigned N, unsigned M, bool turned_on) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime);
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    int result = GreedyHelper(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput, time, 0);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = GreedyHelper(result, pickUp, dropOff, N, M, fakeN, fakeInput, time, 0);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

unsigned Dijkstra(unsigned inter, vector<unsigned> &pickUp, vector<unsigned> &dropOff, unsigned N, unsigned M, unsigned &fakeN, vector<unsigned> &fakeInput) {
    unsigned intersection = inter;

    //Dijkstra starts here
    priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, greater<pair<double, unsigned>>> path;
    unordered_map<unsigned, node> openSet; //Nodes visited but not expanded
    unordered_map<unsigned, node> closedSet; //Nodes already expanded

    //Initialize all structures
    node start(intersection, 0);
    start.travelTime = 0;
    openSet.emplace(intersection, start);
    path.emplace(start.travelTime, intersection);

    //When open set is not empty
    while (!openSet.empty()) {
        //If already expanded, skip
        if (closedSet.find(path.top().second) != closedSet.end()) {
            path.pop();
            continue;
        }
        //Store useful data

        unsigned intersectID = path.top().second;
        node current = openSet.find(intersectID)->second;
        //Update all structures
        path.pop();

        bool ok = 0;
        //If found in pickUP
        if (find(pickUp.begin(), pickUp.end(), intersectID) != pickUp.end()) {
            ok = 1;
            pickUp.erase(find(pickUp.begin(), pickUp.end(), intersectID));
            while (find(pickUp.begin(), pickUp.end(), intersectID) != pickUp.end())
                pickUp.erase(find(pickUp.begin(), pickUp.end(), intersectID));
            //return intersectID;
        }
        //If found in dropOff
        if (find(dropOff.begin(), dropOff.end(), intersectID) != dropOff.end()) {
            unsigned size = dropOff.size();
            for (unsigned i = 0; i < fakeN; i++) {
                if (fakeInput[i + fakeN] == intersectID) {
                    unsigned corresPickup = fakeInput[i];
                    //If picked up
                    if (find(pickUp.begin(), pickUp.end(), corresPickup) == pickUp.end()) {
                        fakeInput[i] = -1;
                        fakeInput[i + fakeN] = -1;
                        dropOff.erase(find(dropOff.begin(), dropOff.end(), intersectID));
                    }
                }
            }
            if (dropOff.size() != size)
                ok = 1;
        }
        if (ok == 1)
            return intersectID;
        openSet.erase(intersectID);
        closedSet.emplace(intersectID, current);
        //Expansion
        for (unsigned i = 0; i < getIntersectionStreetSegmentCount(intersectID); i++) {
            StreetSegmentInfo info = getStreetSegmentInfo(getIntersectionStreetSegment(intersectID, i));
            unsigned tempSegment = getIntersectionStreetSegment(intersectID, i);
            //Chick path legality
            if (intersectID == info.from) {
                unsigned next = info.to;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                //If the target is already expanded or current time is not less than time stored in the target, skip
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                //The best till now, store
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            } else if (!info.oneWay) {
                unsigned next = info.from;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            }
        }
        //Check if finished
        if (pickUp.empty() && dropOff.empty())
            break;
    }

    //If some point(s) not found, assign -1 to corresponding position
    if (!pickUp.empty() || !dropOff.empty()) {
        return -1;
    }

}


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
// Dijkstra algorithm

int find_nearest_point(unsigned intersect_id_start, vector<unsigned> targets, double& time) {
    //Dijkstra starts here
    priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, greater<pair<double, unsigned>>> path;
    unordered_map<unsigned, node> openSet; //Nodes visited but not expanded
    unordered_map<unsigned, node> closedSet; //Nodes already expanded

    //Initialize all structures
    node start(intersect_id_start, 0);
    start.travelTime = 0;
    openSet.emplace(intersect_id_start, start);
    path.emplace(start.travelTime, intersect_id_start);

    //When open set is not empty
    while (!openSet.empty()) {
        //If already expanded, skip
        if (closedSet.find(path.top().second) != closedSet.end()) {
            path.pop();
            continue;
        }
        //Store useful data

        unsigned intersectID = path.top().second;
        node current = openSet.find(intersectID)->second;
        //Update all structures
        path.pop();
        openSet.erase(intersectID);
        closedSet.emplace(intersectID, current);

        //If found
        if (find(targets.begin(), targets.end(), intersectID) != targets.end()) {
            time = current.travelTime;
            return intersectID;
        }
        //Expansion
        for (unsigned i = 0; i < getIntersectionStreetSegmentCount(intersectID); i++) {
            StreetSegmentInfo info = getStreetSegmentInfo(getIntersectionStreetSegment(intersectID, i));
            unsigned tempSegment = getIntersectionStreetSegment(intersectID, i);
            //Chick path legality
            if (intersectID == info.from) {
                unsigned next = info.to;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                //If the target is already expanded or current time is not less than time stored in the target, skip
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                //The best till now, store
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            } else if (!info.oneWay) {
                unsigned next = info.from;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            }
        }
    }

    //If failure
    return -1;
}

int GreedyHelper(unsigned inter, vector<unsigned> &pickUp, vector<unsigned> &dropOff, unsigned N, unsigned M, unsigned &fakeN, vector<unsigned> &fakeInput, double& Time, bool turned_on) {
    assert(inter <= IntersectionID);

    priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, greater<pair<double, unsigned>>> path;
    for (unsigned i = 0; i < unduplicate.size(); i++) {
        path.emplace(find_distance_between_two_points(ld_intersection_position[inter], ld_intersection_position[unduplicate[i]]), unduplicate[i]);
    }
    while (!path.empty()) {
        unsigned intersectID = path.top().second;
        double distance = path.top().first;
        path.pop();
        bool ok = 0;

        if (find(pickUp.begin(), pickUp.end(), intersectID) != pickUp.end()) {
            ok = 1;
            pickUp.erase(find(pickUp.begin(), pickUp.end(), intersectID));
            while (find(pickUp.begin(), pickUp.end(), intersectID) != pickUp.end())
                pickUp.erase(find(pickUp.begin(), pickUp.end(), intersectID));
            return intersectID;
        }
        //If found in dropOff
        if (find(dropOff.begin(), dropOff.end(), intersectID) != dropOff.end()) {
            unsigned size = dropOff.size();
            for (unsigned i = 0; i < fakeN; i++) {
                if (fakeInput[i + fakeN] == intersectID) {
                    unsigned corresPickup = fakeInput[i];
                    //If picked up
                    if (find(pickUp.begin(), pickUp.end(), corresPickup) == pickUp.end()) {
                        fakeInput[i] = -1;
                        fakeInput[i + fakeN] = -1;
                        dropOff.erase(find(dropOff.begin(), dropOff.end(), intersectID));
                    }
                }
            }
            if (dropOff.size() != size)
                ok = 1;
        }
        if (ok) {
            Time = distance / 1000 / 120 * 60;
            return intersectID;
        }
    }
    return -1;
}

int Dijkstra2(unsigned inter, vector<unsigned> &pickUp, vector<unsigned> &dropOff, unsigned N, unsigned M, unsigned &fakeN, vector<unsigned> &fakeInput, double& time, bool turned_on, unsigned &lastID) {
    unsigned intersection = inter;

    //Dijkstra starts here
    priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, greater<pair<double, unsigned>>> path;
    unordered_map<unsigned, node> openSet; //Nodes visited but not expanded
    unordered_map<unsigned, node> closedSet; //Nodes already expanded

    //Initialize all structures
    node start(intersection, 0);
    start.travelTime = 0;
    start.fromSegment = lastID;
    openSet.emplace(intersection, start);
    path.emplace(start.travelTime, intersection);

    //When open set is not empty
    while (!openSet.empty()) {
        //If already expanded, skip
        if (closedSet.find(path.top().second) != closedSet.end()) {
            path.pop();
            continue;
        }
        //Store useful data

        unsigned intersectID = path.top().second;
        node current = openSet.find(intersectID)->second;
        //Update all structures
        path.pop();

        bool ok = 0;
        //If found in pickUP
        if (find(pickUp.begin(), pickUp.end(), intersectID) != pickUp.end()) {
            ok = 1;
            pickUp.erase(find(pickUp.begin(), pickUp.end(), intersectID));
            while (find(pickUp.begin(), pickUp.end(), intersectID) != pickUp.end())
                pickUp.erase(find(pickUp.begin(), pickUp.end(), intersectID));
            //return intersectID;
        }
        //If found in dropOff
        if (find(dropOff.begin(), dropOff.end(), intersectID) != dropOff.end()) {
            unsigned size = dropOff.size();
            for (unsigned i = 0; i < fakeN; i++) {
                if (fakeInput[i + fakeN] == intersectID) {
                    unsigned corresPickup = fakeInput[i];
                    //If picked up
                    if (find(pickUp.begin(), pickUp.end(), corresPickup) == pickUp.end()) {
                        fakeInput[i] = -1;
                        fakeInput[i + fakeN] = -1;
                        dropOff.erase(find(dropOff.begin(), dropOff.end(), intersectID));
                    }
                }
            }
            if (dropOff.size() != size)
                ok = 1;
        }
        if (ok == 1) {
            lastID = current.fromSegment;
            return intersectID;
        }
        openSet.erase(intersectID);
        closedSet.emplace(intersectID, current);
        //Expansion
        for (unsigned i = 0; i < getIntersectionStreetSegmentCount(intersectID); i++) {
            StreetSegmentInfo info = getStreetSegmentInfo(getIntersectionStreetSegment(intersectID, i));
            unsigned tempSegment = getIntersectionStreetSegment(intersectID, i);
            //Chick path legality
            if (intersectID == info.from) {
                unsigned next = info.to;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                //If the target is already expanded or current time is not less than time stored in the target, skip
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                //The best till now, store
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            } else if (!info.oneWay) {
                unsigned next = info.from;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            }
        }
        //Check if finished
        if (pickUp.empty() && dropOff.empty())
            break;
    }

    //If some point(s) not found, assign -1 to corresponding position
    if (!pickUp.empty() || !dropOff.empty()) {
        return -1;
    }

}

vector<unsigned> basicGreedy2(unsigned N, unsigned M, bool turned_on) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime);
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    unsigned lastID = 0;
    int result = Dijkstra2(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput, time, turned_on, lastID);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = Dijkstra2(result, pickUp, dropOff, N, M, fakeN, fakeInput, newTime, turned_on, lastID);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

vector<unsigned> basicGreedy3(unsigned N, unsigned M, bool turned_on) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime);
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    unsigned lastID = 0;
    int result = Dijkstra(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = Dijkstra(result, pickUp, dropOff, N, M, fakeN, fakeInput);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

int find_nearest_point(unsigned intersect_id_start, vector<unsigned> targets, double time, int shit) {
    //Dijkstra starts here
    priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, greater<pair<double, unsigned>>> path;
    unordered_map<unsigned, node> openSet; //Nodes visited but not expanded
    unordered_map<unsigned, node> closedSet; //Nodes already expanded

    //Initialize all structures
    node start(intersect_id_start, 0);
    start.travelTime = 0;
    openSet.emplace(intersect_id_start, start);
    path.emplace(start.travelTime, intersect_id_start);

    //When open set is not empty
    while (!openSet.empty()) {
        //If already expanded, skip
        if (closedSet.find(path.top().second) != closedSet.end()) {
            path.pop();
            continue;
        }
        //Store useful data

        unsigned intersectID = path.top().second;
        node current = openSet.find(intersectID)->second;
        //Update all structures
        path.pop();
        openSet.erase(intersectID);
        closedSet.emplace(intersectID, current);

        //If found
        if (find(targets.begin(), targets.end(), intersectID) != targets.end()) {
            time = current.travelTime;
            return intersectID;
        }
        //Expansion
        for (unsigned i = 0; i < getIntersectionStreetSegmentCount(intersectID); i++) {
            StreetSegmentInfo info = getStreetSegmentInfo(getIntersectionStreetSegment(intersectID, i));
            unsigned tempSegment = getIntersectionStreetSegment(intersectID, i);
            //Chick path legality
            if (intersectID == info.from) {
                unsigned next = info.to;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                //If the target is already expanded or current time is not less than time stored in the target, skip
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                //The best till now, store
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            } else if (!info.oneWay) {
                unsigned next = info.from;
                node temp(intersectID, tempSegment);
                temp.travelTime += current.travelTime;
                if (getStreetSegmentInfo(current.fromSegment).streetID != getStreetSegmentInfo(temp.fromSegment).streetID)
                    temp.travelTime += 0.25;
                if (closedSet.find(next) != closedSet.end())
                    continue;
                else if (openSet.find(next) != openSet.end() && openSet.find(next)->second.travelTime < temp.travelTime)
                    continue;
                openSet.erase(next);
                openSet.emplace(next, temp);
                path.emplace(temp.travelTime, next);
            }
        }
    }

    //If failure
    return -1;
}

vector<unsigned> basicGreedy2(unsigned N, unsigned M, bool turned_on, int shit) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime, shit);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime, shit);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime, shit);
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    unsigned lastID = 0;
    int result = Dijkstra2(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput, time, turned_on, lastID);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = Dijkstra2(result, pickUp, dropOff, N, M, fakeN, fakeInput, newTime, turned_on, lastID);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum, shit);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

vector<unsigned> basicGreedy3(unsigned N, unsigned M, bool turned_on, int shit) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime, shit);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime, shit);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime, shit);
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    unsigned lastID = 0;
    int result = Dijkstra(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = Dijkstra(result, pickUp, dropOff, N, M, fakeN, fakeInput);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum, shit);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

vector<unsigned> basicGreedy(unsigned N, unsigned M, bool turned_on, int shit) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime, shit);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime, shit);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime, shit);
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    int result = GreedyHelper(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput, time, 0);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = GreedyHelper(result, pickUp, dropOff, N, M, fakeN, fakeInput, time, 0);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum, shit);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

vector<unsigned> basicGreedy4(unsigned N, unsigned M, bool turned_on) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime);
            newTravelTime = (double) rand() / RAND_MAX;
            travelTime = (double) rand() / RAND_MAX;
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
                break;
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    unsigned lastID = 0;
    int result = Dijkstra(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = Dijkstra(result, pickUp, dropOff, N, M, fakeN, fakeInput);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

vector<unsigned> basicGreedy5(unsigned N, unsigned M, bool turned_on) {
    vector<unsigned> initialOne;
    vector<unsigned> pickUp;
    vector<unsigned> dropOff;
    vector<unsigned> depot;
    pickUp.insert(pickUp.begin(), input.begin(), input.begin() + N);
    dropOff.insert(dropOff.begin(), input.begin() + N, input.begin() + 2 * N);
    depot.insert(depot.begin(), input.begin() + 2 * N, input.end());

    //Find the nearest one form any depot to any pick-up
    int finalTo;
    unsigned finalFrom;
    double travelTime;
    for (unsigned i = 0; i < M; i++) {
        if (i == 0) {
            finalTo = find_nearest_point(depot[i], pickUp, travelTime);
            finalFrom = depot[i];
        } else {
            if (finalTo == -1) {
                finalTo = find_nearest_point(depot[i], pickUp, travelTime);
                finalFrom = depot[i];
            }
            double newTravelTime;
            int newFinalTo;
            newFinalTo = find_nearest_point(depot[i], pickUp, newTravelTime);
            newTravelTime = (double) rand() / RAND_MAX;
            travelTime = (double) rand() / RAND_MAX;
            if (newTravelTime < travelTime) {
                travelTime = newTravelTime;
                finalTo = newFinalTo;
                finalFrom = depot[i];
            }
        }
    }
    vector<unsigned> failure;
    if (finalTo == -1) {
        return failure;
    }

    initialOne.push_back(finalFrom);
    initialOne.push_back(finalTo);

    //Process everything between the start point and the return point
    while (find(pickUp.begin(), pickUp.end(), finalTo) != pickUp.end()) {
        pickUp.erase(find(pickUp.begin(), pickUp.end(), finalTo));
    }
    //Reconstruct pickUp
    pickUp.insert(pickUp.begin(), (unsigned) finalTo);
    unsigned fakeN = N;
    vector<unsigned> fakeInput = input;
    double time;
    unsigned lastID = 0;
    int result = Dijkstra(pickUp[0], pickUp, dropOff, N, M, fakeN, fakeInput);
    if (result == -1) {
        return failure;
    }

    initialOne.push_back(result);
    while (!pickUp.empty() || !dropOff.empty()) {
        double newTime;


        result = Dijkstra(result, pickUp, dropOff, N, M, fakeN, fakeInput);
        if (result == -1) {
            return failure;
        }
        time += newTime;
        initialOne.push_back(result);
    }


    double dum;
    int finalDepot;
    finalDepot = find_nearest_point(initialOne[initialOne.size() - 1], depot, dum);
    if (finalDepot == -1) {
        return failure;
    }
    initialOne.push_back(finalDepot);
    return initialOne;
}

void calculateFinalPath(vector<unsigned> _input, int index) {
    if (index == 0) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath.insert(finalPath.end(), path.begin(), path.end());
        }
    }
    if (index == 1) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath2.insert(finalPath2.end(), path.begin(), path.end());
        }
    }
    if (index == 2) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath3.insert(finalPath3.end(), path.begin(), path.end());
        }
    }
    if (index == 3) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath4.insert(finalPath4.end(), path.begin(), path.end());
        }
    }
    if (index == 4) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath5.insert(finalPath5.end(), path.begin(), path.end());
        }
    }
    if (index == 5) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath6.insert(finalPath6.end(), path.begin(), path.end());
        }
    }
    if (index == 6) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath7.insert(finalPath7.end(), path.begin(), path.end());
        }
    }
    if (index == 7) {
        for (auto iter = _input.begin(); iter != _input.end() - 1; iter++) {
            vector<unsigned> path = find_path_between_intersections(*iter, *(iter + 1));
            finalPath8.insert(finalPath8.end(), path.begin(), path.end());
        }
    }

}

void initialize(int index, unsigned N, unsigned M, bool turned_on, int shit) {
    if (index == 0) {
        neighbor = basicGreedy(N, M, 0);
    }
    if (index == 1) {
        neighbor1 = basicGreedy2(N, M, 0);
    }
    if (index == 2) {
        neighbor2 = basicGreedy3(N, M, 0);
    }
    if (index == 3) {
        neighbor3 = basicGreedy3(N, M, 0, 0);
    }
    if (index == 4) {
        neighbor4 = basicGreedy2(N, M, 0, 0);
    }
    if (index == 5) {
        neighbor5 = basicGreedy(N, M, 0, 0);
    }
    if (index == 6) {
        neighbor6 = basicGreedy4(N, M, 0);
    }
    if (index == 7) {
        neighbor7 = basicGreedy5(N, M, 0);
    }
}

void SA(vector<unsigned>& optimal_found_so_far, unsigned N, unsigned M) {
    srand(time(NULL));
    double T = 0;
    double difference = 0;
    vector<unsigned> neighbor;
    while (!termination) {
        priority_queue<pair<double, unsigned>, vector<pair<double, unsigned>>, less<pair<double, unsigned>>> path;
        for (int i = 0; i < optimal_found_so_far.size() - 4; i++) {
            path.emplace(find_cost({optimal_found_so_far[i], optimal_found_so_far[i + 1]}), i + 1);
        }
        while (!path.empty()) {
            vector<unsigned> oldSS = optimal_found_so_far;
            double oldCost;
            double newCost;


            unsigned position = path.top().second;
            path.pop();
            bool ok1 = 1;
            bool ok2 = 1;
            bool ok3 = 1;
            for (int i = 0; i < N; i++) {
                if (input[i] == oldSS[position] && input[i + N] == oldSS[position + 1] && find(oldSS.begin() + position + 2, oldSS.end(), input[i + N]) == oldSS.end())
                    ok1 = 0;
                if (input[i] == oldSS[position] && input[i + N] == oldSS[position + 2] && find(oldSS.begin() + position + 3, oldSS.end(), input[i + N]) == oldSS.end())
                    ok2 = 0;
                if (input[i] == oldSS[position + 1] && input[i + N] == oldSS[position + 2] && find(oldSS.begin() + position + 3, oldSS.end(), input[i + N]) == oldSS.end())
                    ok3 = 0;
            }
            vector<unsigned> shitty[5];
            if (!ok1 && !ok2 && !ok3)
                continue;
            else if (ok1 && !ok2 && !ok3){
                shitty[0].push_back(oldSS[position + 1]);
                shitty[0].push_back(oldSS[position]);
                shitty[0].push_back(oldSS[position + 2]);
            }
            else if (!ok1 && ok2 && !ok3){
                shitty[0].push_back(oldSS[position + 2]);
                shitty[0].push_back(oldSS[position + 1]);
                shitty[0].push_back(oldSS[position]);
            }
            else if (!ok1 && !ok2 && ok3){
                shitty[0].push_back(oldSS[position]);
                shitty[0].push_back(oldSS[position + 2]);
                shitty[0].push_back(oldSS[position + 1]);
            }
            else if (ok1 && ok2 && !ok3){
                shitty[0].push_back(oldSS[position + 1]);
                shitty[0].push_back(oldSS[position + 2]);
                shitty[0].push_back(oldSS[position]);
                shitty[1].push_back(oldSS[position + 1]);
                shitty[1].push_back(oldSS[position]);
                shitty[1].push_back(oldSS[position + 2]);
            }
            else if (ok1 && !ok2 && ok3){
                shitty[0].push_back(oldSS[position]);
                shitty[0].push_back(oldSS[position + 2]);
                shitty[0].push_back(oldSS[position + 1]);
                shitty[1].push_back(oldSS[position + 1]);
                shitty[1].push_back(oldSS[position] );
                shitty[1].push_back(oldSS[position + 2]);
            }else if (!ok1 && ok2 && ok3){
                shitty[0].push_back(oldSS[position + 2]);
                shitty[0].push_back(oldSS[position]);
                shitty[0].push_back(oldSS[position + 1]);
                shitty[1].push_back(oldSS[position]);
                shitty[1].push_back(oldSS[position + 2]);
                shitty[1].push_back(oldSS[position + 1]);
            }else if (ok1 && ok2 && ok3){
                shitty[0].push_back(oldSS[position]);
                shitty[0].push_back(oldSS[position + 2]);
                shitty[0].push_back(oldSS[position + 1]);
                shitty[1].push_back(oldSS[position + 1]);
                shitty[1].push_back(oldSS[position]);
                shitty[1].push_back(oldSS[position + 2]);
                shitty[2].push_back(oldSS[position + 1]);
                shitty[2].push_back(oldSS[position + 2]);
                shitty[2].push_back(oldSS[position]);
                shitty[3].push_back(oldSS[position + 2]);
                shitty[3].push_back(oldSS[position]);
                shitty[3].push_back(oldSS[position + 1]);
                shitty[4].push_back(oldSS[position + 2]);
                shitty[4].push_back(oldSS[position + 1]);
                shitty[4].push_back(oldSS[position]);
            }
            bool finished = 0;    
            for(int i = 0; i < 5; i ++){
                if(!shitty[i].empty()){
                    swap_ranges(oldSS.begin() + position, oldSS.begin() + position + 3, shitty[i].begin());
                    newCost = find_cost(oldSS);
                    if (newCost < find_cost(optimal_found_so_far)) {
                        optimal_found_so_far = oldSS;
                        finished = 1;
                    }
                }
            }
            if(finished)
                break;
            /*
        if (difference < 0) {
            optimal_found_so_far = neighbor;
        }

        neighbor = newPath(oldS, N, M, newCost, oldCost);

        //Compute the possibility

        T = T / (1 + 0.00002 * T);
        double possibility = p_func(oldCost, newCost, T);

        //Check if the neighbor should be accepted

        if (possibility > (double) rand() / RAND_MAX) {
            difference += (newCost - oldCost);
            oldS = neighbor;
        }*/
        }
    }
    return;
}