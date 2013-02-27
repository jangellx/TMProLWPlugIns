//
// QuadTree.h
//
//  The quadtree node contains one of two things:  sub-nodes, or data in a bin.
//   If the nodes are are non-NULL, the bin must be empty and visa-versa.
//   When new data is added to the tree, it goes into the bin.  If the bin
//   is full, new sub-nodes area created and filled with the contents of the bin,
//   which is then reset.
//
//  Note that a Flush() operation is impractical, since the same data can be in
//   multiple nodes/bins
//

#ifndef QUAD_TREE_HEADER
#define QUAD_TREE_HEADER

#include <portable/dynarray/dynarray.h>

#include <fstream.h>
#include <iomanip.h>

typedef bool QuadTree_AddFunction( const void *data, float left, float right, float top, float bottom );
typedef void QuadTree_AddFastFunction( const void *data, float left, float right, float top, float bottom, bool *quads );
typedef bool QuadTree_FindFunction( const void *data, const void *compare, float left, float right, float top, float bottom, void *user_data );
typedef void QuadTree_FindFastFunction( const void *data, float left, float right, float top, float bottom, bool *quads, void *user_data );

// QuadTree
template< class T >
class QuadTree {
public:
  QuadTree( float _left, float _right, float _top, float _bottom, QuadTree_AddFunction * _add_func, unsigned long _max_per_bin = 10, unsigned long _max_depth = 6 );
  ~QuadTree();

  void Reset();
  void Reset( float _left, float _right, float _top, float _bottom );

  void Add( T data );                                           // Add new data to the tree
  void AddFast( T data, QuadTree_AddFastFunction * add_fast );  // Add new data to the tree
  void Remove( T data );                                        // Remove data from the tree.

  void AddToArray( DynArray< T > &array, void *compare, QuadTree_FindFunction *find_func = NULL);                   // Build a list of all data matching the test case Add new data to the tree

  void BuildNodeList( DynArray< QuadTree<T> * > &array );                                                           // Build a list of all nodes in the tree
  void BuildLeafNodeList( DynArray< QuadTree<T> * > &array );                                                       // Build a list of all nodes with no sub-nodes in the tree
  void BuildNodeListWith( void *data, DynArray< QuadTree<T> * > &array, QuadTree_AddFunction *_test_func = NULL );  // Build a list of all nodes in the tree that contain the data.  Similar to Add(), but returns a list of matching nodes instead of adding the data to those nodes
  void BuildNodeListWithFast( void *data, DynArray< QuadTree<T> * > &array, QuadTree_AddFastFunction *_test_func ); // Build a list of all nodes in the tree that contain the data.  Similar to Add(), but returns a list of matching nodes instead of adding the data to those nodes

  void OutputToStream( ostream &out, int indent = 0 );                                                              // Output the tree to a stream

  // Accessors
  inline QuadTree< T >  ** GetNodes();        // Get the node list
  inline DynArray< T >   & GetBin();          // Get the data bin

  inline float             GetLeft()      { return left;   }
  inline float             GetRight()     { return right;  }
  inline float             GetTop()       { return top;    }
  inline float             GetBottom()    { return bottom; }

  inline unsigned long     GetMaxPerBin() { return max_per_bin; }
  inline unsigned long     GetMaxDepth()  { return max_depth; }

protected:
  QuadTree< T >         * nodes[4];           // The four nodes of the tree, if bin is empty
  DynArray< T >           bin;                // Data in this quadrant, if all nodes are all NULL

  float                   left;               // Extents of this node
  float                   right;
  float                   top;
  float                   bottom;

  unsigned long           max_per_bin;        // Max entries per bin; after hitting this number, the bin will be subdivided into nodes.
  unsigned long           max_depth;          // Maximum tree depth below this node.  0 means no sub nodes are allowed

  QuadTree_AddFunction  * add_function;       // Add callback function
};

// Constructor
template< class T >
QuadTree< T >::QuadTree( float _left, float _right, float _top, float _bottom, QuadTree_AddFunction *_add_func, unsigned long _max_per_bin, unsigned long _max_depth )
  : left(_left), right(_right), top(_top), bottom(_bottom), max_per_bin(_max_per_bin), max_depth(_max_depth),
    add_function(_add_func), bin(_max_per_bin + 1, DEFAULT_GROW_SIZE, DEFAULT_SHRINK_SIZE, false) {

  nodes[0] = nodes[1] = nodes[2] = nodes[3] = NULL;
}

