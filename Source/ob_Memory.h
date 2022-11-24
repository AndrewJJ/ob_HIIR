/**
@file ob_Memory.h
*/

#pragma once

#include "JuceHeader.h"
#include <atomic>
#include <stdexcept>    // std::length_error

namespace ob {

	/** A templated 1D array. Can be wrapped in an OwnedArray to make a 2D array. */
	template <class ElementType								/**< Data type for array elements. */ >
	class DataArray
	{
	public:
		
		/** Constructor. */
		DataArray ()
		{
			data = juce::HeapBlock<ElementType> ();
			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			mySize = -1;
		}

		/** Constructor. */
        explicit DataArray (const int size,							/**< Number of elements in the array. */
                            const bool initialiseToZero = false		/**< Set this to initialise elements to zero (initialises to false if T is bool). */
                           )
		{
			data = juce::HeapBlock<ElementType> ();
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
			jassert (juce::isPositiveAndBelow (index, mySize));
			return data[index];
		}
		
		/** Accesses an array item (const version). */
		ElementType& operator[] (const int index							/**< Index of the item to be accessed. */) const
		{
			jassert (juce::isPositiveAndBelow (index, mySize));
			return data[index];
		}

		/** Accesses an array item. */
		ElementType& getItem (const int index								/**< Index of the item to be accessed. */)
		{
			jassert (juce::isPositiveAndBelow (index, mySize));
			return data[index];
		}

		/** Accesses an array item (const version). */
		ElementType& getItem (const int index								/**< Index of the item to be accessed. */) const
		{
			jassert (juce::isPositiveAndBelow (index, mySize));
			return data[index];
		}

		/** Returns a pointer to the data. */
		ElementType* getData ()
		{
			return data.getData ();
		}

	private:
		juce::HeapBlock<ElementType> data;
		int mySize;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataArray)
	};
	
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
			jassert (juce::isPositiveAndBelow (index, myLength));
			return data[index];
		}

		/** Accesses an array item. */
		ElementType& getItem (const int index								/**< Index of the item to be accessed. */)
		{
			jassert (juce::isPositiveAndBelow (index, myLength));
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
			#if JUCE_USE_SSE_INTRINSICS
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
				#if JUCE_USE_SSE_INTRINSICS
				_mm_free (data);
				#else
				free (data);
				#endif
			// Set negative so assertions fail if an attempt is made to access elements prior to setSize() being called
			myLength = -1;
		}

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlignedDataArray)
	};

#if JUCE_USE_SSE_INTRINSICS

#if JUCE_MSVC
#pragma warning (push)
#pragma warning (disable: 4100) // unreferenced formal parameter warning
#endif

	/**
	*	Allocator for aligned data - for use with std::vector.
	*
	*	Sourced from https://gist.github.com/gavrus/6480442
	*
	*	Modified from the Mallocator from Stephan T. Lavavej.
	*	<http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx>
	*/
	template <typename T, std::size_t Alignment>
	class aligned_allocator
	{
	public:

		// The following will be the same for virtually all allocators.
		typedef T * pointer;
		typedef const T * const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		typedef std::size_t size_type;
		typedef ptrdiff_t difference_type;

		T * address (T& r) const
		{
			return &r;
		}

		const T * address (const T& s) const
		{
			return &s;
		}

		std::size_t max_size() const
		{
			// The following has been carefully written to be independent of
			// the definition of size_t and to avoid signed/unsigned warnings.
			return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof (T);
		}


		// The following must be the same for all allocators.
		template <typename U>
		struct rebind
		{
			typedef aligned_allocator<U, Alignment> other;
		};

		bool operator!=(const aligned_allocator& other) const
		{
			return !(*this == other);
		}

		void construct (T * const p, const T& t) const
		{
			void * const pv = static_cast<void *>(p);

			new (pv)T (t);
		}

		void destroy (T * const p) const
		{
			p->~T();
		}

		// Returns true if and only if storage allocated from *this
		// can be deallocated from other, and vice versa.
		// Always returns true for stateless allocators.
		bool operator==(const aligned_allocator& other) const
		{
			return true;
		}


		// Default constructor, copy constructor, rebinding constructor, and destructor.
		// Empty for stateless allocators.
		aligned_allocator() { }

		aligned_allocator(const aligned_allocator&) { }

		template <typename U> aligned_allocator (const aligned_allocator<U, Alignment>&) { }

		~aligned_allocator() { }


		// The following will be different for each allocator.
		T * allocate (const std::size_t n) const
		{
			// The return value of allocate(0) is unspecified.
			// Mallocator returns NULL in order to avoid depending
			// on malloc(0)'s implementation-defined behavior
			// (the implementation can define malloc(0) to return NULL,
			// in which case the bad_alloc check below would fire).
			// All allocators can return NULL in this case.
			if (n == 0) {
				return NULL;
			}

			// All allocators should contain an integer overflow check.
			// The Standardization Committee recommends that std::length_error
			// be thrown in the case of integer overflow.
			if (n > max_size())
			{
				throw std::length_error ("aligned_allocator<T>::allocate() - Integer overflow.");
			}

			// Mallocator wraps malloc().
			void * const pv = _mm_malloc (n * sizeof (T), Alignment);

			// Allocators should throw std::bad_alloc in the case of memory allocation failure.
			if (pv == NULL)
			{
				throw std::bad_alloc();
			}

			return static_cast<T *>(pv);
		}

		void deallocate (T * const p, const std::size_t n) const
		{
			_mm_free (p);
		}


		// The following will be the same for all allocators that ignore hints.
		template <typename U>
		T * allocate (const std::size_t n, const U * /* const hint */) const
		{
			return allocate (n);
		}


		// Allocators are not required to be assignable, so
		// all allocators should have a private unimplemented
		// assignment operator. Note that this will trigger the
		// off-by-default (enabled under /Wall) warning C4626
		// "assignment operator could not be generated because a
		// base class assignment operator is inaccessible" within
		// the STL headers, but that warning is useless.
	private:
		aligned_allocator& operator=(const aligned_allocator&);
	};

#if JUCE_MSVC
#pragma warning (pop)
#endif

    // We use the following alias to return an allocator which aligns on 16 byte boundary if SSE is being used
    template <typename T>
    using auto_sse_allocator = aligned_allocator <T, 16>;

#else // JUCE_USE_SSE_INTRINSICS

    // If SSE isn't being used, a standard allocator is returned instead
    template <typename T>
    using auto_sse_allocator = std::allocator <T>;

#endif // JUCE_USE_SSE_INTRINSICS

}	// end namespace ob
