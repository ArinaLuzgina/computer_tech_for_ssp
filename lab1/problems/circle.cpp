#include <set>
#include <gmsh.h>
#include <cmath>
#include <vector>

int main(int argc, char **argv)
{
    gmsh::initialize();
    gmsh::model::add("circle");

    double radius = 1.0;
    double centerX = 0.0;
    double centerY = 0.0;
    double centerZ = 0.0;
    double lc = 0.01;  

    int centerPointTag = gmsh::model::geo::addPoint(centerX, centerY, centerZ, lc);

    // Количество дуг для аппроксимации окружности
    int numArcs = 64;
    double dtheta = 2 * M_PI / numArcs;

    std::vector<int> pointTags;
    std::vector<int> arcTags;

    for (int i = 0; i < numArcs; ++i) {
        double theta1 = i * dtheta;
        double x1 = centerX + radius * cos(theta1);
        double y1 = centerY + radius * sin(theta1);
        int p1 = gmsh::model::geo::addPoint(x1, y1, centerZ, lc);
        pointTags.push_back(p1);
    }

    for (int i = 0; i < numArcs; ++i)
    {
        int p1 = pointTags[i];
        int p2 = pointTags[(i + 1) % numArcs];

        int arcTag = gmsh::model::geo::addCircleArc(p1, centerPointTag, p2);
        arcTags.push_back(arcTag);
    }
    
    int curveLoopTag = gmsh::model::geo::addCurveLoop(arcTags);
    int planeSurfaceTag = gmsh::model::geo::addPlaneSurface({curveLoopTag});


    gmsh::model::geo::synchronize();
    gmsh::model::mesh::generate(2);
    gmsh::write("./../models/circle.msh");
    std::set<std::string> args(argv, argv + argc);
    if(!args.count("-nopopup")) gmsh::fltk::run();
    gmsh::finalize();

    return 0;
  
}