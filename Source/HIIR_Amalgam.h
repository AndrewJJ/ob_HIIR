//#include "hiir/test/AlignedObject.h"
//#include "hiir/Downsampler2x3dnow.h"
//#include "hiir/Upsampler2x3dnow.h"

/*** Start of inlined file: Downsampler2xFpu.h ***/
#if ! defined (hiir_Downsampler2xFpu_HEADER_INCLUDED)
#define	hiir_Downsampler2xFpu_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: Array.h ***/
#if ! defined (hiir_Array_HEADER_INCLUDED)
#define	hiir_Array_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

template <class T, long LEN>
class Array
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(LEN > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef	T	Element;

   enum {         LENGTH   = LEN };

						Array () {};
						Array (const Array <T, LEN> &other);
	Array <T, LEN> &
						operator = (const Array <T, LEN> &other);

	const Element&	operator [] (long pos) const;
	Element &		operator [] (long pos);

	static long		size ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	Element			_data [LENGTH];

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Array &other);
	bool				operator != (const Array &other);

};	// class Array

}	// namespace hiir


/*** Start of inlined file: Array.hpp ***/
#if defined (hiir_Array_CURRENT_CODEHEADER)
	#error Recursive inclusion of Array code header.
#endif
#define	hiir_Array_CURRENT_CODEHEADER

#if ! defined (hiir_Array_CODEHEADER_INCLUDED)
#define	hiir_Array_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <class T, long LEN>
Array <T, LEN>::Array (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}
}

template <class T, long LEN>
Array <T, LEN> &	Array <T, LEN>::operator = (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}

	return (*this);
}

template <class T, long LEN>
const typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos) const
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos)
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
long	Array <T, LEN>::size ()
{
	return (LENGTH);
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Array_CODEHEADER_INCLUDED

#undef hiir_Array_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.hpp ***/

#endif	// hiir_Array_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.h ***/


/*** Start of inlined file: def.h ***/
#if ! defined (hiir_def_HEADER_INCLUDED)
#define	hiir_def_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

const double	PI			= 3.1415926535897932384626433832795;

#if defined (_MSC_VER)

	#define	hiir_FORCEINLINE	__forceinline

#else

	#define	hiir_FORCEINLINE	inline

#endif

}	// namespace hiir

#endif	// hiir_def_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: def.h ***/

namespace hiir
{

template <int NC>
class Downsampler2xFpu
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(NC > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	enum {			NBR_COEFS	= NC	};

						Downsampler2xFpu ();

	void				set_coefs (const double coef_arr []);

	hiir_FORCEINLINE float
						process_sample (const float in_ptr [2]);
	void				process_block (float out_ptr [], const float in_ptr [], long nbr_spl);

	hiir_FORCEINLINE void
						process_sample_split (float &low, float &high, const float in_ptr [2]);
	void				process_block_split (float out_l_ptr [], float out_h_ptr [], const float in_ptr [], long nbr_spl);

	void				clear_buffers ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef	Array <float, NBR_COEFS>	HyperGluar;

	HyperGluar		_coef;
	HyperGluar		_x;
	HyperGluar		_y;

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Downsampler2xFpu &other);
	bool				operator != (const Downsampler2xFpu &other);

};	// class Downsampler2xFpu

}	// namespace hiir


/*** Start of inlined file: Downsampler2xFpu.hpp ***/
#if defined (hiir_Downsampler2xFpu_CURRENT_CODEHEADER)
	#error Recursive inclusion of Downsampler2xFpu code header.
#endif
#define	hiir_Downsampler2xFpu_CURRENT_CODEHEADER

#if ! defined (hiir_Downsampler2xFpu_CODEHEADER_INCLUDED)
#define	hiir_Downsampler2xFpu_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: StageProcFpu.h ***/
#if ! defined (hiir_StageProc_HEADER_INCLUDED)
#define	hiir_StageProc_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: def.h ***/
#if ! defined (hiir_def_HEADER_INCLUDED)
#define	hiir_def_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

const double	PI			= 3.1415926535897932384626433832795;

#if defined (_MSC_VER)

	#define	hiir_FORCEINLINE	__forceinline

#else

	#define	hiir_FORCEINLINE	inline

#endif

}	// namespace hiir

#endif	// hiir_def_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: def.h ***/

namespace hiir
{

template <int REMAINING>
class StageProcFpu
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(REMAINING >= 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static hiir_FORCEINLINE void
						process_sample_pos (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y []);
	static hiir_FORCEINLINE void
						process_sample_neg (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y []);

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

						StageProcFpu ();
						StageProcFpu (const StageProcFpu &other);
	StageProcFpu &	operator = (const StageProcFpu &other);
	bool				operator == (const StageProcFpu &other);
	bool				operator != (const StageProcFpu &other);

};	// class StageProcFpu

}	// namespace hiir


/*** Start of inlined file: StageProcFpu.hpp ***/
#if defined (hiir_StageProc_CURRENT_CODEHEADER)
	#error Recursive inclusion of StageProcFpu code header.
#endif
#define	hiir_StageProc_CURRENT_CODEHEADER

#if ! defined (hiir_StageProc_CODEHEADER_INCLUDED)
#define	hiir_StageProc_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if defined (_MSC_VER)
	#pragma inline_depth (255)
#endif

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <>
hiir_FORCEINLINE void	StageProcFpu <1>::process_sample_pos (const int nbr_coefs, float &spl_0, float &/*spl_1*/, const float coef [], float x [], float y [])
{
	const int		last = nbr_coefs - 1;
	const float		temp = (spl_0 - y [last]) * coef [last] + x [last];
	x [last] = spl_0;
	y [last] = temp;
	spl_0 = temp;
}

template <>
hiir_FORCEINLINE void	StageProcFpu <0>::process_sample_pos (const int /*nbr_coefs*/, float &/*spl_0*/, float &/*spl_1*/, const float /*coef*/ [], float /*x*/ [], float /*y*/ [])
{
	// Nothing (stops recursion)
}

template <int REMAINING>
void	StageProcFpu <REMAINING>::process_sample_pos (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y [])
{
	const int		cnt = nbr_coefs - REMAINING;

	const float		temp_0 =
		(spl_0 - y [cnt + 0]) * coef [cnt + 0] + x [cnt + 0];
	const float		temp_1 =
		(spl_1 - y [cnt + 1]) * coef [cnt + 1] + x [cnt + 1];

	x [cnt + 0] = spl_0;
	x [cnt + 1] = spl_1;

	y [cnt + 0] = temp_0;
	y [cnt + 1] = temp_1;

	spl_0 = temp_0;
	spl_1 = temp_1;

	StageProcFpu <REMAINING - 2>::process_sample_pos (
		nbr_coefs,
		spl_0,
		spl_1,
		&coef [0],
		&x [0],
		&y [0]
	);
}

template <>
hiir_FORCEINLINE void	StageProcFpu <1>::process_sample_neg (const int nbr_coefs, float &spl_0, float &/*spl_1*/, const float coef [], float x [], float y [])
{
	const int		last = nbr_coefs - 1;
	const float		temp = (spl_0 + y [last]) * coef [last] - x [last];
	x [last] = spl_0;
	y [last] = temp;
	spl_0 = temp;
}

