
// Clwran.h
//
// This file contains a series of classes which encapsulate the structures
// used by the Lightwave SDK.  It's purpose is to make those wacky function
// pointers into function references, so they can be used just like any other
// function.  Otherwise, they're the same.
//
// Each class contains a New() function, which returns a pointer to a new'ed
// instance of the class.  This function should be used to create a new
// instance of that class.  The memory allocated must be deleted by the
// program when it is no longer needed.
// 
// Each class also contains a orig_struct pointer, which points to the original
// structure use to create this class instance.  This should be passed to
// functions that need a pointer to the structure.
//
// This particular file contains everything found in lwran.h and lwbase.h
//
// Currently featureed in this file:
//
//    C++ Class                   LW Structure
//   CMonitor                    Monitor
//   CObjectImport               ObjectImport
//   CMessage                    MessageFuncs
//   CLWLoadState                LWLoadState
//   CLWSaveState                LWSaveState
//   CImageFilterAccess          FilterAccess
//   CImageFilterHandler         ImageFilterHandler
//   CPixelFilterAccess          PixelAccess
//   CPixelFilterHandler         PixelFilterHandler
//   CShaderAccess               ShaderAccess
//   CShaderHandler              ShaderHandler
//   CDisplacementAccess         DispalcementAccess
//   CDisplacementHandler        DisplacementHandler
//   CItemMotionAccess           ItemMotionAccess
//   CItemMotionHandler          ItemMotionHandler
//   CItemMotionHandler_V1       ItemMotionHandler
//   CObjReplacementAccess       ObjReplacementAccess
//   CObjReplacementHandler      ObjReplacementHandler
//   CObjReplacementHandler_V1   ObjReplacementHandler
//   CFrameBufferHandler         FrameBufferHandler
//   CAnimSaverHandler           AnimSaverHandler
//   CSceneConverter             SceneConverter
//   CLayoutGeneric              LayoutGeneric
//   CLWItemInfo                 LWItemInfo
//   CLWObjectInfo               LWObjectInfo
//   CLWBoneInfo                 LWBoneInfo
//   CLWLightInfo                LWLightInfo
//   CLWCameraInfo               LWCameraInfo
//   CLWSceneInfo                LWSceneInfo
//   CLWImageList                LWimageList
//   CLWCompInfo                 LWCompInfo
//   CLWBackdropInfo             LWBackdropInfo
//   CLWFogInfo                  LWFogInfo
//   CLWEnvelopeHandler          LWEnvelopeHandler
//   CLWGlobalpool               LWGlobalPool
//

#pragma once

#ifndef LW_SDK_CPP_WRAPPERS_L6_HEADER

#include <stdlib.h>
#include <lwran.h>

// Classes
class CMonitor;
class CObjectImport;
class CMessageFuncs;
class CLWLoadState;
class CLWSaveState;
class CImageFilterAccess;
class CImageFilterHandler;
class CPixelFilterAccess;
class CPixelFilterHandler;
class CShaderAccess;
class CShaderHandler;
class CDisplacementAccess;
class CDispalcementHandler;
class CDispalcementHandler_V1;
class CItemMotionAccess;
class CItemMotionHandler;
class CItemMotionHandler_V1;
class CObjReplacementAccess;
class CObjReplacementHandler;
class CObjReplacementHandler_V1;
class CFrameHandler;
class CAnimSaverHandler;
class CSceneConverter;
class CLayoutGeneric;
class CLWItemInfo;
class CLWObjectInfo;
class CLWBoneInfo;
class CLWLightInfo;
class CLWCameraInfo;
class CLWScemeInfo;
class CLWImageList;
class CLWCompInfo;
class CLWBackdropInfo;
class CLWFogInfo;
class CLWEnvelopeHandler;
class CLWGlobalPool;

// CMonitor
class CMonitor {
public:
  CMonitor( Monitor * monitor );
  
  // Use New() to create a new instance of this class
  static CMonitor *New( Monitor *monitor );

  void  (&Init)(void *, unsigned int);   // Initialize the Monitor
  int   (&Step)(void *, unsigned int);   // Step the Monitor forard
  void  (&Done)(void *);                 // All done

public:
  void * &data;          // Message Data
  Monitor *orig_struct;  // The original structure used to create this class
};

// CObjectImport
class CObjectImport {
public:
  CObjectImport( ObjectImport * obj_import);
  ~CObjectImport();

  // Use New() to create a new instance of this class
  static CObjectImport *New( ObjectImport *obj_import );

  void (&Begin)        (void *, void *);                                 // Begin Importing
  void (&Done)         (void *);                                         // Finished Importing
  void (&NumPoints)    (void *, int total );                             // Number of Points imported
  void (&Points)       (void *, int numPts, const float *xyz );          // The points themselves
  int  (&SurfaceIndex) (void *, const char *name, int *firstTime );      // Surface Index (probably a list of the surfaces imported)
  void (&Polygon)      (void *, int numPts, int surf, int flags, const unsigned short * ); //  Polygons imported?
  void (&SurfaceData)  (void *, const char *name, int size, void *data); // The surfaces themselves

