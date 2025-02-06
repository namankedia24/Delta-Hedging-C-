#ifndef MARKET_DATA_H
#define MARKET_DATA_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class MarketData {
public:
    map<string, double> stockPrices;
    vector<string> allDates;
    
    MarketData();
    MarketData(string interestFile, string stockFile, string optionFile);
    
    double getInterestRate(string date) const;
    double getStockPrice(string date) const;
    double getOptionPrice(string date, double strike, string expiryDate, char type) const;
    
    vector<string> getDatesBetween(string startDate, string endDate);
    vector<double> getDateWiseOptionPrices(vector<string> dates, double strike, string expiryDate, char type) const;
    vector<double> getDateWiseStockPrices(vector<string> dates) const;
    vector<double> getDateWiseRates(vector<string> dates) const;
    static double getTimeToMaturity(const string& currentDate, const string& endDate);

private:
    struct OptionData {
        double strike;
        string expiryDate;
        double price;
        char typeOption;
    };

    map<string, double> interestRates;
    map<string, vector<OptionData>> optionPrices;

    void readInterestRates(const string& filename, int header_lines);
    void readStockPrices(const string& filename, int header_lines);
    void readOptionPrices(const string& filename, int header_lines);
};

#endif