// Destructor
template< class T >
QuadTree< T >::~QuadTree() {
  Reset();
}

// Reset():
template< class T >
inline void QuadTree< T >::Reset() {
  for( int i=0; i < 4; i++ ) {
    if( nodes[i] != NULL ) {
      delete nodes[i];
      nodes[i] = NULL;
    }
  }

  bin.Reset( max_per_bin + 1 );
}

// Reset():
template< class T >
void QuadTree< T >::Reset( float _left, float _right, float _top, float _bottom ) {
  Reset();

  left   = _left;
  right  = _right;
  top    = _top;
  bottom = _bottom;
}

// Add():
template< class T >
inline void QuadTree< T >::Add( T data ) {
  if( !add_function( data, left, right, top, bottom ) )
    return;

  if( nodes[0] == NULL ) {
    // Using the bin still; add the data to it
    bin.Add( data );
    if( (max_depth > 0) && (bin.NumElements() > max_per_bin) ) {
      // Too many entries in bin; empty the bin into the sub-nodes
      nodes[0] = new QuadTree< T >( left,                         left + (right - left) * 0.5f, top,                         top + (bottom - top) * 0.5f, add_function, max_per_bin, max_depth - 1 );
      nodes[1] = new QuadTree< T >( left + (right - left) * 0.5f, right,                        top,                         top + (bottom - top) * 0.5f, add_function, max_per_bin, max_depth - 1 );
      nodes[2] = new QuadTree< T >( left,                         left + (right - left) * 0.5f, top + (bottom - top) * 0.5f, bottom,                      add_function, max_per_bin, max_depth - 1 );
      nodes[3] = new QuadTree< T >( left + (right - left) * 0.5f, right,                        top + (bottom - top) * 0.5f, bottom,                      add_function, max_per_bin, max_depth - 1 );

      for( unsigned long i=0; i < bin.NumElements(); i++ ) {
        nodes[0]->Add( bin[i] );
        nodes[1]->Add( bin[i] );
        nodes[2]->Add( bin[i] );
        nodes[3]->Add( bin[i] );
      }

      bin.Reset( max_per_bin + 1 );
    }
  } else {
    // Add the data to the sub-nodes
    nodes[0]->Add( data );
    nodes[1]->Add( data );
    nodes[2]->Add( data );
    nodes[3]->Add( data );
  }
}

// AddFast():
template< class T >
inline void QuadTree< T >::AddFast( T data, QuadTree_AddFastFunction * add_fast ) {
  if( nodes[0] == NULL ) {
    // Using the bin still; add the data to it
    bin.Add( data );
    if( (max_depth > 0) && (bin.NumElements() > max_per_bin) ) {
      // Too many entries in bin; empty the bin into the sub-nodes
      float center_x = left + (right - left) * 0.5f;
      float center_y = top  + (bottom - top) * 0.5f;

      nodes[0] = new QuadTree< T >( left,     center_x, top,      center_y, add_function, max_per_bin, max_depth - 1 );
      nodes[1] = new QuadTree< T >( center_x, right,    top,      center_y, add_function, max_per_bin, max_depth - 1 );
      nodes[2] = new QuadTree< T >( left,     center_x, center_y, bottom,   add_function, max_per_bin, max_depth - 1 );
      nodes[3] = new QuadTree< T >( center_x, right,    center_y, bottom,   add_function, max_per_bin, max_depth - 1 );

      for( unsigned long i=0; i < bin.NumElements(); i++ ) {
        bool quads[4] = { false, false, false, false };
        add_fast( data, left, right, top, bottom, quads );

        if( quads[0] )   nodes[0]->AddFast( bin[i], add_fast );
        if( quads[1] )   nodes[1]->AddFast( bin[i], add_fast );
        if( quads[2] )   nodes[2]->AddFast( bin[i], add_fast );
        if( quads[3] )   nodes[3]->AddFast( bin[i], add_fast );
      }

      bin.Reset( max_per_bin + 1 );
    }
  } else {
    // Add the data to the sub-nodes
    float center_x = left + (right - left) * 0.5f;
    float center_y = top + (bottom - top) * 0.5f;

    nodes[0]->AddFast( data, add_fast );
    nodes[1]->AddFast( data, add_fast );
    nodes[2]->AddFast( data, add_fast );
    nodes[3]->AddFast( data, add_fast );
  }
}