  CMonitor     *  Monitor;        // Progress Monitor Class

public:
  int            &result;         // Result from object importer
  const char   * &filename;       // Filename imported
  char         * &failedBuf;      // Buffer for entering your "This Failed" text?
  int            &failedLen;      // Failed Buffer Length
  ObjectImport *  orig_struct;    // The original structure used to create this class  
};

// CMessage
class CMessage {
public:
  CMessage( MessageFuncs * message );

  // Use New() to create a new instance of this class
  static CMessage *New( MessageFuncs *message );

  void Info(const char    *text1, const char *text2 = NULL) { //  Pops up an Information message
      (*orig_struct->info)(text1, text2); }
  void Error(const char   *text1, const char *text2 = NULL) { //  Pops up an Error message
      (*orig_struct->error)(text1, text2); }
  void Warning(const char *text1, const char *text2 = NULL) { //  Pops up a Warning message
      (*orig_struct->warning)(text1, text2); }

  MessageFuncs *orig_struct;  // The original structure used to create this class

};

// CLWLoadState
class CLWLoadState {
public:
  CLWLoadState( const LWLoadState *load_state );

  // Use New() to create a new instance of this class
  static CLWLoadState *New( const LWLoadState *load_state );

  int             (&Read)(void *readData, char *buf, int len);   //    Function to read in the data, as defined by Lightwave
  inline int  SimpleRead( char *buf, int len) {                  //    Function to read in the data; doesn't require readData
    return Read( readData, buf, len );
  }

public:
  int           &ioMode;       // I/O Mode?
  void        * &readData;     // Data read in?

  LWLoadState *  orig_struct;  // The original structure used to create this class
};

// CLWSaveState
class CLWSaveState {
public:
  CLWSaveState( const LWSaveState *load_state );

  // Use New() to create a new instance of this class
  static CLWSaveState *New( const LWSaveState *save_state );

  void             (&Write)(void *writeData, char *buf, int len);  // Function to write out the data, as defined by Lightwave
  inline void  SimpleWrite( char *buf, int len) {                  // Function to write out the data; doesn't require writeData
    Write( writeData, buf, len );
  }

public:
  int           &ioMode;       // I/O Mode?
  void        * &writeData;    // Data to write out?
  LWSaveState *  orig_struct;  // The original structure used to create this class
};

// CImageFilterAccess
class CImageFilterAccess {
public:
  CImageFilterAccess( const FilterAccess *filter_access );

  // Use New() to create a new instance of this class
  static CImageFilterAccess *New( const FilterAccess *image_filter );
  
  BufferValue * (&BufLine)  (int type, int y);               // Get a line from an image buffer
  float       * (&FltLine)  (int type, int y);               // Get a line from a float buffer
  void          (&SetRGB)   (int x, int y, BufferValue[3]);  // Set a pixel's RGB value
  void          (&SetAlpha) (int x, int y, BufferValue);     // Set a pixel's Alpha value

  CMonitor      *Monitor;   // Progress Monitor Class

  inline int     Width()  { return width;  }
  inline int     Height() { return height; }
  inline LWFrame Frame()  { return frame; }
  inline LWTime  Start()  { return start; }
  inline LWTime  End()    { return end; }

public:
  const int     &width;        // Image Width
  const int     &height;       // Image Height
  const LWFrame &frame;        // Frame Number
  const LWTime  &start;        // Start Time
  const LWTime  &end;          // End Time.  Usually == start unless Motion Blur is used

  const FilterAccess *orig_struct;  // The original structure used to create this class
};

// CImageFilterHandler
class CImageFilterHandler {
public:
  CImageFilterHandler( ImageFilterHandler *filter_handler );
  
  // Use New() to create a new instance of this class
  static CImageFilterHandler *New( ImageFilterHandler *image_handler );

  LWInstance       (&Create)  (LWError *);                        // Creates an Image Filter Handler Instance
  void             (&Destroy) (LWInstance);                       // Destroys a previously craeted Image Filter Handler Instance
  LWError          (&Copy)    (LWInstance from, LWInstance to);   // Duplicates an Image Filter.  Note the destination is LAST, not FIRST!
  LWError          (&Load)    (LWInstance, const LWLoadState *);  // Called when loading an Image Filter from a scene
  LWError          (&Save)    (LWInstance, const LWSaveState *);  // Called when saving an Image Filter to a scene

  void             (&Process) (LWInstance, const FilterAccess *); // Called to process a rendered frame
  unsigned int     (&Flags)   (LWInstance);                       // Flags on which buffers to get from LW for processing

  const char *     (&DescLn)  (LWInstance);                       // ?

  const LWItemID * (&UseItems)(LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)(LWInstance, const LWItemID *);     // ?

  ImageFilterHandler *  orig_struct;  // The original structure used to create this class

};

// CPixelFilterAccess
class CPixelFilterAccess {
public:
  CPixelFilterAccess( PixelAccess *pixel_access );

  // Use New() to create a new instance of this class
  static CPixelFilterAccess *New( PixelAccess *pixel_access );

