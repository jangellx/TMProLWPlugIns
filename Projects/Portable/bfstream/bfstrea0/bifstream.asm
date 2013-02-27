	TITLE	E:\TMPro_SourceTree\Projects\Portable\bfstream\bifstream.cpp
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
;	COMDAT ?lock@ios@@QAAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?unlock@ios@@QAAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?lockptr@ios@@IAEPAU_CRT_CRITICAL_SECTION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setlock@streambuf@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?clrlock@streambuf@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?lock@streambuf@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?unlock@streambuf@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?lockptr@streambuf@@IAEPAU_CRT_CRITICAL_SECTION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?gptr@streambuf@@IBEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setf@ios@@QAEJJJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@ios@@QBEPAVstreambuf@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?unlockbuf@ios@@QAAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4istream@@IAEAAV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?get@istream@@QAEAAV1@AAE@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?get@istream@@QAEAAV1@AAC@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6ostream@@QAEAAV0@P6AAAV0@AAV0@@Z@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6ostream@@QAEAAV0@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?flush@@YAAAVostream@@AAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4iostream@@IAEAAV0@PAVstreambuf@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?fd@filebuf@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?is_open@filebuf@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@ifstream@@QBEPAVfilebuf@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setmode@ifstream@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@ofstream@@QBEPAVfilebuf@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@fstream@@QBEPAVfilebuf@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bifstream@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gbifstream@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Dbifstream@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bifstream@@QAE@PBDHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bifstream@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bifstream@@QAE@HPADH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?B_Init@bifstream@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bifstream@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setmode@bifstream@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAE@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAC@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@PAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAF@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??5bifstream@@QAEAAV0@AAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetString@bifstream@@QAEPADPADH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7bifstream@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_8bifstream@@7B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	??_7bifstream@@6B@				; bifstream::`vftable'
PUBLIC	??_8bifstream@@7B@				; bifstream::`vbtable'
PUBLIC	??_Gbifstream@@UAEPAXI@Z			; bifstream::`scalar deleting destructor'
PUBLIC	??_Ebifstream@@UAEPAXI@Z			; bifstream::`vector deleting destructor'
PUBLIC	??0bifstream@@QAE@XZ				; bifstream::bifstream
PUBLIC	?B_Init@bifstream@@QAEXXZ			; bifstream::B_Init
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??1ios@@UAE@XZ:NEAR				; ios::~ios
EXTRN	??0ios@@IAE@XZ:NEAR				; ios::ios
EXTRN	??0ifstream@@QAE@XZ:NEAR			; ifstream::ifstream
EXTRN	??1ifstream@@UAE@XZ:NEAR			; ifstream::~ifstream
EXTRN	??0bfstream@@QAE@XZ:NEAR			; bfstream::bfstream
EXTRN	??1bfstream@@QAE@XZ:NEAR			; bfstream::~bfstream
;	COMDAT ??_7bifstream@@6B@
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bifstream.cpp
CONST	SEGMENT
??_7bifstream@@6B@ DD FLAT:??_Ebifstream@@UAEPAXI@Z	; bifstream::`vftable'
CONST	ENDS
;	COMDAT ??_8bifstream@@7B@
CONST	SEGMENT
??_8bifstream@@7B@ DD 00H				; bifstream::`vbtable'
	DD	010H
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T2273	DD	019930520H
	DD	03H
	DD	FLAT:$T2276
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2276	DD	0ffffffffH
	DD	FLAT:$L2266
	DD	00H
	DD	FLAT:$L2268
	DD	01H
	DD	FLAT:$L2269
xdata$x	ENDS
;	COMDAT ??0bifstream@@QAE@XZ
_TEXT	SEGMENT
_$initVBases$ = 8
_this$ = -16
$T2265 = -20
__$EHRec$ = -12
??0bifstream@@QAE@XZ PROC NEAR				; bifstream::bifstream, COMDAT

