/** 
	@file ob_DataStructures.h
*/

#pragma once

#include "JuceHeader.h"
#include <atomic>
#ifdef JUCE_CLANG
#include <mm_malloc.h>
#endif

namespace ob {
namespace General {

	/** A templated 1D array. Can be wrapped in an OwnedArray to make a 2D array. */
	template <class ElementType								/**< Data type for array elements. */ >
	class DataArray
	{
	public:
		
		/** Constructor. */
		DataArray ()
		{
			data = HeapBlock<ElementType> ();
			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			mySize = -1;
		}

		/** Constructor. */
        explicit DataArray (const int size,							/**< Number of elements in the array. */
                            const bool initialiseToZero = false		/**< Set this to initialise elements to zero (initialises to false if T is bool). */
                           )
		{
			data = HeapBlock<ElementType> ();
			setSize (size, initialiseToZero);
		}

		/** Destructor. */
		~DataArray ()
		{
			data.free ();
		}

		/** Returns the number of elements of data. */
		int size () const
		{
			return mySize;
		}

		/** Sets the size of the array. */
		void setSize (const int size,							/**< Number of elements in the array. */
					  const bool initialiseToZero = false		/**< Set this to initialise elements to zero (initialises to false if T is bool). */
					  )
		{
			jassert (size > 0);
			data.allocate (size, initialiseToZero);
			mySize = size;
		}

		/** Accesses an array item. */
		ElementType& operator[] (const int index							/**< Index of the item to be accessed. */)
		{
			jassert (isPositiveAndBelow (index, mySize));
			return data[index];
		}
		
		/** Accesses an array item (const version). */
		ElementType& operator[] (const int index							/**< Index of the item to be accessed. */) const
		{
			jassert (isPositiveAndBelow (index, mySize));
			return data[index];
		}

		/** Accesses an array item. */
		ElementType& getItem (const int index								/**< Index of the item to be accessed. */)
		{
			jassert (isPositiveAndBelow (index, mySize));
			return data[index];
		}

		/** Accesses an array item (const version). */
		ElementType& getItem (const int index								/**< Index of the item to be accessed. */) const
		{
			jassert (isPositiveAndBelow (index, mySize));
			return data[index];
		}

		/** Returns a pointer to the data. */
		ElementType* getData ()
		{
			return data.getData ();
		}

	private:
		HeapBlock<ElementType> data;
		int mySize;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataArray)
	};
	
	/** A one dimensional array of aligned simple data types. Typically used for audio sample buffers aligned for SSE or AVX operations.
		Not thread safe. Memory is not initialised when allocated.
	 */
	template <	class ElementType,			/**< Simple data type - only float, double & int are supported. */
				int alignment				/**< Byte alignment (16 for SSE, 32 for AVX). */
			 >
	class AlignedDataArray
	{
	public:
		/** Constructor. Memory is not allocated or initialised. You will need to call setLength(). */
		AlignedDataArray ()
		{
			init ();
		}

		/** Constructor. Automatically allocates memory. Memory is not initialised. */
		AlignedDataArray (const int length		/**< Number of elements in the array (e.g. samples). */)
		{
			init ();
			setLength (length);
		}

		/** Destructor. Automatically de-allocates memory. */
		~AlignedDataArray ()
		{
			clear ();
		}

		/** Returns the data pointer. */
		ElementType* getData ()
		{
			return data;
		}

		/** Accesses an array item. */
		ElementType& operator[] (const int index							/**< Index of the item to be accessed. */)
		{
			jassert (isPositiveAndBelow (index, myLength));
			return data[index];
		}

		/** Accesses an array item. */
		ElementType& getItem (const int index								/**< Index of the item to be accessed. */)
		{
			jassert (isPositiveAndBelow (index, myLength));
			return data[index];
		}

		/** Return the length. */
		int getLength() const
        {
			jassert (myLength > 0);
			return myLength;
		}

		/** Sets the length. Automatically clears then allocates memory. Memory is not initialised.*/
		void setLength (const int length)
		{
			jassert (length > 0);
			clear ();
			#ifndef OB_SSE_NOT_SUPPORTED
			data = reinterpret_cast<ElementType*>(_mm_malloc (length*sizeof (ElementType), alignment));
			#else
			data = reinterpret_cast<ElementType*>(malloc (length*sizeof (ElementType)));
			#endif
			myLength = length;
		}