  void   (&BufVal)     (int type, int num, BufferValue *);          // A pixel from the buffer (BufferVal)?
  void   (&FltVal)     (int type, int num, float       *);          // A pixel from the buffer (float)?
  void   (&SetRGBA)    (BufferValue[4]);                            // Set a pixel in the buffer
  int    (&Illuminate) (LWItemID light, const double position[3], double direction[3], double color[3]);  // How the light illuminates this pixel
  double (&RayTrace)   (const double position[3], const double direction[3], double color[3]); // Trace a ray to this pixel
  double (&RayCast)    (const double position[3], const double direction[3]);                  // Cast a ray to this pixel
  double (&RayShade)   (const double position[3], const double direction[3], ShaderAccess *);  // Shading a on this pixel?
  
  inline double ScreenX() { return sx; }
  inline double ScreenY() { return sy; }

public:
  double sx;                  // Screen X position in the buffer (read only)
  double sy;                  // Screen Y position in the buffer (read only)

  PixelAccess * orig_struct;  // The original structure used to create this class
};

// CPixelFilterHandler
class CPixelFilterHandler {
public:
  CPixelFilterHandler( PixelFilterHandler *pixel_handler );

  // Use New() to create a new instance of this class
  static CPixelFilterHandler *New( PixelFilterHandler *pixel_filter );

  LWInstance       (&Create)  (LWError *);                        // Creates an Pixel Filter Handler Instance
  void             (&Destroy) (LWInstance);                       // Destroys a previously craeted Pixel Filter Handler Instance
  LWError          (&Copy)    (LWInstance from, LWInstance to);   // Duplicates a Pixel Filter.  Note the destination is LAST, not FIRST!
  LWError          (&Load)    (LWInstance, const LWLoadState *);  // Called when loading a Pixel Filter from a scene
  LWError          (&Save)    (LWInstance, const LWSaveState *);  // Called when saving a Pixel Filter to a scene

  LWError          (&Init)    (LWInstance);                       // Initialize the filter
  void             (&CleanUp) (LWInstance);                       // Clean Up
  LWError          (&NewTime) (LWInstance, LWFrame, LWTime);      // ?
  void             (&Evaluate)(LWInstance, const PixelAccess *);  // Called to evaluate a pixel
  unsigned int     (&Flags)   (LWInstance);                       // Flags on which buffers to get from LW for processing

  const char *     (&DescLn)  (LWInstance);                       // ?

  const LWItemID * (&UseItems)(LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)(LWInstance, const LWItemID *);     // ?

public:
  PixelFilterHandler * orig_struct;  // The original structure used to create this class
};

// CShaderAccess
class CShaderAccess {
public:
  CShaderAccess( ShaderAccess *shader_access );

  // Use New() to create a new instance of this class
  static CShaderAccess *New( ShaderAccess *shader_access );

  int             (&Illuminate) (LWItemID light, const double position[3], double direction[3], double color[3]);  // Get the illumination from the given light on this surface
  double          (&RayTrace)   (const double position[3], const double direction[3], double color[3]);            // RayTrace this surface
  double          (&RayCast)    (const double position[3], const double direction[3]);                             // RayCast this surface
  double          (&RayShade)   (const double position[3], const double direction[3], struct st_ShaderAccess *);   // RayShade this surface

  inline int        ScreenX()        { return sx;        }
  inline int        ScreenY()        { return sy;        }
  inline double   * LocalPos()       { return oPos;      }
  inline double   * WorldPos()       { return wPos;      }
  inline double   * GNorm()          { return gNorm;     }
  inline double     SpotSize()       { return spotSize;  }
  inline double   * RaySource()      { return raySource; }
  inline double     RayLength()      { return rayLength; }
  inline double     Cosine()         { return cosine;    }
  inline double   * LocalTransform() { return oXfrm;     }
  inline double   * WorldTransform() { return wXfrm;     }
  inline LWItemID   ObjectID()       { return objID;     }
  inline int        PolygonIndex()   { return polNum;    }
  inline double   * WNorm()          { return wNorm;     }

