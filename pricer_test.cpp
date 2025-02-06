#include <iostream>
#include "option_pricing.h"
#include "pricer_test.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}