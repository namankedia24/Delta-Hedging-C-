# Option Pricing and Hedging Strategies

## Project Overview
This project focuses on option pricing and hedging strategies, particularly implementing the Black-Scholes-Merton (BSM) model and delta hedging. It consists of two main parts:

1. **Simulating Stock Prices:** Randomly generating stock prices and calculating option prices and hedging errors. The distribution of stock prices, call prices, and cumulative hedging errors are analyzed.
2. **Real Market Data Analysis:** Using real market data of Google Stock to calculate implied volatilities and perform delta hedging.

## Code Structure

### Files and Modules
- **Headers:** `option.h`, `stock.h`, `option_pricing.h`, `market_data.h`
- **Source Files:** `main.cpp`, `option.cpp`, `stock.cpp`, `option_pricing.cpp`, `market_data.cpp`
- **Unit Test Files:** `pricer_test.cpp`, `pricer_test.h`
- **Plotting Scripts:** `plots.py` (Matplotlib used for visualizations)
- **Outputs:**
  - `stock_simulations_plot.png` - Sample of 100 stock price simulations
  - `optionPrices_simulation.png` - Corresponding option prices for the stocks
  - `Hedging_error_distribution.png` - Hedging error distribution
  - `Results.csv` - Tabulated output with hedging errors, PnL, and PnL with hedging for selected dates

### Key Classes and Functions
#### **1. Option Class (`option.h`)**
- Represents an option contract with properties like strike price, initial stock price, risk-free rate, time to maturity, and volatility.
- Provides getters and setters for these properties.

#### **2. Option Pricing Class (`option_pricing.h`)**
- Implements core option pricing and hedging functionalities.
- Key functions:
  - `BSMPricer`: Computes option price and delta using the Black-Scholes-Merton model.
  - `deltaHedgingErrors`: Calculates hedging errors for a stock price path.
  - `calculateImpliedVolatility`: Determines implied volatility from observed market prices using binary search.
  - `deltaHedgingErrorsWithImpliedVol`: Computes hedging errors using implied volatility.
  - `normalCDF`: Standard normal cumulative distribution function.

#### **3. Market Data Class (`market_data.h`)**
- Reads and stores market data from CSV files.
- Provides functions for retrieving interest rates, stock prices, and option prices.
- Utility functions for business day calculations.

#### **4. Stock Class (`stock.h`)**
- Simulates stock price paths.

#### **5. Main Function (`main.cpp`)**
- Simulates stock prices, calculates option prices, and hedging errors.
- Uses real market data for delta hedging with implied volatilities.

## Output and Analysis
### **Stock Simulations**
- Conducted for 1000 runs; a sample of 100 plotted.
- Stocks generally trend upward due to positive drift (μ = 0.05), with randomness introduced by Z(t).

### **Call Option Pricing**
- Call prices start around $5 at step 0, peaking at ~$45 for high-performing stocks.
- Prices decrease as expiration approaches due to theta decay.

### **Hedging Error Distribution**
- Follows a normal distribution, confirming theoretical expectations.
- Mean hedging error is close to zero, indicating effective delta hedging.

### **Delta Hedging with Real Market Data**
- Results.csv contains output for Google options (07/05/2011 - 07/29/2011).
- Delta hedging stabilizes PnL compared to a simple hold strategy.
- Longer periods provide greater stability; hedging reduces exposure to volatility spikes.

## Conclusion
- Delta hedging significantly reduces PnL volatility.
- While unhedged PnL benefits from favorable stock movements, it suffers from adverse ones.
- The hedged strategy offers consistent performance over time.

## Unit Testing
- **Total tests:** 6
  - 2 for implied volatility
  - 2 for delta calculation
  - 1 for BSM pricer
  - 1 for maturity time calculation

## Execution Details
- Default parameters: `K=500`, `startDate=2011-07-05`, `endDate=2011-07-29`, `expiryDate=2011-09-17`
- User input handling is disabled by default but can be enabled in `main.cpp`.

## File Format and Error Handling
- Market data files should be in a folder named `data`.
- The program verifies date format (YYYY-MM) and ensures data availability.
- If an invalid date is provided, execution is terminated with an error message.

---
### **How to Run the Project**
```bash
# Compile the code
 g++ -Wall -pedantic -std=c++11 option.cpp option_pricing.cpp stock.cpp market_data.cpp main.cpp -o main

# Run the main program
./main

# Compile and Run unit tests
g++ -std=c++14 option_pricing.cpp option.cpp market_data.cpp pricer_test.cpp -o main_test -lgtest -lgtest_main -pthread
./main_test
```

### **Visualization**
```bash
# Generate plots
python plots.py
```

---
## Author
**Naman Kedia**