  inline double   * Color()          { return color;     }
  inline void     SetRGB( unsigned char r, unsigned char g, unsigned char b )
    { color[0] = r*255.0f; color[1] = g*255.0f; color[2] = b*255.0f; }
  inline void     SetRGB( double r, double g, double b )
    { color[0] = r; color[1] = g; color[2] = b; }
  inline double   GetLuminous()                       { return luminous;  }
  inline void      SetLuminous(double value)          { luminous = value;  }
  inline double    GetLuminosity()                    { return GetLuminous();  }
  inline void      SetLuminosity(double value)        { SetLuminous( value );  }
  inline double   GetDiffuse()                        { return diffuse;  }
  inline void      SetDiffuse(double value)           { diffuse = value;  }
  inline double    GetDiffusion()                     { return GetDiffusion();  }
  inline void      SetDiffusion(double value)         { SetDiffusion( value );  }
  inline double   GetSpecular()                       { return specular;  }
  inline void      SetSpecular(double value)          { specular = value;  }
  inline double    GetSpecularity()                   { return GetSpecular();  }
  inline void      SetSpecularity(double value)       { SetSpecular( value );  }
  inline double   GetMirror()                         { return mirror;  }
  inline void      SetMirror(double value)            { mirror = value;  }
  inline double    GetReflectivity()                  { return GetMirror();  }
  inline void      SetReflectivity(double value)      { SetMirror( value );  }
  inline double   GetTransparency()                   { return transparency;  }
  inline void      SetTransparency(double value)      { transparency = value;  }
  inline double   GetETA()                            { return eta;  }
  inline void      SetETA(double value)               { eta = value;  }
  inline double    GetIndexOfRefraction()             { return GetETA();  }
  inline void      SetIndexOfRefraction(double value) { SetETA( value );  }
  inline double   GetRoughness()                      { return roughness;  }
  inline void      SetRoughness(double value)         { roughness = value;  }
  inline double    GetGlossiness()                    { return GetRoughness();  }
  inline void      SetGlossiness(double value)        { SetRoughness( value );  }

public:
  int       &sx;            //     Screen X Position (read only)
  int       &sy;            //     Screen Y Position (read only)
  double  * oPos;           // [3] Local (Object) Position? (read only)
  double  * wPos;           // [3] World Position? (read only)
  double  * gNorm;          // [3] Surface Normal? (read only)
  double    &spotSize;      //     Spot Size (read only)
  double  * raySource;      // [3] Ray Source (read only)
  double    &rayLength;     //  Ray Length (read only)
  double    &cosine;        //     Cosine of the angle of the surface to the camera (read only)
  double  * oXfrm;          // [9] Local transformations? (read only)
  double  * wXfrm;          // [9] World transformations? (read only)
  LWItemID  &objID;         //     Object ID (read only)
  int       &polNum;        //     Number of polygons?  Index of this polygon? (read only)
  double  * wNorm;          // [3] Surface Normal? (read only)

  double  * color;          // [3] Surface Color
  double    &luminous;      //     Luminosity
  double    &diffuse;       //     Diffusion
  double    &specular;      //     Specularity
  double    &mirror;        //     Reflectivity
  double    &transparency;  //     Transparency
  double    &eta;           //     Index of Refraction
  double    &roughness;     //     Glossiness

  ShaderAccess *  orig_struct;  // The original structure used to create this class
};

// CShaderHandler
class CShaderHandler {
public:
  CShaderHandler( ShaderHandler *Shader_handler );
  
  // Use New() to create a new instance of this class
  static CShaderHandler *New( ShaderHandler *shader_handler );

  LWInstance       (&Create)    (LWError *);                        // Create a Shader Handler Instance
  void             (&Destroy)   (LWInstance);                       // Destroys a previously craeted Shader Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to);   // Duplicates a Shader.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);  // Called when loading a Shader from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);  // Called when saving a Shader to a scene

  LWError          (&Init)      (LWInstance);                       // Initialize the Shader
  void             (&CleanUp)   (LWInstance);                       // Clean Up
  LWError          (&NewTime)   (LWInstance, LWFrame, LWTime);      // ?
  void             (&Evaluate)  (LWInstance, ShaderAccess *);       // Called to evaluate an object? point?
  unsigned int     (&Flags)     (LWInstance);                       // Flags on which buffers to get from LW for processing

  const char *     (&DescLn)    (LWInstance);                       // ?

  const LWItemID * (&UseItems)  (LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)  (LWInstance, const LWItemID *);     // ?

  ShaderHandler *  orig_struct;  // The original structure used to create this class
};

// CDisplacementAccess
class CDisplacementAccess {
public:
  CDisplacementAccess( DisplacementAccess *displacement_access );

  // Use New() to create a new instance of this class
  static CDisplacementAccess *New( DisplacementAccess *displacement_access );

  // Put some accessors in here
  
public:
  double  * oPos;      // [3] Object Position array
  double  * source;    // [3] ?

  DisplacementAccess *  orig_struct;  // The original structure used to create this class
};

// CDisplacementHandler
class CDisplacementHandler {
public:
  CDisplacementHandler( DisplacementHandler *displacement_handler );
  
  // Use New() to create a new instance of this class
  static CDisplacementHandler *New( DisplacementHandler *displacement_handler );

  LWInstance       (&Create)    (LWError *, LWItemID);                        // Create a Displacement Handler Instance
  void             (&Destroy)   (LWInstance);                                 // Destroys a previously craeted Displacement Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to, LWItemID);   // Duplicates a Displacement.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);            // Called when loading a Displacement from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);            // Called when saving a Displacement to a scene

  LWError          (&NewTime)   (LWInstance, LWItemID, LWFrame, LWTime);            // ?
  void             (&Evaluate)  (LWInstance, DisplacementAccess *); // Called to evaluate an object? point?
  unsigned int     (&Flags)     (LWInstance);                             // Flags on which buffers to get from LW for processing

  const char *     (&DescLn)    (LWInstance);                       // ?

  const LWItemID * (&UseItems)  (LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)  (LWInstance, const LWItemID *);     // ?

public:
  DisplacementHandler *  orig_struct;  // The original structure used to create this class
};


// CDisplacementHandler_V1
class CDisplacementHandler_V1 {
public:
  CDisplacementHandler_V1( DisplacementHandler_V1 *displacement_handler );
  
  // Use New() to create a new instance of this class
  static CDisplacementHandler_V1 *New( DisplacementHandler_V1 *displacement_handler );

