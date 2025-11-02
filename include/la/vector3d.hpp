namespace la {
class Vector3D {
  public:
    Vector3D(const double x, const double y, const double z)
        : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    friend bool operator==(const Vector3D a, const Vector3D b) {
        return a.x() == b.x() && a.y() == b.y() && a.z() == b.z();
    }

    Vector3D operator-(const Vector3D v) const {
        return Vector3D(x() - v.x(), y() - v.y(), z() - v.z());
    }

    Vector3D crossProduct(Vector3D v) const {
        return Vector3D(y() * v.z() - z() * v.y(), z() * v.x() - x() * v.z(),
                        x() * v.y() - y() * v.x());
    }

    double dotProduct(const Vector3D v) const {
        return x_ * v.x() + y_ * v.y() + z_ * v.z();
    }

  private:
    double x_;
    double y_;
    double z_;
};
} // namespace la