; 16   :                          bfstream() {

	push	-1
	push	$L2274
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2265[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2104
	lea	ecx, DWORD PTR [esi+16]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bifstream@@7B@ ; bifstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2265[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2104:
	push	0
	mov	ecx, esi
	call	??0ifstream@@QAE@XZ			; ifstream::ifstream
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	eax, DWORD PTR [esi]
	mov	BYTE PTR __$EHRec$[esp+32], 2
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+esi], OFFSET FLAT:??_7bifstream@@6B@ ; bifstream::`vftable'

; 17   :   B_Init();

	mov	ecx, esi
	call	?B_Init@bifstream@@QAEXXZ		; bifstream::B_Init

; 18   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	mov	eax, esi
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2266:
	mov	eax, DWORD PTR $T2265[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2267
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2267:
	ret	0
$L2268:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ifstream@@UAE@XZ			; ifstream::~ifstream
$L2269:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1bfstream@@QAE@XZ			; bfstream::~bfstream
$L2274:
	mov	eax, OFFSET FLAT:$T2273
	jmp	___CxxFrameHandler
text$x	ENDS
??0bifstream@@QAE@XZ ENDP				; bifstream::bifstream
PUBLIC	??1bifstream@@UAE@XZ				; bifstream::~bifstream
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_Gbifstream@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_Gbifstream@@UAEPAXI@Z PROC NEAR			; bifstream::`scalar deleting destructor', COMDAT
	push	esi
	lea	esi, DWORD PTR [ecx-16]
	push	edi
	lea	edi, DWORD PTR [esi+16]
	mov	ecx, edi
	call	??1bifstream@@UAE@XZ			; bifstream::~bifstream
	mov	ecx, edi
	call	??1ios@@UAE@XZ				; ios::~ios
	test	BYTE PTR ___flags$[esp+4], 1
	je	SHORT $L2287
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L2287:
	mov	eax, esi
	pop	edi
	pop	esi
	ret	4
??_Gbifstream@@UAEPAXI@Z ENDP				; bifstream::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0bifstream@@QAE@PBDHH@Z			; bifstream::bifstream
EXTRN	??0ifstream@@QAE@PBDHH@Z:NEAR			; ifstream::ifstream
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bifstream.cpp
xdata$x	SEGMENT
$T2297	DD	019930520H
	DD	02H
	DD	FLAT:$T2299
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2299	DD	0ffffffffH
	DD	FLAT:$L2292
	DD	00H
	DD	FLAT:$L2294
xdata$x	ENDS
;	COMDAT ??0bifstream@@QAE@PBDHH@Z
_TEXT	SEGMENT
__name$ = 8
__ios$ = 12
__filebuf$ = 16
_$initVBases$ = 20
_this$ = -16
$T2291 = -20
__$EHRec$ = -12
??0bifstream@@QAE@PBDHH@Z PROC NEAR			; bifstream::bifstream, COMDAT

; 25   :                       bfstream() {

	push	-1
	push	$L2298
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2291[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2118
	lea	ecx, DWORD PTR [esi+16]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bifstream@@7B@ ; bifstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2291[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2118:
	mov	ecx, DWORD PTR __ios$[esp+20]
	mov	eax, DWORD PTR __filebuf$[esp+20]
	mov	edx, DWORD PTR __name$[esp+20]
	push	0
	or	cl, -128				; ffffff80H
	push	eax
	push	ecx
	push	edx
	mov	ecx, esi
	call	??0ifstream@@QAE@PBDHH@Z		; ifstream::ifstream
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	eax, DWORD PTR [esi]
	mov	ecx, DWORD PTR [eax+4]

; 26   :   ;
; 27   : }

	mov	eax, esi
	mov	DWORD PTR [ecx+esi], OFFSET FLAT:??_7bifstream@@6B@ ; bifstream::`vftable'
	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2292:
	mov	eax, DWORD PTR $T2291[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2293
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2293:
	ret	0
$L2294:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ifstream@@UAE@XZ			; ifstream::~ifstream
$L2298:
	mov	eax, OFFSET FLAT:$T2297
	jmp	___CxxFrameHandler
text$x	ENDS
??0bifstream@@QAE@PBDHH@Z ENDP				; bifstream::bifstream
PUBLIC	??0bifstream@@QAE@H@Z				; bifstream::bifstream
EXTRN	??0ifstream@@QAE@H@Z:NEAR			; ifstream::ifstream
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bifstream.cpp
xdata$x	SEGMENT
$T2311	DD	019930520H
	DD	03H
	DD	FLAT:$T2313
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2313	DD	0ffffffffH
	DD	FLAT:$L2306
	DD	00H
	DD	FLAT:$L2308
	DD	01H
	DD	FLAT:$L2309
xdata$x	ENDS
;	COMDAT ??0bifstream@@QAE@H@Z
_TEXT	SEGMENT
__filedesc$ = 8
_$initVBases$ = 12
_this$ = -16
$T2305 = -20
__$EHRec$ = -12
??0bifstream@@QAE@H@Z PROC NEAR				; bifstream::bifstream, COMDAT

; 32   :                       bfstream() {

	push	-1
	push	$L2312
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2305[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2124
	lea	ecx, DWORD PTR [esi+16]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bifstream@@7B@ ; bifstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2305[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2124:
	mov	eax, DWORD PTR __filedesc$[esp+20]
	push	0
	push	eax
	mov	ecx, esi
	call	??0ifstream@@QAE@H@Z			; ifstream::ifstream
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	ecx, DWORD PTR [esi]
	mov	BYTE PTR __$EHRec$[esp+32], 2
	mov	edx, DWORD PTR [ecx+4]

; 33   :   B_Init();

	mov	ecx, esi
	mov	DWORD PTR [edx+esi], OFFSET FLAT:??_7bifstream@@6B@ ; bifstream::`vftable'
	call	?B_Init@bifstream@@QAEXXZ		; bifstream::B_Init

; 34   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	mov	eax, esi
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2306:
	mov	eax, DWORD PTR $T2305[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2307
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2307:
	ret	0
$L2308:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ifstream@@UAE@XZ			; ifstream::~ifstream
$L2309:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1bfstream@@QAE@XZ			; bfstream::~bfstream
$L2312:
	mov	eax, OFFSET FLAT:$T2311
	jmp	___CxxFrameHandler
text$x	ENDS
??0bifstream@@QAE@H@Z ENDP				; bifstream::bifstream
PUBLIC	??0bifstream@@QAE@HPADH@Z			; bifstream::bifstream
EXTRN	??0ifstream@@QAE@HPADH@Z:NEAR			; ifstream::ifstream
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bifstream.cpp
xdata$x	SEGMENT
$T2325	DD	019930520H
	DD	03H
	DD	FLAT:$T2327
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2327	DD	0ffffffffH
	DD	FLAT:$L2320
	DD	00H
	DD	FLAT:$L2322
	DD	01H
	DD	FLAT:$L2323
xdata$x	ENDS
;	COMDAT ??0bifstream@@QAE@HPADH@Z
_TEXT	SEGMENT
__filedesc$ = 8
_name$ = 12
__value$ = 16
_$initVBases$ = 20
_this$ = -16
$T2319 = -20
__$EHRec$ = -12
??0bifstream@@QAE@HPADH@Z PROC NEAR			; bifstream::bifstream, COMDAT

; 40   :                       bfstream() {

	push	-1
	push	$L2326
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2319[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2132
	lea	ecx, DWORD PTR [esi+16]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bifstream@@7B@ ; bifstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2319[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2132:
	mov	eax, DWORD PTR __value$[esp+20]
	mov	ecx, DWORD PTR _name$[esp+20]
	mov	edx, DWORD PTR __filedesc$[esp+20]
	push	0
	push	eax
	push	ecx
	push	edx
	mov	ecx, esi
	call	??0ifstream@@QAE@HPADH@Z		; ifstream::ifstream
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	eax, DWORD PTR [esi]
	mov	BYTE PTR __$EHRec$[esp+32], 2
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+esi], OFFSET FLAT:??_7bifstream@@6B@ ; bifstream::`vftable'

; 41   :   B_Init();

	mov	ecx, esi
	call	?B_Init@bifstream@@QAEXXZ		; bifstream::B_Init

; 42   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	mov	eax, esi
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2320:
	mov	eax, DWORD PTR $T2319[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2321
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2321:
	ret	0
$L2322:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ifstream@@UAE@XZ			; ifstream::~ifstream
$L2323:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1bfstream@@QAE@XZ			; bfstream::~bfstream
$L2326:
	mov	eax, OFFSET FLAT:$T2325
	jmp	___CxxFrameHandler
text$x	ENDS
??0bifstream@@QAE@HPADH@Z ENDP				; bifstream::bifstream
PUBLIC	?setmode@bifstream@@QAEHH@Z			; bifstream::setmode
;	COMDAT ?B_Init@bifstream@@QAEXXZ
_TEXT	SEGMENT
?B_Init@bifstream@@QAEXXZ PROC NEAR			; bifstream::B_Init, COMDAT

; 49   :   setmode( binary );

	push	128					; 00000080H
	call	?setmode@bifstream@@QAEHH@Z		; bifstream::setmode

; 50   : }

	ret	0
?B_Init@bifstream@@QAEXXZ ENDP				; bifstream::B_Init
_TEXT	ENDS
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bifstream.cpp
xdata$x	SEGMENT
$T2343	DD	019930520H
	DD	01H
	DD	FLAT:$T2345
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2345	DD	0ffffffffH
	DD	FLAT:$L2338
xdata$x	ENDS
;	COMDAT ??1bifstream@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1bifstream@@UAE@XZ PROC NEAR				; bifstream::~bifstream, COMDAT

; 54   : bifstream::~bifstream() {

	push	-1
	push	$L2344
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	esi, ecx
	mov	DWORD PTR _this$[esp+20], esi
	mov	eax, DWORD PTR [esi-16]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+esi-16], OFFSET FLAT:??_7bifstream@@6B@ ; bifstream::`vftable'

; 55   :   ;
; 56   : }

	lea	edx, DWORD PTR [esi-16]
	xor	ecx, ecx
	test	edx, edx
	mov	DWORD PTR __$EHRec$[esp+28], ecx
	je	SHORT $L2337
	lea	ecx, DWORD PTR [esi-4]
$L2337:
	call	??1bfstream@@QAE@XZ			; bfstream::~bfstream
	lea	ecx, DWORD PTR [esi-4]
	mov	DWORD PTR __$EHRec$[esp+28], -1
	call	??1ifstream@@UAE@XZ			; ifstream::~ifstream
	mov	ecx, DWORD PTR __$EHRec$[esp+20]
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2338:
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 4
	jmp	??1ifstream@@UAE@XZ			; ifstream::~ifstream
$L2344:
	mov	eax, OFFSET FLAT:$T2343
	jmp	___CxxFrameHandler
text$x	ENDS
??1bifstream@@UAE@XZ ENDP				; bifstream::~bifstream
EXTRN	?setmode@filebuf@@QAEHH@Z:NEAR			; filebuf::setmode
;	COMDAT ?setmode@bifstream@@QAEHH@Z
_TEXT	SEGMENT
?setmode@bifstream@@QAEHH@Z PROC NEAR			; bifstream::setmode, COMDAT

; 63   :   return( ifstream::setmode( binary ) );

	mov	eax, DWORD PTR [ecx]
	push	128					; 00000080H
	mov	edx, DWORD PTR [eax+4]
	mov	ecx, DWORD PTR [edx+ecx+4]
	call	?setmode@filebuf@@QAEHH@Z		; filebuf::setmode

; 64   : }

	ret	4
?setmode@bifstream@@QAEHH@Z ENDP			; bifstream::setmode
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAD@Z			; bifstream::operator>>
EXTRN	?get@istream@@QAEAAV1@AAD@Z:NEAR		; istream::get
;	COMDAT ??5bifstream@@QAEAAV0@AAD@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAD@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 74   :   (*this).get( var );

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	eax
	call	?get@istream@@QAEAAV1@AAD@Z		; istream::get

; 75   :   return *this;

	mov	eax, esi
	pop	esi

; 76   : }

	ret	4
??5bifstream@@QAEAAV0@AAD@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAE@Z			; bifstream::operator>>
;	COMDAT ??5bifstream@@QAEAAV0@AAE@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAE@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 82   :   (*this).get( var );

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	eax
	call	?get@istream@@QAEAAV1@AAD@Z		; istream::get

; 83   :   return *this;

	mov	eax, esi
	pop	esi

; 84   : }

	ret	4
??5bifstream@@QAEAAV0@AAE@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAC@Z			; bifstream::operator>>
;	COMDAT ??5bifstream@@QAEAAV0@AAC@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAC@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 90   :   (*this).get( var );

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	eax
	call	?get@istream@@QAEAAV1@AAD@Z		; istream::get

; 91   :   return *this;

	mov	eax, esi
	pop	esi

; 92   : }

	ret	4
??5bifstream@@QAEAAV0@AAC@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@PAD@Z			; bifstream::operator>>
;	COMDAT ??5bifstream@@QAEAAV0@PAD@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@PAD@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 99   :   int i = -1;

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	push	edi
	mov	edi, ecx
	lea	esi, DWORD PTR [eax-1]
$L2160:

; 100  : 
; 101  :   do {
; 102  :     this->get( var[++i] );          // read a char into the array

	inc	esi
	mov	ecx, edi
	push	esi
	call	?get@istream@@QAEAAV1@AAD@Z		; istream::get

; 103  :   } while ( var[i] != '\0' );       // stop when we find a string terminator

	cmp	BYTE PTR [esi], 0
	jne	SHORT $L2160

; 104  : 
; 105  :   return *this;

	mov	eax, edi
	pop	edi
	pop	esi

; 106  : }

	ret	4
??5bifstream@@QAEAAV0@PAD@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAF@Z			; bifstream::operator>>
EXTRN	?read@istream@@QAEAAV1@PADH@Z:NEAR		; istream::read
EXTRN	?ConvertEndian@bfstream@@IAEXAAF@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??5bifstream@@QAEAAV0@AAF@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAF@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 112  : bifstream & bifstream::operator>>( short &var ) {

	push	esi
	push	edi

; 113  :   read( (char *)&var, 2 );             // Read in the value

	mov	edi, DWORD PTR _var$[esp+4]
	push	2
	mov	esi, ecx
	push	edi
	call	?read@istream@@QAEAAV1@PADH@Z		; istream::read

; 114  :   ConvertEndian( var );                // Change the endian, if needed

	push	edi
	lea	ecx, DWORD PTR [esi+12]
	call	?ConvertEndian@bfstream@@IAEXAAF@Z	; bfstream::ConvertEndian

; 115  :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 116  : }

	ret	4
??5bifstream@@QAEAAV0@AAF@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAG@Z			; bifstream::operator>>
EXTRN	?ConvertEndian@bfstream@@IAEXAAG@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??5bifstream@@QAEAAV0@AAG@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAG@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 121  : bifstream & bifstream::operator>>( unsigned short &var ) {

	push	esi
	push	edi

; 122  :   read( (char *)&var, 2 );             // Read in the value

	mov	edi, DWORD PTR _var$[esp+4]
	push	2
	mov	esi, ecx
	push	edi
	call	?read@istream@@QAEAAV1@PADH@Z		; istream::read

; 123  :   ConvertEndian( var );                // Change the endian, if needed

	push	edi
	lea	ecx, DWORD PTR [esi+12]
	call	?ConvertEndian@bfstream@@IAEXAAG@Z	; bfstream::ConvertEndian

; 124  :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 125  : }

	ret	4
??5bifstream@@QAEAAV0@AAG@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAJ@Z			; bifstream::operator>>
EXTRN	?ConvertEndian@bfstream@@IAEXAAJ@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??5bifstream@@QAEAAV0@AAJ@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAJ@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 130  : bifstream & bifstream::operator>>( long &var ) {

	push	esi
	push	edi

; 131  :   read( (char *)&var, 4 );             // Read in the value

	mov	edi, DWORD PTR _var$[esp+4]
	push	4
	mov	esi, ecx
	push	edi
	call	?read@istream@@QAEAAV1@PADH@Z		; istream::read

; 132  :   ConvertEndian( var );                // Change the endian, if needed

	push	edi
	lea	ecx, DWORD PTR [esi+12]
	call	?ConvertEndian@bfstream@@IAEXAAJ@Z	; bfstream::ConvertEndian

; 133  :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 134  : }

	ret	4
??5bifstream@@QAEAAV0@AAJ@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAK@Z			; bifstream::operator>>
EXTRN	?ConvertEndian@bfstream@@IAEXAAK@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??5bifstream@@QAEAAV0@AAK@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAK@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 139  : bifstream & bifstream::operator>>( unsigned long &var ) {

	push	esi
	push	edi

; 140  :   read( (char *)&var, 4 );             // Read in the value

	mov	edi, DWORD PTR _var$[esp+4]
	push	4
	mov	esi, ecx
	push	edi
	call	?read@istream@@QAEAAV1@PADH@Z		; istream::read

; 141  :   ConvertEndian( var );                // Change the endian, if needed

	push	edi
	lea	ecx, DWORD PTR [esi+12]
	call	?ConvertEndian@bfstream@@IAEXAAK@Z	; bfstream::ConvertEndian

; 142  :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 143  : }

	ret	4
??5bifstream@@QAEAAV0@AAK@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAM@Z			; bifstream::operator>>
EXTRN	?ConvertEndian@bfstream@@IAEXAAM@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??5bifstream@@QAEAAV0@AAM@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAM@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 148  : bifstream & bifstream::operator>>( float &var ) {

	push	esi
	push	edi

; 149  :   read( (char *)&var, 4 );             // Read in the value

	mov	edi, DWORD PTR _var$[esp+4]
	push	4
	mov	esi, ecx
	push	edi
	call	?read@istream@@QAEAAV1@PADH@Z		; istream::read

; 150  :   ConvertEndian( var );                // Change the endian, if needed

	push	edi
	lea	ecx, DWORD PTR [esi+12]
	call	?ConvertEndian@bfstream@@IAEXAAM@Z	; bfstream::ConvertEndian

; 151  :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 152  : }

	ret	4
??5bifstream@@QAEAAV0@AAM@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAN@Z			; bifstream::operator>>
EXTRN	?ConvertEndian@bfstream@@IAEXAAN@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??5bifstream@@QAEAAV0@AAN@Z
_TEXT	SEGMENT
_var$ = 8
??5bifstream@@QAEAAV0@AAN@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 157  : bifstream & bifstream::operator>>( double &var ) {

	push	esi
	push	edi

; 158  :   read( (char *)&var, 8 );             // Read in the value

	mov	edi, DWORD PTR _var$[esp+4]
	push	8
	mov	esi, ecx
	push	edi
	call	?read@istream@@QAEAAV1@PADH@Z		; istream::read

; 159  :   ConvertEndian( var );                // Change the endian, if needed

	push	edi
	lea	ecx, DWORD PTR [esi+12]
	call	?ConvertEndian@bfstream@@IAEXAAN@Z	; bfstream::ConvertEndian

; 160  :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 161  : }

	ret	4
??5bifstream@@QAEAAV0@AAN@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAH@Z			; bifstream::operator>>
;	COMDAT ??5bifstream@@QAEAAV0@AAH@Z
_TEXT	SEGMENT
??5bifstream@@QAEAAV0@AAH@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 166  : bifstream & bifstream::operator>>( int &var ) {

	mov	eax, ecx

; 167  :   // As int sizes are platform specific, they are not supported.  Read in
; 168  :   //  longs or shorts instead.
; 169  :   assert( 0 ); // DON'T READ IN INTS!!!
; 170  :   return *this;
; 171  : }

	ret	4
??5bifstream@@QAEAAV0@AAH@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	??5bifstream@@QAEAAV0@AAI@Z			; bifstream::operator>>
;	COMDAT ??5bifstream@@QAEAAV0@AAI@Z
_TEXT	SEGMENT
??5bifstream@@QAEAAV0@AAI@Z PROC NEAR			; bifstream::operator>>, COMDAT

; 176  : bifstream & bifstream::operator>>( unsigned int &var ) {

	mov	eax, ecx

; 177  :   // As int sizes are platform specific, they are not supported.  Read in
; 178  :   //  longs or shorts instead.
; 179  :   assert( 0 ); // DON'T READ IN UNSIGNED INTS!!!
; 180  :   return *this;
; 181  : }

	ret	4
??5bifstream@@QAEAAV0@AAI@Z ENDP			; bifstream::operator>>
_TEXT	ENDS
PUBLIC	?GetString@bifstream@@QAEPADPADH@Z		; bifstream::GetString
;	COMDAT ?GetString@bifstream@@QAEPADPADH@Z
_TEXT	SEGMENT
_string$ = 8
_len$ = 12
?GetString@bifstream@@QAEPADPADH@Z PROC NEAR		; bifstream::GetString, COMDAT

; 188  : char * bifstream::GetString( char * string, int len ) {

	push	ebx
	push	ebp
	push	esi

; 189  :   int i = -1;
; 190  : 
; 191  :   do {
; 192  :     this->get( string[++i] );        // read a char into the array

	mov	esi, DWORD PTR _string$[esp+8]
	push	edi
	mov	edi, ecx
	push	esi
	xor	ebp, ebp
	call	?get@istream@@QAEAAV1@AAD@Z		; istream::get

; 193  : 
; 194  :     // See if the buffer is full
; 195  :     if( i+2 == len ) {

	mov	eax, DWORD PTR _len$[esp+12]
	mov	ebx, 2
	cmp	eax, ebx
	je	SHORT $L2414
	mov	eax, esi
	sub	ebx, eax
$L2209:

; 197  :       break;
; 198  :     }
; 199  : 
; 200  :   } while( string[i] != '\0' );      // stop when we find a string terminator

	cmp	BYTE PTR [esi], 0
	je	SHORT $L2416
	inc	ebp
	inc	esi
	push	esi
	mov	ecx, edi
	call	?get@istream@@QAEAAV1@AAD@Z		; istream::get
	mov	ecx, DWORD PTR _len$[esp+12]
	lea	eax, DWORD PTR [ebx+esi]
	cmp	eax, ecx
	jne	SHORT $L2209

; 193  : 
; 194  :     // See if the buffer is full
; 195  :     if( i+2 == len ) {

	mov	esi, DWORD PTR _string$[esp+12]
$L2414:

; 196  :       string[i+1] = '\0';

	mov	BYTE PTR [esi+ebp+1], 0

; 201  : 
; 202  :   return string;

	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 203  : }

	ret	8
$L2416:

; 201  : 
; 202  :   return string;

	mov	eax, DWORD PTR _string$[esp+12]
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 203  : }

	ret	8
?GetString@bifstream@@QAEPADPADH@Z ENDP			; bifstream::GetString
_TEXT	ENDS
END
