/*============================================================================*/
/*
  Copyright (C) 2014 by Andrew Jerrim <Andrew.Jerrim@gmail.com>

  This code contains portions of other open source products covered by
  separate licenses. Please see the corresponding source files for specific
  terms.
  
  This code is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/
/*============================================================================*/

/** 
	@file ob_HIIR.cpp
*/

#include "ob_HIIR.h"
#include "HIIR_Amalgam.cpp"

namespace ob {
namespace DSP {

HIIR::HIIR()
    : numStages(0),
    maxStageBufferIndex(0),
    prepared(false),
    peakBufferSize(0),
    currentBufferSize(0),
    numChannels(0),
    osFactor(0),
    coeffsInitialised(false),
    coeffsFirstStage{},
    coeffsNthStage{},
	stageBuffers{}
{ }

HIIR::~HIIR()
= default;

void HIIR::prepare (const int oversamplingFactor, const int numberOfChannels, const int bufferSize)
{
	jassert (oversamplingFactor>0);
	jassert (validateOversamplingFactor (oversamplingFactor));
	jassert (numberOfChannels>0);
	jassert (bufferSize>0);

	// Only calculate coeffs once
	if (!coeffsInitialised) initCoeffs();

	// Only reallocate stuff if necessary
	if (numChannels!=numberOfChannels || osFactor!=oversamplingFactor)
	{
		numChannels = numberOfChannels;
		osFactor = oversamplingFactor;
		numStages = calcNumStages (osFactor);

		// Reallocate Resamplers
		maxStageBufferIndex = getStageBufferIndex (numChannels - 1, numStages);
		deleteResamplers();
		allocateResamplers();

		// Reallocate Stage Buffers
		peakBufferSize = 0;
		currentBufferSize = 0;
		//// TODO - Overdimension buffers in first instance? Might avoid resizing if block size fluctuates a bit.
		//// Would be good to log block size used with different hosts/interfaces.
		//setSize(bufferSize*2.0);
		setSize(bufferSize);
		
		setupOversampledChannelArray();
	}
	else
	{
		zeroStageBuffers();
		clearFilters();
	}
	
	prepared = true;
}
void HIIR::prepare (const double currentSampleRate, const int desiredSampleRate, const int numberOfChannels, const int bufferSize)
{
	jassert (currentSampleRate > 0);
	jassert (desiredSampleRate > 0);
	auto osFactor_ = 1;
	while (currentSampleRate * osFactor_ < desiredSampleRate)
	{
		osFactor_ *= 2;
	}
	prepare (osFactor_, numberOfChannels, bufferSize);
}
void HIIR::initCoeffs()
{
	hiir::PolyphaseIir2Designer::compute_coefs_spec_order_tbw (coeffsFirstStage, 13, 0.01);
	hiir::PolyphaseIir2Designer::compute_coefs_spec_order_tbw (coeffsNthStage, 4, 0.255);
	coeffsInitialised = true;
}
void HIIR::allocateResamplers()
{
	firstStageUpsamplers.resize (numChannels);
	lastStageDownsamplers.resize (numChannels);
	if (numStages > 1)
	{
		const auto maxNthStageIndex = getNthStageIndex (numChannels - 1, numStages);
		nthStageUpsamplers.resize (maxNthStageIndex + 1);
		nthStageDownsamplers.resize (maxNthStageIndex + 1);
	}
	clearFilters();
	setCoefficients();
}
void HIIR::deleteResamplers()
{
	firstStageUpsamplers.clear();
	lastStageDownsamplers.clear();
	nthStageUpsamplers.clear();
	nthStageDownsamplers.clear();
}
void HIIR::setupOversampledChannelArray()
{
	oversampledChannelArray.setSize (numChannels, false);
	for (auto ch = 0; ch < numChannels; ++ch)
    {
		const auto sbi = getStageBufferIndex (ch, numStages);
		oversampledChannelArray[ch] = stageBuffers[sbi]->getData ();
	}
}
void HIIR::upSample (const AudioSampleBuffer& inBuffer)
{
	jassert (inBuffer.getNumChannels()==numChannels);
    for (auto ch = 0; ch < numChannels; ++ch)
    {
		upSample (ch, inBuffer.getReadPointer (ch));
	}
}
void HIIR::upSample (const int channelNumber, const float* in)
{
	jassert (prepared);

	if (numStages == 0)
		copy (	stageBuffers[getStageBufferIndex (channelNumber, 0)]->getData(),
				const_cast <float*> (in), currentBufferSize);
	else if (numStages == 1)
	{
		firstStageUpsamplers[channelNumber]
			.process_block (stageBuffers[getStageBufferIndex (channelNumber, 1)]->getData(), in, currentBufferSize);
	}
	else
	{
		// Upsample bottom stage
		firstStageUpsamplers[channelNumber]
			.process_block (stageBuffers[getStageBufferIndex (channelNumber, 1)]->getData(), in, currentBufferSize);
		// Iterate through middle & top stages
		for (auto st = 2; st <= numStages; ++st)
		{
			nthStageUpsamplers[getNthStageIndex (channelNumber,st)]
				.process_block (stageBuffers[getStageBufferIndex (channelNumber, st)]->getData(),
								 stageBuffers[getStageBufferIndex (channelNumber, st-1)]->getData(),
								 getCurrentStageBufferSize (st-1) );
		}
	}
}
void HIIR::downSample (AudioSampleBuffer& outBuffer)
{
	jassert (outBuffer.getNumChannels() == numChannels);
    for (auto ch = 0; ch < numChannels; ++ch)
    {
		downSample (ch, outBuffer.getWritePointer (ch));
	}
}
void HIIR::downSample (const int channelNumber, float* out)
{
	jassert (prepared);
	if (numStages == 0)
		copy (out, stageBuffers[getStageBufferIndex (channelNumber, 0)]->getData (), currentBufferSize);
	else if (numStages == 1)
		lastStageDownsamplers[channelNumber]
			.process_block (out, stageBuffers[getStageBufferIndex (channelNumber, 1)]->getData(), currentBufferSize);
	else
	{
		// Iterate through top & middle stages
		for (auto st = numStages; st > 1; --st)
		{
			nthStageDownsamplers[getNthStageIndex (channelNumber,st)]
				.process_block (stageBuffers[getStageBufferIndex (channelNumber, st-1)]->getData(),
								 stageBuffers[getStageBufferIndex (channelNumber, st)]->getData(),
								 getCurrentStageBufferSize (st-1) );
		}

		// Downsample bottom stage
		lastStageDownsamplers[channelNumber]
			.process_block (out, stageBuffers[getStageBufferIndex (channelNumber, 1)]->getData(), currentBufferSize);
	}
}
float** HIIR::getOverSampledData()
{
	// TODO - check this works OK
	return oversampledChannelArray.getData();
}
float* HIIR::getOverSampledData (const int channelNumber)
{
	return oversampledChannelArray[channelNumber];
}
int HIIR::getOSFactor () const
{
	return osFactor;
}
int HIIR::getSize () const
{
	return currentBufferSize;
}
int HIIR::getOversampledSize () const
{
	return getCurrentStageBufferSize (numStages);
}
void HIIR::setSize (const int size)
{
	jassert (size>0);
	if (size>peakBufferSize)
	{
		peakBufferSize = size;
		if (stageBuffers.size () != maxStageBufferIndex + 1)
		{
			stageBuffers.clear (true);
			for (auto sbi = 0; sbi <= maxStageBufferIndex; ++sbi)
			{
				const auto stageLength = getPeakStageBufferSize (getStageNumFromStageBufferIndex (sbi));
				stageBuffers.add (new General::AlignedDataArray <float, 16> (stageLength));
			}
		}
		else
			for (auto sbi = 0; sbi <= maxStageBufferIndex; ++sbi)
			{
				const auto stageLength = getPeakStageBufferSize (getStageNumFromStageBufferIndex (sbi));
				stageBuffers[sbi]->setLength (stageLength);
			}
	}

	currentBufferSize = size;
}
int HIIR::getNumChannels () const
{
	return numChannels;
}
double HIIR::getGroupDelaySamples () const
{
	// These figures were measured using a modified version of RTL Utility
	// TODO - verify these in a DAW
	switch (osFactor)
	{
		case  2:	return 4.2;
		case  4:	return 5.6;
		case  8:	return 6.6;
		case 16:	return 7.2;
		case 32:	return 7.4;
		case 64:	return 7.5;
		default:	return 7.6;
	}
}
int HIIR::getGroupDelayIntSamples () const
{
	// TODO - is rounding good enough, or do should I tweak it and use a switch statement
	return roundToIntAccurate (getGroupDelaySamples ());
}
void HIIR::zeroStageBuffers()
{
	jassert (stageBuffers.size() == maxStageBufferIndex + 1);	// Check that buffers have been previously allocated
	for (auto sbi = 0; sbi <= maxStageBufferIndex; ++sbi)
		zero (StageBuffers[sbi]->getData(), getPeakStageBufferSize (getStageNumFromStageBufferIndex (sbi)));
}
void HIIR::clearFilters()
{
	for (auto ch = 0; ch < numChannels; ++ch )
	{
		firstStageUpsamplers[ch].clear_buffers();
		lastStageDownsamplers[ch].clear_buffers();

		for (auto st = 2; st <= numStages; ++st)
		{
            const auto stageIndex = getNthStageIndex ( ch, st );
			nthStageUpsamplers[stageIndex].clear_buffers();
			nthStageDownsamplers[stageIndex].clear_buffers();
		}
	}
}
void HIIR::setCoefficients()
{
	jassert(coeffsInitialised);
	for (auto ch = 0; ch < numChannels; ++ch )
	{
		firstStageUpsamplers[ch].set_coefs (coeffsFirstStage);
		lastStageDownsamplers[ch].set_coefs (coeffsFirstStage);

		for (auto st = 2; st <= numStages; ++st)
		{
            const auto stageIndex = getNthStageIndex ( ch, st );
			nthStageUpsamplers[stageIndex].set_coefs (coeffsNthStage);
			nthStageDownsamplers[stageIndex].set_coefs (coeffsNthStage);
		}
	}
}
int HIIR::calcNumStages (const int oversamplingFactor) const
{
	jassert (oversamplingFactor>0);
	const auto result = static_cast<int> (log (static_cast<double> (oversamplingFactor)) / log(2.0));
	jassert (result >= 0);
	return result;
}
bool HIIR::validateOversamplingFactor (const int oversamplingFactor)
{
	jassert (oversamplingFactor>0);
    const auto osDbl = log (static_cast<double> (oversamplingFactor)) / log (2.0);
	const auto osInt = static_cast<int> (osDbl);
	return static_cast<double> (osInt)==osDbl;
}
int HIIR::getNthStageIndex (const int channelNum, const int stageNum) const
{
	jassert (channelNum < numChannels && channelNum >= 0);
	jassert (numStages > 1); // Shouldn't ever be called unless there is more than 1 stage
	return channelNum * (numStages - 1) + stageNum - 2;
}
int HIIR::getStageBufferIndex (const int channelNum, const int stageNum) const
{
	jassert (channelNum < numChannels && channelNum >= 0);
	if (numStages > 1)
		return channelNum * numStages + stageNum - 1;
	else
		return channelNum;
}
int HIIR::getStageNumFromStageBufferIndex (const int StageBufferIndex) const
{
	if (numStages == 0)
		return 0;
	else
		return StageBufferIndex % numStages + 1;
}
int HIIR::getPeakStageBufferSize (const int stageNum) const
{
	return peakBufferSize * static_cast<int> (pow(2.0f, static_cast<float> (stageNum)));
}
int HIIR::getCurrentStageBufferSize (const int stageNum) const
{
	return currentBufferSize *  static_cast<int> (pow (2.0f, static_cast<float> (stageNum)));
}
void HIIR::zero (float* buffer, const int numSamples)
{
	for (int i = 0; i < numSamples; i++)
	{
		buffer[i] = 0.0f;
	}
}
void HIIR::copy (float* destination, float* source, const int numSamples)
{
	memmove (destination, source, static_cast<size_t> (numSamples) * sizeof (float));
}

} // end namespace DSP
} // end namespace ob