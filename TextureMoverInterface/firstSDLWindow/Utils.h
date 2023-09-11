#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include "cordic.h"

 
#define SCREEN_RATIO (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
#define INT_MAX std::numeric_limits<int>::max()

namespace Utils
{

    struct LDPoint : public SDL_Point
    {
        LDPoint();
        LDPoint(int xPos, int yPos);
        LDPoint(const LDPoint& other);
        LDPoint& operator=(const LDPoint& other);

        bool operator==(const LDPoint& other) const;
        bool operator!=(const LDPoint& other) const;

        LDPoint operator+(int i);
        LDPoint operator+(LDPoint& other);

        LDPoint operator-(int i);
        LDPoint operator-(LDPoint& other);

        LDPoint operator*(int i);
        LDPoint operator*(float f);
        LDPoint operator*(LDPoint& other);
        LDPoint operator*(const LDPoint& other);

        LDPoint operator/(int i);
        LDPoint operator/(float f);
        LDPoint operator/(LDPoint& other);
        LDPoint operator/(const LDPoint& other);

        void operator+=(LDPoint& other);
        void operator+=(int i);
        void operator-=(LDPoint& other);
        void operator-=(int i);
        void operator*=(float f);
        void operator/=(float f);

        friend std::ostream& operator<<(std::ostream& stream, LDPoint& p);

        LDPoint& flipXY();
        static LDPoint flipXY(const LDPoint& point);
    };


    namespace Equations
    {

        float approxSqrt(int originalNumber, int precision);
        double getLineLength(const LDPoint& A, const LDPoint& B);
        int sineWaveFunc(int x, int amp, float per);
        int triangleWaveFunc(int x, int amp, float per);
        int arcSineFunc(int y, int amp, float per);
        LDPoint projectPointAlongLine(const LDPoint& A, const LDPoint& B, int distance);
        LDPoint projectPointAlongLine(const LDPoint& A, const LDPoint& B, double lineLength, int distance);
        void flipPoint(LDPoint& point, int xSign = 1, int ySign = 1);
        int numberOfDigits(int i);
    };

    namespace Log
    {
        void log(const std::string& message, const LDPoint& point);
    };




}
