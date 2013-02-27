//
// HashTable.h
//
// This is a simple has table class.  The table is an array of
//  linked lists, with the hashing function determining what
//  elements go in which linked list.
//

#ifndef HASH_TABLE_HEADER
#define HASH_TABLE_HEADER

#include <portable/dynarray/dynarray.h>
#include <portable/linkedlist/LinkedList.h>

typedef unsigned long HashTable_HashFunction( const void *data, unsigned long table_size, void *user_data );
typedef bool          HashTable_FindFunction( const void *data, const void *compare, void *user_data );

struct HashStats {
  unsigned long table_size;             // Number of keys/elements in the array
  unsigned long empty_keys;             // Number of keys/elements without any data
  unsigned long total_entries;          // Includes multiple entries at the same key
  unsigned long total_conflicts;        // Total number of conflicts in all keys
  unsigned long total_keys_conflicted;  // Total number of keys with conflicts
  unsigned long min_conflicts;          // Minimum number of conflicts at a key
  unsigned long max_conflicts;          // Maximum number of conflicts at a key
  float         average_conflicts;      // Average number of conflicts per key
};

template< class T >
class HashTable {
public:
  HashTable( unsigned long _size, HashTable_HashFunction *_hash, HashTable_FindFunction *_find = NULL, void *_user_data = NULL );
  ~HashTable();

  // Management
  inline bool Add( T data );                   // Add a new item to the table.  Returns false if the data already exists
  inline bool Remove( T data );                // Removes an item from the table.  Retrusn false if the data isn't found

  inline bool              Find( void *data, HashTable_HashFunction *search_hash = NULL, HashTable_FindFunction *search_find = NULL );  // Find a particular item.  Returns true if the item is found.  Use GetMatch() to get that item.  Alternate hash and find functions can be used, although they should has the same way that the core functions do
  inline T                 GetLastMatch();     // Get the last match
  inline LinkedList< T > * GetLastMatchBin();  // Get the bin the last match was in

  inline void Reset();                         // Clear the entire hash table
  inline void Flush();                         // Clear the eniter hash table, deleting the data within.  Should only be used on pointer types

  inline void AddToDynArray( DynArray< T > &array );  // Add all the elements in the hash table to a DynArray

  inline void Stats( HashStats &stats );
  inline void Stats( HashStats *stats ) { Stats *stats; }

protected:
  LinkedList< T >        * table;          // The table itself
  unsigned long            table_size;     // Size of the array of linked lists

  HashTable_HashFunction * hash_function;  // Hash function to decide which linked list in the table to search
  HashTable_FindFunction * find_function;  // Used to find an entry within a linked list.  Can be NULL.
  void                   * user_data;      // User data passed to te hash and find functions

  T                        last_match;     // The last match found
  unsigned long            last_match_bin; // Bin that the last match was found in
};

// Constructor
template< class T > 
HashTable<T>::HashTable( unsigned long _size, HashTable_HashFunction *_hash, HashTable_FindFunction *_find, void *_user_data )
  : table_size(_size), hash_function(_hash), find_function(_find), last_match((T)NULL), last_match_bin(-1), user_data(_user_data) {

  assert( table_size > 0 );
  table = new LinkedList< T >[ table_size ];
}

// Destructor
template< class T > 
HashTable<T>::~HashTable() {
  delete[] table;
  table = NULL;
}

// Add():
template< class T >
bool HashTable<T>::Add( T data ) {
  if( Find( data ) )
    return false;

  unsigned long index = hash_function( (void *)data, table_size, user_data );
  assert( index < table_size );

  table[ index ].InsertAsFirst( data );
  return true;
}

// Remove():
template< class T >
bool HashTable<T>::Remove( T data ) {
  if( !Find( data ) )
    return false;

  unsigned long index = hash_function( (void *)data, table_size, user_data );
  for( table[ index ].GotoFirst(); table[ index ].WasLastGotoOK(); table[ index ].GotoNext() ) {
    if( find_function == NULL ) {                                                       // No find function; just compare the data directly
      if( data == table[ index ]->GetCurrentData() ) {
        table[ index ].RemoveCurrent();
        return true;
      }
    } else if( find_function( data, table[ index ]->GetCurrentData(), user_data ) ) {   // Comapre the data with the find function
      table[ index ].RemoveCurrent();
      return true;
    }
  }

  return false;
}

// Find():
template< class T >
bool HashTable<T>::Find( void *data, HashTable_HashFunction *search_hash, HashTable_FindFunction *search_find ) {
  if( search_hash == NULL )
    search_hash = hash_function;

  if( search_find == NULL )
    search_find = find_function;

  unsigned long index = search_hash( (void *)data, table_size, user_data );
  assert( index < table_size );

  last_match = (T)NULL;
  last_match_bin = -1;

  if( index == -1 )
    return false;

  for( table[ index ].GotoFirst(); table[ index ].WasLastGotoOK(); table[ index ].GotoNext() ) {
    if( search_find == NULL ) {                                                       // No find function; just compare the data directly
      if( data == table[ index ].GetCurrentData() ) {
        last_match = table[ index ].GetCurrentData();
        last_match_bin = index;
        return true;
      }
    } else if( search_find( data, table[ index ].GetCurrentData(), user_data ) ) {    // Comapre the data with the find function
      last_match = table[ index ].GetCurrentData();
      last_match_bin = index;
      return true;
    }
  }

  return false;
}

// GetLastMatch():
template< class T >
T HashTable<T>::GetLastMatch() {
  return last_match;
}

// GetLastMatchBin():
template< class T >
LinkedList< T > * HashTable<T>::GetLastMatchBin() {
  if( last_match_bin == -1 )
    return NULL;

  return &(table[ last_match_bin ]);
}

// Reset():
template< class T >
inline void HashTable<T>::Reset() {
  for( unsigned long i=0; i < table_size; i++ )
    table[i].Reset();
}

// Flush():
template< class T >
inline void HashTable<T>::Flush() {
  for( unsigned long i=0; i < table_size; i++ )
    table[i].Flush();
}


// AddToDynArray():
template< class T >
inline void HashTable<T>::AddToDynArray( DynArray< T > &array ) {
  for( unsigned long i=0; i < table_size; i++ ) {
    for( table[i].GotoFirst(); table[i].WasLastGotoOK(); table[i].GotoNext() )
      array.Add( table[i].GetCurrentData() );
  }
}

// Stats():
template< class T >
inline void HashTable<T>::Stats( HashStats &stats ) {
  memset( &stats, 0, sizeof( stats ) );
  stats.table_size = table_size;
  stats.empty_keys = table_size;
  stats.min_conflicts = -1;

  unsigned long key_count = 0;
  for( unsigned long i=0; i < table_size; i++ ) {
    key_count = table[i].Count();
    if( key_count > 0 ) {
      stats.empty_keys--;
      stats.total_entries += key_count;

      if( key_count > 1 ) {
        stats.total_keys_conflicted++;
        key_count--;
        stats.total_conflicts += key_count;

        if( key_count < stats.min_conflicts )
          stats.min_conflicts = key_count;

        if( key_count > stats.max_conflicts )
          stats.max_conflicts = key_count;
      }
    }
  }

  if( stats.min_conflicts == -1 )
    stats.min_conflicts = 0;

  stats.average_conflicts = (float)stats.total_conflicts / (float)stats.table_size;
}

#endif

