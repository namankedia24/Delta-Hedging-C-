#include "stock.h"

using namespace std;

void Stock::init() {
    S0 = 100.0;
    mu = 0.05;
    T = 1.0;
    sigma = 0.2;
    N = 12;
}

Stock::Stock() {
    init();
}

Stock::Stock(double S0, double mu, double time, double vol, double N)
    : S0(S0), mu(mu), T(time), sigma(vol), N(N) {}

Stock::~Stock() {}

vector<vector <double>> Stock::getSimulation(int loops) {
    vector<vector <double>> simulations;
    double s0 = S0;
    double dt = T / N;
    default_random_engine generator;
    normal_distribution<double> distribution(0, 1);
    for (int i = 0; i < loops; i++) {
        vector<double> stockVector(N + 1, 0.0);
        stockVector[0] = s0;
        for (int j = 1; j <= N; j++) {
            double Z = distribution(generator);
            double sPrev = stockVector[j - 1];
            double Sj =  sPrev + (mu * sPrev * dt) + (sigma * sPrev * sqrt(dt) * Z);
            // cout << Sj << sPrev << endl;
            stockVector[j] = Sj;
        }
        simulations.push_back(stockVector);
    }

    ofstream outFile("stock_simulations.csv");
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < N; j++) {
            outFile << simulations[i][j];
            if (j < N - 1) {
                outFile << ",";
            }
        }
        outFile << "\n";
    }
    outFile.close();

    return  simulations;
}


double Stock::getS0() const { return S0; }
double Stock::getMu() const { return mu; }
double Stock::getT() const { return T; }
double Stock::getSigma() const { return sigma; }
double Stock::getN() const { return N; }
