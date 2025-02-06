#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <ctime>
#include <cmath>
#include <random>
#include <vector>
#include <bits/stdc++.h>

#ifndef STOCK_H
#define STOCK_H

using namespace std;

class Stock {
private:
    double S0, mu, T, sigma, N;
    void init();

public:
    Stock();
    Stock(double S0, double mu, double time, double vol, double N);
    ~Stock();
    vector<vector<double>> getSimulation(int loops);
    double getS0() const;
    double getMu() const;
    double getT() const;
    double getSigma() const;
    double getN() const;
};

#endif

