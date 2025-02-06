#include <string>
#include <vector>
#include <bits/stdc++.h>

#ifndef OPTION_H
#define OPTION_H

class Option {
private:
    double K, S, r, T, sigma;
    void init();

public:
    Option();
    Option(double strike, double initalStockPrice, double rate, double time, double vol);
    ~Option();

    double getStrikePrice() const;
    double getInitialStockPrice() const;
    double getRiskFreeRate() const;
    double getTimeToMaturity() const;
    double getVolatility() const;
    void print() const;

    void setStrikePrice(double newK);
    void setInitialStockPrice(double newS);
    void setRiskFreeRate(double newR);
    void setTimeToMaturity(double newT);
    void setVolatility(double newSigma);
};

#endif

