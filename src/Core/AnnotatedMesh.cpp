#include "AnnotatedMesh.h"

void AnnotatedMesh::updateFaceAnnotations(const std::string& region, std::vector<int> faces)
{
    faces_per_region[region] = faces;
}

void AnnotatedMesh::updatePointsAnnotations(std::unordered_map<std::string, SimplePoint> points)
{
    points = points;   
}
