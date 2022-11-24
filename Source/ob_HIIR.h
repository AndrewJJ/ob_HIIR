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
	@file ob_HIIR.h
*/

#pragma once

// HIIR Library
#if JUCE_MSVC
#pragma warning (push) 
#pragma warning (disable: 4127)
#endif
//-------------------------------------------
#include "HIIR_Amalgam.h"
//-------------------------------------------
#if JUCE_MSVC
#pragma warning(pop)
#endif

#include "ob_Memory.h"

namespace ob {

#if JUCE_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcomment"
#endif
    
/**
	Provides a wrapper class to easily make use of Laurent DeSoras' HIIR library for oversampling.

	### Usage
	(Example based on JUCE::AudioProcessor)
    - Add all files from here to your project in the Projucer
    - Uncheck the 'Compile' toggle button for the HIIR_Amalgam.cpp file
	- Instantiate class as a private member variable in an AudioProcessor
	- Call prepare() from AudioProcessor::prepareToPlay()
	- In AudioProcessor::processBlock():
		- Call setSize() to handle the case where the block size has changed since prepare() was called
		- Call upSample() as required
		- Access oversampled data using getOverSampledData()
		- Call downSample() as required

	### Notes
	- The class manages all the filter stages and state data internally. Please note that the state is re-initialised each time prepare() is called.
	- The number of channels can be altered at each call to prepare()
	- The first stage filter is a polyphase IIR with 13 coefficients and a transition bandwidth of 0.01*Fs and 113.4dB of attenuation (110dB specified in design)
    - Subsequent filter stages have 4 coefficients and a transition bandwidth of 0.255*Fs and 118.5dB of attenuation (110dB specified in design)
	- Oversampling factors must be a power of 2
	- Float arrays should be 16 byte aligned for most efficient SSE processing
    
	==============================================================================

	## From the readme.txt for HIIR

	==============================================================================
	==============================================================================

	hiir
	Version 1.11

	An oversampling and Hilbert transform library in C++

	By Laurent de Soras, 2005-2013

	==============================================================================



	Contents:

	1. Legal
	2. What is hiir ?
	3. Using hiir
	4. Compilation
	5. Oversampling to higher ratios
	6. History
	7. Contact



	1. Legal
	--------

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Check the file license.txt to get full information about the license.



	2. What is hiir ?
	-----------------

	hiir is a DSP (digital signal processing) library in C++, with two purposes:

	- Changing the sampling rate of a signal by a factor two, in both
	directions (upsampling and downsampling).
	- Obtaining two signals with a pi/2 phase difference (Hilbert transform)

	These distinct operations are actually sharing the same filter design method
	and processing kernel, that is why they are included in the same package. The
	filter (a two-path polyphase IIR) is very efficient and can be used to achieve
	high-quality oversampling or phase shift at a low CPU cost. It is made of a
	symetric half-band elliptic low-pass filter, hence having an extremely flat
	frequency response in the passband.

	Various implementations are supplied with the library, using special CPU
	instructions to optimise the calculation speed. Currently SSE and 3DNow!
	instruction sets are supported, as well as the classic and portable FPU
	implementation.

	Source code may be downloaded from this webpage:
	http://ldesoras.free.fr/prod.html



	3. Using hiir
	-------------

	To avoid any collision, names have been encapsulated in the namespace "hiir".
	So you have to prefix every name of this library with hiir:: or put a line
	"using namespace hiir;" into your code.

	The filter design class is PolyphaseIir2Designer. It generates coefficients
	for the filters from a specification: stopband attenuation, transition
	bandwidth and/or number of coefficients.

	The main processing classes are Downsampler2x*, Upsampler2x* and PhaseHalfPi*.
	The suffix indicates the implementation. Choose "Fpu" if you are not sure
	about the right one to use. All implementations of a class category have the
	same function syntax, so you can use them with C++ templates.

	The implementations should have a consistent behaviour, based on the FPU one.
	Some of them have specific requirement, like object alignment in memory or
	delay in processing. See the header file (.h) of the class for details about
	the constraints and inconsistencies, and the code file (.cpp/.hpp) for details
	about function calls.

	As you can see, almost all classes are templates based on number of
	coefficients. This means it is not possible to change this number at run-time.
	This is the most important constraint of this library. However the reward is
	the high speed of the execution. Anyway, you could build a wrapper to support
	variable number of coefficients, althought it means that you will have
	probably to compile a large number of variations on the same code.

	The library processes only 32-bit floating point data.

	hiir is intended to be portable, but has little architecture-dependant pieces
	of code. So far, it has been built and tested on:

	- MS Windows / MS Visual C++ 6.0 (FPU/SSE/3DNow)
	- MS Windows / MS Visual C++ 2005 (FPU/SSE/3DNow)
	- MS Windows / MS Visual C++ 2010 (FPU/SSE/3DNow)
	- MS Windows / GCC 4.5.3 (FPU/SSE only)
	- MS Windows / Clang 3.1 (FPU/SSE only)
	- MacOS 10.5 / GCC 4 (FPU/SSE only)

	If you happen to have another system and tweak it to make it run successfully,
	pleeeeease send me your modification so I can include it to the main
	distribution. Run main.cpp in Debug mode before, then in Release mode, in
	order to be sure that everything is fine. I would also be glad to include
	implementations for other processors/compilers.

	References for filter use and design:

	- Scott Wardle, "A Hilbert-Transformer Frequency Shifter for Audio"
	http://www.iua.upf.es/dafx98/papers/

	- Valenzuela and Constantinides, "Digital Signal Processing Schemes for
	Efficient Interpolation and Decimation", IEEE Proceedings, Dec 1983

	- Artur Krukowski, Izzet Kale, "The design of arbitrary-band multi-path
	polyphase IIR filters", ISCAS 2, page 741-744. IEEE, 2001



	4. Compilation and testing
	--------------------------

	Drop the following files into your project or makefile :

	hiir/Array.*
	hiir/def.h
	hiir/Downsampler2x*.*
	hiir/fnc.*
	hiir/PhaseHalfPi*.*
	hiir/PolyphaseIir2Designer.*
	hiir/Stage*.*
	hiir/Upsampler2x*.*

	Other files (in the hiir/test directory) are for testing purpose only, do not
	include them if you just need to use the library; they are not needed to use
	hiir in your own programs.

	hiir may be compiled in two versions: release and debug. Debug version
	has checks that could slow down the code. Define NDEBUG to set the Release
	mode. For example, the command line to compile the test bench on GCC or
	Clang would look like:

	Debug mode:
	g++ -msse -I. -o ./hiir_debug.exe hiir/*.cpp hiir/test/*.cpp
	clang++ -D_X86_ -msse -I. -o ./hiir_debug.exe hiir/*.cpp hiir/test/*.cpp

	Release mode:
	g++ -msse -I. -o ./hiir_release.exe -DNDEBUG -O3 hiir/*.cpp hiir/test/*.cpp
	clang++ -D_X86_ -msse -I. -o ./hiir_release.exe -DNDEBUG -O3 hiir/*.cpp hiir/test/*.cpp

	The "-msse" option enables the compilation of the SSE intrinsics.

	Notes for MS VC++ 6.0 users:

	- You'll need the Processor Pack in order to be able to compile 3DNow! and
	SSE code.

	- The intensive use of recursive templates may slow down a bit the compilation,
	especially if you use many different filter sizes (number of coefficients).
	On MS Visual C++, you will probably have to use the /Zm option to increase
	the memory reserved to the compiler. /Zm500 should be enough to compile the
	test bench.

	- Also, MS Visual C++ issues a lot of warning related to the use of the EBX
	register or lack of FEMMS instruction at the end of a function. This is normal
	and you can safely disable these warning while using hiir classes.

	The included test bench checks roughly the accuracy of the filters. It also
	tests the speed of every available function. Therefore, implementing new
	instruction set should be facilitated.

	If you want to compile and run the test bench, please first edit the
	test/conf.h file, in order to select the instruction sets available for your
	CPU (there is currently no automatic detection). If you are not sure, disable
	all of them.

	In the same file, you have also testing options. You can save on the disk all
	the samples generated during tests in order to check them in a sample editor.
	However the files may take a lot of space on the disk, so it is recommended to
	disable this option if it is not required. The "long tests" options are
	intended to provide extensive checks on various filter sizes (it takes longer
	to compile, but is safer if you want to change anything in the lib).



	5. Oversampling to higher ratios
	--------------------------------

	It is possible to oversample a signal at a higher ratio than 2. You just have
	to cascade up/downsamplers to achieve a power-of-2 ratio. Depending on your
	requirements, you can reduce the filter order as the sampling rate is getting
	bigger by reducing the transition bandwidth (TBW).

	For example, let's suppose one wants 16x downsampling, with 96 dB of stopband
	attenuation and a 0.49*Fs passband. You'll need the following specifications
	for each stage:

	2x -> 1x: TBW = 0.01
	4x -> 2x: TBW = 0.01/2 + 1/4 = 0.255
	8x -> 4x: TBW = 0.01/4 + 1/8 + 1/4 = 0.3775
	16x -> 8x: TBW = 0.01/8 + 1/16 + 1/8 + 1/4 = 0.43865

	The reason is that you do not need to preserve spectrum parts that will be
	wiped out by subsequent stages. Only the spectrum part present after the
	final stage has to be perserved.

	More generally:

	TBW[stage] = (TBW[stage-1] + 0.5) / 2
	or
	TBW[stage] = TBW[0] * (0.5^stage) + 0.5 * (1 - 0.5^stage)

	So transition bandwidth requirement is significatively low until the last
	stage (0). Thus, the optimal performance would be reached by using hiir
	downsampler for the last stage because the requirement on the transition
	bandwidth is important, and by using a classic FIR filtering for other
	stages. Of course, it's possible to use hiir at every stage, but a well-
	optimised polyphase FIR routine is probably more efficient than a 1- or 2-
	coefficent IIR downsampler. Indeed, these IIR SIMD implementations have
	little or no benefit for low-order filters, whereas small FIR filters can
	benefit from SIMD. Check the speed test results to make your mind.



	6. History
	----------

	v1.11 (2012.06.26)
	- Changed the license to the WTFPL
	- Fixed some compilation warnings

	v1.10 (2008.05.28)
	- Changed directory structure
	- Test code is now in its own namespace (hiir::test)
	- Uses intrinsics for SSE code, making the code compilable on GCC.

	v1.00 (2005.03.29)
	- Initial release



	7. Contact
	----------

	Please address any comment, bug report or flame to:

	Laurent de Soras
	http://ldesoras.free.fr

 */

#if JUCE_CLANG
#pragma clang diagnostic pop
#endif

class HIIR
{
public:
	/** Constructor. */
	HIIR();
	/** Destructor. */
	~HIIR();

