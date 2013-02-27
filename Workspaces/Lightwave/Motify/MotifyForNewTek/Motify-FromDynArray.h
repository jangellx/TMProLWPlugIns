//
// DynArray.h
//
// DynArray is a dymaically-resizable array class.  It automatically
//  grows to accomidate new elements added with the Add() function.
//
// There are two modes the array can function in:  Unsorted and Sorted:
//  - Unsorted:
//    The array does not attempt to keep the elements are not sorted
//    at all.  For example, deleting an element from the middle
//    of the array will simply copy the last element to the current
//    element's position and decrement the num_elements counter.
//  - Sorted:
//    The array attempts to keep the original order of the array.
//    If an alement is deleted, the entire array is shifted to the
//    new location.  This is slower than an Unsorted array.  Default
//    mode.
//
// Everytime the array has to grow, a new block of memeory is allocated
//  equal to the size of the old array plus the grow size.  The old
//  array is copied to the new location, and the old block is deleted.
//  Thus, this class should not be used for very large arrays with small
//  grow sizes.
//
// As elements are deleted from arrays, they shrink.  The internals of
//  this are similar to a grow operation.  It's recommended that the
//  shrink size be larger than the grow size.
//
// Note that any time the array changes size (except for Reset() and
//  Flush() which clear everything in the array), a second memory block
//  is allocated at the new size to copy the first block into.  This means
//  that there must be enough memory free to allocate the new memory
//  block, or this operation will fail.
//
// Note also that when the class is deleted and the array contains
//  pointers, none of the elements are deleted.  This can be done
//  manually with the Flush() function.  This is unessesary with
//  non-pointer data, which will be automatically flushed when the
//  array is deleted.
// 
// Being a template class, the entire class consists of just this one
//  file.
// 

#ifndef DYNARRAY_HEADER_INCLUDED
#define DYNARRAY_HEADER_INCLUDED

#ifdef _WINDOWS
  #include <search.h>
#else
  #define __cdecl
#endif

#include <memory.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define DEFAULT_GROW_SIZE    10
#define DEFAULT_SHRINK_SIZE  20
#define DA_NO_MATCH          -1

enum DynArrayBinarySearchModes {
  DA_BSEARCH_EXACT,
  DA_BSEARCH_NEAREST,
  DA_BSEARCH_ROUND_DOWN,
  DA_BSEARCH_ROUND_UP };

template< class T >
class DynArray {
public:
  DynArray( unsigned long initial_size = DEFAULT_GROW_SIZE,
            unsigned long _grow_size   = DEFAULT_GROW_SIZE,
            unsigned long _shrink_size = DEFAULT_SHRINK_SIZE );
  ~DynArray();

  // Grow Control
  inline unsigned long GetGrowSize();                      // Get the Grow Size
  inline unsigned long SetGrowSize( unsigned long _grow ); // Set the Grow Size
  inline unsigned long Grow();                             // Grow immediatly
  inline unsigned long GrowBy(  unsigned long _grow );     // Grow by the given amount immediatly
         unsigned long SetSize( unsigned long _size );     // Set the size of the array.  Elements will be added or deleted as needed.

  // Shrink Control
  inline unsigned long GetShrinkSize();                        // Get the Shrink Size
  inline unsigned long SetShrinkSize( unsigned long _shrink ); // Set the Shrink Size
  inline unsigned long Shrink();                               // Shrink immediatly
  inline unsigned long ShrinkBy(  unsigned long _shrink );     // Shrink by the given amount immediatly

  // Initialize
  inline bool          FillWith( T *from, unsigned long count );

  // Accessors
  inline unsigned long NumElements();                      // Get the number of elements
  inline unsigned long MaxElements();                      // Get the largest valid index before the array has to grow again
  inline unsigned long NumFreeElements();                  // Get the number of elements beyond that aren't used (NumElements() - MaxNumElements())

  // Element Control
  unsigned long Add( T data );                             // Add a new element to the array.  Returns the index of the new number of elements
  unsigned long AddIfNotHere( T data );                    // Add a new elemnt to the array if it's not already there.
  unsigned long InsertAt( unsigned long index, T data );   // Insert a new element before another element.  Returns the new number of elements
  unsigned long InsertSorted( int (__cdecl *compare )(const void *elem1, const void *data ), T data );   // Insert a new element into the list at a point choosen by the compare function.  Returns the new number of elements

