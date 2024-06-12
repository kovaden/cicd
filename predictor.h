#ifndef __PREDICTOR__H_LOADED__
#define __PREDICTOR__H_LOADED__

#include <vector>

class Predictor {
public:
    using features = std::vector<double>;

    virtual ~Predictor() {};
    
    virtual double predict(const features&) const = 0;
};

struct TestData
{
	int class_id;
	Predictor::features feat;

	void clear() {
		class_id = -1;
		feat.clear();
	}
};

#endif