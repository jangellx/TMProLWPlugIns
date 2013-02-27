
// Clwran.cpp
// 
// This file contains the class definitions for the CLWRAN classes.
// Basically, it just uses the contructor to convert the function
// pointers from the structure provided into function references,
// converts and data member pointers into references to those
// pointers (functionally, they work the same as the did in the
// struct), and adds some accessor functions along the way.

#include "Clwran.h"
#include <stdlib.h>

// CMonitor
CMonitor::CMonitor( Monitor *monitor )
  : Init(*monitor->init),
    Step(*monitor->step),
    Done(*monitor->done),
    data( monitor->data),
    orig_struct(  monitor ) {
 ;
}

CMonitor * CMonitor::New( Monitor *monitor ) {
  if( monitor == NULL )
    return NULL;
  else
    return new CMonitor( monitor );
}

// CObjectImport
CObjectImport::CObjectImport( ObjectImport *obj_import )
  : Begin(        *obj_import->begin     ),
    Done(         *obj_import->done      ),
    NumPoints(    *obj_import->numPoints ),
    Points(       *obj_import->points    ),
    SurfaceIndex( *obj_import->surfIndex ),
    Polygon(      *obj_import->polygon   ),
    SurfaceData(  *obj_import->surfData  ),
    result     (   obj_import->result    ),
    filename   (   obj_import->filename  ),
    failedBuf  (   obj_import->failedBuf ),
    failedLen  (   obj_import->failedLen ),
    orig_struct(   obj_import ) {
  
  Monitor = CMonitor::New( obj_import->monitor );
}

CObjectImport::~CObjectImport() {
  delete Monitor;
}

CObjectImport * CObjectImport::New( ObjectImport *obj_import ) {
  if( obj_import == NULL )
    return NULL;
  else
    return new CObjectImport( obj_import );
}


// CMessage
CMessage::CMessage( MessageFuncs *message )
  : orig_struct( message ) {
 ;
}

CMessage * CMessage::New( MessageFuncs *message ) {
  if( message == NULL )
    return NULL;
  else
    return new CMessage( message );
}

// CLWLoadState
CLWLoadState::CLWLoadState( const LWLoadState *load_state )
  : Read(       *load_state->read             ),
    ioMode(      (int)load_state->ioMode      ),
    readData(    (void *)load_state->readData ),
    orig_struct( (LWLoadState *)load_state    ) {
 ;
}

CLWLoadState * CLWLoadState::New( const LWLoadState *load_state ) {
  if( load_state == NULL )
    return NULL;
  else
    return new CLWLoadState( load_state );
}

// CLWSaveState
CLWSaveState::CLWSaveState( const LWSaveState *save_state )
  : Write(      *save_state->write             ),
    ioMode(      (int)save_state->ioMode       ),
    writeData(   (void *)save_state->writeData ),
    orig_struct( (LWSaveState *)save_state     ) {
 ;
}

CLWSaveState * CLWSaveState::New( const LWSaveState *save_state ) {
  if( save_state == NULL )
    return NULL;
  else
    return new CLWSaveState( save_state );
}


// CImageFilterAccess
CImageFilterAccess::CImageFilterAccess( const FilterAccess *filter_access )
  : BufLine(  *filter_access->bufLine  ),
    FltLine(  *filter_access->fltLine  ),
    SetRGB(   *filter_access->setRGB   ),
    SetAlpha( *filter_access->setAlpha ),
    width(     filter_access->width    ),
    height(    filter_access->height   ),
    frame(     filter_access->frame    ),
    start (    filter_access->start    ),
    end(       filter_access->end      ),
    orig_struct(  filter_access ) {

  Monitor = new CMonitor( filter_access->monitor );
}

CImageFilterAccess * CImageFilterAccess::New( const FilterAccess *image_filter ) {
  if( image_filter == NULL )
    return NULL;
  else
    return new CImageFilterAccess( image_filter );
}

