#include <iostream>
#include <vector>
#include <cmath>

#include <boost/geometry.hpp>

namespace bg = boost::geometry;

// use namespace to avoid namespace pollution
namespace PointCloud
{
    typedef bg::model::point<double, 3, bg::cs::cartesian> Point;
    typedef bg::model::multi_point<Point> Vector;

    static PointCloud::Vector GetPointCloudFromFile(std::string file_path);
    static void SendPointCloudToFile(std::string file_path, PointCloud::Vector point_cloud);
}

class Downsampler
{
public:
    enum class OutputMethod
    {
        kFirstPoint,
        kCentroid,
        kMedoid
    };

    inline Downsampler(PointCloud::Vector point_cloud) { point_cloud_ = point_cloud; }

    inline PointCloud::Vector point_cloud() { return point_cloud_; }
    inline void point_cloud(PointCloud::Vector point_cloud) { point_cloud_ = point_cloud; };

    inline OutputMethod output_method() { return output_method_; }
    inline void output_method(Downsampler::OutputMethod output_method) { output_method_ = output_method; }

    PointCloud::Vector OctalDownsample();

private:
    // Goal is to subdivide until stop conditions are met
    // We use indicies to track points (since array access is fast and to avoid copying/mutating original data set)
    // We perform the subdivision by organizing points into the four coordinate planes about a central pivot point
    // The current size is tracked by a bounding box
    // The pivot is then the center of the bounding box
    // void octalHelper(PointCloud::Vector point_cloud);
    void Downsampler::OctalHelper(PointCloud::Vector &result, PointCloud::Vector point_cloud)
    {
        if (point_cloud.size() <= stop_points_) {
            result.insert(result.end(), point_cloud.begin(), point_cloud.end());
            return;
        }

        bg::model::box<PointCloud::Point> box;
        bg::envelope(point_cloud, box);

        PointCloud::Point center;
        bg::centroid(box, center);

        std::vector<PointCloud::Vector> boxes(8);
        for (PointCloud::Point point : point_cloud)
        {
            bool x_greater = point.get<0>() > center.get<0>();
            bool y_greater = point.get<1>() > center.get<1>();
            bool z_greater = point.get<2>() > center.get<2>();
            const int kIndex = (4 * x_greater) + (2 * y_greater) + (z_greater);
            boxes[kIndex].push_back(point);
        }

        for (PointCloud::Vector point_cloud : boxes) { Downsampler::OctalHelper(result, point_cloud); }
    }

    PointCloud::Vector point_cloud_;
    OutputMethod output_method_ = OutputMethod::kCentroid;
    int stop_points_ = 1;
};

int main()
{
    PointCloud::Vector point_cloud = PointCloud::GetPointCloudFromFile("input.csv");
    Downsampler downsampler = Downsampler(point_cloud);
    PointCloud::Vector out;
    out = downsampler.OctalDownsample();
    int count = 0;
    for (PointCloud::Point point : out) {
        count++;
    }
    std::cout << count << std::endl;

    return 0;
}

PointCloud::Vector Downsampler::OctalDownsample()
{
    PointCloud::Vector result;
    Downsampler::OctalHelper(result, point_cloud_);
    return result;
}

PointCloud::Vector PointCloud::GetPointCloudFromFile(std::string file_path)
{
    // std::fstream fin;
    // fin.open(file_path, std::ios::in);

    // PointCloud::Vector point_cloud;
    // std::string line;
    // while (fin >> line) {

    // }


    return {};
}

void PointCloud::SendPointCloudToFile(std::string file_path, PointCloud::Vector point_cloud)
{
}