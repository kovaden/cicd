#define BOOST_TEST_MODULE test_version

#include "lib.h"

#include <boost/test/unit_test.hpp>

#include "bayan/BayanFile.h"
#include "bayan/FileOpener.h"
#include "bayan/ProgramOptions.h"

BOOST_AUTO_TEST_SUITE(test_bayan)

BOOST_AUTO_TEST_CASE(test_file_read)
{
    BayanFile f("CTestTestfile.cmake");

    int maxb = f.max_block();

    for (int i = 0; i < maxb - 1; i++) {
        auto res = f.read(i);

        BOOST_CHECK_EQUAL(res.first, IFile::FRS_OK);
    }
    auto res = f.read(maxb - 1);
    BOOST_CHECK_EQUAL(res.first, IFile::FRS_DONE);

    res = f.read(maxb);
    BOOST_CHECK_EQUAL(res.first, IFile::FRS_EOF);
}

BOOST_AUTO_TEST_CASE(test_file_hashiter)
{
    BayanFile f("CTestTestfile.cmake");

    auto it = f.begin();
    BOOST_CHECK_EQUAL(f.bytes_read(), 0);

    auto val = *it;
    BOOST_CHECK_EQUAL(f.bytes_read(), ProgramOptions::block_size());

    ++it;
    BOOST_CHECK_EQUAL(f.bytes_read(),  ProgramOptions::block_size());

    val = *it;
    BOOST_CHECK_EQUAL(f.bytes_read(),  2 * ProgramOptions::block_size());

    for (auto it2 : f) {
        auto h = it2;
    }

    BOOST_ASSERT(f.opened());
}

BOOST_AUTO_TEST_CASE(test_file_opener)
{
    BayanFile f("CTestTestfile.cmake");

    {
        FileOpener fo(f);
        for (auto it2 : f) {
            auto h = it2;
        }

    }
    BOOST_ASSERT(!f.opened());
}

BOOST_AUTO_TEST_SUITE_END()
