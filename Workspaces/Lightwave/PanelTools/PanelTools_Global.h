//
// PanelTools_Global.h
//

extern TMP_PanelTools panel_tools_functions;

extern "C" {
  int alignLabels               ( LWControlID reference, LWControlID , int padding );
  int alignLabelsArray          ( LWControlID *array, int padding );

  int alignWith                 ( LWControlID reference, LWControlID  );
  int alignWithArray            ( LWControlID *array );

  int centerUnder               ( LWControlID reference, LWControlID , int padding, int put_under );
  int centerUnderArray          ( LWControlID *array, int padding );

  int putUnder                  ( LWControlID reference, LWControlID , int padding );
  int putUnderArray             ( LWControlID *array, int padding );

  int putOnLineWith             ( LWControlID reference, LWControlID , int padding );
  int putOnLineWithArray        ( LWControlID *array, int padding );

  int rightAlign                ( LWControlID reference, LWControlID , int h_padding, int put_under, int v_padding );
  int rightAlignArray           ( LWControlID *array, int h_padding, int put_under, int v_padding );

  int rightAlignWithPanel       ( LWPanelFuncs *, LWPanelID, LWControlID , int h_padding );
  int rightAlignWithPanelArray  ( LWPanelFuncs *, LWPanelID, LWControlID *array, int h_padding, int put_under, int v_padding );

  int bottomAlign               ( LWControlID reference, LWControlID , int padding );
  int bottomAlignArray          ( LWControlID *array, int padding );

  int putOver                   ( LWControlID reference, LWControlID , int padding );
  int putOverArray              ( LWControlID *array, int padding );

  int nudge                     ( LWControlID control, int x, int y );
  int nudgeArray                ( LWControlID *array, int x, int y );
}

