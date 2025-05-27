#include "doctest.h"
#include <cmath>

class Vector2D {
  public:
    Vector2D(const double x, const double y) : x_(x), y_(y) {}
    // TODO: Vector2D from magnitude and direction
    double x() const { return x_; }
    double y() const { return y_; }

    friend bool operator==(const Vector2D a, const Vector2D b) {
        return a.x() == b.x() && a.y() == b.y();
    }

    Vector2D operator*(double c) const { return Vector2D(x_ * c, y_ * c); }

    Vector2D operator+(const Vector2D v) const { return Vector2D(x_ + v.x(), y_ + v.y()); }

    Vector2D operator-(const Vector2D v) const { return Vector2D(x_ - v.x(), y_ - v.y()); }

    double length() const { return std::sqrt(x_ * x_ + y_ * y_); }

    /** @return the direction of the vector in radians in [-pi, pi] */
    double directionRad() const { return atan2(y_, x_); }

    /** @return the direction of the vector in radians in [0, 2pi] */
    double directionRad0To2Pi() const;

    /** @return the direction of the vector in degrees in [-180, 180] */
    double directionDeg() const { return directionRad() * (180.0 / M_PI); }

    /** @return the direction of the vector in degrees in [0, 360] */
    double directionDeg0To360() const { return directionRad0To2Pi() * (180.0 / M_PI); }

  private:
    double x_;
    double y_;
};

double Vector2D::directionRad0To2Pi() const {
    double angle = directionRad();
    return angle < 0 ? angle + 2.0 * M_PI : angle;
}

struct Point2D {
    double x, y;
};

struct DirectedVector2D {
    Point2D start;
    Point2D end;

    Vector2D direction() const { return Vector2D{end.x - start.x, end.y - start.y}; }

    double length() const {
        auto d = direction();
        return std::sqrt(d.x() * d.x() + d.y() * d.y());
    }
};

TEST_CASE("Construct vector") {
    Vector2D v = {1, 2};
    CHECK_EQ(v.x(), 1);
    CHECK_EQ(v.y(), 2);
}

TEST_CASE("Length") {
    Vector2D v = {3, 4};
    CHECK_EQ(v.length(), 5.0);
}

TEST_CASE("Multiply by scalar") {
    Vector2D v = {-1, 2};
    Vector2D expected = {-3, 6};
    CHECK_EQ(v * 3, expected);
}

TEST_CASE("Operator +") {
    Vector2D a = {2, 3};
    Vector2D b = {5, -1};
    Vector2D expected = {7, 2};
    CHECK_EQ(a + b, expected);
}

TEST_CASE("Operator -") {
    Vector2D a = {2, 3};
    Vector2D b = {5, -1};
    Vector2D expected = {-3, 4};
    CHECK_EQ(a - b, expected);
}

TEST_CASE("direction") {
    Vector2D v = {3, 4};
    CHECK(v.directionRad() == doctest::Approx(0.93).epsilon(0.01));
    CHECK(v.directionRad0To2Pi() == doctest::Approx(0.93).epsilon(0.01));
}

TEST_CASE("direction in quadrant 4") {
    Vector2D v = {3, -4};
    CHECK(v.directionRad() == doctest::Approx(-0.93).epsilon(0.01));
    CHECK(v.directionRad0To2Pi() == doctest::Approx(5.36).epsilon(0.01));
}

TEST_CASE("direction in degrees") {
    Vector2D v = {3, 4};
    CHECK(v.directionDeg() == doctest::Approx(53.0).epsilon(0.01));
    CHECK(v.directionDeg0To360() == doctest::Approx(53.0).epsilon(0.01));
}

TEST_CASE("direction in degrees in quadrant 4") {
    Vector2D v = {3, -4};
    CHECK(v.directionDeg0To360() == doctest::Approx(307.0).epsilon(0.01));
}

TEST_CASE("DirectedVector direction and length") {
    DirectedVector2D dv = {{2, 3}, {5, -1}};
    Vector2D direction = dv.direction();
    Vector2D expected = {3, -4};
    CHECK_EQ(direction, expected);
    CHECK_EQ(dv.length(), 5);
}
