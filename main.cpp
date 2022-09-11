#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#include <boost/geometry.hpp>

namespace bg = boost::geometry;

// use namespace to avoid namespace pollution
namespace PointCloud
{
    typedef bg::model::point<double, 3, bg::cs::cartesian> Point;
    typedef bg::model::multi_point<Point> Vector;

    static void GetPointCloudFromFile(std::string file_path, PointCloud::Vector &point_cloud);
    static void SendPointCloudToFile(std::string file_path, PointCloud::Vector &point_cloud);
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

    void OctalDownsample(PointCloud::Vector &);

private:
    // Goal is to subdivide until stop conditions are met
    // We use indicies to track points (since array access is fast and to avoid copying/mutating original data set)
    // We perform the subdivision by organizing points into the four coordinate planes about a central pivot point
    // The current size is tracked by a bounding box
    // The pivot is then the center of the bounding box
    // void octalHelper(PointCloud::Vector point_cloud);
    void OctalHelper(PointCloud::Vector &result, PointCloud::Vector point_cloud)
    {
        if (point_cloud.size() <= stop_points_)
        {
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

        for (PointCloud::Vector point_cloud : boxes)
        {
            Downsampler::OctalHelper(result, point_cloud);
        }
    }

    PointCloud::Vector point_cloud_;
    OutputMethod output_method_ = OutputMethod::kCentroid;
    int stop_points_ = 1;
};

int main()
{
    // PointCloud::Vector point_cloud = PointCloud::GetPointCloudFromFile("input.csv");
    // PointCloud::GetPointCloudFromFile("input.csv");

    // Downsampler downsampler = Downsampler(point_cloud);
    // PointCloud::Vector out;
    // out = downsampler.OctalDownsample();
    // int count = 0;
    // for (PointCloud::Point point : point_cloud)
    // {
    //     count++;
    // }
    // std::cout << count << std::endl;

    return 0;
}

void Downsampler::OctalDownsample(PointCloud::Vector &result)
{
    Downsampler::OctalHelper(result, point_cloud_);
}

static void PointCloud::GetPointCloudFromFile(std::string file_path, PointCloud::Vector &point_cloud)
{
    // std::fstream fin;
    // fin.open(file_path, std::ios::in);

    // std::string line, word, temp;
    // while (fin >> temp)
    // {
    //     std::getline(fin, line);
    //     std::stringstream s(line);

    //     PointCloud::Point point;
    //     std::vector<double> vals;
    //     for (int i = 0; i < 3; i++)
    //     {
    //         std::getline(s, word, ',');
    //         double value = std::stod(word);
    //         vals.push_back(value);
    //     }
    //     bg::set<0>(point, vals[0]);
    //     bg::set<1>(point, vals[1]);
    //     bg::set<2>(point, vals[2]);
    //     point_cloud.push_back(point);
    // }
}

static void PointCloud::SendPointCloudToFile(std::string file_path, PointCloud::Vector &point_cloud)
{
}