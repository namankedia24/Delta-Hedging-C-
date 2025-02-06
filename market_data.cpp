#include "market_data.h"

MarketData::MarketData() {
}

MarketData::MarketData(string interestFile, string stockFile, string optionFile) {
    readInterestRates(interestFile, 1);
    readStockPrices(stockFile, 1);
    readOptionPrices(optionFile, 1);
}

double MarketData::getInterestRate(string date) const {
    auto it = interestRates.find(date);
    if (it != interestRates.end()) {
        return it->second;
    }
    throw runtime_error("Interest rate not found for date: " + date);
}

double MarketData::getStockPrice(string date) const {
    auto it = stockPrices.find(date);
    if (it != stockPrices.end()) {
        return it->second;
    }
    throw runtime_error("Stock price not found for date: " + date);
}

double MarketData::getOptionPrice(string date, double strike, string expiryDate, char type) const {
    auto it = optionPrices.find(date);
    if (it != optionPrices.end()) {
        for (auto& option : it->second) {
            // cout << option.strike << " " << option.expiryDate << endl;
            if (option.strike == strike && option.expiryDate == expiryDate && type == option.typeOption) {
                return option.price;
            }
        }
    }
    throw runtime_error("Option price not found for date: " + date + "and strike: " + to_string(strike));
}

vector<string> MarketData::getDatesBetween(string startDate, string endDate) {
    auto startIt = find(allDates.begin(), allDates.end(), startDate);
    auto endIt = find(allDates.begin(), allDates.end(), endDate);

    if (startIt == allDates.end() || endIt == allDates.end()) {
        throw runtime_error("Option price dates not found for date: " + startDate + "and endDate: " + endDate);
    }

    int startIndex = startIt - allDates.begin();
    int endIndex = endIt - allDates.begin();

    if (endIndex <= startIndex) {
        throw runtime_error("[Enddate<StatDate]Option price invalid dates enddate: " + endDate);
    }

    return vector<string>(startIt, endIt + 1);
}

vector<double> MarketData::getDateWiseOptionPrices(vector<string> dates, double strike, string expiryDate, char type) const {
    vector<double> dateWiseOptionPrices;
    int flag = 0;
    for (auto date : dates) {
        auto it = optionPrices.find(date);
        if (it != optionPrices.end()) {
            flag = 0;
            for (auto& option : it->second) {
                // cout << option.strike << " " << option.expiryDate << endl;
                if (option.strike == strike && option.expiryDate == expiryDate && type == option.typeOption) {
                    dateWiseOptionPrices.push_back(option.price);
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                throw runtime_error("Option price not found for date: " + date + "and strike: " + to_string(strike));
            }
        } else {
            throw runtime_error("Option price not found for date: " + date + "and strike: " + to_string(strike));
        }
    }
    return dateWiseOptionPrices;
}

vector<double> MarketData::getDateWiseStockPrices(vector<string> dates) const {
    vector<double> dateWiseStockPrices;
    for (auto date : dates) {
        auto it = stockPrices.find(date);
        if (it != stockPrices.end()) {
            dateWiseStockPrices.push_back(it->second);
        } else {
            throw runtime_error("Option price not found for date: " + date);
        }
    }
    return dateWiseStockPrices;
}

vector<double> MarketData::getDateWiseRates(vector<string> dates) const {
    vector<double> dateWiseRates;
    for (auto date : dates) {
        auto it = interestRates.find(date);
        if (it != interestRates.end()) {
            dateWiseRates.push_back(it->second);
        } else {
            throw runtime_error("Option price not found for date: " + date);
        }
    }
    return dateWiseRates;
}

void MarketData::readInterestRates(const string& filename, int header_lines) {
    ifstream file(filename);
    string line, date;
    double rate;

    if (!file) {
        cout << "No File Found: " << filename << endl;
        return;
    }

    while (getline(file, line)) {
        if (header_lines-- > 0 ) {
            continue;
        }
        istringstream iss(line);
        if (getline(iss, date, ',') && (iss >> rate)) {
            interestRates[date] = rate / 100.0;
            allDates.push_back(date);
        }
    }
}

void MarketData::readStockPrices(const string& filename, int header_lines) {
    ifstream file(filename);
    string line, date;
    double price;

    if (!file) {
        cout << "No File Found: " << filename << endl;
        return;
    }

    while (getline(file, line)) {
        if (header_lines-- > 0 ) {
            continue;
        }
        istringstream iss(line);
        if (getline(iss, date, ',') && (iss >> price)) {
            stockPrices[date] = price;
        }
    }
}

void MarketData::readOptionPrices(const string& filename, int header_lines) {
    ifstream file(filename);
    string line;
    // char cpFlag;
    // double strike, bid, ask;

    if (!file) {
        cout << "No File Found: " << filename << endl;
        return;
    }

    while (getline(file, line)) {
        if (header_lines-- > 0 ) {
            continue;
        }

        string date, strikeStr, cpFlagStr, expiryDate, bidStr, askStr;
        istringstream iss(line);
        if (getline(iss, date, ',') &&
                getline(iss, expiryDate, ',') &&
                getline(iss, cpFlagStr, ',') &&
                getline(iss, strikeStr, ',') &&
                getline(iss, bidStr, ',') &&
                getline(iss, askStr)) {

            double strike = stod(strikeStr);
            char cpFlag = cpFlagStr[0];
            double bid = stod(bidStr);
            double ask = stod(askStr);

            if (cpFlag == 'C' || cpFlag == 'P') {
                optionPrices[date].push_back({strike, expiryDate, (bid + ask) / 2.0, cpFlag});
            } else {
                cout << "Invalid option type: " << cpFlag << endl;
            }
        } else {
            cout << "Error reading line: " << line << endl;
        }
    }
}

double MarketData::getTimeToMaturity(const string& currentDate, const string& endDate) {
    tm tm1 = {}, tm2 = {};
    istringstream ss1(currentDate), ss2(endDate);
    
    ss1 >> get_time(&tm1, "%Y-%m-%d");
    ss2 >> get_time(&tm2, "%Y-%m-%d");
    
    if (ss1.fail() || ss2.fail()) {
        throw runtime_error("Failed to parse date");
    }
    
    auto t1 = chrono::system_clock::from_time_t(mktime(&tm1));
    auto t2 = chrono::system_clock::from_time_t(mktime(&tm2));
    
    int businessDays = 0;
    auto currentDay = t1;
    
    while (currentDay <= t2) {
        time_t time_temp = chrono::system_clock::to_time_t(currentDay);
        tm local_tm;
        localtime_r(&time_temp, &local_tm);

        if (local_tm.tm_wday > 0 && local_tm.tm_wday < 6) {
            ++businessDays;
        }
        currentDay += chrono::hours(24);
    }
    
    return static_cast<double>(businessDays) / 252.0;
}