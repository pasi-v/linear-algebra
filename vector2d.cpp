#include "doctest.h"


class Vector2D
{
public:
    Vector2D(const double x, const double y)
        : x_(x), y_(y) {}
    double x() const { return x_; }
    double y() const { return y_; }
    double length() const { return std::sqrt(x_ * x_ + y_ * y_); }

private:
    double x_;
    double y_;
};


TEST_CASE("Construct vector")
{
    Vector2D v = {1, 2};
    CHECK_EQ(v.x(), 1);
    CHECK_EQ(v.y(), 2);
}


TEST_CASE("Length")
{
    Vector2D v = {3, 4};
    CHECK_EQ(v.length(), 5.0);
}
