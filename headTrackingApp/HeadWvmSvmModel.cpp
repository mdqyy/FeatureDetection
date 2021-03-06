/*
 * HeadWvmSvmModel.cpp
 *
 *  Created on: 13.11.2012
 *      Author: poschmann
 */

#include "HeadWvmSvmModel.h"
#include "tracking/Sample.h"
#include "DetectorWVM.h"
#include "DetectorSVM.h"
#include "OverlapElimination.h"
#include "VDetectorVectorMachine.h"
#include "FdImage.h"
#include "FdPatch.h"
#include <unordered_map>

using std::unordered_map;

HeadWvmSvmModel::HeadWvmSvmModel(shared_ptr<VDetectorVectorMachine> wvm, shared_ptr<VDetectorVectorMachine> svm,
		shared_ptr<OverlapElimination> oe) : wvm(wvm), svm(svm), oe(oe) {}

HeadWvmSvmModel::~HeadWvmSvmModel() {}

void HeadWvmSvmModel::evaluate(const Mat& image, vector<Sample>& samples) {
	FdImage* fdImage = new FdImage;
	fdImage->load(&image);
	wvm->initPyramids(fdImage);
	wvm->initROI(fdImage);
	vector<FdPatch*> remainingPatches;
	unordered_map<FdPatch*, vector<Sample*> > patch2samples;
	for (vector<Sample>::iterator sit = samples.begin(); sit < samples.end(); ++sit) {
		Sample& sample = *sit;
		sample.setObject(false);
		int faceX = sample.getX();
		int faceY = sample.getY() + (int)(0.08 * sample.getSize());
		int faceSize = (int)(0.6 * sample.getSize());
		FdPatch* patch = wvm->extractPatchToPyramid(fdImage, faceX, faceY, faceSize);
		if (patch == 0) {
			sample.setWeight(0);
		} else {
			if (wvm->detectOnPatch(patch)) {
				remainingPatches.push_back(patch);
				patch2samples[patch].push_back(&sample);
			}
			sample.setWeight(0.5 * patch->certainty[wvm->getIdentifier()]);
		}
	}
	if (!remainingPatches.empty()) {
		//remainingPatches = oe->eliminate(remainingPatches, wvm->getIdentifier());
		remainingPatches = takeDistinctBest(remainingPatches, 10, wvm->getIdentifier());
		svm->initPyramids(fdImage);
		svm->initROI(fdImage);
		vector<FdPatch*> objectPatches = svm->detectOnPatchvec(remainingPatches);
		for (vector<FdPatch*>::iterator pit = objectPatches.begin(); pit < objectPatches.end(); ++pit) {
			vector<Sample*>& patchSamples = patch2samples[(*pit)];
			for (vector<Sample*>::iterator sit = patchSamples.begin(); sit < patchSamples.end(); ++sit) {
				Sample* sample = (*sit);
				sample->setObject(true);
			}
		}
		for (vector<FdPatch*>::iterator pit = remainingPatches.begin(); pit < remainingPatches.end(); ++pit) {
			FdPatch* patch = (*pit);
			double certainty = patch->certainty[svm->getIdentifier()];
			vector<Sample*>& patchSamples = patch2samples[patch];
			for (vector<Sample*>::iterator sit = patchSamples.begin(); sit < patchSamples.end(); ++sit) {
				Sample* sample = (*sit);
				sample->setWeight(2 * sample->getWeight() * certainty);
			}
			patchSamples.clear();
		}
	}
	delete fdImage;
}
