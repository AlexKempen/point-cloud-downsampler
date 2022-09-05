#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

class Point
{
public:
    std::vector<double> point;
    Point(double x, double y, double z);
    Point();

    Point operator+(const Point &);
    void operator+=(const Point &);
    Point operator-(const Point &);
    void operator-=(const Point &);

    Point operator*(const double &);
    void operator*=(const double &);
    Point operator/(const double &);
    void operator/=(const double &);

    static Point getCentroid(const std::vector<Point>);
};

// class PointCloud {
//     private:
//         std::vector<Point> points;
//     public:
//         PointCloud(std::vector<Point> _points);
//         std::vector<Point> getPoints();

// };

class Downsampler
{
private:
    const static int EPSILON = 0.000001;
    std::vector<Point> pointCloud;

public:
    Downsampler(std::vector<Point> _pointCloud);
};

int main()
{
    Point p1 = Point(1, 2, 3);
    Point p2 = Point(2, 3, 4);
    p1 += p2;
    for (double v : p1.point)
    {
        std::cout << " " << v;
    }
    std::cout << std::endl;
    p1 *= 2;
    std::for_each(p1.point.begin(), p1.point.end(), [](const double &n) { std::cout << " " << n; });
    return 0;
}

Point::Point()
{
    point = std::vector<double>{0, 0, 0};
    return;
}
Point::Point(double x, double y, double z) { point = std::vector<double>{x, y, z}; }

Point Point::operator+(const Point &opP)
{
    Point out;
    std::transform(point.cbegin(), point.cend(), opP.point.cbegin(), out.point.begin(), std::plus<double>());
    return out;
}
void Point::operator+=(const Point &opP) { *this = *this + opP; }

Point Point::operator-(const Point &opP)
{
    Point out;
    std::transform(point.cbegin(), point.cend(), opP.point.cbegin(), out.point.begin(), std::minus<double>());
    return out;
}
void Point::operator-=(const Point &opP) { *this = *this - opP; }

Point Point::operator*(const double &n)
{
    Point out;
    std::transform(point.cbegin(), point.cend(), out.point.begin(), [n](double p)
                   { return p * n; });
    return out;
}
void Point::operator*=(const double &n) { *this = *this * n; }

Point Point::operator/(const double &n)
{
    Point out;
    std::transform(point.cbegin(), point.cend(), out.point.begin(), [n](double p)
                   { return p / n; });
    return out;
}
void Point::operator/=(const double &n) { *this = *this / n; }

Point Point::getCentroid(std::vector<Point> points)
{
    Point centroid = Point();
    return std::accumulate(points.begin(), points.end(), centroid, std::plus<Point>()) / points.size();
}

Downsampler::Downsampler(std::vector<Point> _pointCloud)
{
    pointCloud = _pointCloud;
}