	/** Prepares the oversampler for operation, typically called from AudioProcessor::prepareToPlay().
	 */
	void prepare (	const int oversamplingFactor,	/**< The oversampling factor must be a power of 2. */
					const int numberOfChannels,		/**< Number of channels of audio to be processed. */
					const int bufferSize			/**< Number of samples in original audio block being processed. */
				 );

	/** Prepares the oversampler for operation, typically called from AudioProcessor::prepareToPlay().
	 *  This overloaded method calculates the required oversampling factor based on current and desired
	 *  sample rate. If oversampling is not required then the up/down sampling methods just pass the data
	 *  through unchanged.
	 */
	void prepare (	const double currentSampleRate,		/**< Current sample rate in Hz. */
					const int desiredSampleRate,		/**< Desired sample rate in Hz. Note that the oversampling factor will
														 *   be increased by a factor of two until the desired sample rate criterion
														 *   is satisfied. Hence the final sample rate may well exceed the desired 
														 *   target. */
					const int numberOfChannels,			/**< Number of channels of audio to be processed. */
					const int bufferSize				/**< Number of samples in original audio block being processed. */
				  );

	/** Releases allocated memory. prepare() must be called again before further use.
	 */
	void release();
	
	/** Perform upsampling on an AudioBuffer.
	 */
	void upSample   (	const juce::AudioSampleBuffer& inBuffer		/**< Sample buffer to be upsampled, will not be modified. */
					);

