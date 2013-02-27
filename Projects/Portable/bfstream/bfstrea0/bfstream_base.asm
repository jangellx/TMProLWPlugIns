	TITLE	E:\TMPro_SourceTree\Projects\Portable\bfstream\bfstream_base.cpp
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ?SetEndian@bfstream@@QAEXW4bfstream_Endians@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReverseEndian@bfstream@@SAXAAM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReverseEndian@bfstream@@SAXAAN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReverseEndian@bfstream@@SAXAAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReverseEndian@bfstream@@SAXAAF@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReverseEndian@bfstream@@SAXAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReverseEndian@bfstream@@SAXAAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bfstream@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bfstream@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAF@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	??0bfstream@@QAE@XZ				; bfstream::bfstream
;	COMDAT ??0bfstream@@QAE@XZ
_TEXT	SEGMENT
??0bfstream@@QAE@XZ PROC NEAR				; bfstream::bfstream, COMDAT

; 10   : bfstream::bfstream() {

	mov	eax, ecx

; 11   :   SetEndian( ENDIAN_INTEL );

	mov	DWORD PTR [eax], 1

; 12   : }

	ret	0
??0bfstream@@QAE@XZ ENDP				; bfstream::bfstream
_TEXT	ENDS
PUBLIC	??1bfstream@@QAE@XZ				; bfstream::~bfstream
;	COMDAT ??1bfstream@@QAE@XZ
_TEXT	SEGMENT
??1bfstream@@QAE@XZ PROC NEAR				; bfstream::~bfstream, COMDAT

; 16   :   SetEndian( ENDIAN_INTEL );

	mov	DWORD PTR [ecx], 1

; 17   : }

	ret	0
??1bfstream@@QAE@XZ ENDP				; bfstream::~bfstream
_TEXT	ENDS
PUBLIC	?ConvertEndian@bfstream@@IAEXAAM@Z		; bfstream::ConvertEndian
EXTRN	?Reverse@StringTools@@SAPADPADH@Z:NEAR		; StringTools::Reverse
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAM@Z
_TEXT	SEGMENT
_value$ = 8
?ConvertEndian@bfstream@@IAEXAAM@Z PROC NEAR		; bfstream::ConvertEndian, COMDAT

; 21   :   switch( endian_mode ) {

	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L711

; 22   :   case ENDIAN_INTEL:
; 23   : #ifndef WIN32
; 24   :     ReverseEndian( value );
; 25   : #endif
; 26   :     break;
; 27   : 
; 28   :   case ENDIAN_MOTOROLA:
; 29   : #ifdef WIN32
; 30   :     ReverseEndian( value );

	mov	eax, DWORD PTR _value$[esp-4]
	push	4
	push	eax
	call	?Reverse@StringTools@@SAPADPADH@Z	; StringTools::Reverse
	add	esp, 8
$L711:

; 31   : #endif
; 32   :   }
; 33   : }

	ret	4
?ConvertEndian@bfstream@@IAEXAAM@Z ENDP			; bfstream::ConvertEndian
_TEXT	ENDS
PUBLIC	?ConvertEndian@bfstream@@IAEXAAN@Z		; bfstream::ConvertEndian
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAN@Z
_TEXT	SEGMENT
_value$ = 8
?ConvertEndian@bfstream@@IAEXAAN@Z PROC NEAR		; bfstream::ConvertEndian, COMDAT

; 37   :   switch( endian_mode ) {

	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L717

; 38   :   case ENDIAN_INTEL:
; 39   : #ifndef WIN32
; 40   :     ReverseEndian( value );
; 41   : #endif
; 42   :     break;
; 43   : 
; 44   :   case ENDIAN_MOTOROLA:
; 45   : #ifdef WIN32
; 46   :     ReverseEndian( value );

	mov	eax, DWORD PTR _value$[esp-4]
	push	8
	push	eax
	call	?Reverse@StringTools@@SAPADPADH@Z	; StringTools::Reverse
	add	esp, 8
$L717:

; 47   : #endif
; 48   :   }
; 49   : }

	ret	4
?ConvertEndian@bfstream@@IAEXAAN@Z ENDP			; bfstream::ConvertEndian
_TEXT	ENDS
PUBLIC	?ConvertEndian@bfstream@@IAEXAAF@Z		; bfstream::ConvertEndian
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAF@Z
_TEXT	SEGMENT
_value$ = 8
?ConvertEndian@bfstream@@IAEXAAF@Z PROC NEAR		; bfstream::ConvertEndian, COMDAT