// CImageFilterHandler
CImageFilterHandler::CImageFilterHandler( ImageFilterHandler *filter_handler )
  : Create(   *filter_handler->create   ),
    Destroy(  *filter_handler->destroy  ),
    Copy(     *filter_handler->copy     ),
    Load(     *filter_handler->load     ),
    Save(     *filter_handler->save     ),
    Process(  *filter_handler->process  ),
    Flags(    *filter_handler->flags    ),
    DescLn(   *filter_handler->descln   ),
    UseItems( *filter_handler->useItems ),
    ChangeID( *filter_handler->changeID ),
    orig_struct(  filter_handler ) {
 ;
}

CImageFilterHandler * CImageFilterHandler::New( ImageFilterHandler *image_handler ) {
  if( image_handler == NULL )
    return NULL;
  else
    return new CImageFilterHandler( image_handler );
}

// CPixelFilterAccess
CPixelFilterAccess::CPixelFilterAccess( PixelAccess *pixel_access )
  : BufVal(     *pixel_access->bufVal     ),
    FltVal(     *pixel_access->fltVal     ),
    SetRGBA(    *pixel_access->setRGBA    ),
    Illuminate( *pixel_access->illuminate ),
    RayTrace(   *pixel_access->rayTrace   ),
    RayCast(    *pixel_access->rayCast    ),
    RayShade(   *pixel_access->rayShade   ),
    sx(          pixel_access->sx         ),
    sy(          pixel_access->sy         ),
    orig_struct( pixel_access ) {
  ;
}

// CPixelFilterHandler
CPixelFilterHandler::CPixelFilterHandler( PixelFilterHandler *pixel_handler )
  : Create(   *pixel_handler->create   ),
    Destroy(  *pixel_handler->destroy  ),
    Copy(     *pixel_handler->copy     ),
    Load(     *pixel_handler->load     ),
    Save(     *pixel_handler->save     ),
    Init(     *pixel_handler->init     ),
    CleanUp(  *pixel_handler->cleanup  ),
    NewTime(  *pixel_handler->newTime  ),
    Evaluate( *pixel_handler->evaluate ),
    Flags(    *pixel_handler->flags    ),
    DescLn(   *pixel_handler->descln   ),
    UseItems( *pixel_handler->useItems ),
    ChangeID( *pixel_handler->changeID ),
    orig_struct(  pixel_handler ) {
 ;
}

CPixelFilterHandler * CPixelFilterHandler::New( PixelFilterHandler *pixel_handler ) {
  if( pixel_handler == NULL )
    return NULL;
  else
    return new CPixelFilterHandler( pixel_handler );
}

// CShaderAccess
CShaderAccess::CShaderAccess( ShaderAccess *shader_access )
  : Illuminate(  *shader_access->illuminate   ),
    RayTrace(    *shader_access->rayTrace     ),
    RayCast(     *shader_access->rayCast      ),
    RayShade(    *shader_access->rayShade     ),
    sx(           shader_access->sx           ),
    sy(           shader_access->sy           ),
    oPos(         shader_access->oPos         ),
    wPos(         shader_access->wPos         ),
    gNorm(        shader_access->gNorm        ),
    spotSize(     shader_access->spotSize     ),
    raySource(    shader_access->raySource    ),
    rayLength(    shader_access->rayLength    ),
    cosine(       shader_access->cosine       ),
    oXfrm(        shader_access->oXfrm        ),
    wXfrm(        shader_access->wXfrm        ),
    objID(        shader_access->objID        ),
    polNum(       shader_access->polNum       ),
    wNorm(        shader_access->wNorm        ),
    color(        shader_access->color        ),
    luminous(     shader_access->luminous     ),
    diffuse(      shader_access->diffuse      ),
    specular(     shader_access->specular     ),
    mirror(       shader_access->mirror       ),
    transparency( shader_access->transparency ),
    eta(          shader_access->eta          ),
    roughness(    shader_access->roughness    ),
    orig_struct(  shader_access ) {
  ;
}

CShaderAccess * CShaderAccess::New( ShaderAccess *shader_access ) {
  if( shader_access == NULL )
    return NULL;
  else
    return new CShaderAccess( shader_access );
}