  LWInstance       (&Create)    (LWError *);                        // Create a Displacement Handler Instance
  void             (&Destroy)   (LWInstance);                       // Destroys a previously craeted Displacement Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to);   // Duplicates a Displacement.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);  // Called when loading a Displacement from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);  // Called when saving a Displacement to a scene

  LWError          (&NewTime)   (LWInstance, LWItemID, LWFrame, LWTime);            // ?
  void             (&Evaluate)  (LWInstance, DisplacementAccess *); // Called to evaluate an object? point?
  unsigned int     (&Flags)     (LWInstance);                             // Flags on which buffers to get from LW for processing

public:
  DisplacementHandler_V1 *  orig_struct;  // The original structure used to create this class
};

// CItemMotionAccess
class CItemMotionAccess {
public:
  CItemMotionAccess( ItemMotionAccess *motion_access );

  // Use New() to create a new instance of this class
  static CItemMotionAccess *New( ItemMotionAccess *motion_access );

  void (&GetParam) (LWItemParam, LWTime, double vector[3]);  // Get Motion Parameters
  void (&SetParam) (LWItemParam, const double vector[3]);    // Set Motion Parameters

public:
  LWItemID &item;    // Item to access
  LWFrame  &frame;   // Frame number?
  LWTime   &time;    // Time

  ItemMotionAccess *  orig_struct;  // The original structure used to create this class
};

// CItemMotionHandler
class CItemMotionHandler {
public:
  CItemMotionHandler( ItemMotionHandler *motion_handler );

  // Use New() to create a new instance of this class
  static CItemMotionHandler *New( ItemMotionHandler *motion_handler );

  LWInstance       (&Create)    (LWError *, LWItemID);                        // Create a Motion Handler Instance
  void             (&Destroy)   (LWInstance);                                 // Destroys a previously craeted Motion Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to, LWItemID);   // Duplicates a Motion.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);            // Called when loading a Motion from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);            // Called when saving a Motion to a scene

  void             (&Evaluate)  (LWInstance, const ItemMotionAccess *); // Called to evaluate an object? point?

  const char *     (&DescLn)    (LWInstance);                       // ?

  const LWItemID * (&UseItems)  (LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)  (LWInstance, const LWItemID *);     // ?

public:
  ItemMotionHandler *  orig_struct;  // The original structure used to create this class
};

// CItemMotionHandler_V1
class CItemMotionHandler_V1 {
public:
  CItemMotionHandler_V1( ItemMotionHandler_V1 *motion_handler );

  // Use New() to create a new instance of this class
  static CItemMotionHandler_V1 *New( ItemMotionHandler_V1 *motion_handler );

  LWInstance       (&Create)   (LWError *);                       // Create a Motion Handler Instance
  void             (&Destroy)  (LWInstance);                                // Destroys a previously craeted Motion Handler Instance
  LWError          (&Copy)     (LWInstance from, LWInstance to);  // Duplicates a Motion.  Note the destination is LAST, not FIRST!
  LWError          (&Load)     (LWInstance, const LWLoadState *);           // Called when loading a Motion from a scene
  LWError          (&Save)     (LWInstance, const LWSaveState *);           // Called when saving a Motion to a scene

  void             (&Evaluate) (LWInstance, const ItemMotionAccess *); // Called to evaluate an object? point?

public:
  ItemMotionHandler_V1 *  orig_struct;  // The original structure used to create this class
};

// CObjReplacementAccess
class CObjReplacementAccess {
public:
  CObjReplacementAccess( ObjReplacementAccess *objrep_access );

  // Use New() to create a new instance of this class
  static CObjReplacementAccess *New( ObjReplacementAccess *objrep_access );

  // Put some accessors in here

public:
  LWItemID     &objectID;   // ID of the item to replace
  LWFrame      &curFrame;   // Current Frame
  LWFrame      &newFrame;   // New Frame
  LWTime       &curTime;    // Current Time
  LWTime       &newTime;    // New Time
  int          &curType;    // Current Type
  int          &newType;    // New Type
  const char * &curFilename; // Current Object's Filename
  const char * &newFilename; // New Object's Filename

  ObjReplacementAccess *  orig_struct;  // The original structure used to create this class
};

// CObjReplacementHandler
class CObjReplacementHandler {
public:
  CObjReplacementHandler( ObjReplacementHandler *objrep_handler );

  // Use New() to create a new instance of this class
  static CObjReplacementHandler *New( ObjReplacementHandler *objrep_access );

  LWInstance       (&Create)    (LWError *, LWItemID);                        // Create a ObjRep Handler Instance
  void             (&Destroy)   (LWInstance);                                 // Destroys a previously craeted ObjRep Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to, LWItemID);   // Duplicates a ObjRep.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);            // Called when loading a ObjRep from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);            // Called when saving a ObjRep to a scene

  void             (&Evaluate)  (LWInstance, ObjReplacementAccess *);         // Called to do the replacement

  const char *     (&DescLn)    (LWInstance);                       // ?

  const LWItemID * (&UseItems)  (LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)  (LWInstance, const LWItemID *);     // ?

public:
  ObjReplacementHandler *  orig_struct;  // The original structure used to create this class
};

// CObjReplacementHandler_V1
class CObjReplacementHandler_V1 {
public:
  CObjReplacementHandler_V1( ObjReplacementHandler_V1 *objrep_handler );

