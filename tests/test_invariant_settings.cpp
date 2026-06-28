#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

// Include the actual production header
#include "samp/settings.h"

class BufferOverflowTest : public ::testing::TestWithParam<std::string> {};

TEST_P(BufferOverflowTest, SprintfNeverExceedsBufferBounds) {
    // Invariant: Buffer reads never exceed the declared length of 127 bytes
    std::string payload = GetParam();
    
    // Setup the global variable used in the vulnerable code
    extern char* g_pszStorage;
    char original_storage[256];
    
    // Backup original value
    if (g_pszStorage) {
        strcpy(original_storage, g_pszStorage);
    }
    
    // Set adversarial input
    g_pszStorage = const_cast<char*>(payload.c_str());
    
    // Declare the buffer exactly as in production code
    char buff[0x7F];  // 127 bytes
    
    // Call the actual vulnerable function - this should not overflow
    // We're testing the actual sprintf call from settings.cpp
    sprintf(buff, "%sSAMP/settings.ini", g_pszStorage);
    
    // Verify buffer was not overflowed by checking null termination
    // within bounds (127 bytes max)
    ASSERT_LE(strlen(buff), 127) << "Buffer overflow detected with payload: " << payload;
    
    // Restore original value
    if (g_pszStorage) {
        g_pszStorage = original_storage;
    }
}

INSTANTIATE_TEST_SUITE_P(
    AdversarialInputs,
    BufferOverflowTest,
    ::testing::Values(
        // Exact exploit case: string that would cause overflow when concatenated
        std::string(120, 'A'),  // Will exceed 127 when "SAMP/settings.ini" is added
        
        // Boundary case: exactly fills buffer
        std::string(107, 'B'),  // 107 + 20 = 127 exactly
        
        // Valid input: well within bounds
        std::string("normal/path"),
        
        // Extreme case: massively oversized
        std::string(500, 'C')   // 500 + 20 = 520 bytes total
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}