template <>
hiir_FORCEINLINE void	StageProcFpu <0>::process_sample_neg (const int /*nbr_coefs*/, float &/*spl_0*/, float &/*spl_1*/, const float /*coef*/ [], float /*x*/ [], float /*y*/ [])
{
	// Nothing (stops recursion)
}

template <int REMAINING>
void	StageProcFpu <REMAINING>::process_sample_neg (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y [])
{
	const int		cnt = nbr_coefs - REMAINING;

	const float		temp_0 =
		(spl_0 + y [cnt + 0]) * coef [cnt + 0] - x [cnt + 0];
	const float		temp_1 =
		(spl_1 + y [cnt + 1]) * coef [cnt + 1] - x [cnt + 1];

	x [cnt + 0] = spl_0;
	x [cnt + 1] = spl_1;

	y [cnt + 0] = temp_0;
	y [cnt + 1] = temp_1;

	spl_0 = temp_0;
	spl_1 = temp_1;

	StageProcFpu <REMAINING - 2>::process_sample_neg (
		nbr_coefs,
		spl_0,
		spl_1,
		&coef [0],
		&x [0],
		&y [0]
	);
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_StageProc_CODEHEADER_INCLUDED

#undef hiir_StageProc_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcFpu.hpp ***/

#endif	// hiir_StageProc_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcFpu.h ***/

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ctor
Throws: Nothing
==============================================================================
*/

template <int NC>
Downsampler2xFpu <NC>::Downsampler2xFpu ()
:	_coef ()
,	_x ()
,	_y ()
{
	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_coef [i] = 0;
	}
	clear_buffers ();
}

/*
==============================================================================
Name: set_coefs
Description:
   Sets filter coefficients. Generate them with the PolyphaseIir2Designer
   class.
   Call this function before doing any processing.
Input parameters:
	- coef_arr: Array of coefficients. There should be as many coefficients as
	  mentioned in the class template parameter.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xFpu <NC>::set_coefs (const double coef_arr [])
{
	assert (coef_arr != 0);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_coef [i] = static_cast <float> (coef_arr [i]);
	}
}

/*
==============================================================================
Name: process_sample
Description:
   Downsamples (x2) one pair of samples, to generate one output sample.
Input parameters:
	- in_ptr: pointer on the two samples to decimate
Returns: Samplerate-reduced sample.
Throws: Nothing
==============================================================================
*/

template <int NC>
float	Downsampler2xFpu <NC>::process_sample (const float in_ptr [2])
{
	assert (in_ptr != 0);

	float				spl_0 (in_ptr [1]);
	float				spl_1 (in_ptr [0]);

	#if defined (_MSC_VER)
		#pragma inline_depth (255)
	#endif	// _MSC_VER

	StageProcFpu <NBR_COEFS>::process_sample_pos (
		NBR_COEFS,
		spl_0,
		spl_1,
		&_coef [0],
		&_x [0],
		&_y [0]
	);

	return (0.5f * (spl_0 + spl_1));
}

/*
==============================================================================
Name: process_block
Description:
   Downsamples (x2) a block of samples.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl * 2 samples.
	- nbr_spl: Number of samples to output, > 0
Output parameters:
	- out_ptr: Array for the output samples, capacity: nbr_spl samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xFpu <NC>::process_block (float out_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (in_ptr != 0);
	assert (out_ptr != 0);
	assert (out_ptr <= in_ptr || out_ptr >= in_ptr + nbr_spl * 2);
	assert (nbr_spl > 0);

	long				pos = 0;
	do
	{
		out_ptr [pos] = process_sample (&in_ptr [pos * 2]);
		++pos;
	}
	while (pos < nbr_spl);
}

/*
==============================================================================
Name: process_sample_split
Description:
   Split (spectrum-wise) in half a pair of samples. The lower part of the
   spectrum is a classic downsampling, equivalent to the output of
   process_sample().
   The higher part is the complementary signal: original filter response
   is flipped from left to right, becoming a high-pass filter with the same
   cutoff frequency. This signal is then critically sampled (decimation by 2),
   flipping the spectrum: Fs/4...Fs/2 becomes Fs/4...0.
Input parameters:
	- in_ptr: pointer on the pair of input samples
Output parameters:
	- low: output sample, lower part of the spectrum (downsampling)
	- high: output sample, higher part of the spectrum.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xFpu <NC>::process_sample_split (float &low, float &high, const float in_ptr [2])
{
	assert (&low != 0);
	assert (&high != 0);
	assert (in_ptr != 0);

	float				spl_0 = in_ptr [1];
	float				spl_1 = in_ptr [0];

	#if defined (_MSC_VER)
		#pragma inline_depth (255)
	#endif	// _MSC_VER

	StageProcFpu <NBR_COEFS>::process_sample_pos (
		NBR_COEFS,
		spl_0,
		spl_1,
		&_coef [0],
		&_x [0],
		&_y [0]
	);

	low = (spl_0 + spl_1) * 0.5f;
	high = spl_0 - low;	// (spl_0 - spl_1) * 0.5f;
}

/*
==============================================================================
Name: process_block_split
Description:
   Split (spectrum-wise) in half a block of samples. The lower part of the
   spectrum is a classic downsampling, equivalent to the output of
   process_block().
   The higher part is the complementary signal: original filter response
   is flipped from left to right, becoming a high-pass filter with the same
   cutoff frequency. This signal is then critically sampled (decimation by 2),
   flipping the spectrum: Fs/4...Fs/2 becomes Fs/4...0.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl * 2 samples.
	- nbr_spl: Number of samples for each output, > 0
Output parameters:
	- out_l_ptr: Array for the output samples, lower part of the spectrum
	  (downsampling). Capacity: nbr_spl samples.
	- out_h_ptr: Array for the output samples, higher part of the spectrum.
	  Capacity: nbr_spl samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xFpu <NC>::process_block_split (float out_l_ptr [], float out_h_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (in_ptr != 0);
	assert (out_l_ptr != 0);
	assert (out_l_ptr <= in_ptr || out_l_ptr >= in_ptr + nbr_spl * 2);
	assert (out_h_ptr != 0);
	assert (out_h_ptr <= in_ptr || out_h_ptr >= in_ptr + nbr_spl * 2);
	assert (out_h_ptr != out_l_ptr);
	assert (nbr_spl > 0);

	long				pos = 0;
	do
	{
		process_sample_split (
			out_l_ptr [pos],
			out_h_ptr [pos],
			&in_ptr [pos * 2]
		);
		++pos;
	}
	while (pos < nbr_spl);
}

/*
==============================================================================
Name: clear_buffers
Description:
	Clears filter memory, as if it processed silence since an infinite amount
	of time.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xFpu <NC>::clear_buffers ()
{
	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_x [i] = 0;
		_y [i] = 0;
	}
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Downsampler2xFpu_CODEHEADER_INCLUDED

#undef hiir_Downsampler2xFpu_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Downsampler2xFpu.hpp ***/

