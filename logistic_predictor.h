#ifndef __LOGISCTIC_PREDICTOR_H_LOADED__
#define __LOGISCTIC_PREDICTOR_H_LOADED__

#include "predictor.h"

class LogisticPredictor : public Predictor
{
public:
    LogisticPredictor(const std::vector<double>&);
	virtual ~LogisticPredictor() {}

    double predict(const features&) const override;

private:
	std::vector<double> coef;
};

#endif