	/** Perform upsampling on a block of audio data for a single channel.
	 */
	void upSample   (	const int channelNumber,				/**< Channel number to be upsampled (zero indexed). */
						const float* in							/**< Input data block (should be 16 byte aligned). */
					);

	/** Perform downsampling into an AudioBuffer.
	 */
	void downSample (juce::AudioSampleBuffer& outBuffer			/**< Sample buffer to write downsampled data into. */
					);

	/** Perform downsampling into a block of audio data for a single channel.
	 */
	void downSample (	const int channelNumber,				/**< Channel number to be downsampled into (zero indexed). */
						float* out								/**< Output data block. */
					);

	/** Get pointer to array of all channels of oversampled data.
	 */
	float** getOverSampledData();

	/** Get pointer to audio block for a single channel of oversampled data.
	 */
	float* getOverSampledData (const int channelNumber	/**< Channel number (zero indexed). */ );

	/** Returns the oversampling factor.
	 */
	int getOSFactor() const;

	/** Returns the buffer size expected for upsampling from or downsampling to, i.e. the original block size
	 *  specified when prepare() was called (units are samples).
	 */
	int getSize() const;
	
	/** Returns the size of buffers at the oversampled stage (units are samples).
	 */
	int getOversampledSize() const;

	/** Used in AudioProcessor::prepareToPlay() to set the size of the buffers based on the audio
	 *  block size.  
	 *  Note that memory isn't de-allocated if the current buffer size is reduced. Naturally
	 *  there would be a potentially nasty performance hit for processBlock() if a reallocation
	 *  is triggered due to an increased block size since prepare() was called. If this is found
	 *  to occur then it is recommended that prepare() be recoded to over-dimension the buffers 
	 *  sufficiently.
	 */
	void setSize (const int size		/**< Size of audio block in samples (i.e. number of samples in the
										 *   the buffer passed to AudioProcessor::processBlock(). */
				 );

