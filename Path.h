#ifndef PATH_P
#define PATH_P

#include "TerrainMap.h"
#include <iostream>
#include <iostream>
#include <array>

// Abstract class which needs to be extended to contain the actual path finding algorithm

class Path {
public:
    Path(TerrainMap& m, std::string name_in, Point start_in, Point finish_in);
    virtual bool find() = 0;     // Implement this method to find the route and save it in vector<Point> path
    void printStats() const;     // Print out path statistics
    void saveToFile() const;     // Save path to file "name.dat"
    std::string getName() const; // Returns path name

    /** Possible directions of movement. */
    std::array < std::pair<int, int>, 8> directions = {
        std::make_pair(-1, -1),     // One row above, one column left.
        std::make_pair(0, -1),      // One row above, one same column.
        std::make_pair(1, -1),      // One row above, one column right.
        std::make_pair(-1, 0),      // Same row, one column left.
        std::make_pair(1, 0),       // Same row, one column left
        std::make_pair(-1, 1),      // One row below, one column left.
        std::make_pair(0, 1),       // One row below, one same column.
        std::make_pair(1, 1),       // One row below, one column right.
    };
 

protected:
    TerrainMap& map;
    std::vector<Point> path;
    const Point start; 
    const Point finish;
private:
    std::string name;
};

#endif