// CShaderHandler
CShaderHandler::CShaderHandler( ShaderHandler *shader_handler )
  : Create(   *shader_handler->create   ),
    Destroy(  *shader_handler->destroy  ),
    Copy(     *shader_handler->copy     ),
    Load(     *shader_handler->load     ),
    Save(     *shader_handler->save     ),
    Init(     *shader_handler->init     ),
    CleanUp(  *shader_handler->cleanup  ),
    NewTime(  *shader_handler->newTime  ),
    Evaluate( *shader_handler->evaluate ),
    Flags(    *shader_handler->flags    ),
    DescLn(   *shader_handler->descln   ),
    UseItems( *shader_handler->useItems ),
    ChangeID( *shader_handler->changeID ),
    orig_struct( shader_handler ) {
 ;
}

CShaderHandler * CShaderHandler::New( ShaderHandler *shader_handler ) {
  if( shader_handler == NULL )
    return NULL;
  else
    return new CShaderHandler( shader_handler );
}

// CDisplacementAccess
CDisplacementAccess::CDisplacementAccess( DisplacementAccess *displacement_access )
  : oPos(   displacement_access->oPos   ),
    source( displacement_access->source ),
    orig_struct(  displacement_access ) {
  ;
}

CDisplacementAccess * CDisplacementAccess::New( DisplacementAccess *displacement_access ) {
  if( displacement_access == NULL )
    return NULL;
  else
    return new CDisplacementAccess( displacement_access );
}

// CDisplacementHandler
CDisplacementHandler::CDisplacementHandler( DisplacementHandler *displacement_handler )
  : Create(   *displacement_handler->create   ),
    Destroy(  *displacement_handler->destroy  ),
    Copy(     *displacement_handler->copy     ),
    Load(     *displacement_handler->load     ),
    Save(     *displacement_handler->save     ),
    NewTime(  *displacement_handler->newTime  ),
    Evaluate( *displacement_handler->evaluate ),
    Flags(    *displacement_handler->flags    ),
    DescLn(   *displacement_handler->descln   ),
    UseItems( *displacement_handler->useItems ),
    ChangeID( *displacement_handler->changeID ),
    orig_struct(  displacement_handler ) {
 ;
}

CDisplacementHandler * CDisplacementHandler::New( DisplacementHandler *displacement_handler ) {
  if( displacement_handler == NULL )
    return NULL;
  else
    return new CDisplacementHandler( displacement_handler );
}

// CDisplacementHandler_V1
CDisplacementHandler_V1::CDisplacementHandler_V1( DisplacementHandler_V1 *displacement_handler )
  : Create(   *displacement_handler->create   ),
    Destroy(  *displacement_handler->destroy  ),
    Copy(     *displacement_handler->copy     ),
    Load(     *displacement_handler->load     ),
    Save(     *displacement_handler->save     ),
    NewTime(  *displacement_handler->newTime  ),
    Evaluate( *displacement_handler->evaluate ),
    Flags(    *displacement_handler->flags    ),
    orig_struct( displacement_handler ) {
 ;
}

CDisplacementHandler_V1 * CDisplacementHandler_V1::New( DisplacementHandler_V1 *displacement_handler ) {
  if( displacement_handler == NULL )
    return NULL;
  else
    return new CDisplacementHandler_V1( displacement_handler );
}

// CItemMotionAccess
CItemMotionAccess::CItemMotionAccess( ItemMotionAccess *motion_access )
  : GetParam( *motion_access->getParam ),
    SetParam( *motion_access->setParam ),
    item(      motion_access->item     ),
    frame(     motion_access->frame    ),
    time(      motion_access->time     ),
    orig_struct(  motion_access ) {
  ;
}

CItemMotionAccess * CItemMotionAccess::New( ItemMotionAccess *motion_access ) {
  if( motion_access == NULL )
    return NULL;
  else
    return new CItemMotionAccess( motion_access );
}

