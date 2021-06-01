#ifndef MATH_H
#define MATH_H

#include <OpenGL/gl.h>
#include <cassert>
#include <cmath>
#include <iostream>

inline void print()
{
    std::cout << "\n";
}

template <typename T>
void print(T t)
{
    std::cout << t << "\n";
}

template <typename T, typename... Args>
void print(T t, Args... args)
{
    std::cout << t << " ";
    print(args...);
}

constexpr double pi = 3.141592653589793238462643383279502;

constexpr double radians(double theta)
{
    return theta * pi / 180;
}

constexpr double degrees(double theta)
{
    return theta * 180 / pi;
}

inline double norm(double theta)
{
    double percent = theta / 360;
    double revolutions_to_normal = std::floor(percent);
    double degrees_to_normal = revolutions_to_normal * 360;
    return theta - degrees_to_normal;
}

inline double sin_degrees(double theta)
{
    return degrees(std::sin(radians(theta)));
}

inline double cos_degrees(double theta)
{
    return degrees(std::cos(radians(theta)));
}

inline double acos_degrees(double theta)
{
    return degrees(std::acos(radians(theta)));
}

constexpr bool near(double a, double b)
{
    constexpr double epsilon = 0.00001;
    double min = b - epsilon;
    double max = b + epsilon;
    return a >= min && a <= max;
}

struct Matrix {
    double matrix[3][3] {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
    };

    double* operator[](size_t i)
    {
        return matrix[i];
    }

    const double* operator[](size_t i) const
    {
        return matrix[i];
    }

    Matrix operator+(Matrix b) const
    {
        Matrix a = *this;
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                a[i][j] += b[i][j];
            }
        }
        return a;
    }

    Matrix operator*(Matrix b) const
    {
        Matrix a = *this;
        Matrix c;

        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                for (size_t k = 0; k < 3; ++k) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }

        return c;
    }

    Matrix operator*(double scalar) const
    {
        Matrix b = *this;
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                b[i][j] *= scalar;
            }
        }
        return b;
    }
};

struct Vec {
    double x { 0 };
    double y { 0 };
    double z { 0 };

    double length() const
    {
        return std::hypot(x, y, z);
    }

    Vec norm() const
    {
        Vec a = *this;
        return a / a.length();
    }

    double dot(Vec b) const
    {
        Vec a = *this;
        // If a ~ b, dot product is 1. This is really just to avoid giving acos
        // an argument greater than 1.
        return a.near(b) ? 1 : a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec cross(Vec b) const
    {
        Vec a = *this;
        return Vec { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
    }

    double theta(Vec b) const
    {
        Vec a = *this;
        return degrees(std::acos(a.norm().dot(b.norm())));
    }

    Vec flatten_x() const
    {
        Vec a = *this;
        a.x = 0;
        return a;
    }

    Vec flatten_y() const
    {
        Vec a = *this;
        a.y = 0;
        return a;
    }

    Vec flatten_z() const
    {
        Vec a = *this;
        a.z = 0;
        return a;
    }

    bool near(Vec b) const
    {
        Vec a = *this;
        return ::near(a.x, b.x) && ::near(a.y, b.y) && ::near(a.z, b.z);
    }

    Vec operator*(Vec b) const
    {
        Vec a = *this;
        return Vec { a.x * b.x, a.y * b.y, a.z * b.z };
    }

    Vec operator+(Vec b) const
    {
        Vec a = *this;
        return Vec { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    Vec operator-(Vec b) const
    {
        Vec a = *this;
        return Vec { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    Vec operator*(double scalar) const
    {
        return Vec { x * scalar, y * scalar, z * scalar };
    }

    Vec operator/(double scalar) const
    {
        return Vec { x / scalar, y / scalar, z / scalar };
    }

    Vec operator-(double scalar) const
    {
        return Vec { x - scalar, y - scalar, z - scalar };
    }

    Vec operator*(Matrix matrix) const
    {
        Vec a = *this;
        Vec b;

        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                b[i] += a[j] * matrix[i][j];
            }
        }

        return b;
    }

    Vec rotate(Vec k, double theta) const
    {
        theta = -theta;
        theta = radians(theta);
        Vec v = *this;

        constexpr Matrix id = {
            {
                { 1, 0, 0 },
                { 0, 1, 0 },
                { 0, 0, 1 },
            }
        };

        Matrix cross = {
            {
                { 0, -k.z, k.y },
                { k.z, 0, -k.x },
                { -k.y, k.x, 0 },
            }
        };

        Matrix rotation = id + cross * std::sin(theta) + (cross * cross) * (1 - std::cos(theta));

        return v * rotation;
    }

    Vec rotate_x(double theta) const
    {
        Vec a = *this;
        theta = radians(theta);

        Matrix matrix = {
            {
                { 1, 0, 0 },
                { 0, std::cos(theta), std::sin(theta) },
                { 0, -std::sin(theta), std::cos(theta) },
            }
        };

        return a * matrix;
    }

    Vec rotate_y(double theta) const
    {
        Vec a = *this;
        theta = radians(theta);

        Matrix matrix = {
            {
                { std::cos(theta), 0, -std::sin(theta) },
                { 0, 1, 0 },
                { std::sin(theta), 0, std::cos(theta) },
            }
        };

        return a * matrix;
    }

    Vec rotate_z(double theta) const
    {
        Vec a = *this;
        theta = radians(theta);

        Matrix matrix = {
            {
                { std::cos(theta), std::sin(theta), 0 },
                { -std::sin(theta), std::cos(theta), 0 },
                { 0, 0, 1 },
            }
        };

        return a * matrix;
    }

    void draw() const
    {
        glVertex3d(x, y, z);
    }

    void draw_normal() const
    {
        glNormal3d(x, y, z);
    }

    const double& operator[](size_t i) const
    {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                assert(false);
        }
    }

    double& operator[](size_t i)
    {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                assert(false);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, Vec a)
    {
        os << a.x << ", " << a.y << ", " << a.z;
        return os;
    }
};

#endif
