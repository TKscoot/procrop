#pragma once
#include <SDL2/SDL.h>
#include <SDL_rect.h>

#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Point : public SDL_Point
{
    Point operator+=(Point rhs)
    {
        return {this->x + rhs.x, this->y + rhs.y};
    }

    friend Point operator+(Point &lhs, const Point &rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    Point operator-=(Point rhs)
    {
        return {this->x - rhs.x, this->y - rhs.y};
    }

    friend Point operator-(Point &lhs, const Point &rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    Point operator*=(const int &rhs)
    {
        return {this->x * rhs, this->y * rhs};
    }

    friend Point operator*(int &lhs, const Point &rhs)
    {
        return {lhs * rhs.x, lhs * rhs.y};
    }

    friend Point operator*(Point &lhs, const int &rhs)
    {
        return {lhs.x * rhs, lhs.y * rhs};
    }

    Point operator/=(const int &rhs)
    {
        return {this->x / rhs, this->y / rhs};
    }

    friend Point operator/(int &lhs, const Point &rhs)
    {
        return {lhs / rhs.x, lhs / rhs.y};
    }

    friend Point operator/(Point &lhs, const int &rhs)
    {
        return {lhs.x / rhs, lhs.y / rhs};
    }

    float Length()
    {
        return std::sqrt((x * x) + (y * y));
    }

    void Normalize()
    {
        x = x / Length();
        y = y / Length();
    }

    void Test()
    {
        Point p1;
        Point p2;
        Point p3;
        p1 = p3 + p2;
        p1 += p2;
        p1 -= p2;
    }

	std::string ToString(){
		return "x(" + std::to_string(x) + ") y(" + std::to_string(y) + ")";
	}
};

struct PointF : public SDL_FPoint
{
    PointF operator+=(PointF rhs)
    {
        return {this->x + rhs.x, this->y + rhs.y};
    }

    friend PointF operator+(PointF &lhs, const PointF &rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    PointF operator-=(PointF rhs)
    {
        return {this->x - rhs.x, this->y - rhs.y};
    }

    friend PointF operator-(PointF &lhs, const PointF &rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    PointF operator*=(const int &rhs)
    {
        return {this->x * rhs, this->y * rhs};
    }

    friend PointF operator*(int &lhs, const PointF &rhs)
    {
        return {lhs * rhs.x, lhs * rhs.y};
    }

    friend PointF operator*(PointF &lhs, const int &rhs)
    {
        return {lhs.x * rhs, lhs.y * rhs};
    }

    PointF operator/=(const int &rhs)
    {
        return {this->x / rhs, this->y / rhs};
    }

    friend PointF operator/(int &lhs, const PointF &rhs)
    {
        return {lhs / rhs.x, lhs / rhs.y};
    }

    friend PointF operator/(PointF &lhs, const int &rhs)
    {
        return {lhs.x / rhs, lhs.y / rhs};
    }

    float Length()
    {
        return std::sqrt(std::pow(x, 2.0f) + std::pow(x, 2.0f));
    }

    void Normalize()
    {
        if (Length() != 0.0f)
        {

            x = x / Length();
            y = y / Length();
        }
    }

    void Test()
    {
        PointF p1;
        PointF p2;
        PointF p3;
        p1 = p3 + p2;
        p1 += p2;
        p1 -= p2;
    }
};

// Strings
using String = std::string;
using StringStream = std::stringstream;

// IDs
using ResourceID = std::uint32_t;

// Datastructures
template <typename T>
using Vector = std::vector<T>;

// template<typename T, typename Count>
// using Array = std::array<T, Count>;

template <typename First, typename Second> using UnorderedMap = std::unordered_map<First, Second>;

template <typename First, typename Second> using Set = std::set<First, Second>;

template <typename First, typename Second> using Pair = std::pair<First, Second>;
