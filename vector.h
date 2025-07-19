#include <initializer_list>
#include <vector>

/**
 * An n-dimensional vector useful for linear algebra calculations
 */
class Vector {
  public:
    /** @return a Vector from the initializer list data */
    Vector(std::initializer_list<double> data) : data_(std::move(data)) {}

    /** @return a Zero Vector with size s */
    Vector(int s);

    /** @return the element at i with range check */
    double &operator[](int i) { return data_.at(i); }

    /** @return the element at i without range check */
    double operator[](int i) const { return data_.at(i); }

    /** @return true if the vector elements are the same */
    friend bool operator==(const Vector &a, const Vector &b) { return a.data_ == b.data_; }

    /** @return the size (dimensions) of the vector */
    int size() const { return data_.size(); }

    /** @return the sum of this and the other vector */
    Vector operator+(const Vector &v) const;

    /** @return this vector multiplied by the scalar c */
    Vector operator*(double c) const;

    /** @return the difference between this and the other vector */
    Vector operator-(const Vector &v) const;

    /** @return the dot product betwee this and the other vector */
    double dot_product(const Vector &v) const;

    /** @return the length (magnitude) of this vector */
    double length() const { return std::sqrt(dot_product(*this)); }

    /** @return the magnitude of the difference of the vectors */
    double difference(const Vector &v) const;

    /** @return the angle between the vectors in radians */
    double angle(const Vector &v) const;

    /** @return the projection of v onto this vector */
    Vector proj(const Vector &v) const;

  private:
    std::vector<double> data_;
};
