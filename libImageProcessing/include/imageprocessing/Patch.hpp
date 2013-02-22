/*
 * Patch.hpp
 *
 *  Created on: 15.02.2013
 *      Author: poschmann & huber
 */

#ifndef PATCH_HPP_
#define PATCH_HPP_

#include "opencv2/core/core.hpp"

using cv::Mat;

namespace imageprocessing {

/**
 * Image patch with data, extracted from an image.
 */
class Patch {
public:

	/**
	 * Constructs a new empty patch. All values will be zero and the data will be an empty.
	 */
	Patch() :
			x(0), y(0), width(0), height(0), data() {}

	/**
	 * Constructs a new patch.
	 *
	 * @param[in] x The original x-coordinate of the center of this patch.
	 * @param[in] y The original y-coordinate of the center of this patch.
	 * @param[in] width The original width.
	 * @param[in] height The original height.
	 * @param[in] data The patch data (might be an image patch or a feature vector).
	 */
	explicit Patch(int x, int y, int width, int height, Mat data) :
			x(x), y(y), width(width), height(height), data(data) {}

	/**
	 * Copy constructor that clones the patch data.
	 *
	 * @param[in] other The patch that should be copied.
	 */
	Patch(const Patch& other) :
			x(other.x), y(other.y), width(other.width), height(other.height), data(other.data.clone()) {}

	~Patch() {}

	/**
	 * Assignment operator that clones the patch data.
	 *
	 * @param[in] other The patch whose data should be assigned to this one.
	 */
	Patch& operator=(const Patch& other) {
		x = other.x;
		y = other.y;
		width = other.width;
		height = other.height;
		data = other.data.clone();
		return *this;
	}

	/**
	 * @return The original x-coordinate of the center of this patch.
	 */
	int getX() const {
		return x;
	}

	/**
	 * @return The original y-coordinate of the center of this patch.
	 */
	int getY() const {
		return y;
	}

	/**
	 * @return The original width.
	 */
	int getWidth() const {
		return width;
	}

	/**
	 * @return The original height.
	 */
	int getHeight() const {
		return height;
	}

	/**
	 * @return The scale factor of the x-axis from the original size to the image data size.
	 */
	double getScaleFactorX() const {
		return static_cast<double>(data.cols) / static_cast<double>(width);
	}

	/**
	 * @return The scale factor of the y-axis from the original size to the image data size.
	 */
	double getScaleFactorY() const {
		return static_cast<double>(data.rows) / static_cast<double>(height);
	}

	/**
	 * @return The patch data (might be an image patch or a feature vector).
	 */
	Mat& getData() {
		return data;
	}

	/**
	 * @return The patch data (might be an image patch or a feature vector).
	 */
	const Mat& getData() const {
		return data;
	}

private:

	int x;        ///< The original x-coordinate of the center of this patch.
	int y;        ///< The original y-coordinate of the center of this patch.
	int width;    ///< The original width.
	int height;   ///< The original height.
	Mat data;     ///< The patch data (might be an image patch or a feature vector).
};

} /* namespace imageprocessing */
#endif /* PATCH_HPP_ */