// BuildNodeList():
template< class T >
void QuadTree< T >::BuildNodeList( DynArray< QuadTree<T> * > &array ) {
  array.Add( this );
  node[0]->BuildNodeList( array );
  node[1]->BuildNodeList( array );
  node[2]->BuildNodeList( array );
  node[3]->BuildNodeList( array );
}

// BuildLeafNodeList():
template< class T >
void QuadTree< T >::BuildLeafNodeList( DynArray< QuadTree<T> * > &array ) {
  if( node[0] == NULL ) {
    array.Add( this );
  } else {
    node[0]->BuildLeafNodeList( array );
    node[1]->BuildLeafNodeList( array );
    node[2]->BuildNodeList( array );
    node[3]->BuildLeafNodeList( array );
  }
}

// BuildNodeListWith():
template< class T >
void QuadTree< T >::BuildNodeListWith( void *data, DynArray< QuadTree<T> * > &array, QuadTree_AddFunction *_test_func ) {
  QuadTree_AddFunction *test_function = (_test_func == NULL) ? add_function : _test_func;
  if( !test_function( data, left, right, top, bottom ) )
    return;

  if( nodes[0] == NULL ) {
    array.Add( this );
  } else {
    nodes[0]->BuildNodeListWith( data, array, _test_func );
    nodes[1]->BuildNodeListWith( data, array, _test_func );
    nodes[2]->BuildNodeListWith( data, array, _test_func );
    nodes[3]->BuildNodeListWith( data, array, _test_func );
  }
}

// BuildNodeListWithFast():
template< class T >
void QuadTree< T >::BuildNodeListWithFast( void *data, DynArray< QuadTree<T> * > &array, QuadTree_AddFastFunction *_test_func ) {
  QuadTree_AddFastFunction *test_function = /*(_test_func == NULL) ? add_function :*/ _test_func;

  if( nodes[0] == NULL ) {
    array.Add( this );
  } else {
    float center_x = left + (right - left) * 0.5f;
    float center_y = top + (bottom - top) * 0.5f;

    bool quads[4] = { false, false, false, false };
    test_function( data, x, y, quads );

    if( quads[0] )   nodes[0]->BuildNodeListWithFast( bin[i], add_fast );
    if( quads[1] )   nodes[1]->BuildNodeListWithFast( bin[i], add_fast );
    if( quads[2] )   nodes[2]->BuildNodeListWithFast( bin[i], add_fast );
    if( quads[3] )   nodes[3]->BuildNodeListWithFast( bin[i], add_fast );
  }
}

// Remove():
template< class T >
inline void QuadTree< T >::Remove( T data ) {
  if( nodes[0] == NULL ) {
    unsigned long match = bin.FindIndexOf( data );
    if( match != DA_NO_MATCH )
      bin.Remove( match );
  } else {
    nodes[0]->Remove( data );
    nodes[1]->Remove( data );
    nodes[2]->Remove( data );
    nodes[3]->Remove( data );
  }
}

// GetNodes():
template< class T >
inline QuadTree< T > **QuadTree< T >::GetNodes() {
  return nodes;
}

// GetBin():
template< class T >
inline DynArray< T > & QuadTree< T >::GetBin() {
  return bin;
}

// OutputToStream():
template< class T >
void QuadTree< T >::OutputToStream( ostream &out, int indent ) {
  out << setw(indent * 2) << "" << "UL: " << left << ", " << top << "   LR:  " << right << ", " << bottom << endl;
  if( nodes[0] == NULL ) {
    out << setw( (indent + 1) * 2 ) << "" << bin.NumElements() << " in bin" << endl;
  } else {
    nodes[0]->OutputToStream( out, indent + 1 );
    nodes[1]->OutputToStream( out, indent + 1 );
    nodes[2]->OutputToStream( out, indent + 1 );
    nodes[3]->OutputToStream( out, indent + 1 );
  }
}

#endif

