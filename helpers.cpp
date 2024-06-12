#include "helpers.h"

#include <string>
#include <sstream>
#include <iterator>


bool read_test_data(std::istream& stream, TestData &test_data) {
    std::string line;
    std::getline(stream, line);

    test_data.clear();
    std::istringstream linestream{line};

    linestream >> test_data.class_id;
    double value;
    while (!linestream.eof()) {
        linestream >> value;
        test_data.feat.push_back(value);
    }
    return !stream.eof();
}

std::vector<double> read_vector(std::istream& stream) {
    std::vector<double> result;

    std::copy(std::istream_iterator<double>(stream),
              std::istream_iterator<double>(),
              std::back_inserter(result));
    return result;
}

Predictor::features FeatureReader::read()
{    
    std::istringstream linestream{line};
    double value;
    Predictor::features ret;
    while (!linestream.eof()) {
        linestream >> value;
        ret.push_back(value);
    }
    std::getline(_stream, line);
    return ret;
}
