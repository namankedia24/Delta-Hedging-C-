#include <iostream>
#include "option.h"
#include "option_pricing.h"
#include <ctime>
#include <cmath>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "stock.h"
#include "market_data.h"

using namespace std;

int main(int argc, char **argv) {

    Option_Price callPricer= Option_Price('C');
    Option_Price putPricer= Option_Price('P');

// PART 1 code is here

    // random default values
    double strike = 105;
    double initialStockPrice = 100;
    double timeToMaturity = 0.4;
    double rate = 0.025;
    double volatility = .24;
    double mu = 0.05;
    int N = 100;

    

    int loops = 1000;

    Stock stock = Stock(initialStockPrice, mu, timeToMaturity, volatility, N);

    cout << ("Calculating 1000 simulations for stocks.") << endl;
    vector<vector <double>> simulations = stock.getSimulation(loops);
    cout << ("Writing 100 simulations for stocks in stock_simulations.csv") << endl;

    vector<vector<double>> allHedgingErrors;
    
    cout << ("Calculating option prices.") << endl;
     for (int i = 0; i < loops; i++) {
        vector <double> simulation = simulations[i];
        Option o0 = Option(strike, simulation[0], rate, timeToMaturity, volatility);
        vector<double> hedgingErrors = callPricer.deltaHedgingErrors(simulation, o0, N);
        allHedgingErrors.push_back(hedgingErrors);
    }
    cout << ("Writing simulations for options in optionPrices_simulations.csv") << endl;

    ofstream outFile("hedging_results.csv");
    outFile << "Path,FinalStockPrice,FinalHedgingError\n";
    cout << ("Calculating simulations for hedging errors") << endl;
    for (int i = 0; i < loops; ++i) {
        outFile << i + 1 << "," << simulations[i].back() << "," << allHedgingErrors[i].back() << "\n";
    }
    outFile.close();
    cout << ("Writing simulations in hedging_results.csv") << endl;
    cout << ("Part 1 completed") << endl;


/////////////////////////////////////////////////// PART 1 code ends here /////////////////////////////////

/////////////////////////////////////////////////// PART 2 code starts here /////////////////////////////////



    cout << ("Part 2 started ............") << endl;
    MarketData marketData("./data/interest.csv", "./data/sec_GOOG.csv", "./data/op_GOOG.csv");

    // // Set parameters
    string startDate = "2011-07-05";
    string endDate = "2011-07-29";
    string expiryDate = "2011-09-17";
    double strikeReal = 500;
    char type = 'C';


    while(false) {
        cout << "Enter option parameters:\n";
        cout << "Strike price (K): ";
        cin >> strikeReal;
        cout << "T0 (YYYY-MM-DD): ";
        cin >> startDate;
        cout << "Tn (YYYY-MM-DD): ";
        cin >> endDate;
        cout << "T Expiry (YYYY-MM-DD): ";
        cin >> expiryDate;
        cout << "Option type (C for call, P for put): ";
        cin >> type;

        if (cin.fail() || strikeReal <= 0 || 
            (type != 'C'  && type != 'P')) {
            cout << endl;
            cout << ("Invalid input. Please check all parameters and try again.") << endl;
            break;
        }

        vector<string> relevantDates = marketData.getDatesBetween(startDate, endDate);
        vector<double> relevantOptionPrices = marketData.getDateWiseOptionPrices(relevantDates, strikeReal, expiryDate, type);
        vector<double> relevantStockPrices = marketData.getDateWiseStockPrices(relevantDates);
        vector<double> relevantRates = marketData.getDateWiseRates(relevantDates);


        double T = marketData.getTimeToMaturity(startDate, expiryDate);

        Option opt = Option(strikeReal, relevantStockPrices[0], relevantRates[0], T, 0);
        if (type == 'C') {
            callPricer.deltaHedgingErrorsWithImpliedVol(relevantStockPrices, relevantOptionPrices,relevantRates, relevantDates, opt);
        } else if (type == 'P') {
            putPricer.deltaHedgingErrorsWithImpliedVol(relevantStockPrices, relevantOptionPrices,relevantRates, relevantDates, opt);
        } else {
            cout << "Invalid Option Type";
        }

    }

    // startDate = "2011-01-03";
    // endDate = "2011-10-07";
    // expiryDate = "2013-01-19";
    // strikeReal = 360;
    // type = 'C';

    while (true) {
        cout << "Writing to file for K= " << strikeReal << " startDate: " << startDate << " enddate: " << endDate << " expiryDate: " << expiryDate << endl; 
        vector<string> relevantDates = marketData.getDatesBetween(startDate, endDate);
        vector<double> relevantOptionPrices = marketData.getDateWiseOptionPrices(relevantDates, strikeReal, expiryDate, type);
        vector<double> relevantStockPrices = marketData.getDateWiseStockPrices(relevantDates);
        vector<double> relevantRates = marketData.getDateWiseRates(relevantDates);


        double T = marketData.getTimeToMaturity(startDate, expiryDate);

        Option opt = Option(strikeReal, relevantStockPrices[0], relevantRates[0], T, 0);
        if (type == 'C') {
            callPricer.deltaHedgingErrorsWithImpliedVol(relevantStockPrices, relevantOptionPrices,relevantRates, relevantDates, opt);
        } else if (type == 'P') {
            putPricer.deltaHedgingErrorsWithImpliedVol(relevantStockPrices, relevantOptionPrices,relevantRates, relevantDates, opt);
        } else {
            cout << "Invalid Option Type";
        }
        break;
    }



    // cout << marketData.getOptionPrice(startDate, strikeReal, expiryDate, type) << endl;



    return 0;

}