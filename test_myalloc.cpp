#include "myalloc.h"
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

class AllocTest : public ::testing::Test
{
  protected:
    ElementAllocator<int, 10> ialloc;

  void SetUp() override {

  }
  
  void TearDown() override {
  }
};

TEST_F (AllocTest, test_allocate_by1)
{
    for (int i = 0; i < 10; i++) {
        ialloc.allocate(1);
    }

    ialloc.allocate(1);

    ASSERT_EQ(2, ialloc.nchunks());
}

TEST_F (AllocTest, test_allocate_N)
{
    ialloc.allocate(10);
    ialloc.allocate(10);
    ialloc.allocate(10);

    ASSERT_EQ(3, ialloc.nchunks());
}

static constexpr const int test_item_count = 20;

TEST_F (AllocTest, test_free_by1)
{
    std::vector<int *> pointers(test_item_count);

    for (int i = 0; i < test_item_count; i++) {
        pointers[i] = ialloc.allocate(1);
    }
    ASSERT_EQ(2, ialloc.nchunks());

    ialloc.deallocate(pointers[2], 1);
    ialloc.deallocate(pointers[3], 1);
    ialloc.deallocate(pointers[4], 1);

    int *p2 = ialloc.allocate(1);
    ASSERT_EQ(p2, pointers[2]);

    int *p3 = ialloc.allocate(1);
    ASSERT_EQ(p3, pointers[3]);

    int *p4 = ialloc.allocate(1);
    ASSERT_EQ(p4, pointers[4]);

    ialloc.deallocate(pointers[0], 1);
    int *p0 = ialloc.allocate(1);
    ASSERT_EQ(p0, pointers[0]);

    ialloc.deallocate(pointers[9], 1);
    int *p9 = ialloc.allocate(1);
    ASSERT_EQ(p9, pointers[9]);
}

TEST_F (AllocTest, test_free_byN) {
    std::vector<int *> pointers(test_item_count);

    for (int i = 0; i < test_item_count; i++) {
        pointers[i] = ialloc.allocate(1);
    }
    ASSERT_EQ(2, ialloc.nchunks());

    ialloc.deallocate(pointers[2], 3);
    ialloc.deallocate(pointers[8], 2);

    int *p2 = ialloc.allocate(3);
    ASSERT_EQ(p2, pointers[2]);

    int *p3 = ialloc.allocate(1);
    ASSERT_EQ(p3, pointers[8]);

    int *p4 = ialloc.allocate(1);
    ASSERT_EQ(p4, pointers[9]);

    ASSERT_EQ(2, ialloc.nchunks());

    ialloc.deallocate(pointers[18], 2);
    int *p18 = ialloc.allocate(2);
    ASSERT_EQ(p18, pointers[18]);

    ASSERT_EQ(2, ialloc.nchunks());
}