#endif	// hiir_Downsampler2xFpu_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Downsampler2xFpu.h ***/



/*** Start of inlined file: Upsampler2xFpu.h ***/
#if ! defined (hiir_Upsampler2xFpu_HEADER_INCLUDED)
#define	hiir_Upsampler2xFpu_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: Array.h ***/
#if ! defined (hiir_Array_HEADER_INCLUDED)
#define	hiir_Array_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

template <class T, long LEN>
class Array
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(LEN > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef	T	Element;

   enum {         LENGTH   = LEN };

						Array () {};
						Array (const Array <T, LEN> &other);
	Array <T, LEN> &
						operator = (const Array <T, LEN> &other);

	const Element&	operator [] (long pos) const;
	Element &		operator [] (long pos);

	static long		size ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	Element			_data [LENGTH];

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Array &other);
	bool				operator != (const Array &other);

};	// class Array

}	// namespace hiir


/*** Start of inlined file: Array.hpp ***/
#if defined (hiir_Array_CURRENT_CODEHEADER)
	#error Recursive inclusion of Array code header.
#endif
#define	hiir_Array_CURRENT_CODEHEADER

#if ! defined (hiir_Array_CODEHEADER_INCLUDED)
#define	hiir_Array_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <class T, long LEN>
Array <T, LEN>::Array (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}
}

template <class T, long LEN>
Array <T, LEN> &	Array <T, LEN>::operator = (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}

	return (*this);
}

template <class T, long LEN>
const typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos) const
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos)
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
long	Array <T, LEN>::size ()
{
	return (LENGTH);
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Array_CODEHEADER_INCLUDED

#undef hiir_Array_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.hpp ***/

#endif	// hiir_Array_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.h ***/

namespace hiir
{

template <int NC>
class Upsampler2xFpu
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(NC > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	enum {			NBR_COEFS	= NC	};

						Upsampler2xFpu ();

	void				set_coefs (const double coef_arr [NBR_COEFS]);
	inline void		process_sample (float &out_0, float &out_1, float input);
	void				process_block (float out_ptr [], const float in_ptr [], long nbr_spl);
	void				clear_buffers ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef	Array <float, NBR_COEFS>	HyperGluar;

	HyperGluar		_coef;
	HyperGluar		_x;
	HyperGluar		_y;

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Upsampler2xFpu &other);
	bool				operator != (const Upsampler2xFpu &other);

};	// class Upsampler2xFpu

}	// namespace hiir


/*** Start of inlined file: Upsampler2xFpu.hpp ***/
#if defined (hiir_Upsampler2xFpu_CURRENT_CODEHEADER)
	#error Recursive inclusion of Upsampler2xFpu code header.
#endif
#define	hiir_Upsampler2xFpu_CURRENT_CODEHEADER

#if ! defined (hiir_Upsampler2xFpu_CODEHEADER_INCLUDED)
#define	hiir_Upsampler2xFpu_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: StageProcFpu.h ***/
#if ! defined (hiir_StageProc_HEADER_INCLUDED)
#define	hiir_StageProc_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: def.h ***/
#if ! defined (hiir_def_HEADER_INCLUDED)
#define	hiir_def_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

const double	PI			= 3.1415926535897932384626433832795;

#if defined (_MSC_VER)

	#define	hiir_FORCEINLINE	__forceinline

#else

	#define	hiir_FORCEINLINE	inline

#endif

}	// namespace hiir

#endif	// hiir_def_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: def.h ***/

namespace hiir
{

template <int REMAINING>
class StageProcFpu
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(REMAINING >= 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static hiir_FORCEINLINE void
						process_sample_pos (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y []);
	static hiir_FORCEINLINE void
						process_sample_neg (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y []);

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

						StageProcFpu ();
						StageProcFpu (const StageProcFpu &other);
	StageProcFpu &	operator = (const StageProcFpu &other);
	bool				operator == (const StageProcFpu &other);
	bool				operator != (const StageProcFpu &other);

};	// class StageProcFpu

}	// namespace hiir


/*** Start of inlined file: StageProcFpu.hpp ***/
#if defined (hiir_StageProc_CURRENT_CODEHEADER)
	#error Recursive inclusion of StageProcFpu code header.
#endif
#define	hiir_StageProc_CURRENT_CODEHEADER

#if ! defined (hiir_StageProc_CODEHEADER_INCLUDED)
#define	hiir_StageProc_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if defined (_MSC_VER)
	#pragma inline_depth (255)
#endif

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <>
hiir_FORCEINLINE void	StageProcFpu <1>::process_sample_pos (const int nbr_coefs, float &spl_0, float &/*spl_1*/, const float coef [], float x [], float y [])
{
	const int		last = nbr_coefs - 1;
	const float		temp = (spl_0 - y [last]) * coef [last] + x [last];
	x [last] = spl_0;
	y [last] = temp;
	spl_0 = temp;
}

template <>
hiir_FORCEINLINE void	StageProcFpu <0>::process_sample_pos (const int /*nbr_coefs*/, float &/*spl_0*/, float &/*spl_1*/, const float /*coef*/ [], float /*x*/ [], float /*y*/ [])
{
	// Nothing (stops recursion)
}

template <int REMAINING>
void	StageProcFpu <REMAINING>::process_sample_pos (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y [])
{
	const int		cnt = nbr_coefs - REMAINING;

	const float		temp_0 =
		(spl_0 - y [cnt + 0]) * coef [cnt + 0] + x [cnt + 0];
	const float		temp_1 =
		(spl_1 - y [cnt + 1]) * coef [cnt + 1] + x [cnt + 1];

	x [cnt + 0] = spl_0;
	x [cnt + 1] = spl_1;

	y [cnt + 0] = temp_0;
	y [cnt + 1] = temp_1;

	spl_0 = temp_0;
	spl_1 = temp_1;

	StageProcFpu <REMAINING - 2>::process_sample_pos (
		nbr_coefs,
		spl_0,
		spl_1,
		&coef [0],
		&x [0],
		&y [0]
	);
}

template <>
hiir_FORCEINLINE void	StageProcFpu <1>::process_sample_neg (const int nbr_coefs, float &spl_0, float &/*spl_1*/, const float coef [], float x [], float y [])
{
	const int		last = nbr_coefs - 1;
	const float		temp = (spl_0 + y [last]) * coef [last] - x [last];
	x [last] = spl_0;
	y [last] = temp;
	spl_0 = temp;
}

template <>
hiir_FORCEINLINE void	StageProcFpu <0>::process_sample_neg (const int /*nbr_coefs*/, float &/*spl_0*/, float &/*spl_1*/, const float /*coef*/ [], float /*x*/ [], float /*y*/ [])
{
	// Nothing (stops recursion)
}

