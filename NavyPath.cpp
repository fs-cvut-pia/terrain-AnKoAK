#include "NavyPath.h"

#include <iostream>
#include <algorithm>
#include <climits>

NavyPath::NavyPath(TerrainMap& m, std::string name_in, Point start_in, Point finish_in)
: Path{m, name_in, start_in, finish_in }
, inspectedPoints{m.nx, m.ny}
, m_pathLength {INT_MAX}  // Set initial length to inifinity.
{
    
    inspectedPoints.zero();
    pointsToInspect.push(start_in);  
    queuePoints.push_back(start_in); 
}

bool NavyPath::find()
{
    while (!pointsToInspect.empty())
    {
        Point inspectedPoint = pointsToInspect.front();
        std::cout << "(" << inspectedPoint.x << "," << inspectedPoint.y << ")" << std::endl;
        inspectSurroundings(inspectedPoint);
        pointsToInspect.pop();
        // Delete address from currently inspected points.
        deleteFromQueuePoints(inspectedPoint);
        // Select point as inspected.
        inspectedPoints(inspectedPoint) = 1;

        // Check for target.
        if (inspectedPoint == finish)
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
    std::cout << m_pathLength << std::endl;
    if (m_pathLength != LLONG_MAX)
    {
        return true;
    }
    return false;
}

void NavyPath::setPath(Point point)
{
    Point * pointP = &point;
    while ( pointP != nullptr)
    {
        path.push_back(*pointP);
        pointP = pointP->predecessor;
    }
}

int NavyPath::getPathLength(Point point)
{
    int pathLength = 0;
    Point * actualPoint = &point;
    while (actualPoint != nullptr) {
        pathLength++;
        actualPoint = actualPoint->predecessor;
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
    auto it = std::find(queuePoints.begin(), queuePoints.end(), point);

    return it != queuePoints.end() ? true : false;
}

void NavyPath::deleteFromQueuePoints(Point point)
{
    auto it = std::find(queuePoints.begin(), queuePoints.end(), point);

    int position = std::distance(queuePoints.begin(), it);
    
    queuePoints.erase(queuePoints.begin() + position);
}

void NavyPath::inspectSurroundings(Point point)
{
    // Inspect all points in the circle.
    for (auto direction : directions) 
    {
        Point neighbourPoint = Point(point.x + direction.first, point.y + direction.second);

        // Check if the coords are valid / inside the map and within altitude condition.
        if (map.validCoords(neighbourPoint) && isSea(neighbourPoint))
        {
            // Check if the point is already to be inspect or was inspected.
            if (!(isInQueue(neighbourPoint) || wasInspected(neighbourPoint)) )
            {
                pointsToInspect.push(neighbourPoint);
                queuePoints.push_back(neighbourPoint);
                neighbourPoint.predecessor = &point;

            }
        }
    }
}