  bool          MoveTo( unsigned long dest, unsigned long index ); // Moves an element to the destination location provided.  Only really useful in sorted arrays
  bool          Swap(   unsigned long one,  unsigned long two   ); // Swaps the two indices provided.  Only really useful in sorted arrays
  void          Reverse();                                         // Reverses an array (first element becomes last and visa-versa)

  inline T    & GetAt( unsigned long index );              // Get the data at an index
  inline T    & SetAt( unsigned long index, T data );      // Set the data at an index

         unsigned long Remove( unsigned long index );      // Remove an element from the array. Returns the new number of elements
         unsigned long RemoveItem( T item );               // Remove element that matches item
         unsigned long RemoveHead();                       // Remove the first element. Returns the new number of elements
  inline unsigned long RemoveTail();                       // Remove the last element. Returns the new number of elements

  // Array Control
                  bool Reset( unsigned long initial_size = DEFAULT_GROW_SIZE );  // Clear the array entirely
                  bool Flush();                     // Does a reset, but also frees the memory used by all elements.  Be sure to call this ONLY on pointer-type elements!
  inline unsigned long PurgeExtra();                // Frees any memory not currently used (ie:  the difference between max_elements and num_elements)
         unsigned long PurgeNulls();                // Removes any elements equal to NULL.

  unsigned long InsertArrayAt( unsigned long index, DynArray< T > &other_array );// Insert another DynArray into this one before another element.  Returns the new number of elements

  // Search Routines
  unsigned long FindIndexOf( T find );              // Find the index of the element that matches data provided.
  unsigned long BinarySearch( int (__cdecl *compare )(const void *elem1, const void *data ), const void *data, DynArrayBinarySearchModes mode = DA_BSEARCH_EXACT );  // Binary search routine.  This assumes that the list is sorted in a low-to-high ordering.

  // Sort Routines
  void QSort( int (__cdecl *compare )(const void *elem1, const void *elem2 ) );  // Use qsort to sort the array with the provided function

  // Settings
  inline bool SetSorted( bool _sorted );            // Change between Sorted and Unsorted mode
  inline bool GetSorted();                          // Get the Sorted mode

  // Operator Overloads
  inline T &operator[]( unsigned long index );           // Get a reference to the data at an index
         DynArray & operator+=( DynArray &other_array ); // Append an array to the end of this one (reference)
  inline DynArray & operator+=( DynArray *other_array ); // Append an array to the end of this one (pointer)
         DynArray & operator=(  DynArray &other_array ); // Copy one array into another (reference)
  inline DynArray & operator=(  DynArray *other_array ); // Copy one array into another (pointer)

  // Array-Level
  inline T * GetArray() { return array; }           // Returns a pointer to the array itself.  NOTE:  THIS SHOULD BE CONSIDERED READ_ONLY

protected:
  unsigned long grow_size;                          // Current grow size
  unsigned long shrink_size;                        // Current shrink size
  unsigned long num_elements;                       // Number of elements currently used
  unsigned long max_elements;                       // Number of elements allocated.  Always >= the num_elements

  bool sorted_mode;                                 // true if the array is to be sorted

  T *array;                                         // The array itself
};


// ---
// --------------------------------------------------- Member Functions ---
// ---

// Constructor
template< class T > 
DynArray<T>::DynArray( unsigned long initial_size, unsigned long _grow_size, unsigned long _shrink_size )
  : grow_size(_grow_size), shrink_size(_shrink_size), num_elements(0), array(NULL), sorted_mode(true),
    max_elements(initial_size) {

  assert( max_elements > 0 );
  array = new T[ max_elements ];
}

// Destructor
template< class T > 
DynArray<T>::~DynArray() {
  delete array;
  array = NULL;
}

// GetGrowSize()
//  Get the current grow size.
template< class T > 
inline unsigned long DynArray<T>::GetGrowSize() {
  return grow_size;
}