	private:

		ElementType* data;
		int myLength;

		void ValidateType (int    &) const {}
		void ValidateType (double &) const {}
		void ValidateType (float  &) const {}

		/** Initialisation routine. */
		void init ()
		{
			ElementType checkType;
			ValidateType (checkType);

			#if JUCE_MSVC
				#pragma warning (push)
				#pragma warning (disable: 4127) // conditional expression is constant warning
			#endif
            jassert (alignment == 16 || alignment == 32);
			#if JUCE_MSVC
				#pragma warning (pop)
			#endif

			data = nullptr;
			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			myLength = -1;
		}

		/** De-allocates all memory. */
		void clear ()
		{
			if (data != nullptr)
				#ifndef OB_SSE_NOT_SUPPORTED
				_mm_free (data);
				#else
				free (data);
				#endif
			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			myLength = -1;
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlignedDataArray)
	};

	/**	A two dimensional array of aligned simple data types. All of the rows have the same length (i.e. the array is rectangular).
		Typically used for audio sample buffers aligned for SSE or AVX operations. Not thread safe. Memory is not initialised when allocated.
	*/
	template <	class ElementType,			/**< Simple data type - only float, double & int are supported. */
				int alignment				/**< Byte alignment (16 for SSE, 32 for AVX). */
			 >
	class AlignedRectangularArray
	{
	public:
		/** Constructor. No rows allocated so will need to call setSize(). */
		AlignedRectangularArray ()
		{
			init ();
		}

		/** Constructor. Automatically allocates memory. Memory is not initialised. */
		AlignedRectangularArray (const int numRows,		/**< Number of rows. */
								 const int length		/**< Length of each row. */)
		{
			init ();
			setSize (numRows, length);
		}

		/** Destructor. Automatically de-allocates memory. */
		~AlignedRectangularArray ()
		{
			clear ();
		}

		/** Returns the data pointer. */
		ElementType** getData ()
		{
			return data;
		}

		/** Returns a pointer to the specified row. */
		ElementType* operator[] (const int index		/**< Index of row. */) const
		{
			jassert (myNumRows > 0 && isPositiveAndBelow(index, myNumRows));
			return data[index];
		}

		/** Returns a pointer to the specified row. */
		ElementType* getRow (const int index		/**< Index of row. */)
		{
			jassert (myNumRows > 0 && isPositiveAndBelow (index, myNumRows));
			return data[index];
		}

		/** Return the number of rows allocated. */
		int getNumRows() const
		{
			jassert (myNumRows > 0);
			return myNumRows;
		}

		/** Return the row length. */
		int getLength() const
		{
			jassert (myNumRows > 0);
			return myLength;
		}

		/** Set size of the array. */
		void setSize (const int numRows,		/**< Number of rows. */
					  const int length			/**< Length of rows. */
					  )
		{
			jassert (numRows > 0 && length > 0);
			if (numRows != myNumRows || length != myLength)
			{
				clear ();
				myNumRows = numRows;
				myLength = length;
				data = new ElementType*[myNumRows];
				for (int row = 0; row < myNumRows; ++row)
					#ifndef OB_SSE_NOT_SUPPORTED
					data[row] = reinterpret_cast<ElementType*>(_mm_malloc (length*sizeof (ElementType), alignment));
					#else
					data[row] = reinterpret_cast<ElementType*>(malloc (length*sizeof (ElementType)));
					#endif
			}
		}

	private:

		ElementType** data;
		int myNumRows;
		int myLength;

		void ValidateType (int    &) const {}
		void ValidateType (double &) const {}
		void ValidateType (float  &) const {}

		/** Initialisation routine. */
		void init ()
		{
			ElementType checkType;
			ValidateType (checkType);

			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			myNumRows = -1;
			myLength = -1;

			data = nullptr;
		}

		/** Deallocates all memory. */
		void clear ()
		{
			if (myNumRows>0)
				for (int row = 0; row < myNumRows; ++row)
					#ifndef OB_SSE_NOT_SUPPORTED
					_mm_free (data[row]);
					#else
					free (data[row]);
					#endif
			if (data != nullptr)
				delete[] data;
			
			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			myNumRows = -1;
			myLength = -1;
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlignedRectangularArray)
	};

}	// end namespace General
}	// end namespace ob