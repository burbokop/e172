#include <complex>
#include <math.h>


#include "vector.h"
#include "math.h"
#include "complex.h"
#include <ostream>


namespace e172 {

Vector::Vector() {
    this->m_x = 0;
    this->m_y = 0;
}

Vector::Vector(double x, double y) {
    this->m_x = x;
    this->m_y = y;
}

Vector::Vector(const Complex &complex) : Vector(complex.real(), complex.imag()) {}

Vector Vector::createByAngle(double module, double angle) {
    return Vector(module * e172::Math::cos(angle), module * e172::Math::sin(angle));
}

Vector Vector::createRandom(double max) {
    return Vector::createByAngle(std::fmod(rand(), max), rand());
}

Vector Vector::operator+(const Vector &term) const {
    return Vector(this->m_x + term.m_x, this->m_y + term.m_y);
}

void Vector::operator+=(const Vector &term) {
    this->m_x += term.m_x;
    this->m_y += term.m_y;
}

Vector Vector::operator-(const Vector &subtrahend) const {
    return Vector(this->m_x - subtrahend.m_x, this->m_y - subtrahend.m_y);
}

size_t Vector::fractalLevel(size_t limit, const ComplexFunction &f) const {
    return Math::fractalLevel(toComplex(), limit, f);
}

void Vector::operator-=(const Vector &subtrahend) {
    this->m_x -= subtrahend.m_x;
    this->m_y -= subtrahend.m_y;
}

Vector Vector::operator*(double multiplier) const {
    return Vector(this->m_x * multiplier, this->m_y * multiplier);
}

bool Vector::sameDirection(const Vector &other) const {
    return *this * other > 0;
}

Vector operator*(double scalar, const Vector &vector) {
    return Vector(vector.m_x * scalar, vector.m_y * scalar);
}

void Vector::operator*=(double multiplier) {
    this->m_x *= multiplier;
    this->m_y *= multiplier;
}

Vector Vector::operator/(double divider) const {
    return Vector(this->m_x / divider, this->m_y / divider);
}

void Vector::operator/=(double divider) {
    this->m_x /= divider;
    this->m_y /= divider;
}

bool Vector::operator==(Vector vector) const {
    return e172::Math::cmpf(this->m_x, vector.m_x) && e172::Math::cmpf(this->m_y, vector.m_y);
}

bool Vector::operator!=(Vector vector) const {
    return !e172::Math::cmpf(this->m_x, vector.m_x) || !e172::Math::cmpf(this->m_y, vector.m_y);
}

double Vector::operator*(const Vector& multiplier) const {
    return this->m_x * multiplier.m_x + this->m_y * multiplier.m_y;
}

double Vector::module() const {
    return std::sqrt(this->m_x * this->m_x + this->m_y * this->m_y);
}

double Vector::cheapModule() const {
    return std::abs(m_x) + std::abs(m_y);
}

Vector Vector::normalized() const {
    const auto module = this->module();
    if(!e172::Math::cmpf(module, 0)) {
        return Vector(this->m_x / module, this->m_y / module);
    } else {
        return Vector();
    }
}

Vector Vector::projection(const Vector &b) const {
    const auto bm2 = b * b;
    if(bm2 != e172::Math::null) {
        const auto k = (*this * b) / bm2;
        return { k * b.x(), k * b.y() };
    }
    return {};
}

Vector Vector::leftNormal() const {
    return { m_y, -m_x };
}

Vector Vector::rightNormal() const {
    return { -m_y, m_x };
}

double Vector::angle() const {
    double cos = this->normalized() * Vector(1, 0);
    if(this->m_y >= 0) return e172::Math::acos(cos);
    else return -e172::Math::acos(cos);
}



Vector Vector::relativisticAddition(Vector term, double c) const {
    double termModule = term.module();
    if(!e172::Math::cmpf(termModule, 0)) {
        c *= RELATIVISTIC_ADDITION_CONSTANT;
        const Vector classicSum = *this + term;

        double thisModule = this->module();
        double classicSumModule = classicSum.module();

        double k = e172::Math::sqrt((1 + thisModule * termModule / (c * c)));
        double u = classicSumModule / k;

        return Vector(u * classicSum.m_x / classicSumModule, u * classicSum.m_y / classicSumModule);
    }
    return *this;
}

double Vector::tg() const {
    if(!e172::Math::cmpf(this->m_y, 0))
        return -(this->m_x / this->m_y);
    return 0.0;
}

unsigned Vector::quarter(unsigned offset) const {
    if(this->m_y >= 0) {
        if(this->m_x >= 0) {
            return (0 + offset) % 4;
        }
        else {
            return (1 + offset) % 4;
        }
    }
    else {
        if(this->m_x >= 0) {
            return (3 + offset) % 4;
        }
        else {
            return (2 + offset) % 4;
        }
    }
}

double Vector::map(Vector *destination, double value) const {
    double result = 0;
    double kd = destination->m_y - destination->m_x;
    double kt = this->m_y - this->m_x;

    if(e172::Math::cmpf(kt, 0)) result = value * kd;
    else result = value * kd / kt;

    result += destination->m_x;

    return result;
}

Vector Vector::operator+(double term) const {
    double module = this->module();
    if(module != 0.0) {
        double mul = 1 + term / module;
        return Vector(this->m_x * mul, this->m_y * mul);
    }
    return Vector();
}

Vector Vector::operator-(double subtrahend) const  {
    double module = this->module();
    if(module != 0.0) {
        double mul = 1 - subtrahend / module;
        return Vector(this->m_x * mul, this->m_y * mul);
    }
    return Vector();
}

Vector Vector::operator-() const {
    return { -m_x, -m_y };
}

bool Vector::moduleLessComparator(const Vector &v0, const Vector &v1) {
    return v0.module() < v1.module();
}

std::ostream &operator<<(std::ostream &os, const e172::Vector &dt) {
    return os << "Vector(" << std::to_string(dt.x()) << ", " << std::to_string(dt.y()) << ")";
}

bool operator<(const e172::Vector &vec0, const e172::Vector &vec1) {
    return vec0.cheapModule() < vec1.cheapModule();
}

}