// SetGrowSize()
//  Set the current grow size.  Returns the previous
//  grow size.
template< class T > 
inline unsigned long DynArray<T>::SetGrowSize( unsigned long _grow ) {
  unsigned long old_grow = grow_size;
  grow_size = _grow;
  return old_grow;
}

// Grow()
//  Grow the array immediatly.  Note that this requires enough memory
//  to allocate the new array that the old array will be copied into.
//  If the new array cannot be allocated, this function returns -1.
template< class T > 
inline unsigned long DynArray<T>::Grow() {
  return SetSize( max_elements + grow_size );
}

// GrowBy()
//  Grow the array by the given number of elements.  Returns
//  the new number of elements if successful, or -1 if the new
//  array cannot be allocated
template< class T > 
inline unsigned long DynArray<T>::GrowBy( unsigned long _grow ) {
  return SetSize( max_elements + _grow );
}

// SetSize()
//  Set the size of the array to the given number of elements.
//  The array size will be increased or decreased as nessesary,
//  and the array will be truncated if needed.  Returns -1 on error.
template< class T > 
unsigned long DynArray<T>::SetSize( unsigned long _size ) {
  T *new_array = new T[ _size ];                           // Allocate the new array

  if( !new_array )                                         // Make sure the new array was allocated
    return -1;

  // Move the old memory block to the new location (copy a
  //  total of _size or max_elements elements, whichever is smaller)
  if( _size < max_elements )
    memmove( new_array, array, sizeof(T) * _size);
  else 
    memmove( new_array, array, sizeof(T) * max_elements );

  delete[] array;                                          // Free the old array
  array = new_array;                                       // Change the array pointer to point to the new array

  max_elements = _size;                                    // Set the new size of the array (AFTER memmove!)

  if( num_elements > max_elements )                        // Truncate any unneeded elements if the array was made smaller
    num_elements = max_elements;

  return num_elements;
}

// GetShrinkSize()
//  Get the current shrink size.
template< class T > 
inline unsigned long DynArray<T>::GetShrinkSize() {
  return shrink_size;
}

// SetShrinkSize()
//  Set the current shrink size.  Returns the previous
//  shrink size.
template< class T > 
inline unsigned long DynArray<T>::SetShrinkSize( unsigned long _shrink ) {
  unsigned long old_shrink = shrink_size;
  shrink_size = _shrink;
  return old_shrink;
}

// Shrink()
//  Shrink the array immediatly.  Note that this requires enough memory
//  to allocate the new array that the old array will be copied into.
//  If the new array cannot be allocated, this function returns 0.
//  If the new array would be < 1 elements long, the array will not
//  shrink and the previous number of elements will be returned.
//  Elements beyond the end of the new array will be lost.
template< class T > 
inline unsigned long DynArray<T>::Shrink() {
  if( ((long)max_elements - (long)shrink_size) < 1 )
    return max_elements;

  return SetSize( max_elements - shrink_size );
}

// ShrinkBy()
//  Shrink the array by the given number of elements.  Returns
//  the new number of elements if successful, or -1 if the new
//  array cannot be allocated.  Elements beyond the end of the
//  new array will be lost
template< class T > 
inline unsigned long DynArray<T>::ShrinkBy( unsigned long _shrink ) {
  if( ((long)max_elements - (long)_shrink) < 1 )
    return max_elements;

  return SetSize( max_elements - _shrink );
}

// FillWith():
//  Clears the array and fills it with the array specified.  Returns
//   false if the from array is NULL, or count == 0.
template< class T >
inline bool DynArray<T>::FillWith( T *from, unsigned long count ) {
  if( from == NULL )
    return false;

  if( count == 0 )
    return false;

  Reset();
  SetSize( count );
  memcpy( array, from, (count * sizeof( T ) ) );

  return true;
}

// NumElements()
//  Get the number of elements currently used.
template< class T > 
inline unsigned long DynArray<T>::NumElements() {
  return num_elements;
}

// MaxElements()
//  Get the maximum number of elements that can be used without
//  growing the array.
template< class T > 
inline unsigned long DynArray<T>::MaxElements() {
  return max_elements;
}

// NumFreeElements()
//  Get the number of elements that have been allocated, but have
//  yet to be used.
template< class T > 
inline unsigned long DynArray<T>::NumFreeElements() {
  return( max_elements - num_elements );
}

