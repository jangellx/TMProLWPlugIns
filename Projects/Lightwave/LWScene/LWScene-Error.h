//
// LWScene-Error.h
//

#ifndef HEADER_LWSCENE_ERROR
#define HEADER_LWSCENE_ERROR

// Error Enums
enum LWSceneError {
  LWSERROR_OK                         = 0,     // No Error

  LWSERROR_SCENE_NOT_FOUND,                    // File-Level
  LWSERROR_ERROR_OPENING_SCENE,
  LWSERROR_ERROR_READING_FROM_SCENE,
  LWSERROR_UNEXPECTED_EOF,

  LWSERROR_NOT_A_LW_SCENE,                     // Scene-Level
  LWSERROR_UNSUPPORTED_VERSION,

  LWSERROR_INVALID_ITEM_TYPE,                  // Item-Level
  LWSERROR_UNKNOWN_ITEM_TYPE,

  LWSERROR_ERROR_READING_MOTION,               // Motion-Level
  LWSERROR_ERROR_WRITING_MOTION,
  LWSERROR_BAD_MOTION_FILE,
  LWSERROR_UNSUPPORTED_MOT_VERSION,
  LWSERROR_NO_MOTION_CHANNELS,
  LWSERROR_MISSING_END_BEHAVIOR,

  LWSERROR_ERROR_READING_ENVELOPE,             // Envelope-Level
  LWSERROR_BAD_ENVELOPE_FILE,
  LWSERROR_UNSUPPORTED_ENV_VERSION,
  LWSERROR_FPS_CANT_BE_ZERO,

  NUM_LWSCENE_ERRORS };                        // Error Count

static const char *LWSceneErrorStrings[] = {
  "No Error",

  "Scene file doesn't exist",
  "An error occured opening the scene file",
  "An error occured reading from the scene file",
  "Unexpected end of file encountered",

  "File is not a Lightwave Scene",
  "Unsupported Lightwave Scene File version",

  "Invalid Item Type specified",
  "Unknown Item Type specified",

  "An error occured reading motion data",
  "An error occured writing motion data",
  "Bad motion file data encountered",
  "Unsupported Motion File version",
  "No channels found in motion",
  "Motion is missing EndBehavior",

  "An error occured reading envelope data",
  "Bad envelope file data encountered",
  "Unsupported Envelope File version",
  "Envelope frames per second must be greater than 0",

  "Don't use me -- I'm the end of the list" };

#endif