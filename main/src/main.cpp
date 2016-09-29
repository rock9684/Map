#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
using namespace std;
int main() {
//Load map. If unsuccessful, ask user to reinput
    while(!cin.eof()){
        if(loadMap()){
           draw_map();
        }
        close_map();
    }
    
    
    // try out your m1.h functions here

    return 0;
}
