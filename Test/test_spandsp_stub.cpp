#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "spandsp_stub.h"
#include <sstream>
#include <iostream>

// Test fixture for SpanDSP stub tests
class SpandspStubTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout to capture output
        original_cout_buffer = std::cout.rdbuf();
        std::cout.rdbuf(test_output.rdbuf());
    }

    void TearDown() override {
        // Restore original cout
        std::cout.rdbuf(original_cout_buffer);
    }

    std::stringstream test_output;
    std::streambuf* original_cout_buffer;
};

// Test spandsp_start_fax_session function
TEST_F(SpandspStubTest, StartFaxSession) {
    bool result = spandsp_start_fax_session();
    
    EXPECT_TRUE(result);
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[SpanDSP] Initializing T.30 fax session"));
}

// Test spandsp_encode_page function
TEST_F(SpandspStubTest, EncodePageValidData) {
    std::vector<uint8_t> input_page = {0x01, 0x02, 0x03, 0x04, 0x05};
    std::vector<uint8_t> output_frames;
    
    bool result = spandsp_encode_page(input_page, output_frames);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(output_frames.size(), 10); // Stub creates 10 bytes of 0xAA
    EXPECT_EQ(output_frames[0], 0xAA);
    
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[SpanDSP] Encoding fax page (5 bytes)"));
}

TEST_F(SpandspStubTest, EncodePageEmptyData) {
    std::vector<uint8_t> empty_page;
    std::vector<uint8_t> output_frames;
    
    bool result = spandsp_encode_page(empty_page, output_frames);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(output_frames.size(), 10); // Stub still creates 10 bytes
    
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[SpanDSP] Encoding fax page (0 bytes)"));
}

TEST_F(SpandspStubTest, EncodePageLargeData) {
    std::vector<uint8_t> large_page(2048, 0xFF);
    std::vector<uint8_t> output_frames;
    
    bool result = spandsp_encode_page(large_page, output_frames);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(output_frames.size(), 10);
    
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[SpanDSP] Encoding fax page (2048 bytes)"));
}

// Test spandsp_end_fax_session function
TEST_F(SpandspStubTest, EndFaxSession) {
    std::vector<uint8_t> final_frames;
    
    bool result = spandsp_end_fax_session(final_frames);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(final_frames.size(), 5); // Stub creates 5 bytes of 0xFF
    EXPECT_EQ(final_frames[0], 0xFF);
    
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("[SpanDSP] Finalizing fax session"));
}

// Integration test for complete fax encoding flow
TEST_F(SpandspStubTest, CompleteFaxFlow) {
    std::vector<uint8_t> page1 = {0x01, 0x02, 0x03};
    std::vector<uint8_t> page2 = {0x04, 0x05, 0x06, 0x07};
    std::vector<uint8_t> encoded_frames;
    std::vector<uint8_t> final_frames;
    
    // Start session
    EXPECT_TRUE(spandsp_start_fax_session());
    
    // Encode multiple pages
    EXPECT_TRUE(spandsp_encode_page(page1, encoded_frames));
    EXPECT_TRUE(spandsp_encode_page(page2, encoded_frames));
    
    // End session
    EXPECT_TRUE(spandsp_end_fax_session(final_frames));
    
    std::string output = test_output.str();
    EXPECT_THAT(output, ::testing::HasSubstr("Initializing T.30 fax session"));
    EXPECT_THAT(output, ::testing::HasSubstr("Encoding fax page (3 bytes)"));
    EXPECT_THAT(output, ::testing::HasSubstr("Encoding fax page (4 bytes)"));
    EXPECT_THAT(output, ::testing::HasSubstr("Finalizing fax session"));
}

// Mock-based test for T.30 protocol simulation
class MockT30Protocol {
public:
    MOCK_METHOD(bool, negotiate_capabilities, ());
    MOCK_METHOD(bool, send_page_data, (const std::vector<uint8_t>& data));
    MOCK_METHOD(bool, send_end_of_page, ());
    MOCK_METHOD(bool, send_end_of_document, ());
};

TEST_F(SpandspStubTest, T30ProtocolSimulation) {
    MockT30Protocol mock_t30;
    
    // Set expectations
    EXPECT_CALL(mock_t30, negotiate_capabilities())
        .Times(1)
        .WillOnce(::testing::Return(true));
    
    EXPECT_CALL(mock_t30, send_page_data(::testing::_))
        .Times(2)
        .WillRepeatedly(::testing::Return(true));
    
    EXPECT_CALL(mock_t30, send_end_of_page())
        .Times(2)
        .WillRepeatedly(::testing::Return(true));
    
    EXPECT_CALL(mock_t30, send_end_of_document())
        .Times(1)
        .WillOnce(::testing::Return(true));
    
    // Simulate T.30 protocol flow
    EXPECT_TRUE(mock_t30.negotiate_capabilities());
    
    std::vector<uint8_t> page1 = {0x01, 0x02};
    std::vector<uint8_t> page2 = {0x03, 0x04};
    
    EXPECT_TRUE(mock_t30.send_page_data(page1));
    EXPECT_TRUE(mock_t30.send_end_of_page());
    
    EXPECT_TRUE(mock_t30.send_page_data(page2));
    EXPECT_TRUE(mock_t30.send_end_of_page());
    
    EXPECT_TRUE(mock_t30.send_end_of_document());
}
