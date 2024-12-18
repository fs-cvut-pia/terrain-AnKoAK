#include "NavyPath.h"

#include <iostream>
#include <algorithm>
#include <climits>

NavyPath::NavyPath(TerrainMap& m, std::string name_in, Point start_in, Point finish_in)
: Path{m, name_in, start_in, finish_in }
, inspectedPoints{m.nx, m.ny}
, waitingPoints{m.nx, m.ny}
, predecessors{m.nx, m.ny}
, m_pathLength {INT_MAX}  // Set initial length to inifinity.
{
    
    inspectedPoints.zero();
    waitingPoints.zero();
    pointsToInspect.push(start);  
    waitingPoints(start) = 1; 
    Point inspectedPoint = pointsToInspect.front();
    std::cout << "Queue (" << inspectedPoint.x << "," << inspectedPoint.y << ")" << std::endl;

}

bool NavyPath::find()
{
    Point inspectedPoint = pointsToInspect.front();
    std::cout << "Start " << start.x << ", " << start.y << std::endl;
    
    while (!pointsToInspect.empty())
    {
        Point inspectedPoint = pointsToInspect.front();
        std::cout << "(" << inspectedPoint.x << "," << inspectedPoint.y << ")" << std::endl;
        inspectSurroundings(inspectedPoint);
        pointsToInspect.pop();
        // Select point as inspected.
        inspectedPoints(inspectedPoint) = 1;

        // Check for target.
        if (inspectedPoint == finish)
        {
            std::cout << "Target hit" << std::endl;
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
    std::cout << m_pathLength << std::endl;
    if (m_pathLength != LLONG_MAX)
    {
        return true;
    }
    return false;
}

void NavyPath::setPath(Point point)
{
    while ( point != start)
    {
        path.push_back(point);
        point = predecessors(point);
    }
}

int NavyPath::getPathLength(Point point)
{
    int pathLength = 0;
    
    while ( point != start)
    {
        pathLength++;
        point = predecessors(point);
    }

    std::cout << "Path length from point (" 
    << point.x << ", " << point.y << "): "
    << pathLength << std::endl;
    return pathLength;
}

bool NavyPath::isSea(Point point) const
{
    return map.alt(point) < 0;
}

bool NavyPath::wasInspected(Point point)
{
    return inspectedPoints(point);
}

bool NavyPath::isInQueue(Point point)
{
    return waitingPoints(point);
}

void NavyPath::deleteFromQueuePoints(Point point)
{
    // Delete.
}

void NavyPath::inspectSurroundings(Point point)
{
    // Inspect all points in the circle.
    for (auto direction : directions) 
    {
        Point neighbourPoint = Point(point.x + direction.first, point.y + direction.second);

        // Check if the coords are valid / inside the map and within altitude condition.
        if (map.validCoords(neighbourPoint) && (isSea(neighbourPoint) || neighbourPoint == finish) )
        {
            // Check if the point is already to be inspect or was inspected.
            if (!(isInQueue(neighbourPoint) || wasInspected(neighbourPoint) ) )
            {
                pointsToInspect.push(neighbourPoint);
                // Set predecessor of the waiting point.
                predecessors(neighbourPoint) = point;
                // Set point as waiting in queue.
                waitingPoints(neighbourPoint) = 1;

            }
        }
    }
}