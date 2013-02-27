//
// LinkedList.h
//

#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

// LinkedListLink
template< class T >
class LinkedListLink {
public:
  LinkedListLink( T _data, LinkedListLink< T > *_next = NULL, LinkedListLink< T > *_prev = NULL ) : data(_data), next(_next), prev(_prev) { ; }
  ~LinkedListLink() { ; }

  // Accessors():
  inline T                     GetData()                             { return data;  }
  inline void                  SetData( T _data )                    { data = _data; }

  inline LinkedListLink< T > * GetPrev()                             { return prev;  }
  inline void                  SetPrev( LinkedListLink< T > *_prev ) { prev = _prev; }

  inline LinkedListLink< T > * GetNext()                             { return next;  }
  inline void                  SetNext( LinkedListLink< T > *_next ) { next = _next; }

protected:
  T data;                       // Data this link points to
  LinkedListLink< T > *prev;    // Previous link
  LinkedListLink< T > *next;    // Next link
};

//
// --------------------------------------------------------------------------------
//

// LinkedList
template< class T >
class LinkedList {
public:
  LinkedList() : first(NULL), current(NULL), was_last_goto_ok(false) { ; }
  ~LinkedList() { Reset(); }

  // Management
  inline unsigned long Count();

  inline void Reset();               // Resets the list, but doesn't destroy the data in the links
  inline void Flush();               // Resets the list, destroying the data in the links.  Should be called only on pointer-type items.

  inline void PurgeNulls();          // Remove all links pointing to NULL data.

  // Accessors
  inline T    GetCurrentData();         // Get the data at the current link.  Returns (T)NULL if current is invalid
  inline void SetCurrentData( T data ); // Change the current link's data

  inline bool GotoFirst();           // Set current to first, and get the data at that link.  Returns false if there aren't any links
  inline bool GotoPrev();            // Set current to prev, and get the data at that link.  Returns false if there aren't any links before this item
  inline bool GotoNext();            // Set current to next, and get the data at that link.  Retrusn false if there aren't any links after this item
  inline bool WasLastGotoOK();       // Returns true if the last goto command was OK

  inline bool IsCurrentValid();      // Returns true if the current link isn't NULL (at least one link in list)
  inline bool IsPrevValid();         // Returns true if the previous link isn't NULL (at least one link before this one)
  inline bool IsNextValid();         // Returns true if the next link isn't NULL (at least one link after this one)
  inline bool HasAnyLinks();         // Retrusn true if first isn't NULL

  // Insertion/Removal
  inline T    RemoveCurrent();       // Removes the current link from the list.  The next link becomes the current one
  inline T    DeleteCurrent();       // Removes the current link from the list and deletes the data within

  inline T    InsertAfterCurrent( T data );  // Insert after the current item
  inline T    InsertBeforeCurrent( T data ); // Insert after the current item
  inline T    InsertAsFirst( T data );       // Inserts at the head of the list

  inline T    SwapWithNext();        // Swap the current and next links in the list
  inline T    SwapWithPrev();        // Swap the current and prev links in the list

  // Search
  inline bool FindFirst( T data );   // Sets current to the first link with this data (start from first link); returns false if no match is found
  inline bool FindNext( T data );    // Sets current to the next link with this data (start from link after current); returns false if no match is found

  inline void AddAndResetSource( LinkedList< T > &other_list );  // Adds the other list to this one, clearing the other list afterword

protected:
  inline LinkedListLink< T > *GetFirst() { return first; }
  inline void                 SoftReset();

  LinkedListLink< T > *first;             // First link in the list
  LinkedListLink< T > *current;           // Current link in the list

  bool                 was_last_goto_ok;  // True if the last goto was valid
};

// Count()
template< class T >
inline unsigned long LinkedList< T >::Count() {
  unsigned long count = 0;
  for( LinkedListLink< T > *link = first; link != NULL; link = link->GetNext() )
    count++;

  return count;
}

// Reset():
template< class T >
inline void LinkedList< T >::Reset() {
  LinkedListLink< T > *next;
  for( LinkedListLink< T > *link = first; link != NULL; link = next ) {
    next = link->GetNext();
    delete link;
  }

  SoftReset();
}

// Flush():
template< class T >
inline void LinkedList< T >::Flush() {
  LinkedListLink< T > *next;
  for( LinkedListLink< T > *link = first; link != NULL; link = next ) {
    next = link->GetNext();

    // If this crashes, you probably tried to flush something that isn't a pointer type
    if( link->GetData() != NULL )
      delete link->GetData();

    delete link;
  }

  SoftReset();
}

// SoftReset():
template< class T >
inline void LinkedList< T >::SoftReset( void ) {
  first   = NULL;
  current = NULL;
  was_last_goto_ok = false;
}

// GetCurrentData():
template< class T >
inline T LinkedList< T >::GetCurrentData() {
  return ((current == NULL) ? (T)NULL : current->GetData());
}

// SetCurrentData():
template< class T >
inline void LinkedList< T >::SetCurrentData( T data ) {
  if( current == NULL )
    return (T)NULL;

  current->SetData( data );
}

// GotoFirst():
template< class T >
inline bool LinkedList< T >::GotoFirst() {
  current = first;
  was_last_goto_ok = (current != NULL);
  return was_last_goto_ok;
}

