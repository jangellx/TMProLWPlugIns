//
// Simplify.h
//
// This file contains function prototypes and similar
// information that is used by the .c file.
//

// Prototypes

// Replacement Plug-In
extern XCALL_ (int) Simplify_Activate(  long version, GlobalFunc *global,
                                        ObjReplacementHandler *local, void *serverData );

extern XCALL_ (int) Simplify_Interface( long version, GlobalFunc *global,
                                        void *local, void *serverData );

// Generic Plug-Ins
extern XCALL_ (int) Simplify_TogglePreview( long version, GlobalFunc *global,
                                            void *local, void *serverData );

extern XCALL_ (int) Simplify_ToggleRender( long version, GlobalFunc *global,
                                           void *local, void *serverData );

extern XCALL_ (int) Simplify_ToggleLOD( long version, GlobalFunc *global,
                                        void *local, void *serverData );

extern XCALL_ (int) Simplify_ToggleAutoLoad( long version, GlobalFunc *global,
                                             void *local, void *serverData );

extern XCALL_ (int) Simplify_GlobalControl( long version, GlobalFunc *global,
                                            void *local, void *serverData );