    /** Returns number of channels defined when prepare() was last called.
	 */
	int getNumChannels() const;

	/** Returns number of samples group delay for the end to end filtering chain. This is
	 *  dependent on the current oversampling factor and can be used to calculate delay compensation.
	 */
	double getGroupDelaySamples() const;

	/** Returns number of samples group delay for the end to end filtering chain. This is
	*  dependent on the current oversampling factor and can be used to calculate delay compensation.
	*/
	int getGroupDelayIntSamples() const;

	/** Returns true if the oversampling factor is a power of 2. */
	static bool validateOversamplingFactor (const int oversamplingFactor);


// Protected rather than private so that we can run unit tests on these methods from a derived class
protected:
	int numStages;
	/** Calculates the number of stages required to achieve the specified oversampling factor. */
	int calcNumStages (const int oversamplingFactor) const;

	// An Nth Stage Index is required so we know where to find the correct up/downsampler for a channel/stage
	// combination. Note that the first stage is referenced as stage 1, and that the Nth Stage Index ignores
	// the first/last stage as these have their own array of up/downsamplers (one element per channel).
	int getNthStageIndex (const int channelNum, const int stageNum) const;

	// A Stage Buffer Index is required so we know where to find the correct buffer for a channel/stage
	// combination. This is used for referencing the stageBuffers array. Note that the first stage is stage 1, not 0.
	int maxStageBufferIndex;
	int getStageBufferIndex (const int channelNum, const int stageNum) const;
	int getStageNumFromStageBufferIndex (const int StageBufferIndex) const;
	int getPeakStageBufferSize (const int stageNum) const;
	int getCurrentStageBufferSize (const int stageNum) const;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HIIR)

	bool	prepared;
	int		peakBufferSize;
	int		currentBufferSize;
	int		numChannels;
	int		osFactor;
	DataArray <float*> oversampledChannelArray;	// Holds pointers to channel array for oversampled data

	bool	coeffsInitialised;
	double	coeffsFirstStage[13];
	double	coeffsNthStage[4];

	/** Initialises coefficients for 1st and Nth stage IIR filters. */
	void initCoeffs();

	// Number of array elements depends on number of channels
    std::vector < hiir::Upsampler2x<13>,   auto_sse_allocator < hiir::Upsampler2x<13> > >    firstStageUpsamplers;
    std::vector < hiir::Downsampler2x<13>, auto_sse_allocator < hiir::Downsampler2x<13> > >  lastStageDownsamplers;
	// Number of array elements depends on number of channels AND number of stages
    std::vector < hiir::Upsampler2x<4>,   auto_sse_allocator < hiir::Upsampler2x<4> > >      nthStageUpsamplers;
    std::vector < hiir::Downsampler2x<4>, auto_sse_allocator < hiir::Downsampler2x<4> > >    nthStageDownsamplers;

	/** Allocates the arrays of resampling filters. */
	void allocateResamplers();
	/** Clears the arrays of resampling filters. */
	void deleteResamplers();

	/** Sets up the channel pointer array which can be used to access the oversampled data. 
	    Should only be called after setSize(). */
	void setupOversampledChannelArray();

	// This multidimensional array holds all the stage buffers (i.e. the buffers which hold the audio
	// data at each up/down sampling stage. The number of array elements depends on number of channels
	// AND the number of stages AND bufferSize
    juce::OwnedArray <AlignedDataArray <float, 16>> stageBuffers;

    /** Zero all stage buffers (without changing size/number of channels). This is taken care of by prepare(),
	 *  so should not need to be called by the user. */
	void zeroStageBuffers();

	/** Clear all filters within the up/down samplers (without changing size/number of channels).
	 * This is taken care of by prepare(), so should not need to be called by the user. */
    void clearFilters();

	/** Set coefficients for the up/down samplers. This is taken care of by prepare(),
	 * so should not need to be called by the user. */
    void setCoefficients();
	
	// Set each sample of buffer to zero - only called by zeroStageBuffers() in prepare(), so not performance critical
	void zero (float* buffer, const int numSamples);
	
	// Copy source buffer into destination buffer (buffers may overlap)
	void copy (float* destination, float* source, const int numSamples);
	
};

} // end namespace ob
