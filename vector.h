#include <initializer_list>
#include <vector>

/**
 * An n-dimensional vector useful for linear algebra calculations
 */
class Vector {
  public:
    /** @return a Vector from the initializer list data */
    static Vector from_values(std::initializer_list<double> data);

    Vector() = default;

    /** @return a Zero Vector with size s */
    explicit Vector(std::size_t s) : data_(s, 0.0) {}

    explicit Vector(int s) : Vector(static_cast<std::size_t>(s)) {
        if (s < 0)
            throw std::invalid_argument("negative size");
    }

    /** @return the element at i with range check */
    double &operator[](int i) { return data_.at(i); }

    /** @return the element at i without range check */
    double operator[](int i) const { return data_.at(i); }

    /** @return true if the vector elements are the same */
    friend bool operator==(const Vector &a, const Vector &b) { return a.data_ == b.data_; }

    /** @return the size (dimensions) of the vector */
    int size() const { return data_.size(); }

    /** @return the data of the vector */
    const std::vector<double> &data() const { return data_; }

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

    /** @return true if all elements are zero, false otherwise */
    bool is_zero() const;

    /** @return index of first non-zero element, -1 if all zeroes */
    int first_non_zero_column() const;

    /** @return first non-zero element, 0 if all zeros */
    int leading_element() const;

    /** @return true if leading element is 1, false otherwise */
    bool has_leading_one() const { return leading_element() == 1; };

    /**
     * @return true if this is a standard basis vector, i.e. has 1 in one
     * element and the rest are zeros. */
    bool is_standard_basis() const;

  private:
    std::vector<double> data_;
};
