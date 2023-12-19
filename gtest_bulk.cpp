#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "inputproc.h"

#include "command.h"
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
    MOCK_METHOD(void, add_block, (std::unique_ptr<ICommandBatch> &&block), (override));
    MOCK_METHOD(void, execute, (std::unique_ptr<ICommandBatch> &&batch), (override));
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

    InputProcessor iproc_test(2, exec, bc);

    EXPECT_CALL(exec, add_block(testing::_)).Times(2);

    iproc_test.process("hello");
    iproc_test.process("hello");
    iproc_test.process("world");

}

TEST_F (InputProcessorTest, test_process_block)
{
    MockCommandExecutor exec;
    TestCommandBatchCreator bc;

    InputProcessor iproc_test(2, exec, bc);

    EXPECT_CALL(exec, add_block(testing::_)).Times(2);

    iproc_test.process("hello");
    iproc_test.process("{");
    iproc_test.process("world");
    iproc_test.process("}");
}

TEST_F (InputProcessorTest, test_process_block_levels)
{
    MockCommandExecutor exec;
    TestCommandBatchCreator bc;

    InputProcessor iproc_test(2, exec, bc);

    EXPECT_CALL(exec, add_block(testing::_)).Times(3);

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

    InputProcessor iproc_test(2, exec, bc);

    EXPECT_CALL(exec, add_block(testing::_)).Times(1);

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