template <int REMAINING>
void	StageProcFpu <REMAINING>::process_sample_neg (const int nbr_coefs, float &spl_0, float &spl_1, const float coef [], float x [], float y [])
{
	const int		cnt = nbr_coefs - REMAINING;

	const float		temp_0 =
		(spl_0 + y [cnt + 0]) * coef [cnt + 0] - x [cnt + 0];
	const float		temp_1 =
		(spl_1 + y [cnt + 1]) * coef [cnt + 1] - x [cnt + 1];

	x [cnt + 0] = spl_0;
	x [cnt + 1] = spl_1;

	y [cnt + 0] = temp_0;
	y [cnt + 1] = temp_1;

	spl_0 = temp_0;
	spl_1 = temp_1;

	StageProcFpu <REMAINING - 2>::process_sample_neg (
		nbr_coefs,
		spl_0,
		spl_1,
		&coef [0],
		&x [0],
		&y [0]
	);
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_StageProc_CODEHEADER_INCLUDED

#undef hiir_StageProc_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcFpu.hpp ***/

#endif	// hiir_StageProc_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcFpu.h ***/

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ctor
Throws: Nothing
==============================================================================
*/

template <int NC>
Upsampler2xFpu <NC>::Upsampler2xFpu ()
:	_coef ()
,	_x ()
,	_y ()
{
	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_coef [i] = 0;
	}
	clear_buffers ();
}

/*
==============================================================================
Name: set_coefs
Description:
   Sets filter coefficients. Generate them with the PolyphaseIir2Designer
   class.
   Call this function before doing any processing.
Input parameters:
	- coef_arr: Array of coefficients. There should be as many coefficients as
	  mentioned in the class template parameter.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xFpu <NC>::set_coefs (const double coef_arr [NBR_COEFS])
{
	assert (coef_arr != 0);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_coef [i] = static_cast <float> (coef_arr [i]);
	}
}

/*
==============================================================================
Name: process_sample
Description:
	Upsamples (x2) the input sample, generating two output samples.
Input parameters:
	- input: The input sample.
Output parameters:
	- out_0: First output sample.
	- out_1: Second output sample.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xFpu <NC>::process_sample (float &out_0, float &out_1, float input)
{
	assert (&out_0 != 0);
	assert (&out_1 != 0);

	float				even = input;
	float				odd = input;
	StageProcFpu <NBR_COEFS>::process_sample_pos (
		NBR_COEFS,
		even,
		odd,
		&_coef [0],
		&_x [0],
		&_y [0]
	);
	out_0 = even;
	out_1 = odd;
}

/*
==============================================================================
Name: process_block
Description:
	Upsamples (x2) the input sample block.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl samples.
	- nbr_spl: Number of input samples to process, > 0
Output parameters:
	- out_0_ptr: Output sample array, capacity: nbr_spl * 2 samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xFpu <NC>::process_block (float out_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (out_ptr != 0);
	assert (in_ptr != 0);
	assert (out_ptr >= in_ptr + nbr_spl || in_ptr >= out_ptr + nbr_spl);
	assert (nbr_spl > 0);

	long				pos = 0;
	do
	{
		process_sample (
			out_ptr [pos * 2    ],
			out_ptr [pos * 2 + 1],
			in_ptr [pos]
		);
		++ pos;
	}
	while (pos < nbr_spl);
}

/*
==============================================================================
Name: clear_buffers
Description:
	Clears filter memory, as if it processed silence since an infinite amount
	of time.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xFpu <NC>::clear_buffers ()
{
	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_x [i] = 0;
		_y [i] = 0;
	}
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Upsampler2xFpu_CODEHEADER_INCLUDED

#undef hiir_Upsampler2xFpu_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Upsampler2xFpu.hpp ***/

#endif	// hiir_Upsampler2xFpu_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Upsampler2xFpu.h ***/


/*** Start of inlined file: Downsampler2xSse.h ***/
#if ! defined (hiir_Downsampler2xSse_HEADER_INCLUDED)
#define	hiir_Downsampler2xSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: Array.h ***/
#if ! defined (hiir_Array_HEADER_INCLUDED)
#define	hiir_Array_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

template <class T, long LEN>
class Array
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(LEN > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef	T	Element;

   enum {         LENGTH   = LEN };

						Array () {};
						Array (const Array <T, LEN> &other);
	Array <T, LEN> &
						operator = (const Array <T, LEN> &other);

	const Element&	operator [] (long pos) const;
	Element &		operator [] (long pos);

	static long		size ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	Element			_data [LENGTH];

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Array &other);
	bool				operator != (const Array &other);

};	// class Array

}	// namespace hiir


/*** Start of inlined file: Array.hpp ***/
#if defined (hiir_Array_CURRENT_CODEHEADER)
	#error Recursive inclusion of Array code header.
#endif
#define	hiir_Array_CURRENT_CODEHEADER

#if ! defined (hiir_Array_CODEHEADER_INCLUDED)
#define	hiir_Array_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <class T, long LEN>
Array <T, LEN>::Array (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}
}

template <class T, long LEN>
Array <T, LEN> &	Array <T, LEN>::operator = (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}

	return (*this);
}

template <class T, long LEN>
const typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos) const
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos)
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
long	Array <T, LEN>::size ()
{
	return (LENGTH);
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Array_CODEHEADER_INCLUDED

#undef hiir_Array_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.hpp ***/

#endif	// hiir_Array_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.h ***/


/*** Start of inlined file: def.h ***/
#if ! defined (hiir_def_HEADER_INCLUDED)
#define	hiir_def_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

const double	PI			= 3.1415926535897932384626433832795;

#if defined (_MSC_VER)

	#define	hiir_FORCEINLINE	__forceinline

#else

	#define	hiir_FORCEINLINE	inline

#endif

}	// namespace hiir

#endif	// hiir_def_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: def.h ***/


/*** Start of inlined file: StageDataSse.h ***/
#if ! defined (hiir_StageDataSse_HEADER_INCLUDED)
#define	hiir_StageDataSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<xmmintrin.h>

namespace hiir
{

class StageDataSse
{

public:

	union
	{
		__m128			_align;			// Just to ensure alignement
		float				_coef [4];		// a_{4n+1}, a_{4n}, a_{4n+3}, a_{4n+2}
	};
	float				_mem [4];		// y of the stage

};	// class StageDataSse

}	// namespace hiir

#endif	// hiir_StageDataSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageDataSse.h ***/

namespace hiir
{

template <int NC>
class Downsampler2xSse
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(NC > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	enum {			NBR_COEFS	= NC	};

						Downsampler2xSse ();

	void				set_coefs (const double coef_arr []);

	hiir_FORCEINLINE float
						process_sample (const float in_ptr [2]);
	void				process_block (float out_ptr [], const float in_ptr [], long nbr_spl);

	hiir_FORCEINLINE void
						process_sample_split (float &low, float &high, const float in_ptr [2]);
	void				process_block_split (float out_l_ptr [], float out_h_ptr [], const float in_ptr [], long nbr_spl);

	void				clear_buffers ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	enum {			STAGE_WIDTH	= 4	};
	enum {			NBR_STAGES = (NBR_COEFS + STAGE_WIDTH - 1) / STAGE_WIDTH	};

	typedef	Array <StageDataSse, NBR_STAGES + 1>	Filter;	// Stage 0 contains only input memory

	Filter			_filter;		// Should be the first member (thus easier to align)

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Downsampler2xSse &other);
	bool				operator != (const Downsampler2xSse &other);

};	// class Downsampler2xSse

}	// namespace hiir


