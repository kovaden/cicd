#define BOOST_TEST_MODULE test_version

#include "lib.h"

#include <gtest/gtest.h>

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv); 
    try {
        return RUN_ALL_TESTS();
    } catch (std::exception &ex) {
        printf ("FAIL: %s", ex.what());
        return -1;
    }
    
}

class HelloWorldTest : public ::testing::Test
{
  protected:

  void SetUp() override {

  }
  
  void TearDown() override {
  }
};

TEST_F (HelloWorldTest, test_version)
{
    int v = version();

    ASSERT_NE(v, 0);
    ASSERT_LT(v, 100);
}
