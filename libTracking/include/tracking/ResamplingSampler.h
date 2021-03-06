/*
 * ResamplingSampler.h
 *
 *  Created on: 15.08.2012
 *      Author: poschmann
 */

#ifndef RESAMPLINGSAMPLER_H_
#define RESAMPLINGSAMPLER_H_

#include "tracking/Sampler.h"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int.hpp"
#include <memory>

using std::shared_ptr;

namespace tracking {

class ResamplingAlgorithm;
class TransitionModel;

/**
 * Creates new samples by resampling the previous ones and moving them according to a transition model.
 * Additionally, some samples are randomly sampled across the image.
 */
class ResamplingSampler : public Sampler {
public:

	/**
	 * Constructs a new resampling sampler.
	 *
	 * @param[in] count The number of samples.
	 * @param[in] randomRate The percentage of samples that should be equally distributed.
	 * @param[in] resamplingAlgorithm The resampling algorithm.
	 * @param[in] transitionModel The transition model.
	 * @param[in] minSize The minimum size of a sample relative to the width or height of the image (whatever is smaller).
	 * @param[in] maxSize The maximum size of a sample relative to the width or height of the image (whatever is smaller).
	 */
	explicit ResamplingSampler(unsigned int count, double randomRate,
			shared_ptr<ResamplingAlgorithm> resamplingAlgorithm, shared_ptr<TransitionModel> transitionModel,
			float minSize = 0.1, float maxSize = 0.8);

	~ResamplingSampler();

	void sample(const vector<Sample>& samples, const vector<double>& offset, const Mat& image,
			vector<Sample>& newSamples);

	/**
	 * @return The number of samples.
	 */
	inline int getCount() {
		return count;
	}

	/**
	 * @param[in] The new number of samples.
	 */
	inline void setCount(unsigned int count) {
		this->count = count;
	}

	/**
	 * @return The percentage of samples that should be equally distributed.
	 */
	inline double getRandomRate() {
		return randomRate;
	}

	/**
	 * @param[in] The new percentage of samples that should be equally distributed.
	 */
	inline void setRandomRate(double randomRate) {
		this->randomRate = std::max(0.0, std::min(1.0, randomRate));
	}

private:

	/**
	 * Determines whether a sample is valid for a certain image.
	 *
	 * @param[in] sample The sample.
	 * @param[in] image The image.
	 * @return True if the sample is valid, false otherwise.
	 */
	bool isValid(const Sample& sample, const Mat& image);

	/**
	 * Randomly samples new valid values for a sample.
	 *
	 * @param[in,out] sample The sample.
	 * @param[in] image The image.
	 */
	void sampleValid(Sample& sample, const Mat& image);

	unsigned int count; ///< The number of samples.
	double randomRate;  ///< The percentage of samples that should be equally distributed.
	shared_ptr<ResamplingAlgorithm> resamplingAlgorithm; ///< The resampling algorithm.
	shared_ptr<TransitionModel> transitionModel;         ///< The transition model.

	float minSize; ///< The minimum size of a sample relative to the width or height of the image (whatever is smaller).
	float maxSize; ///< The maximum size of a sample relative to the width or height of the image (whatever is smaller).

	boost::mt19937 generator;          ///< Random number generator.
	boost::uniform_int<> distribution; ///< Uniform integer distribution.
};

} /* namespace tracking */
#endif /* RESAMPLINGSAMPLER_H_ */
