#include "logistic_predictor.h"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <numeric>

template <typename T>
auto sigmoid(T x) -> double
{
	return 1.0 / (1.0 + exp(-x));
}

LogisticPredictor::LogisticPredictor(const std::vector<double> &model) : coef(model)
{
	assert(!coef.empty());
}

double LogisticPredictor::predict(const features &feat) const
{
	if (feat.size() + 1 != coef.size())
	{
		throw std::invalid_argument("Feature size does not match model size");
	}

	auto z = std::inner_product(feat.begin(), feat.end(), ++coef.begin(), coef.front());

	return sigmoid(z);
}