// Add()
//  Add an element to the array.  The array will automatically
//  grow as needed.  Returns -1 if a nessesary grow operation
//  failed.  Returns the new number of elements if successful.
template< class T > 
unsigned long DynArray<T>::Add( T data ) {
  int ok = 1;
  if( num_elements == max_elements )
    ok = Grow() != -1;                                   // Grow the array, as needed

  if( !ok )
    return -1;

  array[ NumElements() ] = data;                         // Add the new element's data
  return ++num_elements;                                 // Increment the number of elements and return them
}

// AddIfNotHere()
//  Add an element to the array if it's not already contained.
//  The array will automatically grow as needed.  Returns 0
//  if a nessesary grow operation failed.  Returns the new
//  number of elements if successful.
template< class T >
unsigned long DynArray<T>::AddIfNotHere( T data ) {

    // find index of item
  unsigned long index = FindIndexOf( data );

  if( index != DA_NO_MATCH )
    return NumElements();

  return Add( data );
}


// InsertAt()
//  Inserts a new element in at the given index.  Any elements at or
//  after this index will be moved ahead as needed.  Returns the new
//  number of elements if successful, and -1 if a nessesary grow
//  operation fails.
template< class T > 
unsigned long DynArray<T>::InsertAt( unsigned long index, T data) {
  if( num_elements == index  ) {
    Add( data );
    return num_elements;
  }

  assert( index < num_elements );
  if( num_elements == max_elements ) {
    if( Grow() == -1 )
      return -1;
  }

  memmove( &(array[ index+1 ]), &(array[ index ]), sizeof(T) * ( num_elements-index ) );
  array[ index ] = data;
  return ++num_elements;
}

// InsertArrayAt():
//  Insert another DynArray into this one before another element.
//   Returns the new number of elements
template< class T > 
unsigned long DynArray<T>::InsertArrayAt( unsigned long index, DynArray< T > &other_array ) {
  if( max_elements < num_elements + other_array.NumElements() ) {
    if( SetSize( num_elements + other_array.NumElements() ) == -1 )
      return -1;
  }
  if( num_elements == index ) {
    memcpy( &(array[ index ]), other_array.GetArray(), sizeof(T) * other_array.NumElements() );
  } else {
    memmove( &(array[ index + other_array.NumElements() ]), &(array[ index ]), sizeof(T) * ( num_elements-index ) );
    memcpy( &(array[ index ]), other_array.GetArray(), sizeof(T) * ( other_array.NumElements() ) );
  }

  num_elements += other_array.NumElements();
  return num_elements;
}

// InsertSorted()
//  Inserts a new element in at the given index.  This uses BinarySearch()
//   to find the best place to insert the data using the compare function
//   to test, and then calls InsertAt() to insert the data.  Returns -1
//   if InsertAt fails or there is already an element that exactly matches
//   the compare function.
template< class T > 
unsigned long DynArray<T>::InsertSorted( int (__cdecl *compare )(const void *elem1, const void *data ), T data ) {
  unsigned long i = BinarySearch( compare, (const void *)data, DA_BSEARCH_ROUND_UP );

  // No match; must be at the head of the list
  if( i == DA_NO_MATCH )
    return InsertAt( num_elements, data );

  // Exact match; fail
  if( compare( (const void *)array[i], (const void *)data ) == 0 )
    return -1;

  // Some other point in list
  return InsertAt( i, data );
}

// MoveTo():
//  Moves an element from it's current index to the destination location index.
//   Only really useful in sorted arrays.  In unsorted arrays, the Swap function
//   is called instead for speed.
//  This _SHOULD_ work with most data types.  You may have problems
//   with classes whose destructor deletes data that is used by other
//   functions/instances.  If this is the case, you may want to write
//   a custom move function.
template< class T > 
bool DynArray<T>::MoveTo( unsigned long dest, unsigned long index ) {
  assert( index < num_elements );
  assert( dest  < num_elements );

  if( index == dest )      // Indices are the same; return OK
    return true;

  if( sorted_mode ) {
    T temp;
    memcpy( &temp, &(array[ index ]), sizeof( T ) );

    if( index > dest )
      memmove( &(array[dest + 1]), &(array[dest]), sizeof( T ) * (index - dest) );
    else
      memmove( &(array[index]), &(array[index+1]), sizeof( T ) * (dest - index) );

    memcpy( &(array[ dest ]), &temp, sizeof( T ) );

    return true;
  } else
    return Swap( index, dest );
}