/*** Start of inlined file: Downsampler2xSse.hpp ***/
#if defined (hiir_Downsampler2xSse_CURRENT_CODEHEADER)
	#error Recursive inclusion of Downsampler2xSse code header.
#endif
#define	hiir_Downsampler2xSse_CURRENT_CODEHEADER

#if ! defined (hiir_Downsampler2xSse_CODEHEADER_INCLUDED)
#define	hiir_Downsampler2xSse_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: StageProcSse.h ***/
#if ! defined (hiir_StageProcSse_HEADER_INCLUDED)
#define	hiir_StageProcSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: def.h ***/
#if ! defined (hiir_def_HEADER_INCLUDED)
#define	hiir_def_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

const double	PI			= 3.1415926535897932384626433832795;

#if defined (_MSC_VER)

	#define	hiir_FORCEINLINE	__forceinline

#else

	#define	hiir_FORCEINLINE	inline

#endif

}	// namespace hiir

#endif	// hiir_def_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: def.h ***/

#include <xmmintrin.h>

namespace hiir
{

class StageDataSse;

template <int CUR>
class StageProcSse
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(CUR >= 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static hiir_FORCEINLINE void
						process_sample_pos (StageDataSse *stage_ptr, __m128 &y, __m128 &mem);
	static hiir_FORCEINLINE void
						process_sample_neg (StageDataSse *stage_ptr, __m128 &y, __m128 &mem);

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	enum {			PREV	= CUR - 1	};

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

						StageProcSse ();
						StageProcSse (const StageProcSse &other);
	StageProcSse &	operator = (const StageProcSse &other);
	bool				operator == (const StageProcSse &other);
	bool				operator != (const StageProcSse &other);

};	// class StageProcSse

}	// namespace hiir


/*** Start of inlined file: StageProcSse.hpp ***/
#if defined (hiir_StageProcSse_CURRENT_CODEHEADER)
	#error Recursive inclusion of StageProcSse code header.
#endif
#define	hiir_StageProcSse_CURRENT_CODEHEADER

#if ! defined (hiir_StageProcSse_CODEHEADER_INCLUDED)
#define	hiir_StageProcSse_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: StageDataSse.h ***/
#if ! defined (hiir_StageDataSse_HEADER_INCLUDED)
#define	hiir_StageDataSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<xmmintrin.h>

namespace hiir
{

class StageDataSse
{

public:

	union
	{
		__m128			_align;			// Just to ensure alignement
		float				_coef [4];		// a_{4n+1}, a_{4n}, a_{4n+3}, a_{4n+2}
	};
	float				_mem [4];		// y of the stage

};	// class StageDataSse

}	// namespace hiir

#endif	// hiir_StageDataSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageDataSse.h ***/

#if defined (_MSC_VER)
	#pragma inline_depth (255)
#endif

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <int CUR>
void	StageProcSse <CUR>::process_sample_pos (StageDataSse *stage_ptr, __m128 &y, __m128 &mem)
{
	StageProcSse <CUR - 1>::process_sample_pos (stage_ptr, y, mem);

	const __m128	x = mem;
	_mm_store_ps (stage_ptr [PREV]._mem, y);

	mem = _mm_load_ps (stage_ptr [CUR]._mem);
	y = _mm_sub_ps (y, mem);
	const __m128	coef = _mm_load_ps (stage_ptr [CUR]._coef);
	y = _mm_mul_ps (y, coef);
	y = _mm_add_ps (y, x);
}

template <>
hiir_FORCEINLINE void	StageProcSse <0>::process_sample_pos (StageDataSse * /* stage_ptr */, __m128 & /* y */, __m128 & /* mem */)
{
	// Nothing, stops the recursion
}

template <int CUR>
void	StageProcSse <CUR>::process_sample_neg (StageDataSse *stage_ptr, __m128 &y, __m128 &mem)
{
	StageProcSse <CUR - 1>::process_sample_neg (stage_ptr, y, mem);

	const __m128	x = mem;
	_mm_store_ps (stage_ptr [PREV]._mem, y);

	mem = _mm_load_ps (stage_ptr [CUR]._mem);
	y = _mm_add_ps (y, mem);
	const __m128	coef = _mm_load_ps (stage_ptr [CUR]._coef);
	y = _mm_mul_ps (y, coef);
	y = _mm_sub_ps (y, x);
}

