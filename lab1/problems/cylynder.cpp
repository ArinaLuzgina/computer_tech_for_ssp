#include <vector>
#include <cmath>
#include <gmsh.h>
#include <iostream>
#include <set>

struct centerPoint
{
    double x;
    double y;
    double z;
};
struct circle
{
    struct centerPoint center = {0.0, 0.0, 0.0};
    double radius = 1.0;
    double lc = 0.01;  
    int centerPointTag;
    std::vector<int> pointTags;
    std::vector<int> arcTags;

};

struct circle prepareCircle(int numArcs=32, struct centerPoint center={0.0, 0.0, 0.0}, double radius=1.0, double lc=1e-1);
std::vector<int> draw_cylynder_side(struct circle circle_up, struct circle circle_down, int number_of_points);
std::vector<int> draw_curves(struct circle circle_down, struct circle circle_up, std::vector<int> side_lines_tags, int number_of_points);
std::vector<int> draw_surfaces(std::vector<int> curves_tags, int number_of_tags);

int main(int argc, char **argv)
{
    gmsh::initialize();
    gmsh::model::add("cylynder");

    int numArcs = 32;
    struct centerPoint center_bottom = {0.0, 0.0, 0.0};
    struct centerPoint center_up = {0.0, 0.0, 1.0};

    /*lines*/
    std::cout << "line initalizing has been started" << std::endl;
    struct circle bottom_circle = prepareCircle(numArcs, center_bottom);
    struct circle up_circle = prepareCircle(numArcs, center_up);
    std::vector<int> side_lines_tags = draw_cylynder_side(bottom_circle, up_circle, numArcs);
    std::cout << "line initalizing has been ended" << std::endl;

    /*surfaces*/
    std::cout << "curves initalizing has been started" << std::endl;

    std::vector<int> curves_tags = draw_curves(bottom_circle, up_circle, side_lines_tags, numArcs);
    std::cout << "curves initalizing has been ended" << std::endl;

    /* planes*/
    std::cout << "surfaces initalizing has been started" << std::endl;
    std::vector<int> surfaces_tags = draw_surfaces(curves_tags, numArcs);
    std::cout << "surfaces initalizing has been ended" << std::endl;

    /* volume */

    

    /*evreything else*/
    gmsh::model::geo::synchronize();
    gmsh::model::mesh::generate(3);
    gmsh::write("./../models/cylynder.msh");
    std::set<std::string> args(argv, argv + argc);
    if(!args.count("-nopopup")) gmsh::fltk::run();
    gmsh::finalize();
    
    return 0;
}


struct circle prepareCircle(int numArcs/*=32*/, struct centerPoint center/*={0.0, 0.0, 0.0}*/, double radius/*=1.0*/, double lc/*=1e-1*/)
{
    circle circle_s;
    circle_s.center = center;
    circle_s.radius = radius;
    circle_s.lc = lc;

    circle_s.centerPointTag = gmsh::model::geo::addPoint(center.x, center.y, center.z, lc);
    circle_s.pointTags;
    circle_s.arcTags;

    double dtheta = 2 * M_PI / numArcs;
    for (int i = 0; i < numArcs; ++i) {
        double theta1 = i * dtheta;
        double x1 = center.x + radius * cos(theta1);
        double y1 = center.y + radius * sin(theta1);
        int p1 = gmsh::model::geo::addPoint(x1, y1, center.z, lc);
        circle_s.pointTags.push_back(p1); 
    }

    for (int i = 0; i < numArcs; ++i)
    {
        int p1 = circle_s.pointTags[i];
        int p2 = circle_s.pointTags[(i + 1) % numArcs];
        int arcTag = gmsh::model::geo::addCircleArc(p1, circle_s.centerPointTag, p2);
        circle_s.arcTags.push_back(arcTag);
    }
    return circle_s;
}

std::vector<int> draw_cylynder_side(struct circle circle_down, struct circle circle_up, int number_of_points)
{
    std::vector<int> side_lines_tags;
    for(int i = 0; i < number_of_points; i ++){
        int line = gmsh::model::geo::addLine(circle_down.pointTags[i], circle_up.pointTags[i]);
        side_lines_tags.push_back(line);
    }
    return side_lines_tags;
}

std::vector<int> draw_curves(struct circle circle_down, struct circle circle_up, std::vector<int> side_lines_tags, int number_of_points)
{
    std::vector<int> curves_tags;
    int curveLoopTag = gmsh::model::geo::addCurveLoop(circle_down.arcTags);
    curves_tags.push_back(curveLoopTag);
    
    for(int i = 0; i <= number_of_points; i ++){
        curveLoopTag = gmsh::model::geo::addCurveLoop({side_lines_tags[i % number_of_points], circle_up.arcTags[i% number_of_points], -side_lines_tags[(i + 1) % number_of_points], -circle_down.arcTags[i% number_of_points]});
        curves_tags.push_back(curveLoopTag);       
    }
    curveLoopTag = gmsh::model::geo::addCurveLoop(circle_up.arcTags);
    curves_tags.push_back(curveLoopTag);

    return curves_tags;
}

std::vector<int> draw_surfaces(std::vector<int> curves_tags, int number_of_tags){
    std::vector<int> surfaces_tags;
    for(int i = 0; i < number_of_tags + 3; i ++){
        std::cout << i <<std::endl;
        int surf_tag = gmsh::model::geo::addPlaneSurface({curves_tags[i]});
        surfaces_tags.push_back(surf_tag);
    }
    return surfaces_tags;
}
