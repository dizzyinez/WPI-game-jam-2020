#pragma once

/*
 * 2 dimensional vector class
 * has a lot of operator overloads
 */
struct v2d
{
        float x = 0;
        float y = 0;

        v2d() : x(0), y(0)
        {
        }
        v2d(float _x, float _y) : x(_x), y(_y)
        {
        }
        v2d(const v2d& v) : x(v.x), y(v.y)
        {
        }
        float mag() {return __builtin_sqrtf(x * x + y * y);}
        v2d  norm() {float r = 1 / mag(); return v2d(x * r, y * r);}
        v2d  perp() {return v2d(-y, x);}
        float dot(const v2d& rhs) {return this->x * rhs.x + this->y * rhs.y;}
        float cross(const v2d& rhs) {return this->x * rhs.y - this->y * rhs.x;}
        v2d operator +(const v2d& rhs) {return v2d(this->x + rhs.x, this->y + rhs.y);}
        v2d operator -(const v2d& rhs) {return v2d(this->x - rhs.x, this->y - rhs.y);}
        v2d operator *(const float& rhs) {return v2d(this->x * rhs, this->y * rhs);}
        v2d operator /(const float& rhs) {return v2d(this->x / rhs, this->y / rhs);}
        v2d& operator +=(const v2d& rhs) {this->x += rhs.x; this->y += rhs.y; return *this;}
        v2d& operator -=(const v2d& rhs) {this->x -= rhs.x; this->y -= rhs.y; return *this;}
        v2d& operator *=(const float& rhs) {this->x *= rhs; this->y *= rhs; return *this;}
        v2d& operator /=(const float& rhs) {this->x /= rhs; this->y /= rhs; return *this;}
};