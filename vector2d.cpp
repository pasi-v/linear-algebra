#include "doctest.h"


class Vector2D
{
public:
    Vector2D(const double x, const double y)
        : x_(x), y_(y) {}
    double x() const { return x_; }
    double y() const { return y_; }

    friend bool operator==(const Vector2D a, const Vector2D b) {
        return a.x() == b.x() && a.y() == b.y();
    }

    Vector2D operator*(double c) const { return Vector2D(x_ * c, y_ * c); }
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


TEST_CASE("Multiply by scalar")
{
    Vector2D v = {-1, 2};
    Vector2D expected = {-3, 6};
    CHECK_EQ(v * 3, expected);
}