template <>
hiir_FORCEINLINE void	StageProcSse <0>::process_sample_neg (StageDataSse * /* stage_ptr */, __m128 & /* y */, __m128 & /* mem */)
{
	// Nothing, stops the recursion
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_StageProcSse_CODEHEADER_INCLUDED

#undef hiir_StageProcSse_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcSse.hpp ***/

#endif	// hiir_StageProcSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcSse.h ***/

#include	<xmmintrin.h>

#include	<cassert>

namespace hiir
{

static const float	Downsampler2xSse_half = 0.5f;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ctor
Throws: Nothing
==============================================================================
*/

template <int NC>
Downsampler2xSse <NC>::Downsampler2xSse ()
:	_filter ()
{
	for (int i = 0; i < NBR_STAGES + 1; ++i)
	{
		_filter [i]._coef [0] = 0;
		_filter [i]._coef [1] = 0;
		_filter [i]._coef [2] = 0;
		_filter [i]._coef [3] = 0;
	}
	if ((NBR_COEFS & 1) != 0)
	{
		const int		pos = (NBR_COEFS ^ 1) & (STAGE_WIDTH - 1);
		_filter [NBR_STAGES]._coef [pos] = 1;
	}

	clear_buffers ();
}

/*
==============================================================================
Name: set_coefs
Description:
   Sets filter coefficients. Generate them with the PolyphaseIir2Designer
   class.
   Call this function before doing any processing.
Input parameters:
	- coef_arr: Array of coefficients. There should be as many coefficients as
	  mentioned in the class template parameter.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSse <NC>::set_coefs (const double coef_arr [])
{
	assert (coef_arr != 0);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		const int		stage = (i / STAGE_WIDTH) + 1;
		const int		pos = (i ^ 1) & (STAGE_WIDTH - 1);
		_filter [stage]._coef [pos] = static_cast <float> (coef_arr [i]);
	}
}

/*
==============================================================================
Name: process_sample
Description:
   Downsamples (x2) one pair of samples, to generate one output sample.
Input parameters:
	- in_ptr: pointer on the two samples to decimate
Returns: Samplerate-reduced sample.
Throws: Nothing
==============================================================================
*/

template <int NC>
float	Downsampler2xSse <NC>::process_sample (const float in_ptr [2])
{
	assert (in_ptr != 0);

	// Combines two input samples and two mid-processing data
	const __m128	spl_in = _mm_loadu_ps (in_ptr);
	const __m128	spl_mid = _mm_load_ps (_filter [NBR_STAGES]._mem);
	__m128			y = _mm_shuffle_ps (spl_in, spl_mid, 0x44);

	__m128			mem = _mm_load_ps (_filter [0]._mem);

	// Processes each stage
	StageProcSse <NBR_STAGES>::process_sample_pos (&_filter [0], y, mem);

	_mm_store_ps (_filter [NBR_STAGES]._mem, y);

	// Averages both paths and outputs the result
	const __m128	dup_y = y;
	y = _mm_shuffle_ps (y, y, 0x80);
	y = _mm_add_ps (y, dup_y);
	y = _mm_shuffle_ps (y, y, 3);
	y = _mm_mul_ss (y, _mm_load_ss (&Downsampler2xSse_half));
	float				result;
	_mm_store_ss (&result, y);

	return (result);
}

/*
==============================================================================
Name: process_block
Description:
   Downsamples (x2) a block of samples.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl * 2 samples.
	- nbr_spl: Number of samples to output, > 0
Output parameters:
	- out_ptr: Array for the output samples, capacity: nbr_spl samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSse <NC>::process_block (float out_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (in_ptr != 0);
	assert (out_ptr != 0);
	assert (out_ptr <= in_ptr || out_ptr >= in_ptr + nbr_spl * 2);
	assert (nbr_spl > 0);

	long				pos = 0;
	do
	{
		out_ptr [pos] = process_sample (in_ptr + pos * 2);
		++ pos;
	}
	while (pos < nbr_spl);
}

/*
==============================================================================
Name: process_sample_split
Description:
   Split (spectrum-wise) in half a pair of samples. The lower part of the
   spectrum is a classic downsampling, equivalent to the output of
   process_sample().
   The higher part is the complementary signal: original filter response
   is flipped from left to right, becoming a high-pass filter with the same
   cutoff frequency. This signal is then critically sampled (decimation by 2),
   flipping the spectrum: Fs/4...Fs/2 becomes Fs/4...0.
Input parameters:
	- in_ptr: pointer on the pair of input samples
Output parameters:
	- low: output sample, lower part of the spectrum (downsampling)
	- high: output sample, higher part of the spectrum.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSse <NC>::process_sample_split (float &low, float &high, const float in_ptr [2])
{
	assert (&low != 0);
	assert (&high != 0);
	assert (in_ptr != 0);

	// Combines two input samples and two mid-processing data
	const __m128	spl_in = _mm_loadu_ps (in_ptr);
	const __m128	spl_mid = _mm_load_ps (_filter [NBR_STAGES]._mem);
	__m128			y = _mm_shuffle_ps (spl_in, spl_mid, 0x44);

	__m128			mem = _mm_load_ps (_filter [0]._mem);

	// Processes each stage
	StageProcSse <NBR_STAGES>::process_sample_pos (&_filter [0], y, mem);

	_mm_store_ps (_filter [NBR_STAGES]._mem, y);

	//Outputs the result
	__m128			dup_y = y;
	y = _mm_shuffle_ps (y, y, 0x80);
	y = _mm_add_ps (y, dup_y);
	y = _mm_shuffle_ps (y, y, 3);
	y = _mm_mul_ss (y, _mm_load_ss (&Downsampler2xSse_half));
	_mm_store_ss (&low, y);

	dup_y = _mm_shuffle_ps (dup_y, dup_y, 3);
	dup_y = _mm_sub_ps (dup_y, y);
	_mm_store_ss (&high, dup_y);
}

/*
==============================================================================
Name: process_block_split
Description:
   Split (spectrum-wise) in half a block of samples. The lower part of the
   spectrum is a classic downsampling, equivalent to the output of
   process_block().
   The higher part is the complementary signal: original filter response
   is flipped from left to right, becoming a high-pass filter with the same
   cutoff frequency. This signal is then critically sampled (decimation by 2),
   flipping the spectrum: Fs/4...Fs/2 becomes Fs/4...0.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl * 2 samples.
	- nbr_spl: Number of samples for each output, > 0
Output parameters:
	- out_l_ptr: Array for the output samples, lower part of the spectrum
	  (downsampling). Capacity: nbr_spl samples.
	- out_h_ptr: Array for the output samples, higher part of the spectrum.
	  Capacity: nbr_spl samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSse <NC>::process_block_split (float out_l_ptr [], float out_h_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (in_ptr != 0);
	assert (out_l_ptr != 0);
	assert (out_l_ptr <= in_ptr || out_l_ptr >= in_ptr + nbr_spl * 2);
	assert (out_h_ptr != 0);
	assert (out_h_ptr <= in_ptr || out_h_ptr >= in_ptr + nbr_spl * 2);
	assert (out_h_ptr != out_l_ptr);
	assert (nbr_spl > 0);

	long				pos = 0;
	do
	{
		process_sample_split (out_l_ptr [pos], out_h_ptr [pos], in_ptr + pos * 2);
		++ pos;
	}
	while (pos < nbr_spl);
}

/*
==============================================================================
Name: clear_buffers
Description:
	Clears filter memory, as if it processed silence since an infinite amount
	of time.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSse <NC>::clear_buffers ()
{
	for (int i = 0; i < NBR_STAGES + 1; ++i)
	{
		_filter [i]._mem [0] = 0;
		_filter [i]._mem [1] = 0;
		_filter [i]._mem [2] = 0;
		_filter [i]._mem [3] = 0;
	}
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Downsampler2xSse_CODEHEADER_INCLUDED

#undef hiir_Downsampler2xSse_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Downsampler2xSse.hpp ***/

#endif	// hiir_Downsampler2xSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Downsampler2xSse.h ***/


/*** Start of inlined file: Upsampler2xSse.h ***/
#if ! defined (hiir_Upsampler2xSse_HEADER_INCLUDED)
#define	hiir_Upsampler2xSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: Array.h ***/
#if ! defined (hiir_Array_HEADER_INCLUDED)
#define	hiir_Array_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

template <class T, long LEN>
class Array
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(LEN > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef	T	Element;

   enum {         LENGTH   = LEN };

						Array () {};
						Array (const Array <T, LEN> &other);
	Array <T, LEN> &
						operator = (const Array <T, LEN> &other);

	const Element&	operator [] (long pos) const;
	Element &		operator [] (long pos);

	static long		size ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	Element			_data [LENGTH];

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Array &other);
	bool				operator != (const Array &other);

};	// class Array

}	// namespace hiir


/*** Start of inlined file: Array.hpp ***/
#if defined (hiir_Array_CURRENT_CODEHEADER)
	#error Recursive inclusion of Array code header.
#endif
#define	hiir_Array_CURRENT_CODEHEADER

#if ! defined (hiir_Array_CODEHEADER_INCLUDED)
#define	hiir_Array_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <class T, long LEN>
Array <T, LEN>::Array (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}
}

template <class T, long LEN>
Array <T, LEN> &	Array <T, LEN>::operator = (const Array <T, LEN> &other)
{
	for (long pos = 0; pos < LENGTH; ++pos)
	{
		_data [pos] = other._data [pos];
	}

	return (*this);
}

