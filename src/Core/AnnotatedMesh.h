#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using SimplePoint = std::tuple<float, float, float>;

class AnnotatedMesh
{
public:
    AnnotatedMesh(const std::string& path): path(path) {}
    void updateFaceAnnotations(const std::string& region, std::vector<int> faces);
    void updatePointsAnnotations(std::unordered_map<std::string, SimplePoint> points);
private:
    const std::string path;
    std::unordered_map<std::string, std::vector<int>> faces_per_region;
    std::unordered_map<std::string, SimplePoint> points;
};