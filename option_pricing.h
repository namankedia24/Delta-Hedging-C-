#ifndef OPTION_PRICE_H
#define OPTION_PRICE_H

#include <tuple>
#include "option.h"

using namespace std;

class Option_Price {
public:
    char flag;

    Option_Price(char f);
    tuple<double, double> BSMPricer(Option o);
    vector<double> deltaHedgingErrors(vector<double> stockPrices, Option o, int N);
    vector<double> deltaHedgingErrorsWithImpliedVol(vector<double> stockPrices, vector<double> optionPrices, vector<double> rates, vector<string> dates, Option opt);
    double calculateImpliedVolatility(const Option& opt, double marketPrice); 

private: 
    double BinomialDelta(double S, double u, double d, double Cu, double Cd) const;
    double normalCDF(double x) const;

};

#endif