//
// CL6Host.h
//
// This includes the following classes:
//
//  CMessage            MesageFuncs
//  CMessage2           MesageFuncs2
//

#include <stdlib.h>

#include <lwhost.h>

// CMessage
class CMessage {
public:
  CMessage( LWMessageFuncs * message );

  // Use New() to create a new instance of this class
  static CMessage *New( LWMessageFuncs *message );

  inline void Info(const char    *text1, const char *text2 = NULL) { //  Pops up an Information message
      (*orig_struct->info)(text1, text2); }

  inline void Error(const char   *text1, const char *text2 = NULL) { //  Pops up an Error message
      (*orig_struct->error)(text1, text2); }

  inline void Warning(const char *text1, const char *text2 = NULL) { //  Pops up a Warning message
      (*orig_struct->warning)(text1, text2); }

  LWMessageFuncs *orig_struct;  // The original structure used to create this class
};

// CMessage2
class CMessage2 {
public:
  CMessage2( LWMessageFuncs * message );

  // Use New() to create a new instance of this class
  static CMessage2 *New( LWMessageFuncs *message );

  inline void Info(const char    *text1, const char *text2 = NULL) { //  Pops up an Information message
      (*orig_struct->info)(text1, text2); }

  inline void Error(const char   *text1, const char *text2 = NULL) { //  Pops up an Error message
      (*orig_struct->error)(text1, text2); }

  inline void Warning(const char *text1, const char *text2 = NULL) { //  Pops up a Warning message
      (*orig_struct->warning)(text1, text2); }

  inline int OKCancel( const char *title, const char *text1, const char *text2 = NULL) {    //  Pops up an OK/Cancel requester
      (*orig_struct->okCancel)(title, text1, text2); }

  inline int YesNo( const char *title, const char *text1, const char *text2 = NULL) {       //  Pops up a Yes/No requester
      (*orig_struct->yesNo)(title, text1, text2); }

  inline int YesNoCancel( const char *title, const char *text1, const char *text2 = NULL) { //  Pops up a Yes/No/Cancel requester
      (*orig_struct->yesNoCan)(title, text1, text2); }

  inline int YesNoAll( const char *title, const char *text1, const char *text2 = NULL) {    //  Pops up a Yes/No/All requester
      (*orig_struct->yesNoAll)(title, text1, text2); }

  LWMessageFuncs *orig_struct;  // The original structure used to create this class
};