; 53   :   switch( endian_mode ) {

	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L723

; 54   :   case ENDIAN_INTEL:
; 55   : #ifndef WIN32
; 56   :     ReverseEndian( value );
; 57   : #endif
; 58   :     break;
; 59   : 
; 60   :   case ENDIAN_MOTOROLA:
; 61   : #ifdef WIN32
; 62   :     ReverseEndian( value );

	mov	eax, DWORD PTR _value$[esp-4]
	push	2
	push	eax
	call	?Reverse@StringTools@@SAPADPADH@Z	; StringTools::Reverse
	add	esp, 8
$L723:

; 63   : #endif
; 64   :   }
; 65   : }

	ret	4
?ConvertEndian@bfstream@@IAEXAAF@Z ENDP			; bfstream::ConvertEndian
_TEXT	ENDS
PUBLIC	?ConvertEndian@bfstream@@IAEXAAJ@Z		; bfstream::ConvertEndian
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAJ@Z
_TEXT	SEGMENT
_value$ = 8
?ConvertEndian@bfstream@@IAEXAAJ@Z PROC NEAR		; bfstream::ConvertEndian, COMDAT

; 69   :   switch( endian_mode ) {

	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L729

; 70   :   case ENDIAN_INTEL:
; 71   : #ifndef WIN32
; 72   :     ReverseEndian( value );
; 73   : #endif
; 74   :     break;
; 75   : 
; 76   :   case ENDIAN_MOTOROLA:
; 77   : #ifdef WIN32
; 78   :     ReverseEndian( value );

	mov	eax, DWORD PTR _value$[esp-4]
	push	4
	push	eax
	call	?Reverse@StringTools@@SAPADPADH@Z	; StringTools::Reverse
	add	esp, 8
$L729:

; 79   : #endif
; 80   :   }
; 81   : }

	ret	4
?ConvertEndian@bfstream@@IAEXAAJ@Z ENDP			; bfstream::ConvertEndian
_TEXT	ENDS
PUBLIC	?ConvertEndian@bfstream@@IAEXAAG@Z		; bfstream::ConvertEndian
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAG@Z
_TEXT	SEGMENT
_value$ = 8
?ConvertEndian@bfstream@@IAEXAAG@Z PROC NEAR		; bfstream::ConvertEndian, COMDAT

; 85   :   switch( endian_mode ) {

	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L735

; 86   :   case ENDIAN_INTEL:
; 87   : #ifndef WIN32
; 88   :     ReverseEndian( value );
; 89   : #endif
; 90   :     break;
; 91   : 
; 92   :   case ENDIAN_MOTOROLA:
; 93   : #ifdef WIN32
; 94   :     ReverseEndian( value );

	mov	eax, DWORD PTR _value$[esp-4]
	push	2
	push	eax
	call	?Reverse@StringTools@@SAPADPADH@Z	; StringTools::Reverse
	add	esp, 8
$L735:

; 95   : #endif
; 96   :   }
; 97   : }

	ret	4
?ConvertEndian@bfstream@@IAEXAAG@Z ENDP			; bfstream::ConvertEndian
_TEXT	ENDS
PUBLIC	?ConvertEndian@bfstream@@IAEXAAK@Z		; bfstream::ConvertEndian
;	COMDAT ?ConvertEndian@bfstream@@IAEXAAK@Z
_TEXT	SEGMENT
_value$ = 8
?ConvertEndian@bfstream@@IAEXAAK@Z PROC NEAR		; bfstream::ConvertEndian, COMDAT

; 101  :   switch( endian_mode ) {

	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L741

; 102  :   case ENDIAN_INTEL:
; 103  : #ifndef WIN32
; 104  :     ReverseEndian( value );
; 105  : #endif
; 106  :     break;
; 107  : 
; 108  :   case ENDIAN_MOTOROLA:
; 109  : #ifdef WIN32
; 110  :     ReverseEndian( value );

	mov	eax, DWORD PTR _value$[esp-4]
	push	4
	push	eax
	call	?Reverse@StringTools@@SAPADPADH@Z	; StringTools::Reverse
	add	esp, 8
$L741:

; 111  : #endif
; 112  :   }
; 113  : }

	ret	4
?ConvertEndian@bfstream@@IAEXAAK@Z ENDP			; bfstream::ConvertEndian
_TEXT	ENDS
END