template <class T, long LEN>
const typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos) const
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
typename Array <T, LEN>::Element &	Array <T, LEN>::operator [] (long pos)
{
	assert (pos >= 0);
	assert (pos < LENGTH);

	return (_data [pos]);
}

template <class T, long LEN>
long	Array <T, LEN>::size ()
{
	return (LENGTH);
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Array_CODEHEADER_INCLUDED

#undef hiir_Array_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.hpp ***/

#endif	// hiir_Array_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Array.h ***/


/*** Start of inlined file: StageDataSse.h ***/
#if ! defined (hiir_StageDataSse_HEADER_INCLUDED)
#define	hiir_StageDataSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<xmmintrin.h>

namespace hiir
{

class StageDataSse
{

public:

	union
	{
		__m128			_align;			// Just to ensure alignement
		float				_coef [4];		// a_{4n+1}, a_{4n}, a_{4n+3}, a_{4n+2}
	};
	float				_mem [4];		// y of the stage

};	// class StageDataSse

}	// namespace hiir

#endif	// hiir_StageDataSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageDataSse.h ***/

#include	<xmmintrin.h>

namespace hiir
{

template <int NC>
class Upsampler2xSse
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(NC > 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	enum {			NBR_COEFS	= NC	};

						Upsampler2xSse ();

	void				set_coefs (const double coef_arr [NBR_COEFS]);
	inline void		process_sample (float &out_0, float &out_1, float input);
	void				process_block (float out_ptr [], const float in_ptr [], long nbr_spl);
	void				clear_buffers ();

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	enum {			STAGE_WIDTH	= 4	};
	enum {			NBR_STAGES = (NBR_COEFS + STAGE_WIDTH - 1) / STAGE_WIDTH	};

	typedef	Array <StageDataSse, NBR_STAGES + 1>	Filter;	// Stage 0 contains only input memory

	Filter			_filter;		// Should be the first member (thus easier to align)

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool				operator == (const Upsampler2xSse &other);
	bool				operator != (const Upsampler2xSse &other);

};	// class Upsampler2xSse

}	// namespace hiir


/*** Start of inlined file: Upsampler2xSse.hpp ***/
#if defined (hiir_Upsampler2xSse_CURRENT_CODEHEADER)
	#error Recursive inclusion of Upsampler2xSse code header.
#endif
#define	hiir_Upsampler2xSse_CURRENT_CODEHEADER

#if ! defined (hiir_Upsampler2xSse_CODEHEADER_INCLUDED)
#define	hiir_Upsampler2xSse_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: StageProcSse.h ***/
#if ! defined (hiir_StageProcSse_HEADER_INCLUDED)
#define	hiir_StageProcSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: def.h ***/
#if ! defined (hiir_def_HEADER_INCLUDED)
#define	hiir_def_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

const double	PI			= 3.1415926535897932384626433832795;

#if defined (_MSC_VER)

	#define	hiir_FORCEINLINE	__forceinline

#else

	#define	hiir_FORCEINLINE	inline

#endif

}	// namespace hiir

#endif	// hiir_def_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: def.h ***/

#include <xmmintrin.h>

namespace hiir
{

class StageDataSse;

template <int CUR>
class StageProcSse
{

	// Template parameter check, not used
	typedef	int	ChkTpl1 [(CUR >= 0) ? 1 : -1];

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static hiir_FORCEINLINE void
						process_sample_pos (StageDataSse *stage_ptr, __m128 &y, __m128 &mem);
	static hiir_FORCEINLINE void
						process_sample_neg (StageDataSse *stage_ptr, __m128 &y, __m128 &mem);

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	enum {			PREV	= CUR - 1	};

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

						StageProcSse ();
						StageProcSse (const StageProcSse &other);
	StageProcSse &	operator = (const StageProcSse &other);
	bool				operator == (const StageProcSse &other);
	bool				operator != (const StageProcSse &other);

};	// class StageProcSse

}	// namespace hiir


/*** Start of inlined file: StageProcSse.hpp ***/
#if defined (hiir_StageProcSse_CURRENT_CODEHEADER)
	#error Recursive inclusion of StageProcSse code header.
#endif
#define	hiir_StageProcSse_CURRENT_CODEHEADER

#if ! defined (hiir_StageProcSse_CODEHEADER_INCLUDED)
#define	hiir_StageProcSse_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*** Start of inlined file: StageDataSse.h ***/
#if ! defined (hiir_StageDataSse_HEADER_INCLUDED)
#define	hiir_StageDataSse_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<xmmintrin.h>

namespace hiir
{

class StageDataSse
{

public:

	union
	{
		__m128			_align;			// Just to ensure alignement
		float				_coef [4];		// a_{4n+1}, a_{4n}, a_{4n+3}, a_{4n+2}
	};
	float				_mem [4];		// y of the stage

};	// class StageDataSse

}	// namespace hiir

#endif	// hiir_StageDataSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageDataSse.h ***/

#if defined (_MSC_VER)
	#pragma inline_depth (255)
#endif

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

template <int CUR>
void	StageProcSse <CUR>::process_sample_pos (StageDataSse *stage_ptr, __m128 &y, __m128 &mem)
{
	StageProcSse <CUR - 1>::process_sample_pos (stage_ptr, y, mem);

	const __m128	x = mem;
	_mm_store_ps (stage_ptr [PREV]._mem, y);

	mem = _mm_load_ps (stage_ptr [CUR]._mem);
	y = _mm_sub_ps (y, mem);
	const __m128	coef = _mm_load_ps (stage_ptr [CUR]._coef);
	y = _mm_mul_ps (y, coef);
	y = _mm_add_ps (y, x);
}

template <>
hiir_FORCEINLINE void	StageProcSse <0>::process_sample_pos (StageDataSse * /* stage_ptr */, __m128 & /* y */, __m128 & /* mem */)
{
	// Nothing, stops the recursion
}

template <int CUR>
void	StageProcSse <CUR>::process_sample_neg (StageDataSse *stage_ptr, __m128 &y, __m128 &mem)
{
	StageProcSse <CUR - 1>::process_sample_neg (stage_ptr, y, mem);

	const __m128	x = mem;
	_mm_store_ps (stage_ptr [PREV]._mem, y);

	mem = _mm_load_ps (stage_ptr [CUR]._mem);
	y = _mm_add_ps (y, mem);
	const __m128	coef = _mm_load_ps (stage_ptr [CUR]._coef);
	y = _mm_mul_ps (y, coef);
	y = _mm_sub_ps (y, x);
}

