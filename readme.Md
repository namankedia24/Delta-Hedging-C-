How to run:
 g++ -Wall -pedantic -std=c++11 option.cpp option_pricing.cpp stock.cpp market_data.cpp main.cpp -o main

./main

How to test:
 g++ -std=c++14 option_pricing.cpp option.cpp market_data.cpp pricer_test.cpp -o main_test -lgtest -lgtest_main -pthread

./main_test



