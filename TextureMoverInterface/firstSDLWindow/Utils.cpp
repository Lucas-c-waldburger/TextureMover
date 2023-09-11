#include "Utils.h"

using namespace Utils;

/*---------------------------------------------- LDPoint ----------------------------------------------*/

Utils::LDPoint::LDPoint() : SDL_Point() { x = y = 0; };
Utils::LDPoint::LDPoint(int xPos, int yPos) : SDL_Point() { x = xPos; y = yPos; };
Utils::LDPoint::LDPoint(const LDPoint& other) { x = other.x; y = other.y; };
LDPoint& Utils::LDPoint::operator=(const LDPoint& other) { if (*this != other) { this->x = other.x; this->y = other.y; } return *this; }

bool Utils::LDPoint::operator==(const LDPoint& other) const { return this->x == other.x && this->y == other.y; }
bool Utils::LDPoint::operator!=(const LDPoint& other) const { return this->x != other.x || this->y != other.y; }

LDPoint Utils::LDPoint::operator+(int i) { return LDPoint{ this->x + i, this->y + i }; }
LDPoint Utils::LDPoint::operator+(LDPoint& other) { return LDPoint{ this->x + other.x, this->y + other.y }; }

LDPoint Utils::LDPoint::operator-(int i) { return LDPoint{ this->x - i, this->y - i }; }
LDPoint Utils::LDPoint::operator-(LDPoint& other) { return LDPoint{ this->x - other.x, this->y - other.y }; }

LDPoint Utils::LDPoint::operator*(int i) { return LDPoint{ this->x * i, this->y * i }; }
LDPoint Utils::LDPoint::operator*(float f) { return LDPoint{ static_cast<int>(this->x * f), static_cast<int>(this->y * f) }; }
LDPoint Utils::LDPoint::operator*(LDPoint& other) { return LDPoint{ this->x * other.x, this->y * other.y }; }
LDPoint Utils::LDPoint::operator*(const LDPoint& other) { return LDPoint{ this->x * other.x, this->y * other.y }; }

LDPoint Utils::LDPoint::operator/(int i) { return LDPoint{ this->x / i, this->y / i }; }
LDPoint Utils::LDPoint::operator/(float f) { return LDPoint{ static_cast<int>(this->x / f), static_cast<int>(this->y / f) }; }
LDPoint Utils::LDPoint::operator/(LDPoint& other) { return LDPoint{ this->x / other.x, this->y / other.y }; }
LDPoint Utils::LDPoint::operator/(const LDPoint& other) { return LDPoint{ this->x / other.x, this->y / other.y }; }

void Utils::LDPoint::operator+=(LDPoint& other) { this->x += other.x; this->y += other.y; }
void Utils::LDPoint::operator+=(int i) { this->x += i; this->y += i; }
void Utils::LDPoint::operator-=(LDPoint& other) { this->x -= other.x; this->y -= other.y; }
void Utils::LDPoint::operator-=(int i) { this->x -= i; this->y -= i; }
void Utils::LDPoint::operator*=(float f) { this->x *= f; this->y *= f; }
void Utils::LDPoint::operator/=(float f) { this->x /= f; this->y /= f; }

std::ostream& Utils::operator<<(std::ostream& stream, LDPoint& p) { stream << p.x << ',' << p.y; return stream; };

LDPoint& Utils::LDPoint::flipXY()
{
	int tempY = y;
	y = x;
	x = tempY;

	return *this;
}

LDPoint Utils::LDPoint::flipXY(const LDPoint& point)
{
	LDPoint flippedPoint;

	flippedPoint.x = point.y;
	flippedPoint.y = point.x;

	return flippedPoint;
}



/*---------------------------------------------- Equations ----------------------------------------------*/

float Utils::Equations::approxSqrt(int originalNumber, int precision = 7)
{
	int tensBase = std::pow(10, (numberOfDigits(originalNumber) >> 1));
	int guess1 = (2 * tensBase), guess2 = (6 * tensBase);
	float approxSqrt = (originalNumber - (guess1 * guess1) < abs(originalNumber - (guess2 * guess2))) ? guess1 : guess2;

	for (int i = 0; i < precision; i++)
		approxSqrt = (approxSqrt + originalNumber / approxSqrt) / 2;

	return approxSqrt;
}

double Utils::Equations::getLineLength(const LDPoint& A, const LDPoint& B)
{
	return approxSqrt(((B.x - A.x) * (B.x - A.x)) + ((B.y - A.y) * (B.y - A.y)));
}

int Utils::Equations::sineWaveFunc(int x, int amp, float per)
{

	return amp * sin(per * x);
}

int Utils::Equations::triangleWaveFunc(int x, int amp, float per)
{
	return (amp /(per / 2)) * ((per / 2) - abs(x % static_cast<int>(2 * (per / 2) - per)));
}

int Utils::Equations::arcSineFunc(int y, int amp, float per)
{
	// +amp makes vert wave start going right, -amp left 
	return amp * asin(per * y);
}



LDPoint Utils::Equations::projectPointAlongLine(const LDPoint& A, const LDPoint& B, int distance)
{
	double lenAB = getLineLength(A, B);

	LDPoint C;
	C.x = A.x + (distance * (B.x - A.x) / lenAB);
	C.y = A.y + (distance * (B.y - A.y) / lenAB);

	return C;
}

LDPoint Utils::Equations::projectPointAlongLine(const LDPoint& A, const LDPoint& B, double lineLength, int distance)
{
	LDPoint C;

	float xAdjust = (distance * (B.x - A.x) / lineLength);
	//int xAdjustInt = (abs(xAdjust) < 0.5) ? std::floor(xAdjust) : std::ceil(xAdjust);

	float yAdjust = (distance * (B.y - A.y) / lineLength);
	//int yAdjustInt = (abs(yAdjust) < 0.5) ? std::floor(yAdjust) : std::ceil(yAdjust);

	C.x = A.x + xAdjust;
	C.y = A.y + yAdjust;

	return C;
}

void Utils::Equations::flipPoint(LDPoint& point, int xSign, int ySign)
{
	int tempY = point.y;
	point.y = point.x;
	point.x = tempY;

	point.x *= xSign; point.y *= ySign;
}

int Utils::Equations::numberOfDigits(int i)
{
	int numDigits = 0;

	do ++numDigits;
	while (i /= 10);

	return numDigits;
}

void Utils::Log::log(const std::string& message, const LDPoint& point)
{
	std::cout << message << ": (" << point.x << ", " << point.y << ")\n";
}


