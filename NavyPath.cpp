#include "NavyPath.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <climits>


/** Possible directions of movement. */
std::array < std::pair<int, int>, 8> directions = {
    std::make_pair(-1, -1),     // One row above, one column left.
    std::make_pair(0, -1),      // One row above, one same column.
    std::make_pair(1, -1),      // One row above, one column right.
    std::make_pair(-1, 0),      // Same row, one column left.
    std::make_pair(1, 0),       // Same row, one column left.
    std::make_pair(-1, 1),      // One row below, one column left.
    std::make_pair(0, 1),       // One row below, one same column.
    std::make_pair(1, 1),       // One row below, one column right.
};


NavyPath::NavyPath(TerrainMap& m, std::string name_in, Point start_in, Point finish_in)
: Path{m, name_in, start_in, finish_in }
, inspectedPoints{m.nx, m.ny}
, m_pathLength {INT_MAX}  // Set initial length to inifinity.
{
    
    inspectedPoints.zero();
    pointsToInspect.push(&start_in);   
}

bool NavyPath::find()
{
    while (!pointsToInspect.empty())
    {
        Point * inspectedPoint = pointsToInspect.front();
        inspectSurroundings(inspectedPoint);
        pointsToInspect.pop();
        // Delete address from currently inspected points.
        deleteFromQueuePoints(inspectedPoint);
        // Select point as inspected.
        inspectedPoints(*inspectedPoint) = 1;

        // Check for target.
        if (inspectedPoint == &finish)
        {
            int pathLength = getPathLength(inspectedPoint);

            // Check and possibly set new shortest path.
            if (pathLength < m_pathLength)
            {
                m_pathLength = pathLength;
                setPath(inspectedPoint);
            }
        }
    }

    // If m_pathLength has changed from inintialization, 
    // at least once the target was hit.
    if (m_pathLength != INT_MAX)
    {
        return true;
    }
    return false;
}

void NavyPath::setPath(const Point * point)
{
    while ( point != nullptr)
    {
        path.push_back(*point);
        point = point->predecessor;
    }
}

int NavyPath::getPathLength(const Point * point)
{
    int pathLength = 0;
    Point * actualPoint = const_cast<Point *>(point);
    while (actualPoint != nullptr) {
        pathLength++;
        actualPoint = actualPoint->predecessor;
    }

    std::cout << "Path length from point (" 
    << point->x << ", " << point->y << "): "
    << pathLength << std::endl;
    return pathLength;
}

bool NavyPath::isSea(const Point * point) const
{
    return map.alt(* point) < 0;
}

bool NavyPath::wasInspected(const Point * point)
{
    return inspectedPoints(*point);
}

bool NavyPath::isInQueue(const Point * point)
{
    auto it = std::find(queuePoints.begin(), queuePoints.end(), point);

    return it != queuePoints.end() ? true : false;
}

void NavyPath::deleteFromQueuePoints(const Point * point)
{
    auto it = std::find(queuePoints.begin(), queuePoints.end(), point);

    int position = std::distance(queuePoints.begin(), it);
    
    queuePoints.erase(queuePoints.begin() + position);
}

void NavyPath::inspectSurroundings(Point * point)
{
    // Inspect all points in the circle.
    for (auto direction : directions) 
    {
        Point neighbourPoint = Point(point->x + direction.first, point->y + direction.second);

        // Check if the coords are valid / inside the map and within altitude condition.
        if (map.validCoords(neighbourPoint) && isSea(&neighbourPoint))
        {
            // Check if the point is already to be inspect or was inspected.
            if (!(isInQueue(&neighbourPoint) || wasInspected(& neighbourPoint)) )
            {
                pointsToInspect.push(&neighbourPoint);
                queuePoints.push_back(&neighbourPoint);
                neighbourPoint.predecessor = point;
            }
        }
    }
}