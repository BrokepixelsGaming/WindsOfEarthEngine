// Vector.h
#pragma once

#include "Macros.h"
#include "tools/Utilities/WOEMath.h"
//---------------------------------------------------------------------------------------------------------------------
// 2D vector
//---------------------------------------------------------------------------------------------------------------------
template<typename Type>
class Vector2
{
public:

    Type x, y;

    // construction
    Vector2(void) { x = y = 0; }
    Vector2(const Vector2& v2) { x = v2.x; y = v2.y; }
    Vector2(const Type _x, const Type _y) { x = _x; y = _y; }

    // primary operators
    Vector2& operator=(const Vector2& other) { x = other.x; y = other.y; return (*this); }
    Vector2& operator*=(Type scalar) { x *= scalar; y *= scalar; return (*this); }
    bool operator==(const Vector2& other) { return (x == other.x && y == other.y); }
    bool operator!=(const Vector2& other) { return (x != other.x || y != other.y); }
    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return (*this); }
    Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return (*this); }
    const Vector2 operator-() { return Vector2(-x, -y); }  // inverse of the vector

    // other math functions
    Type Length(void) const;
    Type LengthSquared(void) const { return ((x * x) + (y * y)); }
    void Normalize(void);
    bool IsExtremelyCloseTo(const Vector2& right) const;
    void RotateVectorByDegrees(double degree);

};


template<typename Type>
Vector2<Type> operator+(const Vector2<Type>& left, const Vector2<Type>& right)
{
    return (Vector2<Type>(left.x + right.x, left.y + right.y));
};


template<typename Type>
Vector2<Type> operator-(const Vector2<Type>& left, const Vector2<Type>& right)
{
    return (Vector2<Type>(left.x - right.x, left.y - right.y));
};

template<typename Type>
inline Vector2<Type> operator*(const Vector2<Type>& left, const Vector2<Type>& right)
{
    return (Vector2<Type>(left.x * right.x, left.y * right.y));
};

template<typename Type>
inline Vector2<Type> operator*(const Vector2<Type>& left, const Type right)
{
    return (Vector2<Type>(left.x * right, left.y * right));
};

template<typename Type, typename Type2>
inline Vector2<Type> operator*(const Vector2<Type>& left, const Type2 right)
{
    return (Vector2<Type>(left.x * right, left.y * right));
};

template<typename Type>
inline Vector2<Type> operator*(const Type left, const Vector2<Type>& right)
{
    return (Vector2<Type>(left * right.x, left * right.y));
};


template<typename Type>
Vector2<Type> operator/(const Vector2<Type>& left, const Type right)
{
    return (Vector2<Type>(left.x / right, left.y / right));
};

template<typename Type>
inline Type Vector2<Type>::Length(void) const
{
    return (Type)sqrt((x * x) + (y * y));
};

template<typename Type>
inline void Vector2<Type>::Normalize(void)
{
    if (x == 0 && y == 0)
        return;
    Type len = Length();
    x /= len;
    y /= len;
};

template<typename Type>
inline bool Vector2<Type>::IsExtremelyCloseTo(const Vector2& right) const
{
    Vector2 diff = right - (*this);
    if (diff.Length() < 0.01f)
        return true;
    return false;



};

// Will rotated the Vector by a set degrees. 
template<typename Type>
inline void Vector2<Type>::RotateVectorByDegrees(double degree)
{
    static_assert(std::is_floating_point<Type>::value);

    Type xRotated = x; 
    Type yRotated = y;

    if (x == 0)
    {
        x = static_cast<Type>(0.0000000000001);
    }
    else if (y == 0)
    {
        y = static_cast<Type>(0.0000000000001);
    }
    double angleRadians = degree * kDoublePI / 180; // Set the radians to rotate. 

    xRotated = static_cast<Type>(x * std::cos(angleRadians) - y * std::sin(angleRadians));
    yRotated = static_cast<Type>(x * std::sin(angleRadians) + y * std::cos(angleRadians));

    // need to check for the zero as it will screw up the rotation. 

    x = xRotated;
    y = yRotated;

};

using Vec2 = Vector2<float>;

