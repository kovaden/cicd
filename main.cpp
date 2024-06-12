#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "logistic_predictor.h"
#include "helpers.h"

using namespace std;

tuple<string, string> parse_args(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "Usage: ./fashio_mnist test.csv model" << endl;
        exit(1);
    }

    return {argv[1], argv[2]};
}


int main(int argc, char *argv[]) 
{
    auto [test_data_file, model] = parse_args(argc, argv);

    ifstream test_data_str{test_data_file};
    ifstream model_str{model};

    vector<LogisticPredictor> predictors;    

    FeatureReader fr(model_str);

    while (!fr.is_eof())
    {
        predictors.emplace_back(fr.read());
    }

    TestData test_data;

    int match = 0;
    int count = 0;

    while (read_test_data(test_data_str, test_data)) {

        std::vector<double> options;
        for (auto &predictor : predictors) {
            double p = predictor.predict(test_data.feat);
            options.push_back(p);
        }

        auto it = max_element(options.begin(), options.end());

        int y_pred = distance(options.begin(), it);

        // cout << "Pred: " << y_pred << " Expected: " << test_data.class_id << "\n";

        if (y_pred == test_data.class_id) {
            match++;
        }
        count ++;
    };

    cout << (match * 1.0 / count) << endl;

    return 0;
}