// Swap():
//  Swaps the two indices provided.  Only really useful in sorted arrays,
//   but you can use it without trouble on unsorted arrays.
//  This _SHOULD_ work with most data types.  You may have problems
//   with classes whose destructor deletes data that is used by other
//   functions/instances.  If this is the case, you may want to write
//   a custom swap function.
template< class T > 
bool DynArray<T>::Swap( unsigned long one, unsigned long two ) {
  assert( one < num_elements );
  assert( two < num_elements );

  if( one == two )      // Indices are the same; return OK
    return true;

  T swap;
  memcpy( &swap,         &(array[one]), sizeof( T ) );
  memcpy( &(array[one]), &(array[two]), sizeof( T ) );
  memcpy( &(array[two]), &swap,         sizeof( T ) );

  return true;
};

// Reverse():
//  Reverses the array.  The first element becomes the last and visa-versa.
template< class T > 
void DynArray<T>::Reverse() {
  if( num_elements < 2 )
    return;

  unsigned long total = num_elements/2;
  for( unsigned long i=0; i < total; i++ )
    Swap( i, num_elements - i - 1 );
}

// GetAt()
//  Returns a reference to the data at the given index in the
//  array.
template< class T >
inline T & DynArray<T>::GetAt( unsigned long index ) {
  assert( index < num_elements );
  return array[index];
}

// SetAt()
//  Set the data at the given index.  Returns a reference to that index
//  in the array.
template< class T >
inline T & DynArray<T>::SetAt( unsigned long index, T data ) {
  assert( index < num_elements );
  array[index] = data;
  return array[index];
}

// Remove()
//  Removes the element at the given index.  If in Sorted mode,
//  the array is shifted to fill in the gap.  If in Unsorted mode,
//  the last element of the array is moved to the deleted position
//  and the number of elements is decremented.
template< class T >
unsigned long DynArray<T>::Remove( unsigned long index ) {
  assert( index < num_elements );
  if( sorted_mode ) {
    memmove( &( array[index] ), &( array[index+1] ),
             sizeof(T) * (num_elements - index - 1) );
  } else {
    array[index] = array[num_elements-1];
  }

  --num_elements;

  // See if we should shrink the array
  int ok = true;
  if( (long)num_elements < (long)max_elements - (long)shrink_size )
    ok = Shrink() != -1;

  if( !ok )
    return 0;

  return num_elements;
}

// RemoveItem()
// Removes the elements that matches item. If in Sorted mode,
//  the array is shifted to fill in the gap.  If in Unsorted mode,
//  the last element of the array is moved to the deleted position
//  and the number of elements is decremented.
template< class T >
unsigned long DynArray<T>::RemoveItem( T item ) {

  // find index of item
  unsigned long index = FindIndexOf( item );

  if( index == DA_NO_MATCH )
    return NumElements();

  // now remove it
  return Remove( index );
}


// RemoveHead()
//  Removes the first element from the array.  Note that this
//  is a bit slow, as it has to move the entire array back one
//  element.
template< class T >
inline unsigned long DynArray<T>::RemoveHead() {
//  memmove( array, array + sizeof(T), (--num_elements) * sizeof(T) );
  memmove( &(array[0]), &(array[1]), (--num_elements) * sizeof(array[0]) );

  // See if we should shrink the array
  int ok = true;
  if( (long)num_elements < (long)max_elements - (long)shrink_size )
    ok = Shrink() != -1;

  if( !ok )
    return 0;

  return num_elements;
}

// RemoveTail()
//  Removes the last element from the array.  Basically, it
//  just decrements the number of elements.  Returns the new
//  number of elements.
template< class T >
inline unsigned long DynArray<T>::RemoveTail() {
  --num_elements;

  // See if we should shrink the array
  int ok = true;
  if( num_elements < (unsigned long)((long)max_elements - (long)shrink_size) )
    ok = Shrink() != -1;

  if( !ok )
    return 0;

  return num_elements;
}

