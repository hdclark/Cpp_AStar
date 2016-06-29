#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

#define BRDSIZ 20
bool SEEDED = false;

#include "Utilities.h"


int myrand(void) { // Random number generator: 0-BRDSIZ range.
    if(!SEEDED) {
        unsigned int iseed = (unsigned int)::time(NULL);
        ::srand(iseed);
        SEEDED = true;
    }
    return ::rand() % BRDSIZ;
}


int main() {

    std::vector<std::vector<int>> some_local;

    std::vector<int> beginning;
    beginning.push_back(myrand()); // x
    beginning.push_back(myrand()); // y
    beginning.push_back(myrand()); // z

    // beginning.push_back(5); //x
    // beginning.push_back(6); //y
    // beginning.push_back(7); //z


    std::vector<int> end;
    end.push_back(myrand()); // x
    end.push_back(myrand()); // y
    end.push_back(myrand()); // z

    // end.push_back(10); //x
    // end.push_back(11); //y
    // end.push_back(12); //z


    some_local = AStar(beginning, end);

    std::cout << "# Start node:  ";
    for(size_t i = 0; i < beginning.size(); ++i) std::cout << beginning[i] << "  ";
    std::cout << std::endl;

    std::cout << "# Target node:  ";
    for(size_t i = 0; i < end.size(); ++i) std::cout << end[i] << "  ";
    std::cout << std::endl;


    std::cout << "# Final path:  (x,y,z)" << std::endl;
    for(size_t j = 0; j < some_local.size(); ++j) {
        for(size_t i = 0; i < some_local[j].size(); ++i) std::cout << some_local[j][i] << "  ";
        std::cout << std::endl;
    }

    return 0;
}
