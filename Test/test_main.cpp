#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    
    // Set up global test environment
    std::cout << "Starting FOIP Unit Tests..." << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "FOIP Unit Tests completed." << std::endl;
    
    return result;
}