// Reset()
//  Clear out the array entirely, effectively re-initializing the
//  class.  If this returns false, the new array could not be
//  created.  The class should not be used until the array is
//  re-created by calling Reset() with a smaller initial size.
template< class T >
bool DynArray<T>::Reset( unsigned long initial_size ) {
  delete[] array;
  num_elements = 0;

  array = new T[initial_size];
  if( !array ) {
    max_elements = 0;
    return false;
  }
  
  max_elements = initial_size;

  return true;
}

// Flush()
//  Delete the elements within the array.  This should only be called
//  when the array contains pointer types.
template< class T >
bool DynArray<T>::Flush() {
  for( unsigned long i=0; i < num_elements; i++ ) {
    //  NOTE:  If you get an error here, it's probably because you're
    //         trying to use this on a DynArray that doesn't contain
    //         pointers.  That won't work.  Use Reset() instead.
    delete array[i];
  }

  Reset();

  return true;
}

// PurgeExtra()
//  Free any extra memory that is allocated, but is not currently used.
//  Returns the new number of elements.
template< class T >
inline unsigned long DynArray<T>::PurgeExtra() {
  return SetSize( num_elements );
}

// PurgeNulls()
//  Eliminate any NULL pointers in the array.  This should only be
//  called on arrays contianing pointer types.  This should be notably
//  faster in unsorted mode than sorted mode when there are a large
//  number of nulls in the array.  Returns the new number of elements.
template< class T >
inline unsigned long DynArray<T>::PurgeNulls() {
  for( unsigned long i=0; i < num_elements; i++ ) {
    if( array[i] == NULL )
      Remove(i--);                                // Decrement it so we can test the new i for NULL
  }

  // See if we should shrink the array
  int ok = true;
  while( (long)num_elements < (long)max_elements - (long)shrink_size ) {
    ok = Shrink() != -1;

    if( !ok )
      return 0;
  }
 
  return num_elements;
}

// FindIndexOf()
//  Finds the index that matches the data provided.  Returns
//  DA_NO_MATCH if the data is not in the array.
template< class T >
unsigned long DynArray<T>::FindIndexOf( T find ) {
  for( unsigned long  i=0; i < num_elements; i++ ) {
    if( array[i] == find )
      return i;
  }

  return DA_NO_MATCH;
}