// CItemMotionHandler
CItemMotionHandler::CItemMotionHandler( ItemMotionHandler *motion_handler )
  : Create(   *motion_handler->create   ),
    Destroy(  *motion_handler->destroy  ),
    Copy(     *motion_handler->copy     ),
    Load(     *motion_handler->load     ),
    Save(     *motion_handler->save     ),
    Evaluate( *motion_handler->evaluate ),
    DescLn(   *motion_handler->descln   ),
    UseItems( *motion_handler->useItems ),
    ChangeID( *motion_handler->changeID ),
    orig_struct(  motion_handler ) {
 ;
}

CItemMotionHandler * CItemMotionHandler::New( ItemMotionHandler *motion_handler ) {
  if( motion_handler == NULL )
    return NULL;
  else
    return new CItemMotionHandler( motion_handler );
}

// CItemMotionHandler_V1
CItemMotionHandler_V1::CItemMotionHandler_V1( ItemMotionHandler_V1 *motion_handler )
  : Create(   *motion_handler->create   ),
    Destroy(  *motion_handler->destroy  ),
    Copy(     *motion_handler->copy     ),
    Load(     *motion_handler->load     ),
    Save(     *motion_handler->save     ),
    Evaluate( *motion_handler->evaluate ),
    orig_struct(  motion_handler ) {
 ;
}

CItemMotionHandler_V1 * CItemMotionHandler_V1::New( ItemMotionHandler_V1 *motion_handler ) {
  if( motion_handler == NULL )
    return NULL;
  else
    return new CItemMotionHandler_V1( motion_handler );
}

// CObjReplacementAccess
CObjReplacementAccess::CObjReplacementAccess( ObjReplacementAccess *objrep_access )
  : objectID(     objrep_access->objectID    ),
    curFrame(     objrep_access->curFrame    ),
    newFrame(     objrep_access->newFrame    ),
    curTime(      objrep_access->curTime     ),
    newTime(      objrep_access->newTime     ),
    curType(      objrep_access->curType     ),
    newType(      objrep_access->newType     ),
    curFilename(  objrep_access->curFilename ),
    newFilename(  objrep_access->newFilename ),
    orig_struct(  objrep_access ) {
  ;
}

CObjReplacementAccess * CObjReplacementAccess::New( ObjReplacementAccess *objrep_access ) {
  if( objrep_access == NULL )
    return NULL;
  else
    return new CObjReplacementAccess( objrep_access );
}

// CObjReplacementHandler
CObjReplacementHandler::CObjReplacementHandler( ObjReplacementHandler *objrep_handler )
  : Create(   *objrep_handler->create   ),
    Destroy(  *objrep_handler->destroy  ),
    Copy(     *objrep_handler->copy     ),
    Load(     *objrep_handler->load     ),
    Save(     *objrep_handler->save     ),
    Evaluate( *objrep_handler->evaluate ),
    DescLn(   *objrep_handler->descln   ),
    UseItems( *objrep_handler->useItems ),
    ChangeID( *objrep_handler->changeID ),
    orig_struct(  objrep_handler ) {
 ;
}

CObjReplacementHandler * CObjReplacementHandler::New( ObjReplacementHandler *objrep_handler ) {
  if( objrep_handler == NULL )
    return NULL;
  else
    return new CObjReplacementHandler( objrep_handler );
}

// CObjReplacementHandler_V1
CObjReplacementHandler_V1::CObjReplacementHandler_V1( ObjReplacementHandler_V1 *objrep_handler )
  : Create(   *objrep_handler->create   ),
    Destroy(  *objrep_handler->destroy  ),
    Copy(     *objrep_handler->copy     ),
    Load(     *objrep_handler->load     ),
    Save(     *objrep_handler->save     ),
    Evaluate( *objrep_handler->evaluate ),
    orig_struct(  objrep_handler ) {
 ;
}

CObjReplacementHandler_V1 * CObjReplacementHandler_V1::New( ObjReplacementHandler_V1 *objrep_handler ) {
  if( objrep_handler == NULL )
    return NULL;
  else
    return new CObjReplacementHandler_V1( objrep_handler );
}

