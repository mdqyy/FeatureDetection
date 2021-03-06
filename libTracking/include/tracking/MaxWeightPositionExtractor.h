/*
 * MaxWeightPositionExtractor.h
 *
 *  Created on: 25.07.2012
 *      Author: poschmann
 */

#ifndef MAXWEIGHTPOSITIONEXTRACTOR_H_
#define MAXWEIGHTPOSITIONEXTRACTOR_H_

#include "tracking/PositionExtractor.h"

namespace tracking {

/**
 * Position extractor that uses the position of the sample with the highest weight.
 */
class MaxWeightPositionExtractor : public PositionExtractor {
public:

	/**
	 * Constructs a new max weight position extractor.
	 */
	explicit MaxWeightPositionExtractor();

	~MaxWeightPositionExtractor();

	optional<Sample> extract(const vector<Sample>& samples);
};

} /* namespace tracking */
#endif /* MAXWEIGHTPOSITIONEXTRACTOR_H_ */