// BinarySearch():
//  Performs a binary search on the array using the compare function provided.
//   Returns the index of the item if found, or DA_NO_MATCH if it's not here.
//  The compare function takes two arguments: the item being tested, and
//   the data being searched for.  It should return 0 if a match is found, 1
//   if the match is greater than the data, and -1 if the match is less than
//   the data.  For example, if data was a long:
//
//     if( (long)data == elem1->value )
//       return 0;
//     return (long)data < elem1->value ? -1 : 1;
//
//  mode determines how to match.  This can be an exact match, nearest
//   match, round down match or round up match.
//  This routine assumes that the list is sorted in a low-to-high ordering
//   based on the data being searched on.
template< class T >
unsigned long DynArray<T>::BinarySearch( int (__cdecl *compare )(const void *elem1, const void *data ), const void *data, DynArrayBinarySearchModes mode ) {
  if( num_elements == 0 )
    return DA_NO_MATCH;

  // Pre-Process: handle data past the ends of the array
  if( mode == DA_BSEARCH_EXACT ) {                  // Exact Match
    if( compare( (void *)array[0], data ) < 0 )
      return DA_NO_MATCH;
    else if( compare( (void *)array[ num_elements - 1 ], data ) > 0 )
      return DA_NO_MATCH;

  } else if( mode == DA_BSEARCH_NEAREST ) {         // Nearest Match
    if( compare( (void *)array[ num_elements - 1 ], data ) <= 0 )
      return num_elements - 1;
    else if( compare( (void *)array[0], data ) >= 0 )
      return 0;

  } else if( mode == DA_BSEARCH_ROUND_DOWN ) {      // Round Down
    if( compare( (void *)array[ num_elements - 1 ], data ) >= 0 )
      return num_elements - 1;
    else if( compare( (void *)array[0], data ) < 0 )
      return DA_NO_MATCH;

  } else if( mode == DA_BSEARCH_ROUND_UP ) {        // Round Up
    if( compare( (void *)array[ num_elements - 1 ], data ) > 0 )
      return DA_NO_MATCH;
    else if( compare( (void *)array[0], data ) <= 0 )
      return 0;
  }

  // Search
  long a = 0;                       // Start of Search Range
  long x = (long)num_elements - 1;  // End of Search Range
  long i = x;                       // Current being we're tested
  int match;

  while( true ) {
    if( x < a )
      break;

    i = (a + x) / 2;

    match = compare( (void *)array[i], data );
    if( match == 0 )
      return i;

    if( match > 0 )
      a = i + 1;
    else
      x = i - 1;
  }

/*
  do {
    i=a+( (x-a)/2 );
    match = compare( (void *)array[i], data );
    if( match == 0 )
      return i;

    if( match > 0 )
      x = i - 1;
    else
      a = i + 1;

    if( (x < a) || ((long)x < 0) )
      break;
  } while( true );
*/

  // Post Process
  if( mode == DA_BSEARCH_EXACT ) {               // Exact Match
    return DA_NO_MATCH;

  } else if( mode == DA_BSEARCH_NEAREST ) {      // Nearest Match
    return i;

  } else if( mode == DA_BSEARCH_ROUND_UP ) {     // Round Up
    if( compare( (void *)array[i], data ) > 0 )
      return i+1;
    return i;

  } else if( mode == DA_BSEARCH_ROUND_DOWN ) {   // Round Down
    if( compare( (void *)array[i], data ) < 0 )
      return i-1;
    return i;
  }

  // This line should never be reached
  assert( false );
  return DA_NO_MATCH;
}

// SetSorted()
//  Set the sorted mode.  true means sorted; false means unsorted.
//  Returns the previous mode.
template< class T >
inline bool DynArray<T>::SetSorted( bool _sorted ) {
  bool old = _sorted;
  sorted_mode = _sorted;
  return old;
}

// GetSorted()
//  Returns the current sorted state.
template< class T >
inline bool DynArray<T>::GetSorted() {
  return sorted_mode;
}

// operator[]
//  Returns a reference to the data at the given index of the
//  array.  When using this operand, treat it just like a normal,
//  static-sized array.  This will assert if the array passed
//  in is greater than the number of elements in the array.
template< class T >
inline T & DynArray<T>::operator[]( unsigned long index ) {
  assert( index < num_elements );
  return array[index];
}

// operator+=
//  Add the other array to this one.  Using this is faster
//   than doing a for loop.  The max size of the array is
//   increased to the new number of elements plus the grow
//   size.
template< class T >
DynArray<T> & DynArray<T>::operator+=( DynArray &other_array ) {
  // Check this!  It hasn't been tested at all!
  SetSize( num_elements + other_array.num_elements + grow_size );
  num_elements += other_array.num_elements;
  
  memcpy( &( array[ num_elements ] ), other_array.array, other_array.num_elements * sizeof( T ) );

  return *this;
}

template< class T >
inline DynArray<T> & DynArray<T>::operator+=( DynArray *other_array ) {
  return( *this += *other_array );
}


// operator=
//  Copy the other array into this one.
template< class T >
DynArray<T> & DynArray<T>::operator=( DynArray &other_array ) {
  grow_size    = other_array.grow_size;
  num_elements = other_array.num_elements;
  max_elements = other_array.max_elements;

  sorted_mode = other_array.sorted_mode;

  delete[] array;
  array = new T[ max_elements ];

  memcpy( array, other_array.array, sizeof(T) * num_elements );

  return *this;
}

template< class T >
inline DynArray<T> & DynArray<T>::operator=( DynArray *other_array ) {
  return( *this = *other_array );
}

// QSort():
//  Use qsort to sort the array with the provided function.  See
//   the C-standard qsort() function for more information.
template< class T >
void DynArray<T>::QSort( int (__cdecl *compare )(const void *arg1, const void *arg2 ) ) {
  qsort( array, num_elements, sizeof( T ), compare );
}


#endif