  // Use New() to create a new instance of this class
  static CObjReplacementHandler_V1 *New( ObjReplacementHandler_V1 *objrep_handler );

  LWInstance       (&Create)    (LWError *);                                  // Create a ObjRep Handler Instance
  void             (&Destroy)   (LWInstance);                                 // Destroys a previously craeted ObjRep Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to);             // Duplicates a ObjRep.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);            // Called when loading a ObjRep from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);            // Called when saving a ObjRep to a scene

  void             (&Evaluate)  (LWInstance, ObjReplacementAccess *);   // Called to do the replacement

public:
  ObjReplacementHandler_V1 *  orig_struct;  // The original structure used to create this class
};

// CFrameBufferHandler
class CFrameBufferHandler {
public:
  CFrameBufferHandler( FrameBufferHandler *frame_buffer_handler );

  // Use New() to create a new instance of this class
  static CFrameBufferHandler *New( FrameBufferHandler *framebuffer_handler );

  LWInstance       (&Create)   (LWError *);                        // Create a Frame Buffer Handler Instance
  void             (&Destroy)  (LWInstance);                       // Destroys a previously craeted Frame Buffer Handler Instance
  LWError          (&Copy)     (LWInstance from, LWInstance to);   // Duplicates a Frame Buffer.  Note the destination is LAST, not FIRST!
  LWError          (&Load)     (LWInstance, const LWLoadState *);  // Called when loading a Frame Buffer from a scene
  LWError          (&Save)     (LWInstance, const LWSaveState *);  // Called when saving a Frame Buffer to a scene

  LWError          (&Open)     (LWInstance, int w, int h);         // Open the FrameBuffer display window
  void             (&Close)    (LWInstance);                       // Close the FrameBuffer display window
  LWError          (&Begin)    (LWInstance);                       // Begin rendering to the frame buffer
  LWError          (&Write)    (LWInstance, const BufferValue *R, const BufferValue *G, const BufferValue *B, const BufferValue *alpha ); // write a pixel to the display?
  void             (&Pause)    (LWInstance);                       // ?

  const char *     (&DescLn)   (LWInstance);                       // ?

  const LWItemID * (&UseItems) (LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID) (LWInstance, const LWItemID *);     // ?

public:
  FrameBufferHandler *  orig_struct;  // The original structure used to create this class
};

// CAnimSaverHandler
class CAnimSaverHandler {
public:
  CAnimSaverHandler( AnimSaverHandler *anim_saver_handler );

  // Use New() to create a new instance of this class
  static CAnimSaverHandler *New( AnimSaverHandler *anim_saver_handler );

  LWInstance       (&Create)    (LWError *);                        // Create a Anim Saver Handler Instance
  void             (&Destroy)   (LWInstance);                       // Destroys a previously craeted Anim Saver Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to);   // Duplicates a Anim Saver.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);  // Called when loading a Anim Saver from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);  // Called when saving a Anim Saver to a scene

  LWError          (&Open)      (LWInstance, int w, int h, const char *filename); // Open the AnimSaver display window
  void             (&Close)     (LWInstance);                       // Close the AnimSaver display window
  LWError          (&Begin)     (LWInstance);                       // Begin rendering to the frame buffer
  LWError          (&Write)     (LWInstance, const BufferValue *R, const BufferValue *G, const BufferValue *B, const BufferValue *alpha ); // write a pixel to the anim file?

  const char *     (&DescLn)    (LWInstance);                       // ?

  const LWItemID * (&UseItems)  (LWInstance);                       // Used to decide if we need to use information about items from the scene (ie:  Null objects that control the values of a filter)
  void             (&ChangeID)  (LWInstance, const LWItemID *);     // ?

public:
  AnimSaverHandler *  orig_struct;  // The original structure used to create this class
};

// CSceneConverter
class CSceneConverter{
public:
  CSceneConverter( SceneConverter *scene_conv );

  // Use New() to create a new instance of this class
  static CSceneConverter *New( SceneConverter *scene_conv );

  void         (&DeleteTemp) (const char *tmpScene);

public:
  const char * &filename;      // Filename to of the scene import
  LWError      &readFailure;   // Read Failure
  const char * &tmpScene;      // ?

  SceneConverter *  orig_struct;  // The original structure used to create this class
};

// CLayoutGeneric
class CLayoutGeneric{
public:
  CLayoutGeneric( LayoutGeneric *layout_generic );

  // Use New() to create a new instance of this class
  static CLayoutGeneric *New( LayoutGeneric *layout_generic );

  int         (&SaveScene) (const char *file);                    // Tell LW to save the current scene
  int         (&LoadScene) (const char *file, const char *name ); // Tell LW to load the current scene and display the given name

public:
  LayoutGeneric *  orig_struct;  // The original structure used to create this class
};

// CLWItemInfo
class CLWItemInfo {
public:
  CLWItemInfo( LWItemInfo *item_info );

  // Use New() to create a new instance of this class
  static CLWItemInfo *New( LWItemInfo *item_info );

