#ifndef TEST_H
#define TEST_H

#include <gtest/gtest.h>
#include "option_pricing.h"
#include "market_data.h"

using namespace std;

class PricerTest : public ::testing::Test {
protected:
    Option_Price callPricer{'C'};
    Option_Price putPricer{'P'};
};

TEST_F(PricerTest, BSMPricerCallOption) {
    Option o(180, 100, 0.05, 4, 0.2);
    auto result = callPricer.BSMPricer(o);
    
    EXPECT_NEAR(get<0>(result), 4.23, 0.01);
    EXPECT_NEAR(get<1>(result), 0.22, 0.01);
}

TEST_F(PricerTest, CalculateImpliedVolatility) {
    Option opt = Option(500, 532, 0.0019, .2143, 0);
    double marketPrice = 44.2;
    double impliedVol = callPricer.calculateImpliedVolatility(opt, marketPrice);
    EXPECT_NEAR(impliedVol, 0.26, 0.02);
}

TEST_F(PricerTest, CalculateImpliedVolatility2) {
    Option opt = Option(360, 604.35, 0.0029, 2.123, 0);
    double marketPrice = 266.5;
    double impliedVol = callPricer.calculateImpliedVolatility(opt, marketPrice);
    EXPECT_NEAR(impliedVol, 0.36, 0.02);
}

TEST_F(PricerTest, CalculateDelta) {
    Option opt = Option(360, 604.35, 0.0029, 2.123, 0);
    double marketPrice = 266.5;
    double impliedVol = callPricer.calculateImpliedVolatility(opt, marketPrice);
    opt.setVolatility(impliedVol);
    auto result = callPricer.BSMPricer(opt);
    EXPECT_NEAR(get<0>(result), 266.5, 0.1);
    EXPECT_NEAR(get<1>(result), 0.89, 0.1);
}

TEST_F(PricerTest, CalculateDelta2) {
    Option opt = Option(500, 532, 0.0019, .2143, 0);
    double marketPrice = 44.2;
    double impliedVol = callPricer.calculateImpliedVolatility(opt, marketPrice);
    opt.setVolatility(impliedVol);
    auto result = callPricer.BSMPricer(opt);
    EXPECT_NEAR(get<0>(result), 44.2, 0.1);
    EXPECT_NEAR(get<1>(result), 0.79, 0.1);
}

TEST_F(PricerTest, CalculateTimeToMaturity) {
    double T = MarketData().getTimeToMaturity("2011-07-05", "2011-09-17");
    EXPECT_NEAR(T, .2143, 0.001);
}

#endif