// This function shows the usage of AStar_Custom_nhbr(..) function to specify
// using non-diagonal neighbours only.

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <cmath>
#include <cstdlib>
#include <ctime>

#define BRDSIZ 40
bool SEEDED = false;
std::string thepath[BRDSIZ + 1][BRDSIZ + 1];

#include "Utilities.h"

int myrand(void) { // Random number generator: 0-BRDSIZ range.
    if(!SEEDED) {
        unsigned int iseed = (unsigned int)::time(NULL);
        ::srand(iseed);
        SEEDED = true;
    }
    return ::rand() % BRDSIZ;
}


void My_Neighbour(const std::vector<int> &a, std::vector<std::vector<int>> &nns) { // nns == nearest neighbours.

    std::vector<int> up, dn;

    // Non-diagonals only. (Any dimension.)
    for(unsigned int i = 0; i < a.size(); ++i) {
        up.clear();
        dn.clear();
        for(unsigned int j = 0; j < a.size(); ++j) {
            if(i == j) {
                up.push_back(a[j] + static_cast<int>(1));
                dn.push_back(a[j] - static_cast<int>(1));
            } else {
                up.push_back(a[j]);
                dn.push_back(a[j]);
            }
        }
        nns.push_back(up);
        nns.push_back(dn);
    }

    return;
}


int main() {

    std::vector<std::vector<int>> final_path;

    std::vector<int> beginning;
    beginning.push_back(myrand()); // x
    beginning.push_back(myrand()); // y

    //    beginning.push_back(0); //x
    //    beginning.push_back(52); //y

    std::vector<int> end;
    end.push_back(myrand()); // x
    end.push_back(myrand()); // y

    //    end.push_back(45); //x
    //    end.push_back(62); //y
    while(beginning == end) {
        end.clear();
        end.push_back(myrand()); // x
        end.push_back(myrand()); // y
    }

    // Colour the board unused.
    for(unsigned int i = 0; i <= (BRDSIZ); ++i)
        for(unsigned int j = 0; j <= (BRDSIZ); ++j) {
            thepath[i][j] = "░";
        }

    final_path = AStar_Custom_nhbr(beginning, end, &My_Neighbour);

    std::cout << "Start node:  " << beginning[0] << " " << beginning[1] << std::endl;
    std::cout << "Target node:  " << end[0] << " " << end[1] << std::endl;

    // Plot the path with filled circles.
    for(unsigned int i = 0; i < final_path.size(); ++i) {
        size_t x = final_path[i][0];
        size_t y = final_path[i][1];
        thepath[x][y] = "◉";
    }

    // Fill the endpoints with special glyphs.
    thepath[end[0]][end[1]] = "◬";
    thepath[beginning[0]][beginning[1]] = "◦";

    // Draw the board.
    for(int j = (BRDSIZ); j >= 0; --j) {
        std::cout << std::endl;
        std::cout.width(2);
        std::cout << (j);
        std::cout.width(1);
        std::cout << "  ";

        for(unsigned int i = 0; i <= BRDSIZ; ++i) {
            std::cout << thepath[i][j] << " ";
        }
    }
    std::cout << std::endl << std::endl << "    ";
    for(unsigned int i = 0; i <= BRDSIZ; ++i) {
        if(static_cast<int>(i / 10) == 0) {
            std::cout << "  ";
        } else {
            std::cout << static_cast<int>(i / 10) << " ";
        }
    }
    std::cout << std::endl << "    ";

    for(unsigned int i = 0; i <= BRDSIZ; ++i) std::cout << i % 10 << " ";

    std::cout << std::endl;
    std::cout << "Key:" << std::endl;
    std::cout << "   ░ -- Unvisited node (it was visitable.)" << std::endl;
    std::cout << "   █ -- Blocked node (it was not visitable.)" << std::endl;
    std::cout << "   ◉ -- Trajectory node." << std::endl;
    std::cout << "   ◬ -- Target node." << std::endl;
    std::cout << "   ◦ -- Start node." << std::endl;


    return 0;
}
