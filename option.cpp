#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <ctime>
#include <cmath>
#include <bits/stdc++.h>

#include "option.h"

using namespace std;

void Option::init() {
    K = 100.0;
    S = 100.0;
    r = 0.05;
    T = 1.0;
    sigma = 0.2;
}

Option::Option() {
    init();
}

Option::Option(double strike, double initalStockPrice, double rate, double time, double vol)
: K(strike), S(initalStockPrice), r(rate), T(time), sigma(vol) {}

Option::~Option() {}

double Option::getStrikePrice() const {
    return K;
}
double Option::getInitialStockPrice() const {
    return S;
}
double Option::getRiskFreeRate() const {
    return r;
}
double Option::getTimeToMaturity() const {
    return T;
}
double Option::getVolatility() const {
    return sigma;
}

void Option::setStrikePrice(double newK) {
    K = newK;
}

void Option::setInitialStockPrice(double newS) {
    S = newS;
}

void Option::setRiskFreeRate(double newR) {
    r = newR;
}

void Option::setTimeToMaturity(double newT) {
    T = newT;
}

void Option::setVolatility(double newSigma) {
    sigma = newSigma;
}

void Option::print() const {
    cout << "Option Details: " << fixed << setprecision(4)
         << "K=" << K << ", "
         << "S0=" << S << ", "
         << "r=" << r * 100 << "%, "
         << "T=" << T << ", "
         << "sigma=" << sigma * 100 << "%" << endl;
}