template <>
hiir_FORCEINLINE void	StageProcSse <0>::process_sample_neg (StageDataSse * /* stage_ptr */, __m128 & /* y */, __m128 & /* mem */)
{
	// Nothing, stops the recursion
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_StageProcSse_CODEHEADER_INCLUDED

#undef hiir_StageProcSse_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcSse.hpp ***/

#endif	// hiir_StageProcSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: StageProcSse.h ***/

#include	<xmmintrin.h>

#include	<cassert>

namespace hiir
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ctor
Throws: Nothing
==============================================================================
*/

template <int NC>
Upsampler2xSse <NC>::Upsampler2xSse ()
:	_filter ()
{
	for (int i = 0; i < NBR_STAGES + 1; ++i)
	{
		_filter [i]._coef [0] = 0;
		_filter [i]._coef [1] = 0;
		_filter [i]._coef [2] = 0;
		_filter [i]._coef [3] = 0;
	}
	if ((NBR_COEFS & 1) != 0)
	{
		const int		pos = (NBR_COEFS ^ 1) & (STAGE_WIDTH - 1);
		_filter [NBR_STAGES]._coef [pos] = 1;
	}

	clear_buffers ();
}

/*
==============================================================================
Name: set_coefs
Description:
   Sets filter coefficients. Generate them with the PolyphaseIir2Designer
   class.
   Call this function before doing any processing.
Input parameters:
	- coef_arr: Array of coefficients. There should be as many coefficients as
	  mentioned in the class template parameter.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xSse <NC>::set_coefs (const double coef_arr [NBR_COEFS])
{
	assert (coef_arr != 0);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		const int		stage = (i / STAGE_WIDTH) + 1;
		const int		pos = (i ^ 1) & (STAGE_WIDTH - 1);
		_filter [stage]._coef [pos] = static_cast <float> (coef_arr [i]);
	}
}

/*
==============================================================================
Name: process_sample
Description:
	Upsamples (x2) the input sample, generating two output samples.
Input parameters:
	- input: The input sample.
Output parameters:
	- out_0: First output sample.
	- out_1: Second output sample.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xSse <NC>::process_sample (float &out_0, float &out_1, float input)
{
	assert (&out_0 != 0);
	assert (&out_1 != 0);

	const __m128	spl_in = _mm_load_ss (&input);
	const __m128	spl_mid = _mm_load_ps (_filter [NBR_STAGES]._mem);
	__m128			y = _mm_shuffle_ps (spl_in, spl_mid, 0x40);

	__m128			mem = _mm_load_ps (_filter [0]._mem);

	StageProcSse <NBR_STAGES>::process_sample_pos (&_filter [0], y, mem);

	_mm_store_ps (_filter [NBR_STAGES]._mem, y);

	// The latest shufps/movss instruction pairs can be freely inverted
	y = _mm_shuffle_ps (y, y, 0xE3);
	_mm_store_ss (&out_0, y);
	y = _mm_shuffle_ps (y, y, 0xE2);
	_mm_store_ss (&out_1, y);
}

/*
==============================================================================
Name: process_block
Description:
	Upsamples (x2) the input sample block.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl samples.
	- nbr_spl: Number of input samples to process, > 0
Output parameters:
	- out_0_ptr: Output sample array, capacity: nbr_spl * 2 samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xSse <NC>::process_block (float out_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (out_ptr != 0);
	assert (in_ptr != 0);
	assert (out_ptr >= in_ptr + nbr_spl || in_ptr >= out_ptr + nbr_spl);
	assert (nbr_spl > 0);

	long				pos = 0;
	do
	{
		process_sample (out_ptr [pos * 2], out_ptr [pos * 2 + 1], in_ptr [pos]);
		++ pos;
	}
	while (pos < nbr_spl);
}

/*
==============================================================================
Name: clear_buffers
Description:
	Clears filter memory, as if it processed silence since an infinite amount
	of time.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2xSse <NC>::clear_buffers ()
{
	for (int i = 0; i < NBR_STAGES + 1; ++i)
	{
		_filter [i]._mem [0] = 0;
		_filter [i]._mem [1] = 0;
		_filter [i]._mem [2] = 0;
		_filter [i]._mem [3] = 0;
	}
}

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}	// namespace hiir

#endif	// hiir_Upsampler2xSse_CODEHEADER_INCLUDED

#undef hiir_Upsampler2xSse_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Upsampler2xSse.hpp ***/

#endif	// hiir_Upsampler2xSse_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: Upsampler2xSse.h ***/


/*** Start of inlined file: fnc.h ***/
#if ! defined (hiir_fnc_HEADER_INCLUDED)
#define	hiir_fnc_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

inline int	round_int (double x);
inline int	ceil_int (double x);

template <class T>
T	ipowp (T x, long n);

}	// namespace hiir


/*** Start of inlined file: fnc.hpp ***/
#if defined (hiir_fnc_CURRENT_CODEHEADER)
	#error Recursive inclusion of fnc code header.
#endif
#define	hiir_fnc_CURRENT_CODEHEADER

#if ! defined (hiir_fnc_CODEHEADER_INCLUDED)
#define	hiir_fnc_CODEHEADER_INCLUDED

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include	<cassert>
#include	<cmath>

namespace std { }

namespace hiir
{

int	round_int (double x)
{
	using namespace std;

	return (static_cast <int> (floor (x + 0.5)));
}

int	ceil_int (double x)
{
	using namespace std;

	return (static_cast <int> (ceil (x)));
}

template <class T>
T	ipowp (T x, long n)
{
	assert (n >= 0);

	T					z (1);
	while (n != 0)
	{
		if ((n & 1) != 0)
		{
			z *= x;
		}
		n >>= 1;
		x *= x;
	}

	return (z);
}

}	// namespace hiir

#endif	// hiir_fnc_CODEHEADER_INCLUDED

#undef hiir_fnc_CURRENT_CODEHEADER

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: fnc.hpp ***/

#endif	// hiir_fnc_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: fnc.h ***/


/*** Start of inlined file: PolyphaseIir2Designer.h ***/
#if ! defined (hiir_PolyphaseIir2Designer_HEADER_INCLUDED)
#define	hiir_PolyphaseIir2Designer_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif

/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

namespace hiir
{

class PolyphaseIir2Designer
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static int		compute_nbr_coefs_from_proto (double attenuation, double transition);
	static double	compute_atten_from_order_tbw (int nbr_coefs, double transition);

	static int		compute_coefs (double coef_arr [], double attenuation, double transition);
	static void		compute_coefs_spec_order_tbw (double coef_arr [], int nbr_coefs, double transition);

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	static void		compute_transition_param (double &k, double &q, double transition);
	static int		compute_order (double attenuation, double q);
	static double	compute_atten (double q, int order);
	static double	compute_coef (int index, double k, double q, int order);
	static double	compute_acc_num (double q, int order, int c);
	static double	compute_acc_den (double q, int order, int c);

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

						PolyphaseIir2Designer ();
						~PolyphaseIir2Designer ();
						PolyphaseIir2Designer (const PolyphaseIir2Designer &other);
	PolyphaseIir2Designer &
						operator = (const PolyphaseIir2Designer &other);
	bool				operator == (const PolyphaseIir2Designer &other);
	bool				operator != (const PolyphaseIir2Designer &other);

};	// class PolyphaseIir2Designer

}	// namespace hiir

#endif	// hiir_PolyphaseIir2Designer_HEADER_INCLUDED

/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*** End of inlined file: PolyphaseIir2Designer.h ***/