// CFrameBufferHandler
CFrameBufferHandler::CFrameBufferHandler( FrameBufferHandler *frame_buffer_handler )
  : Create(   *frame_buffer_handler->create   ),
    Destroy(  *frame_buffer_handler->destroy  ),
    Copy(     *frame_buffer_handler->copy     ),
    Load(     *frame_buffer_handler->load     ),
    Save(     *frame_buffer_handler->save     ),
    Open(     *frame_buffer_handler->open     ),
    Close(    *frame_buffer_handler->close    ),
    Begin(    *frame_buffer_handler->begin    ),
    Write(    *frame_buffer_handler->write    ),
    Pause(    *frame_buffer_handler->pause    ),
    DescLn(   *frame_buffer_handler->descln   ),
    UseItems( *frame_buffer_handler->useItems ),
    ChangeID( *frame_buffer_handler->changeID ),
    orig_struct(  frame_buffer_handler ) {
 ;
}

CFrameBufferHandler * CFrameBufferHandler::New( FrameBufferHandler *frame_buffer_handler ) {
  if( frame_buffer_handler == NULL )
    return NULL;
  else
    return new CFrameBufferHandler( frame_buffer_handler );
}

// CAnimSaverHandler
CAnimSaverHandler::CAnimSaverHandler( AnimSaverHandler *anim_saver_handler )
  : Create(   *anim_saver_handler->create   ),
    Destroy(  *anim_saver_handler->destroy  ),
    Copy(     *anim_saver_handler->copy     ),
    Load(     *anim_saver_handler->load     ),
    Save(     *anim_saver_handler->save     ),
    Open(     *anim_saver_handler->open     ),
    Close(    *anim_saver_handler->close    ),
    Begin(    *anim_saver_handler->begin    ),
    Write(    *anim_saver_handler->write    ),
    DescLn(   *anim_saver_handler->descln   ),
    UseItems( *anim_saver_handler->useItems ),
    ChangeID( *anim_saver_handler->changeID ),
    orig_struct(  anim_saver_handler ) {
 ;
}

CAnimSaverHandler * CAnimSaverHandler::New( AnimSaverHandler *anim_saver_handler ) {
  if( anim_saver_handler == NULL )
    return NULL;
  else
    return new CAnimSaverHandler( anim_saver_handler );
}

// CSceneConverter
CSceneConverter::CSceneConverter( SceneConverter *scene_conv )
  : DeleteTemp(  *scene_conv->deleteTmp   ),
    filename(     scene_conv->filename    ),
    readFailure(  scene_conv->readFailure ),
    tmpScene(     scene_conv->tmpScene    ),
    orig_struct(  scene_conv ) {
 ;
}

CSceneConverter * CSceneConverter::New( SceneConverter *scene_conv ) {
  if( scene_conv == NULL )
    return NULL;
  else
    return new CSceneConverter( scene_conv );
}

// CLayoutGeneric
CLayoutGeneric::CLayoutGeneric( LayoutGeneric *layout_generic )
  : SaveScene(  *layout_generic->saveScene ),
    LoadScene(  *layout_generic->loadScene ),
    orig_struct( layout_generic ) {
 ;
}

CLayoutGeneric * CLayoutGeneric::New( LayoutGeneric *layout_generic ) {
  if( layout_generic == NULL )
    return NULL;
  else
    return new CLayoutGeneric( layout_generic );
}

// CLWItemInfo
CLWItemInfo::CLWItemInfo( LWItemInfo *item_info )
  : First(      *item_info->first      ),
    Next(       *item_info->next       ),
    FirstChild( *item_info->firstChild ),
    NextChild(  *item_info->nextChild  ),
    Parent(     *item_info->parent     ),
    Target(     *item_info->target     ),
    Goal(       *item_info->goal       ),
    Type(       *item_info->type       ),
    Name(       *item_info->name       ),
    Param(      *item_info->param      ),
    Limits(     *item_info->limits     ),
    SetTag(     *item_info->setTag     ),
    GetTag(     *item_info->getTag     ),
    orig_struct( item_info ) {
 ;
}

