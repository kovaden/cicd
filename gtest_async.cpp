#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "libasync/inputproc.h"

#include "libasync/command.h"
#include "libasync/athread.h"


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

class InputProcessorTest : public ::testing::Test
{
protected:
};

class MockCommandExecutor : public ICommandExecutor
{
public:
    MOCK_METHOD(void, add_batch, (std::unique_ptr<ICommandBatch> batch), (override));

    MOCK_METHOD(void, add_handler, (std::shared_ptr<IOutHandler> handler), (override));
};

class TestCommandBatchCreator : public ICommandBatchCreator
{
public:
    ICommandBatch *create() override {
        return new CommandBlock();
    }
};

using ::testing::AtLeast;

TEST_F (InputProcessorTest, test_process_simple)
{
    MockCommandExecutor exec;
    TestCommandBatchCreator bc;

    CommandProcessor iproc_test(2, bc,  exec);

    EXPECT_CALL(exec, add_batch(testing::_)).Times(2);

    iproc_test.process("hello");
    iproc_test.process("hello");
    iproc_test.process("world");

}

TEST_F (InputProcessorTest, test_process_block)
{
    MockCommandExecutor exec;
    TestCommandBatchCreator bc;

    CommandProcessor iproc_test(2, bc, exec);

    EXPECT_CALL(exec, add_batch(testing::_)).Times(2);

    iproc_test.process("hello");
    iproc_test.process("{");
    iproc_test.process("world");
    iproc_test.process("}");
}

TEST_F (InputProcessorTest, test_process_block_levels)
{
    MockCommandExecutor exec;
    TestCommandBatchCreator bc;

    CommandProcessor iproc_test(2, bc, exec);

    EXPECT_CALL(exec, add_batch(testing::_)).Times(3);

    iproc_test.process("hello");
    iproc_test.process("{");
    iproc_test.process("world1");
    iproc_test.process("world2");
    iproc_test.process("{");
    iproc_test.process("world3");
    iproc_test.process("}");
    iproc_test.process("world4");
    iproc_test.process("}");
    iproc_test.process("world5"); // Unfinished command counts
}

TEST_F (InputProcessorTest, test_process_block_unfinish)
{
    MockCommandExecutor exec;
    TestCommandBatchCreator bc;

    CommandProcessor iproc_test(2, bc, exec);

    EXPECT_CALL(exec, add_batch(testing::_)).Times(1);

    iproc_test.process("hello");
    iproc_test.process("{");
    iproc_test.process("world1");
    iproc_test.process("world2");
    iproc_test.process("{");
    iproc_test.process("world3");
    iproc_test.process("}");
    iproc_test.process("world4");
    iproc_test.process("world5"); // Unfinished block ignored
}


class WaitQueueTest : public ::testing::Test
{
protected:
};

TEST_F (WaitQueueTest, test_push_pull_st)
{
    WaitQueue wq;
    TestCommandBatchCreator tbc;

    for (int i = 0; i < 1000; i++) {
        auto tb = std::shared_ptr<ICommandBatch>(tbc.create());
        wq.push(tb);

        auto res = wq.pull_wait();
    }

    ASSERT_EQ(wq.size(), 0);
}

TEST_F (WaitQueueTest, test_push_pull_st2)
{
    WaitQueue wq;
    TestCommandBatchCreator tbc;
    std::atomic<bool> done;

    for (int i = 0; i < 1000; i++) {
        auto tb = std::shared_ptr<ICommandBatch>(tbc.create());
        wq.push(tb);
    }

    done = true;

    ASSERT_EQ(wq.size(), 1000);

    while (wq.size() != 0) {
        auto item = wq.pull_wait();
    }

    ASSERT_EQ(wq.size(), 0);
}

TEST_F (WaitQueueTest, test_push_pull_mt)
{
    WaitQueue wq;
    TestCommandBatchCreator tbc;

    std::atomic<bool> done{false};

    int count = 0;

    std::thread producer{[&wq, &tbc, &done](){
        int i = 0;
        for (i = 0; i < 1000; i++) {
            auto tb = std::shared_ptr<ICommandBatch>(tbc.create());
            wq.push(tb);
        }
        done = true;
        ASSERT_EQ(i, 1000);
    }};

    std::thread consumer{[&wq, &count](){
        while (count < 1000) {
            auto tb = wq.pull_wait();
            count ++;
        }
    }};

    producer.join();
    consumer.join();

    ASSERT_EQ(wq.size(), 0);

}