// GotoPrev():
template< class T >
inline bool LinkedList< T >::GotoPrev() {
  if( current == NULL ) {
    was_last_goto_ok = false;
    return false;
  }
   
  current = current->GetPrev();
  if( current == NULL ) {
    was_last_goto_ok = false;
    return false;
  }

  was_last_goto_ok = true;
  return true;
}

// GotoNext():
template< class T >
inline bool LinkedList< T >::GotoNext() {
  if( current == NULL ) {
    was_last_goto_ok = false;
    return false;
  }
   
  current = current->GetNext();
  if( current == NULL ) {
    was_last_goto_ok = false;
    return false;
  }

  was_last_goto_ok = true;
  return true;
}

// WasLastGotoOK():
template< class T >
inline bool LinkedList< T >::WasLastGotoOK() {
  return was_last_goto_ok;
}

// IsCurrentValid():
template< class T >
inline bool LinkedList< T >::IsCurrentValid() {
  return ((current == NULL) ? false : true);
}

// IsPrevValid():
template< class T >
inline bool LinkedList< T >::IsPrevValid() {
  if( current == NULL )
    return false;

  return ((current->GetPrev() == NULL) ? false : true);
}

// IsNextValid():
template< class T >
inline bool LinkedList< T >::IsNextValid() {
  if( current == NULL )
    return false;

  return ((current->GetNext() == NULL) ? false : true);
}

// HasAnyLinks():
template< class T >
inline bool LinkedList< T >::HasAnyLinks() {
  return ((first == NULL) ? false : true);
}

// RemoveCurrent():
template< class T >
inline T LinkedList< T >::RemoveCurrent() {
  if( current == NULL )
    return (T)NULL;

  LinkedListLink< T > * prev = current->GetPrev();
  LinkedListLink< T > * next = current->GetNext();

  if( first == current )
    first = next;

  delete current;
  current = NULL;

  if( prev != NULL )
    prev->SetNext( next );

  if( next != NULL ) {
    current = next;
    next->SetPrev( prev );
  }

  return ((current == NULL) ? NULL : current->GetData());
}

// DeleteCurrent():
template< class T >
inline T LinkedList< T >::DeleteCurrent() {
  if( current == NULL )
    return (T)NULL;

  // If this crashes; data wasn't a pointer, so you shouldn't have tried it.
  delete current->GetData();
  return RemoveCurrent();
}

// InsertAfterCurrent():
template< class T >
inline T LinkedList< T >::InsertAfterCurrent( T data ) {
  LinkedListLink< T > *link = new LinkedListLink< T >( data );

  if( current != NULL ) {
    link->SetPrev( current );
    link->SetNext( current->GetNext() );

    if( current->GetNext() != NULL )
      current->GetNext()->SetPrev( link );

    current->SetNext( link );
  }

  if( first == current )
    first = link;

  current = link;
  return data;
}

// InsertBeforeCurrent():
template< class T >
inline T LinkedList< T >::InsertBeforeCurrent( T data ) {
  LinkedListLink< T > *link = new LinkedListLink< T >( data );

  if( current != NULL ) {
    link->SetPrev( current->GetPrev() );
    link->SetNext( current );

    if( current->GetPrev() != NULL )
      current->GetPrev()->SetNext( link );

    current->SetPrev( link );
  }

  if( first == current )
    first = link;

  current = link;
  return data;
}

// InsertAsFirst():
template< class T >
inline T LinkedList< T >::InsertAsFirst( T data ) {
  LinkedListLink< T > *link = new LinkedListLink< T >( data );

  if( first != NULL ) {
    first->SetPrev( link );
    link->SetNext( first );
  }

  first = link;
  return data;
}

// SwapWithNext():
template< class T >
inline T LinkedList< T >::SwapWithNext() {
  if( current == NULL )
    return (T)NULL;

  if( current->GetNext() == NULL )
    return current->GetData();

  T temp_data = current->GetData();
  current->SetData( current->GetNext()->GetData() );
  current->GetNext()->SetData( temp_data );
}

// SwapWithPrev():
template< class T >
inline T LinkedList< T >::SwapWithPrev() {
  if( current == NULL )
    return (T)NULL;

  if( current->GetPrev() == NULL )
    return current->GetData();

  T temp_data = current->GetData();
  current->SetData( current->GetPrev()->GetData() );
  current->GetPrev()->SetData( temp_data );
}

// FindFirst():
template< class T >
inline bool LinkedList< T >::FindFirst( T data ) {
  for( LinkedListLink< T > *link = first; link != NULL; link = link->GetNext() ) {
    if( link->GetCurrentData() == data ) {
      current = link;
      return true;
    }
  }

  return false;
}

// FindNext():
template< class T >
inline bool LinkedList< T >::FindNext( T data ) {
  for( LinkedListLink< T > *link = current->GetNext(); link != NULL; link = link->GetNext() ) {
    if( link->GetCurrentData() == data ) {
      current = link;
      return true;
    }
  }

  return false;
}

// AddAndResetSource():
template< class T >
inline void LinkedList< T >::AddAndResetSource( LinkedList< T > &other_list ) {
  if( other_list.GetFirst() == NULL )
    return;

  LinkedListLink< T > *link;
  if( first == NULL ) {
    first = other_list.GetFirst();
  } else {
    for( link = first; link->GetNext() != NULL; link = link->GetNext() ) { ; }

    link->SetNext( other_list.GetFirst() );
    other_list.GetFirst()->SetPrev( link );
  }

  other_list.SoftReset();
}

#endif

