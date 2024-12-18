#include "Path.h"
#include <cmath>
#include <fstream>



Path::Path(TerrainMap& m, std::string name_in, Point start_in, Point finish_in) 
: map(m)
, name(name_in)
, start(start_in)
, finish(finish_in) 
{};

void Path::printStats() const {
    bool land = false;
    bool water = false;
    double length = 0.0;
    double alt = 0.0;

    if (path.size() == 0) {
        std::cout << "Path empty." << std::endl;
        return;
    }

    int max_alt = map.alt(path[0]);

    for (int i=1; i<path.size(); ++i) {
        Point u = path[i];
        Point u_prev = path[i-1];
        if (i < path.size() - 1 && map.alt(u) > 0) land = true;
        if (map.alt(u) < 0) water = true;
        length += (u - u_prev).length();
        alt += std::abs(map.alt(u) - map.alt(u_prev));
        if (map.alt(u) > max_alt) max_alt = map.alt(u);
    }

    std::cout << "Path designated start = [" << start.x << ", " << start.y << "], finish = [" << finish.x << ", " << finish.y << "]" << std::endl;

    if (path[0] != start)
        std::cout << "First point on path [" << path[0].x << ", " << path[0].y << "] does not correspond to the designated starting point [" << start.x << ", " << start.y << "] !" << std::endl;

    if (path[path.size()-1] != finish)
        std::cout << "Last point on path [" << path[path.size()-1].x << ", " << path[path.size()-1].y << "] does not correspond to the designated finish point [" << finish.x << ", " << finish.y << "] !" << std::endl;

    if (land) std::cout << "Path type: land" << std::endl;
    if (water) std::cout << "Path type: water" << std::endl;
    std::cout << "Path length: " << length << " km" << std::endl;
    std::cout << "Total elevation gain + loss: " << alt << " m" << std::endl;
    std::cout << "Max. elevation: " << max_alt << " m" << std::endl;
}

std::string Path::getName() const {
    return name;
}

void Path::saveToFile() const {
    std::ofstream output(name + ".dat");

    if (!output) throw std::runtime_error("Cannot open file " + name + ".dat");

    for (Point u : path) {
        output << u.x << " " << u.y << std::endl;
    }
}