CLWItemInfo * CLWItemInfo::New( LWItemInfo *item_info ) {
  if( item_info == NULL )
    return NULL;
  else
    return new CLWItemInfo( item_info );
}

// CLWObjectInfo
CLWObjectInfo::CLWObjectInfo( LWObjectInfo *object_info )
  : Filename(    *object_info->filename    ),
    NumPoints(   *object_info->numPoints   ),
    NumPolygons( *object_info->numPolygons ),
    ShadowOpts(  *object_info->shadowOpts  ),
    Dissolve(    *object_info->dissolve    ),
    orig_struct(  object_info ) {
 ;
}

CLWObjectInfo * CLWObjectInfo::New( LWObjectInfo *obj_info ) {
  if( obj_info == NULL )
    return NULL;
  else
    return new CLWObjectInfo( obj_info );
}

// CLWBoneInfo
CLWBoneInfo::CLWBoneInfo( LWBoneInfo *bone_info )
  : Flags(      *bone_info->flags      ),
    RestParam(  *bone_info->restParam  ),
    RestLength( *bone_info->restLength ),
    Limits(     *bone_info->limits     ),
    orig_struct( bone_info ) {
 ;
}

CLWBoneInfo * CLWBoneInfo::New( LWBoneInfo *bone_info ) {
  if( bone_info == NULL )
    return NULL;
  else
    return new CLWBoneInfo( bone_info );
}

// CLWLightInfo
CLWLightInfo::CLWLightInfo( LWLightInfo *light_info )
  : Ambient(    *light_info->ambient     ),
    Type(       *light_info->type        ),
    Color(      *light_info->color       ),
    ShadowType( *light_info->shadowType  ),
    ConeAngles( *light_info->coneAngles  ),
    Flags(      *light_info->flags       ),
    Range(      *light_info->range       ),
    orig_struct( light_info ) {
 ;
}

CLWLightInfo * CLWLightInfo::New( LWLightInfo *light_info ) {
  if( light_info == NULL )
    return NULL;
  else
    return new CLWLightInfo( light_info );
}

// CLWCameraInfo
CLWCameraInfo::CLWCameraInfo( LWCameraInfo *camera_info )
  : ZoomFactor(    *camera_info->zoomFactor    ),
    FocalLength(   *camera_info->focalLength   ),
    FocalDistance( *camera_info->focalDistance ),
    FStop(         *camera_info->fStop         ),
    BlurLength(    *camera_info->blurLength    ),
    FOVAngles(     *camera_info->fovAngles     ),
    orig_struct(    camera_info ) {
 ;
}

CLWCameraInfo * CLWCameraInfo::New( LWCameraInfo *camera_info ) {
  if( camera_info == NULL )
    return NULL;
  else
    return new CLWCameraInfo( camera_info );
}

// CLWSceneInfo
CLWSceneInfo::CLWSceneInfo( LWSceneInfo *scene_info )
  : name(               scene_info->name               ),
    filename(           scene_info->filename           ),
    numPoints(          scene_info->numPoints          ),
    numPolygons(        scene_info->numPolygons        ),
    renderType(         scene_info->renderType         ),
    renderOpts(         scene_info->renderOpts         ),
    frameStart(         scene_info->frameStart         ),
    frameEnd(           scene_info->frameEnd           ),
    frameStep(          scene_info->frameStep          ),
    framesPerSecond(    scene_info->framesPerSecond    ),
    frameWidth(         scene_info->frameWidth         ),
    frameHeight(        scene_info->frameHeight        ),
    pixelAspect(        scene_info->pixelAspect        ),
    minSamplesPerPixel( scene_info->minSamplesPerPixel ),
    maxSamplesPerPixel( scene_info->maxSamplesPerPixel ),
    limitedRegion(      scene_info->limitedRegion      ),
    recursionDepth(     scene_info->recursionDepth     ),
    orig_struct(        scene_info ) {
 ;
}

CLWSceneInfo * CLWSceneInfo::New( LWSceneInfo *scene_info ) {
  if( scene_info == NULL )
    return NULL;
  else
    return new CLWSceneInfo( scene_info );
}