  LWItemID        (&First)       (LWItemType, LWItemID);                 // Get the first item of the given type in the scene
  LWItemID        (&Next)        (LWItemID);                             // Get the next item in the scene
  LWItemID        (&FirstChild)  (LWItemID parent );                     // Get the first child of this object
  LWItemID        (&NextChild)   (LWItemID parent, LWItemID prevChild ); // Get the next child
  LWItemID        (&Parent)      (LWItemID);                             // Get the parent object
  LWItemID        (&Target)      (LWItemID);                             // Get the target object
  LWItemID        (&Goal)        (LWItemID);                             // Get the IK goal
  int             (&Type)        (LWItemID);                             // Get the item type
  const char *    (&Name)        (LWItemID);                             // Get the item's name exactly the way the user sees it (ie:  "Box.lwo (3)"
  void            (&Param)       (LWItemID, LWItemParam, LWTime, double vector[3] );     // Get motion data from the item
  unsigned int    (&Limits)      (LWItemID, LWItemParam, double min[3], double max[3] ); // Get the IK Min/Max values
  
  const char *    (&GetTag)      (LWItemID, int);                        // Get a comment/tag from an item
  void            (&SetTag)      (LWItemID, int, const char *);          // Set a comment/tag for an item

public:
  LWItemInfo *  orig_struct;  // The original structure used to create this class
};

// CLWObjectInfo
class CLWObjectInfo {
public:
  CLWObjectInfo( LWObjectInfo *object_info );

  // Use New() to create a new instance of this class
  static CLWObjectInfo *New( LWObjectInfo *obj_info );

  const char * (&Filename)    (LWItemID);         //  Get an object's path and filename
  int          (&NumPoints)   (LWItemID);         //  Get the number of points in the object
  int          (&NumPolygons) (LWItemID);         //  Get the number of polygons in the object
  unsigned int (&ShadowOpts)  (LWItemID);         //  Get the object's Shadow Options
  double       (&Dissolve)    (LWItemID, LWTime); //  Get the object's Dissolve Amount

public:
  LWObjectInfo *  orig_struct;  // The original structure used to create this class
};

// CLWBoneInfo
class CLWBoneInfo {
public:
  CLWBoneInfo( LWBoneInfo *bone_info );

  // Use New() to create a new instance of this class
  static CLWBoneInfo *New( LWBoneInfo *bone_info );

  unsigned int (&Flags)      (LWItemID);                                // Get the bone's flags
  void         (&RestParam)  (LWItemID, LWItemParam, double vector[3]); // Get the bone's Rest Position data
  double       (&RestLength) (LWItemID);                                // Get the bone's Rest Length
  void         (&Limits)     (LWItemID, double *inner, double *outer);    // Get the bone's IK Limits

public:
  LWBoneInfo *  orig_struct;  // The original structure used to create this class
};

// CLWLightInfo
class CLWLightInfo {
public:
  CLWLightInfo( LWLightInfo *light_info );

  // Use New() to create a new instance of this class
  static CLWLightInfo *New( LWLightInfo *light_info );

  void         (&Ambient)    (LWTime, double color[3]);                // Get the Ambient light color
  int          (&Type)       (LWItemID);                               // Get the Light's Type
  void         (&Color)      (LWItemID, LWTime, double color[3]);      // Get the Light's Color
  int          (&ShadowType) (LWItemID);                               // Get the Light's Shadow Type
  void         (&ConeAngles) (LWItemID, double *radius, double *edge); // Get the Light's Min/Max Cone Angle
  unsigned int (&Flags)      (LWItemID);                               // Get the Light's Flags
  double       (&Range)      (LWItemID);                               // Get the Light's Falloff Distance

public:
  LWLightInfo *  orig_struct;  // The original structure used to create this class
};

// CLWCameraInfo
class CLWCameraInfo {
public:
  CLWCameraInfo( LWCameraInfo *camera_info );
  
  // Use New() to create a new instance of this class
  static CLWCameraInfo *New( LWCameraInfo *camera_info );

  double       (&ZoomFactor)     (LWItemID, LWTime);
  double       (&FocalLength)    (LWItemID, LWTime);
  double       (&FocalDistance)  (LWItemID, LWTime);
  double       (&FStop)          (LWItemID, LWTime);
  double       (&BlurLength)     (LWItemID, LWTime);
  void         (&FOVAngles)      (LWItemID, LWTime, double *horizontal, double *vertical );

public:
  LWCameraInfo *  orig_struct;  // The original structure used to create this class
};

// CLWSceneInfo
class CLWSceneInfo {
public:
  CLWSceneInfo( LWSceneInfo *scene_info );

  // Use New() to create a new instance of this class
  static CLWSceneInfo *New( LWSceneInfo *scene_info );

  // Put some accessors in here

public:
  const char * &name;
  const char * &filename;
  int          &numPoints;
  int          &numPolygons;
  int          &renderType;
  int          &renderOpts;
  LWFrame      &frameStart;
  LWFrame      &frameEnd;
  LWFrame      &frameStep;
  double       &framesPerSecond;
  int          &frameWidth;
  int          &frameHeight;
  double       &pixelAspect;
  int          &minSamplesPerPixel;
  int          &maxSamplesPerPixel;
  int         * limitedRegion;         // { x0, y0, x1, y1 }

  int          &recursionDepth;

  LWSceneInfo * orig_struct;  // The original structure used to create this class
};

