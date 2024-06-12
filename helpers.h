#pragma once

#include <istream>
#include <vector>

#include "predictor.h"

bool read_test_data(std::istream& stream, TestData &test_data);

std::vector<double> read_vector(std::istream&);

class FeatureReader
{
    public:
        FeatureReader(std::istream &stream) : _stream(stream) {
		    std::getline(_stream, line);
		}

        Predictor::features read();

		bool is_eof() {
			return _stream.eof();
		}

    private:
        std::istream &_stream;
	    std::string line;
};

