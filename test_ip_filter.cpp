#include "ip_filter.h"
#include <gtest/gtest.h>

#include <vector>
#include <string>

using namespace std;

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

class IPFilterTest : public ::testing::Test
{
  protected:

  void SetUp() override {

  }
  
  void TearDown() override {
  }
};

TEST_F (IPFilterTest, test_split)
{
    const std::string inp = "";

    auto v = split(inp, '.');
    ASSERT_EQ(v.size(), 1);
    ASSERT_STREQ(v[0].c_str(), "");

    v = split("hello.world", '.');
    ASSERT_EQ(v.size(), 2);
    ASSERT_STREQ(v[0].c_str(), "hello");
    ASSERT_STREQ(v[1].c_str(), "world");

    v = split("hello.world.", '.');
    ASSERT_EQ(v.size(), 3);

}

TEST_F (IPFilterTest, test_str2ip) {
    std::vector<std::string> sip1{"1","2","3","4"};

    ip_addr ip = str2ip(sip1);
    ASSERT_EQ(ip[0], 1);
    ASSERT_EQ(ip[1], 2);
    ASSERT_EQ(ip[2], 3);
    ASSERT_EQ(ip[3], 4);
}

TEST_F (IPFilterTest, test_revsort) {
    
    std::vector<ip_addr> ips{
            {222,173,235,246},
            {1,29,168,152},
            {222,82,198,61},
            {1,70,44,170},
            {1,1,234,8},
            {222,130,177,64}
    };
    
    revsort(ips);

    ASSERT_EQ(ips[0][0], 222);
    ASSERT_EQ(ips[0][1], 173);
    ASSERT_EQ(ips[1][0], 222);
    ASSERT_EQ(ips[1][1], 130);
    ASSERT_EQ(ips[2][0], 222);
    ASSERT_EQ(ips[2][1], 82);
    ASSERT_EQ(ips[3][0], 1);
    ASSERT_EQ(ips[3][1], 70);
    ASSERT_EQ(ips[4][0], 1);
    ASSERT_EQ(ips[4][1], 29);
    ASSERT_EQ(ips[5][0], 1);
    ASSERT_EQ(ips[5][1], 1);
}
