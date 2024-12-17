#ifndef NAVYPATH_H
#define NAVYPATH_H

#include "Path.h"

#include <iostream>

#include <queue>
#include <vector>

class NavyPath : public Path 
{

public: 
    NavyPath(TerrainMap& m, std::string name_in, Point start_in, Point finish_in);
    bool find() override;
    void printStats() const;     // Print out path statistics
    void saveToFile() const;     // Save path to file "name.dat"
    std::string getName() const; // Returns path name

protected:
    /** Check if given point is sea. */
    bool isSea(const Point * point) const;
    /** Check if the point has been inspected. */
    bool wasInspected (const Point * point);
    /** Check if the point is in the queue already. */
    bool isInQueue(const Point * point);
    /** Inspect other points around the given point
    and add them to the queue
     */
    void inspectSurroundings (Point * point);
    /** Delte address of point if inspected. */
    void deleteFromQueuePoints (const Point * point);
    /** Get the length of the path. */
    int getPathLength(const Point * point);
    /** Set new shortest path from the point to the beginning. */
    void setPath(const Point * point);

    /** Matrix representing inspected points. */
    Matrix<int> inspectedPoints;
    /** Matrix representing addresses points is in the queue. */
    std::vector<Point *> queuePoints;
    std::queue<Point *> pointsToInspect;
    /** Length of the shortest path. */
    int m_pathLength;
    

};
#endif

/**

It makes no sense to check point two times.
Therefore in queue indicates if neigbour point is in the queue before
there is shorter or same distance than from the actualy inspected point.
The neighbour point being in the queue already indicates that the point 
which inserted this neghbour point to the queue before is either on lower perimeter from
start as the inspected point or on the same perimeter meaning same distance from beginning.
 */