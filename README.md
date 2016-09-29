# Map
A simple map application parsing data from OpenStreetMap to implement functionalities including pathfinding, simple navigation and 
showing information of all intersections, points of interests and one-way streets, etc.

The GUI is implemented with EasyGL runing on X11.

The part of the app's functionalies are required to be able to solve some large Travelling Courier Problems, basically all the implementation in m4.cpp & m4.h. Involved use of algorithms including Dijkstra, A star, Similated Annealing, 2-opt/3-opt and the use of multi-thread technique.

This version including the following cities: Toronto, Hamilton, Newyork, Cairo, Moscow, London, Saint Helena