// CLWImageList
class CLWImageList {
public:
  CLWImageList( LWImageList *image_list );
  
  // Use New() to create a new instance of this class
  static CLWImageList *New( LWImageList *image_list );

  LWImageID    (&First)     (void);                       // Get the fist image
  LWImageID    (&Next)      (LWImageID);                  // Get the next iamge
  LWImageID    (&Load)      (const char *);               // Load an image into Lightwave
  const char * (&Name)      (LWImageID);                  // Image name as it is presented to the user
  const char * (&Filename)  (LWImageID, LWFrame);         // Image path and filename
  int          (&IsColor)   (LWImageID);                  // Is the image color (or greyscale)?
  void         (&NeedAA)    (LWImageID);                  // ?
  void         (&Size)      (LWImageID, int *w, int *h ); // Get the Image Size
  BufferValue  (&Luma)      (LWImageID, int x, int y );   // Get the intensity of the image at this pixel.
  void         (&RGB)       (LWImageID, int x, int y, BufferValue[3] );                            // Get the color of the image at this pixel
  double       (&LumaSpot)  (LWImageID, double x, double y, double spotSize, int blend );            // Get the average intensity of a part of an image?
  void         (&RGBSpot)   (LWImageID, double x, double y, double spotSize, int blend, double[3] ); // Get the average color of a part of an image?
  void         (&Clear)     (LWImageID);                  // Un-load the image from Lightwave

public:
  LWImageList * orig_struct;  // The original structure used to create this class
};

// CLWCompInfo
class CLWCompInfo {
public:
  CLWCompInfo( LWCompInfo *comp_info );

  // Use New() to create a new instance of this class
  static CLWCompInfo *New( LWCompInfo *comp_info );

  // Put some accessors here

public: 
  LWImageID    &bg;          // Background Image
  LWImageID    &fg;          // Foreground Image
  LWImageID    &fgAlpha;     // Foreground Alpha Image  

  LWCompInfo * orig_struct;  // The original structure used to create this class
};

// CLWBackdropInfo
class CLWBackdropInfo {
public:
  CLWBackdropInfo( LWBackdropInfo *backdrop_info );

  // Use New() to create a new instance of this class
  static CLWBackdropInfo *New( LWBackdropInfo *backdrop_info );

  void     (&Backdrop)(LWTime, const double ray[3], double color[3]); // Get the backdrop info

  LWBackdropInfo * orig_struct;  // The original structure used to create this class
};

// CLWFogInfo
class CLWFogInfo {
public:
  CLWFogInfo( LWFogInfo *fog_info );

  // Use New() to create a new instance of this class
  static CLWFogInfo *New( LWFogInfo *fog_info );

  double     (&MinDist)   (LWTime);                 // Get the Fog's Near Distance
  double     (&MaxDist)   (LWTime);                 // Get the Fog's Far Distance
  double     (&MinAmount) (LWTime);                 // Get the Fog's Min Amount
  double     (&MaxAmount) (LWTime);                 // Get the Fog's Max Amount
  void       (&Color)     (LWTime, double col[3]);  // Get the Fog's Color

public:
  int           &type;      // Fog Type
  int           &flags;     // Fog flags

  LWFogInfo * orig_struct;  // The original structure used to create this class
};

// CLWEnvelopeHandler
class CLWEnvelopeHandler {
public:
  CLWEnvelopeHandler( LWEnvelopeHandler *envelope_handler );

  // Use New() to create a new instance of this class
  static CLWEnvelopeHandler *New( LWEnvelopeHandler *envelope_handler );

  LWInstance       (&Create)    (LWError *);                        // Create a Envelope Handler Instance
  void             (&Destroy)   (LWInstance);                       // Destroys a previously craeted Envelope Handler Instance
  LWError          (&Copy)      (LWInstance from, LWInstance to);   // Duplicates a Envelope.  Note the destination is LAST, not FIRST!
  LWError          (&Load)      (LWInstance, const LWLoadState *);  // Called when loading a Envelope from a scene
  LWError          (&Save)      (LWInstance, const LWSaveState *);  // Called when saving a Envelope to a scene

  double           (&Evaluate)  (LWInstance, LWTime);               // Called to do the replacement

  int              (&Interface) (LWInstance);                       // LWPanels GUI?

public:
  LWEnvelopeHandler * orig_struct;  // The original structure used to create this class
};

// CGlobalPool
class CGlobalPool {
public:
  CGlobalPool( GlobalPool *global_pool );
  
  // Use New() to create a new instance of this class
  static CGlobalPool *New( GlobalPool *global_pool );

  MemChunk         (&First)  (void);                     // Get the first chunk of memory
  MemChunk         (&Next)   (MemChunk);                 // Get the next chunk of memory
  const char *     (&ID)     (MemChunk);                 // Get the chunk's ID
  int              (&Size)   (MemChunk);                 // Get the size of a chunk of memory
  MemChunk         (&Find)   (const char *ID);           // Find the chunk matching the given ID
  MemChunk         (&Create) (const char *ID, int size); // Create a new memory chunk with the given ID and size (bytes?)

public:
  GlobalPool * orig_struct;  // The original structure used to create this class
};

#endif