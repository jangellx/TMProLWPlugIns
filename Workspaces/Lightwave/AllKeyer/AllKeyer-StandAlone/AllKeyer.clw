; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProcessThread
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AllKeyer.h"

ClassCount=5
Class1=CAllKeyerApp
Class2=CAllKeyerDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ALLKEYER_PROCESSING
Resource2=IDR_MAINFRAME
Resource3=IDR_MAIN_MENU
Class4=CAllKeyerDlg_Processing
Resource4=IDR_ACCELERATORS
Resource5=IDR_ABOUTBOX
Class5=CProcessThread
Resource6=IDD_ALLKEYER_MAIN

[CLS:CAllKeyerApp]
Type=0
HeaderFile=AllKeyer.h
ImplementationFile=AllKeyer.cpp
Filter=N
LastObject=IDM_SAVE_SETTINGS

[CLS:CAllKeyerDlg]
Type=0
HeaderFile=AllKeyerDlg.h
ImplementationFile=AllKeyerDlg.cpp
Filter=D
LastObject=CAllKeyerDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=AllKeyerDlg.h
ImplementationFile=AllKeyerDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ALLKEYER_PROCESSING]
Type=1
Class=CAllKeyerDlg_Processing
ControlCount=16
Control1=ID_PAUSE,button,1342242817
Control2=ID_ABORT,button,1342242816
Control3=ID_SKIP,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_RO_COMPARE,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RO_TO,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_RO_FROM,static,1342308352
Control11=IDC_PROGRESS_SCENE,msctls_progress32,1350565888
Control12=IDC_RO_SCENE,static,1342308352
Control13=IDC_PROGRESS_OBJECT,msctls_progress32,1350565888
Control14=IDC_RO_OBJECT,static,1342308352
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287

[DLG:IDD_ALLKEYER_MAIN]
Type=1
Class=CAllKeyerDlg
ControlCount=31
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_COMPARE,edit,1350631552
Control4=IDC_OPEN_COMPARE,button,1342242944
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_SOURCE,edit,1350631552
Control7=IDC_OPEN_SOURCE,button,1342242944
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_DESTINATION,edit,1350631552
Control10=IDC_OPEN_DESTINATION,button,1342242944
Control11=IDC_COMBO_KEY_MODE,combobox,1344339971
Control12=IDC_COMBO_COMPARE_MODE,combobox,1344339971
Control13=IDC_CHECK_USEFRAME0,button,1342373923
Control14=IDC_CHECK_LASTKEY,button,1342373923
Control15=IDC_CHECK_PAUSEONERRORS,button,1342373923
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_LOGFILE,edit,1350631552
Control18=IDC_OPEN_LOGFILE,button,1342242944
Control19=IDC_COMBO_LOG_MODE,combobox,1344339971
Control20=IDC_CHECK_APPEND,button,1342373923
Control21=ID_HELP,button,1342373888
Control22=IDCANCEL,button,1342373888
Control23=ID_ABOUTBOX,button,1342373888
Control24=IDC_STATIC,button,1342177287
Control25=IDC_STATIC,button,1342177287
Control26=IDC_STATIC,static,1342308352
Control27=IDC_RO_STATUS,static,1342308352
Control28=IDC_STATIC,static,1342308354
Control29=IDC_STATIC,static,1342308354
Control30=IDC_STATIC,static,1342308352
Control31=IDC_EDIT_KEY_VALUE,edit,1350631552

[CLS:CAllKeyerDlg_Processing]
Type=0
HeaderFile=allkeyerdlg_processing.h
ImplementationFile=allkeyerdlg_processing.cpp
BaseClass=CDialog
LastObject=CAllKeyerDlg_Processing
Filter=D
VirtualFilter=dWC

[MNU:IDR_MAIN_MENU]
Type=1
Class=CAllKeyerDlg
Command1=IDM_SAVE_SETTINGS
Command2=ID_ABOUTBOX
Command3=IDCANCEL
CommandCount=3

[DLG:IDR_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308481
Control3=IDC_STATIC,static,1342308353
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308353

[ACL:IDR_ACCELERATORS]
Type=1
Class=CAllKeyerDlg
Command1=IDCANCEL
Command2=IDM_SAVE_SETTINGS
Command3=IDC_COMBO_LOG_MODE
CommandCount=3

[CLS:CProcessThread]
Type=0
HeaderFile=ProcessThread.h
ImplementationFile=ProcessThread.cpp
BaseClass=CWinThread
Filter=N
VirtualFilter=TC
LastObject=CProcessThread

