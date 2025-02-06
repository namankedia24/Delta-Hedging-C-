#include "option_pricing.h"
#include "option.h"
#include <cmath>
#include <tuple>

using namespace std;

Option_Price::Option_Price(char f)
    : flag(f) {}

tuple<double, double> Option_Price::BSMPricer(Option o) {
    double T = o.getTimeToMaturity();
    double sigma = o.getVolatility();
    double r = o.getRiskFreeRate();
    double K = o.getStrikePrice();
    double s0 = o.getInitialStockPrice();
    double priceOption=0.0;
    double delta=0.0;

    double d1 = (log(s0 / K) + ((r + (0.5 * sigma * sigma)) * T)) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    
    double N_d1 = normalCDF(d1);
    double N_d2 = normalCDF(d2);
    
    if (flag == 'c' || flag == 'C') {
        priceOption = (s0 * N_d1) - (K * exp(-r * T) * N_d2);
        delta = N_d1;
    } else {
        priceOption = (K * exp(-r * T) * (1 - N_d2)) - (s0 * (1 - N_d1));
        delta = N_d1 - 1.0;
    }
    return make_tuple(priceOption,delta);
}

vector<double> Option_Price::deltaHedgingErrors(vector<double> stockPrices, Option o, int N) {
    vector<double> hedgingErrors(N + 1, 0.0);
    vector<double> optionPrices(N + 1, 0.0);
    double dt = o.getTimeToMaturity() / N;
    double stockPrev = stockPrices[0];
    double optionPricePrev, deltaPrev, optionPrice, delta;
    tie(optionPricePrev, deltaPrev) = BSMPricer(o);
    double bondPrev = optionPricePrev - (deltaPrev * stockPrev);

    hedgingErrors[0] = 0; 
    optionPrices[0] = optionPricePrev;

    for (int i = 1; i <= N; ++i) {
        double newStockPrice = stockPrices[i];
        double timeToMaturity = o.getTimeToMaturity() - (i * dt);
        Option o2 = Option(o.getStrikePrice(), newStockPrice, o.getRiskFreeRate(), timeToMaturity, o.getVolatility());
        tie(optionPrice, delta) = BSMPricer(o2);
        double bond = (deltaPrev * newStockPrice) + (bondPrev * exp(o.getRiskFreeRate() * dt)) - (delta * newStockPrice);
        hedgingErrors[i] = (deltaPrev * newStockPrice) + (bondPrev * exp(o.getRiskFreeRate() * dt)) - optionPrice;
        optionPrices[i] = optionPrice;
        bondPrev = bond;
        deltaPrev = delta;
    }

    ofstream outFile("optionPrices_simulations.csv", ios::app);
    for (int i = 1; i <= 100; i++) {
        outFile << optionPrices[i];
        if(i<100) {
            outFile << ",";
        }
    }
    outFile << "\n";
    outFile.close();

    return hedgingErrors;
}


vector<double> Option_Price::deltaHedgingErrorsWithImpliedVol(vector<double> stockPrices, vector<double> optionPrices, vector<double> rates, vector<string> dates, Option opt) {

    ofstream outFile("results.csv");
    outFile  << "date,S,V,implied_volatility,delta,HE,PNL,PNL_with_hedge" << endl;
    double bond = 0.0;
    double deltaPrev = 0.0;
    double pnlWithHedgePrev = 0;

    for(int index=0; index<dates.size(); index++) {
        string currDate = dates[index];
        double currStock = stockPrices[index];
        double currOptPrice = optionPrices[index];
        double currRate = rates[index];
        double dt = 1/252.0;
        double timeToMaturity = opt.getTimeToMaturity() - (index * dt);
        Option optNew = Option(opt.getStrikePrice(), currStock, currRate, timeToMaturity, 0);
        double impliedVol = calculateImpliedVolatility(optNew, currOptPrice);
        optNew.setVolatility(impliedVol);
        double delta,BSMprice;
        tie(BSMprice, delta) = BSMPricer(optNew);
        double pnl = 0;
        double pnlWithHedge = 0;

        double prevRate;
        if(index==0) {
            bond = currOptPrice - (delta * currStock);
        } else {
            prevRate = rates[index-1];
            // cout << bond << " " << deltaPrev << " " << currOptPrice << " " << currStock << " " << delta << " " << prevRate << " " << dt << " " <<  pnlWithHedge << endl;
            pnlWithHedge = (deltaPrev * currStock) + (bond * exp(prevRate * dt)) - currOptPrice;
            bond = (deltaPrev * currStock) + (bond * exp(prevRate * dt)) - (delta * currStock);
            pnl = optionPrices[0] - currOptPrice;
            // cout << bond << " " << deltaPrev << " " << currOptPrice << " " << currStock << " " << delta << " " << prevRate << " " << dt << " " <<  pnlWithHedge << endl;
            // break;
        }
        outFile << currDate << "," << currStock << "," << currOptPrice << "," << impliedVol << "," << delta << "," << (pnlWithHedge - pnlWithHedgePrev) << "," << pnl << "," << pnlWithHedge << endl;
        pnlWithHedgePrev = pnlWithHedge;
        deltaPrev = delta;    
    }
    outFile.close();
    vector<double> hedgingErrors(1, 0.0);

    cout << "Results have been written to result.csv" << endl;
    return hedgingErrors;
}

double Option_Price::calculateImpliedVolatility(const Option& opt, double marketPrice) {
    double price,delta;
    double low = 0.0001, high = 1.0;
    int iter = 0;
    double mid = (low + high) / 2.0;
    Option tempOpt = opt;
    tempOpt.setVolatility(mid);
    tie(price, delta) = BSMPricer(tempOpt);
    // tempOpt.print();
    // cout << mid << " " << price << endl;
    while (abs(price - marketPrice) > 0.00001 && iter++ < 1000) {
        // tempOpt.print();
        // cout << mid << " " << price << endl;
        mid = (low + high) / 2.0;
        tempOpt.setVolatility(mid);
        tie(price, delta) = BSMPricer(tempOpt);
        if (abs(price - marketPrice) < 0.00001) {
                return mid;
        }
        if (price < marketPrice) {
            low = mid;
        }
        else {
          high = mid;   
        }
    }
    return mid;
}

double Option_Price::normalCDF(double x) const {
    return 0.5 * (1 + erf(x / sqrt(2)));
}