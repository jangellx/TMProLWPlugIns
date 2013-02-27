//
// OctTree.h
//

#ifndef OCT_TREE_HEADER
#define OCT_TREE_HEADER

#include <portable/dynarray/dynarray.h>

#include <fstream.h>
#include <iomanip.h>

typedef bool OctTree_AddFunction( const void *data, float left, float right, float top, float bottom, float front, float back );
typedef bool OctTree_FindFunction( const void *data, const void *compare, float left, float right, float top, float bottom, float front, float back, void *user_data );

// OctTree
template< class T >
class OctTree {
public:
  OctTree( float _left, float _right, float _top, float _bottom, float _front, float _back, OctTree_AddFunction * _add_func, unsigned long _max_per_bin = 10, unsigned long _max_depth = 5 );
  ~OctTree();

  void Reset();

  void Add( T data );                         // Add new data to the tree
  void Remove( T data );                      // Remove data from the tree.

  void AddToArray( DynArray< T > &array, void *compare, OctTree_FindFunction *find_func = NULL);              // Build a list of all data matching the test case Add new data to the tree

  void BuildNodeList( DynArray< OctTree<T> * > &array );                                                      // Build a list of all nodes in the tree
  void BuildLeafNodeList( DynArray< OctTree<T> * > &array );                                                  // Build a list of all nodes with no sub-nodes in the tree
  void BuildNodeListWith( T data, DynArray< OctTree<T> * > &array, OctTree_AddFunction *_test_func = NULL );  // Build a list of all nodes in the tree that contain the data.  Similar to Add(), but returns a list of matching nodes instead of adding the data to those nodes

  void OutputToStream( ostream &out, int indent = 0 );                                                        // Output the tree to a stream

  // Accessors
  inline OctTree< T >   * GetNodes();        // Get the node list
  inline DynArray< T >   & GetBin();         // Get the data bin

  inline float             GetLeft()      { return left;   }
  inline float             GetRight()     { return right;  }
  inline float             GetTop()       { return top;    }
  inline float             GetBottom()    { return bottom; }

  inline unsigned long     GetMaxPerBin() { return max_per_bin; }
  inline unsigned long     GetMaxDepth()  { return max_depth; }

protected:
  OctTree< T >          * nodes[8];           // The eight nodes of the tree, if bin is empty
  DynArray< T >           bin;                // Data in this Octrant, if all nodes are all NULL

  float                   left;               // Extents of this node
  float                   right;
  float                   top;
  float                   bottom;
  float                   front;
  float                   back;

  unsigned long           max_per_bin;        // Max entries per bin; after hitting this number, the bin will be subdivided into nodes.
  unsigned long           max_depth;          // Maximum tree depth below this node.  0 means no sub nodes are allowed

  OctTree_AddFunction  * add_function;        // Add callback function
};

// Constructor
template< class T >
OctTree< T >::OctTree( float _left, float _right, float _top, float _bottom, float _front, float _back,  OctTree_AddFunction *_add_func, unsigned long _max_per_bin, unsigned long _max_depth )
  : left(_left), right(_right), top(_top), bottom(_bottom), front(_front), back(_back), max_per_bin(_max_per_bin), max_depth(_max_depth),
    add_function(_add_func), bin(_max_per_bin + 1, DEFAULT_GROW_SIZE, DEFAULT_SHRINK_SIZE, false) {

  nodes[0] = nodes[1] = nodes[2] = nodes[3] = NULL;
  nodes[4] = nodes[5] = nodes[6] = nodes[7] = NULL;
}

// Destructor
template< class T >
OctTree< T >::~OctTree() {
  Reset();
}

// Reset():
template< class T >
inline void OctTree< T >::Reset() {
  for( int i=0; i < 8; i++ ) {
    if( nodes[i] != NULL ) {
      delete nodes[i];
      nodes[i] = NULL;
    }
  }

  bin.Reset( max_per_bin + 1 );
}