// CLWImageList
CLWImageList::CLWImageList( LWImageList *image_list )
  : First(    *image_list->first    ),
    Next(     *image_list->next     ),
    Load(     *image_list->load     ),
    Name(     *image_list->name     ),
    Filename( *image_list->filename ),
    IsColor(  *image_list->isColor  ),
    NeedAA(   *image_list->needAA   ),
    Size(     *image_list->size     ),
    Luma(     *image_list->luma     ),
    RGB(      *image_list->RGB      ),
    LumaSpot( *image_list->lumaSpot ),
    RGBSpot(  *image_list->RGBSpot  ),
    Clear(    *image_list->clear    ),
    orig_struct(  image_list ) {
 ;
}

CLWImageList * CLWImageList::New( LWImageList *image_list ) {
  if( image_list == NULL )
    return NULL;
  else
    return new CLWImageList( image_list );
}

// CLWCompInfo
CLWCompInfo::CLWCompInfo( LWCompInfo *comp_info )
  : bg(      comp_info->bg      ),
    fg(      comp_info->fg      ),
    fgAlpha( comp_info->fgAlpha ),
    orig_struct(  comp_info ) {
 ;
}

CLWCompInfo * CLWCompInfo::New( LWCompInfo *comp_info ) {
  if( comp_info == NULL )
    return NULL;
  else
    return new CLWCompInfo( comp_info );
}

// CLWBackdropInfo
CLWBackdropInfo::CLWBackdropInfo( LWBackdropInfo *backdrop_info )
  : Backdrop( *backdrop_info->backdrop ),
    orig_struct(  backdrop_info ) {
 ;
}

CLWBackdropInfo * CLWBackdropInfo::New( LWBackdropInfo *backdrop_info ) {
  if( backdrop_info == NULL )
    return NULL;
  else
    return new CLWBackdropInfo( backdrop_info );
}

// CLWFogInfo
CLWFogInfo::CLWFogInfo( LWFogInfo *fog_info )
  : MinDist(   *fog_info->minDist   ),
    MaxDist(   *fog_info->maxDist   ),
    MinAmount( *fog_info->minAmt    ),
    MaxAmount( *fog_info->maxAmt    ),
    Color(     *fog_info->color     ),
    type(       fog_info->type      ),
    flags(      fog_info->flags     ),
    orig_struct(  fog_info ) {
 ;
}

CLWFogInfo * CLWFogInfo::New( LWFogInfo *fog_info ) {
  if( fog_info == NULL )
    return NULL;
  else
    return new CLWFogInfo( fog_info );
}

// CLWEnvelopeHandler
CLWEnvelopeHandler::CLWEnvelopeHandler( LWEnvelopeHandler *envelope_handler )
  : Create(    *envelope_handler->create    ),
    Destroy(   *envelope_handler->destroy   ),
    Copy(      *envelope_handler->copy      ),
    Load(      *envelope_handler->load      ),
    Save(      *envelope_handler->save      ),
    Evaluate(  *envelope_handler->evaluate  ),
    Interface( *envelope_handler->interface ),
    orig_struct(  envelope_handler ) {
 ;
}


CLWEnvelopeHandler * CLWEnvelopeHandler::New( LWEnvelopeHandler *envelope_handler ) {
  if( envelope_handler == NULL )
    return NULL;
  else
    return new CLWEnvelopeHandler( envelope_handler );
}

// CGlobalPool
CGlobalPool::CGlobalPool( GlobalPool *global_pool )
  : First(  *global_pool->first  ),
    Next(   *global_pool->next   ),
    ID(     *global_pool->ID     ),
    Size(   *global_pool->size   ),
    Find(   *global_pool->find   ),
    Create( *global_pool->create ),
    orig_struct(  global_pool ) {
 ;
}

CGlobalPool * CGlobalPool::New( GlobalPool *global_pool ) {
  if( global_pool == NULL )
    return NULL;
  else
    return new CGlobalPool( global_pool );
}
