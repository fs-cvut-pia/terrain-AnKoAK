#include "RoadPath.h"

#include <iostream>
#include <algorithm>
#include <climits>
#include <cmath>

/** Distance between two adjacent grid cells in meters. */
#define SCALE 1000

RoadPath::RoadPath(TerrainMap& m, std::string name_in, Point start_in, Point finish_in)
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

bool RoadPath::find()
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

void RoadPath::setPath(Point point)
{
    while ( point != start)
    {
        path.push_back(point);
        point = predecessors(point);
    }
}

int RoadPath::getPathLength(Point point)
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

bool RoadPath::isRoad(Point point1, Point point2) const
{   
    return (100 * (map.alt(point1) - map.alt(point2) ) / SCALE) < 6;
}

bool RoadPath::isLand(Point point) const
{   
    return map.alt(point) >= 0;
}

bool RoadPath::wasInspected(Point point)
{
    return inspectedPoints(point);
}

bool RoadPath::isInQueue(Point point)
{
    return waitingPoints(point);
}

void RoadPath::deleteFromQueuePoints(Point point)
{
    // Delete.
}

void RoadPath::inspectSurroundings(Point point)
{
    // Inspect all points in the circle.
    for (auto direction : directions) 
    {
        Point neighbourPoint = Point(point.x + direction.first, point.y + direction.second);

        // Check if the coords are valid / inside the map and within altitude condition.
        if (map.validCoords(neighbourPoint) 
        && ((isRoad(neighbourPoint, point) && isLand(neighbourPoint) )  
        || neighbourPoint == finish) )
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