// Add():
template< class T >
inline void OctTree< T >::Add( T data ) {
  if( !add_function( data, left, right, top, bottom ) )
    return;

  if( nodes[0] == NULL ) {
    // Using the bin still; add the data to it
    bin.Add( data );
    if( (max_depth > 0) && (bin.NumElements() > max_per_bin) ) {
      // Too many entries in bin; empty the bin into the sub-nodes
      nodes[0] = new OctTree< T >( left,                         left + (right - left) * 0.5f, top,                         top + (bottom - top) * 0.5f, front, front + (back - front) * 0.5f, add_function, max_per_bin, max_depth - 1 );
      nodes[1] = new OctTree< T >( left + (right - left) * 0.5f, right,                        top,                         top + (bottom - top) * 0.5f, front, front + (back - front) * 0.5f, add_function, max_per_bin, max_depth - 1 );
      nodes[2] = new OctTree< T >( left,                         left + (right - left) * 0.5f, top + (bottom - top) * 0.5f, bottom,                      front, front + (back - front) * 0.5f, add_function, max_per_bin, max_depth - 1 );
      nodes[3] = new OctTree< T >( left + (right - left) * 0.5f, right,                        top + (bottom - top) * 0.5f, bottom,                      front, front + (back - front) * 0.5f, add_function, max_per_bin, max_depth - 1 );

      nodes[4] = new OctTree< T >( left,                         left + (right - left) * 0.5f, top,                         top + (bottom - top) * 0.5f, front + (back - front) * 0.5f, back,  add_function, max_per_bin, max_depth - 1 );
      nodes[5] = new OctTree< T >( left + (right - left) * 0.5f, right,                        top,                         top + (bottom - top) * 0.5f, front + (back - front) * 0.5f, back,  add_function, max_per_bin, max_depth - 1 );
      nodes[6] = new OctTree< T >( left,                         left + (right - left) * 0.5f, top + (bottom - top) * 0.5f, bottom,                      front + (back - front) * 0.5f, back,  add_function, max_per_bin, max_depth - 1 );
      nodes[7] = new OctTree< T >( left + (right - left) * 0.5f, right,                        top + (bottom - top) * 0.5f, bottom,                      front + (back - front) * 0.5f, back,  add_function, max_per_bin, max_depth - 1 );

      for( unsigned long i=0; i < bin.NumElements(); i++ ) {
        nodes[0]->Add( bin[i] );
        nodes[1]->Add( bin[i] );
        nodes[2]->Add( bin[i] );
        nodes[3]->Add( bin[i] );
        nodes[4]->Add( bin[i] );
        nodes[5]->Add( bin[i] );
        nodes[6]->Add( bin[i] );
        nodes[7]->Add( bin[i] );
      }

      bin.Reset( max_per_bin + 1 );
    }
  } else {
    // Add the data to the sub-nodes
    nodes[0]->Add( data );
    nodes[1]->Add( data );
    nodes[2]->Add( data );
    nodes[3]->Add( data );
    nodes[4]->Add( data );
    nodes[5]->Add( data );
    nodes[6]->Add( data );
    nodes[7]->Add( data );
  }
}

// BuildNodeList():
template< class T >
void OctTree< T >::BuildNodeList( DynArray< OctTree<T> * > &array ) {
  array.Add( this );
  node[0]->BuildNodeList( array );
  node[1]->BuildNodeList( array );
  node[2]->BuildNodeList( array );
  node[3]->BuildNodeList( array );
  node[4]->BuildNodeList( array );
  node[5]->BuildNodeList( array );
  node[6]->BuildNodeList( array );
  node[7]->BuildNodeList( array );
}

// BuildLeafNodeList():
template< class T >
void OctTree< T >::BuildLeafNodeList( DynArray< OctTree<T> * > &array ) {
  if( node[0] == NULL ) {
    array.Add( this );
  } else {
    node[0]->BuildLeafNodeList( array );
    node[1]->BuildLeafNodeList( array );
    node[2]->BuildLeafNodeList( array );
    node[3]->BuildLeafNodeList( array );
    node[4]->BuildLeafNodeList( array );
    node[5]->BuildLeafNodeList( array );
    node[6]->BuildLeafNodeList( array );
    node[7]->BuildLeafNodeList( array );
  }
}

// BuildNodeListWith():
template< class T >
void OctTree< T >::BuildNodeListWith( T data, DynArray< OctTree<T> * > &array, OctTree_AddFunction *_test_func ) {
  OctTree_AddFunction *test_function = (_test_func == NULL) ? add_function : _test_func;
  if( !test_function( data, left, right, top, bottom, front, back ) )
    return;

  if( nodes[0] == NULL ) {
    array.Add( this );
  } else {
    nodes[0]->BuildNodeListWith( data, array );
    nodes[1]->BuildNodeListWith( data, array );
    nodes[2]->BuildNodeListWith( data, array );
    nodes[3]->BuildNodeListWith( data, array );
    nodes[4]->BuildNodeListWith( data, array );
    nodes[5]->BuildNodeListWith( data, array );
    nodes[6]->BuildNodeListWith( data, array );
    nodes[7]->BuildNodeListWith( data, array );
  }
}

// Remove():
template< class T >
inline void OctTree< T >::Remove( T data ) {
  if( nodes[0] == NULL ) {
    unsigned long match = bin.FindIndexOf( data );
    if( match != DA_NO_MATCH )
      bin.Remove( match );
  } else {
    nodes[0]->Remove( data );
    nodes[1]->Remove( data );
    nodes[2]->Remove( data );
    nodes[3]->Remove( data );
    nodes[4]->Remove( data );
    nodes[5]->Remove( data );
    nodes[6]->Remove( data );
    nodes[7]->Remove( data );
  }
}

// GetNodes():
template< class T >
inline OctTree< T > * OctTree< T >::GetNodes() {
  return nodes;
}

// GetBin():
template< class T >
inline DynArray< T > & OctTree< T >::GetBin() {
  return bin;
}

// OutputToStream():
template< class T >
void QuadTree< T >::OutputToStream( ostream &out, int indent ) {
  out << setw(indent * 2) << "" << "UL: " << left << ", " << top << "   LR:  " << right << ", " << bottom << "   F/B:  " << front << ", " << back << endl;
  if( nodes[0] == NULL ) {
    out << setw( (indent + 1) * 2 ) << "" << bin.NumElements() << " in bin" << endl;
  } else {
    nodes[0]->OutputToStream( out, indent + 1 );
    nodes[1]->OutputToStream( out, indent + 1 );
    nodes[2]->OutputToStream( out, indent + 1 );
    nodes[3]->OutputToStream( out, indent + 1 );
    nodes[4]->OutputToStream( out, indent + 1 );
    nodes[5]->OutputToStream( out, indent + 1 );
    nodes[6]->OutputToStream( out, indent + 1 );
    nodes[7]->OutputToStream( out, indent + 1 );
  }
}

#endif

