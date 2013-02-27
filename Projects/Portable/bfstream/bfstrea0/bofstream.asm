	TITLE	E:\TMPro_SourceTree\Projects\Portable\bfstream\bofstream.cpp
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
;	COMDAT ??6ostream@@QAEAAV0@P6AAAV0@AAV0@@Z@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6ostream@@QAEAAV0@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?put@ostream@@QAEAAV1@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?put@ostream@@QAEAAV1@C@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?write@ostream@@QAEAAV1@PBEH@Z
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
;	COMDAT ?rdbuf@ofstream@@QBEPAVfilebuf@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setmode@ofstream@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@fstream@@QBEPAVfilebuf@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bofstream@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gbofstream@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Dbofstream@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bofstream@@QAE@PBDHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bofstream@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bofstream@@QAE@HPADH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?B_Init@bofstream@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bofstream@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setmode@bofstream@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@E@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@C@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@PBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@PBE@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@F@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@G@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@J@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@M@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??6bofstream@@QAEAAV0@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7bofstream@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_8bofstream@@7B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	??_7bofstream@@6B@				; bofstream::`vftable'
PUBLIC	??_8bofstream@@7B@				; bofstream::`vbtable'
PUBLIC	??_Gbofstream@@UAEPAXI@Z			; bofstream::`scalar deleting destructor'
PUBLIC	??_Ebofstream@@UAEPAXI@Z			; bofstream::`vector deleting destructor'
PUBLIC	??0bofstream@@QAE@XZ				; bofstream::bofstream
PUBLIC	?B_Init@bofstream@@QAEXXZ			; bofstream::B_Init
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??1ios@@UAE@XZ:NEAR				; ios::~ios
EXTRN	??0ios@@IAE@XZ:NEAR				; ios::ios
EXTRN	??0ofstream@@QAE@XZ:NEAR			; ofstream::ofstream
EXTRN	??1ofstream@@UAE@XZ:NEAR			; ofstream::~ofstream
EXTRN	??0bfstream@@QAE@XZ:NEAR			; bfstream::bfstream
EXTRN	??1bfstream@@QAE@XZ:NEAR			; bfstream::~bfstream
;	COMDAT ??_7bofstream@@6B@
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bofstream.cpp
CONST	SEGMENT
??_7bofstream@@6B@ DD FLAT:??_Ebofstream@@UAEPAXI@Z	; bofstream::`vftable'
CONST	ENDS
;	COMDAT ??_8bofstream@@7B@
CONST	SEGMENT
??_8bofstream@@7B@ DD 00H				; bofstream::`vbtable'
	DD	0cH
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T2255	DD	019930520H
	DD	03H
	DD	FLAT:$T2258
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2258	DD	0ffffffffH
	DD	FLAT:$L2248
	DD	00H
	DD	FLAT:$L2250
	DD	01H
	DD	FLAT:$L2251
xdata$x	ENDS
;	COMDAT ??0bofstream@@QAE@XZ
_TEXT	SEGMENT
_$initVBases$ = 8
_this$ = -16
$T2247 = -20
__$EHRec$ = -12
??0bofstream@@QAE@XZ PROC NEAR				; bofstream::bofstream, COMDAT

; 11   :                          bfstream() {

	push	-1
	push	$L2256
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2247[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2092
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bofstream@@7B@ ; bofstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2247[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2092:
	push	0
	mov	ecx, esi
	call	??0ofstream@@QAE@XZ			; ofstream::ofstream
	lea	ecx, DWORD PTR [esi+8]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	eax, DWORD PTR [esi]
	mov	BYTE PTR __$EHRec$[esp+32], 2
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+esi], OFFSET FLAT:??_7bofstream@@6B@ ; bofstream::`vftable'

; 12   :   B_Init();

	mov	ecx, esi
	call	?B_Init@bofstream@@QAEXXZ		; bofstream::B_Init

; 13   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	mov	eax, esi
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2248:
	mov	eax, DWORD PTR $T2247[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2249
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2249:
	ret	0
$L2250:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1ofstream@@UAE@XZ			; ofstream::~ofstream
$L2251:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1bfstream@@QAE@XZ			; bfstream::~bfstream
$L2256:
	mov	eax, OFFSET FLAT:$T2255
	jmp	___CxxFrameHandler
text$x	ENDS
??0bofstream@@QAE@XZ ENDP				; bofstream::bofstream
PUBLIC	??1bofstream@@UAE@XZ				; bofstream::~bofstream
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_Gbofstream@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
??_Gbofstream@@UAEPAXI@Z PROC NEAR			; bofstream::`scalar deleting destructor', COMDAT
	push	esi
	lea	esi, DWORD PTR [ecx-12]
	push	edi
	lea	edi, DWORD PTR [esi+12]
	mov	ecx, edi
	call	??1bofstream@@UAE@XZ			; bofstream::~bofstream
	mov	ecx, edi
	call	??1ios@@UAE@XZ				; ios::~ios
	test	BYTE PTR ___flags$[esp+4], 1
	je	SHORT $L2269
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L2269:
	mov	eax, esi
	pop	edi
	pop	esi
	ret	4
??_Gbofstream@@UAEPAXI@Z ENDP				; bofstream::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0bofstream@@QAE@PBDHH@Z			; bofstream::bofstream
EXTRN	??0ofstream@@QAE@PBDHH@Z:NEAR			; ofstream::ofstream
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bofstream.cpp
xdata$x	SEGMENT
$T2279	DD	019930520H
	DD	02H
	DD	FLAT:$T2281
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2281	DD	0ffffffffH
	DD	FLAT:$L2274
	DD	00H
	DD	FLAT:$L2276
xdata$x	ENDS
;	COMDAT ??0bofstream@@QAE@PBDHH@Z
_TEXT	SEGMENT
__name$ = 8
__ios$ = 12
__filebuf$ = 16
_$initVBases$ = 20
_this$ = -16
$T2273 = -20
__$EHRec$ = -12
??0bofstream@@QAE@PBDHH@Z PROC NEAR			; bofstream::bofstream, COMDAT

; 20   :                       bfstream() {

	push	-1
	push	$L2280
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2273[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2106
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bofstream@@7B@ ; bofstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2273[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2106:
	mov	ecx, DWORD PTR __ios$[esp+20]
	mov	eax, DWORD PTR __filebuf$[esp+20]
	mov	edx, DWORD PTR __name$[esp+20]
	push	0
	or	cl, -128				; ffffff80H
	push	eax
	push	ecx
	push	edx
	mov	ecx, esi
	call	??0ofstream@@QAE@PBDHH@Z		; ofstream::ofstream
	lea	ecx, DWORD PTR [esi+8]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	eax, DWORD PTR [esi]
	mov	ecx, DWORD PTR [eax+4]

; 21   :   ;
; 22   : }

	mov	eax, esi
	mov	DWORD PTR [ecx+esi], OFFSET FLAT:??_7bofstream@@6B@ ; bofstream::`vftable'
	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2274:
	mov	eax, DWORD PTR $T2273[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2275
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2275:
	ret	0
$L2276:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1ofstream@@UAE@XZ			; ofstream::~ofstream
$L2280:
	mov	eax, OFFSET FLAT:$T2279
	jmp	___CxxFrameHandler
text$x	ENDS
??0bofstream@@QAE@PBDHH@Z ENDP				; bofstream::bofstream
PUBLIC	??0bofstream@@QAE@H@Z				; bofstream::bofstream
EXTRN	??0ofstream@@QAE@H@Z:NEAR			; ofstream::ofstream
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bofstream.cpp
xdata$x	SEGMENT
$T2293	DD	019930520H
	DD	03H
	DD	FLAT:$T2295
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2295	DD	0ffffffffH
	DD	FLAT:$L2288
	DD	00H
	DD	FLAT:$L2290
	DD	01H
	DD	FLAT:$L2291
xdata$x	ENDS
;	COMDAT ??0bofstream@@QAE@H@Z
_TEXT	SEGMENT
__filedesc$ = 8
_$initVBases$ = 12
_this$ = -16
$T2287 = -20
__$EHRec$ = -12
??0bofstream@@QAE@H@Z PROC NEAR				; bofstream::bofstream, COMDAT

; 27   :                       bfstream() {

	push	-1
	push	$L2294
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2287[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2112
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bofstream@@7B@ ; bofstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2287[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2112:
	mov	eax, DWORD PTR __filedesc$[esp+20]
	push	0
	push	eax
	mov	ecx, esi
	call	??0ofstream@@QAE@H@Z			; ofstream::ofstream
	lea	ecx, DWORD PTR [esi+8]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	ecx, DWORD PTR [esi]
	mov	BYTE PTR __$EHRec$[esp+32], 2
	mov	edx, DWORD PTR [ecx+4]

; 28   :   B_Init();

	mov	ecx, esi
	mov	DWORD PTR [edx+esi], OFFSET FLAT:??_7bofstream@@6B@ ; bofstream::`vftable'
	call	?B_Init@bofstream@@QAEXXZ		; bofstream::B_Init

; 29   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	mov	eax, esi
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2288:
	mov	eax, DWORD PTR $T2287[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2289
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2289:
	ret	0
$L2290:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1ofstream@@UAE@XZ			; ofstream::~ofstream
$L2291:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1bfstream@@QAE@XZ			; bfstream::~bfstream
$L2294:
	mov	eax, OFFSET FLAT:$T2293
	jmp	___CxxFrameHandler
text$x	ENDS
??0bofstream@@QAE@H@Z ENDP				; bofstream::bofstream
PUBLIC	??0bofstream@@QAE@HPADH@Z			; bofstream::bofstream
EXTRN	??0ofstream@@QAE@HPADH@Z:NEAR			; ofstream::ofstream
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bofstream.cpp
xdata$x	SEGMENT
$T2307	DD	019930520H
	DD	03H
	DD	FLAT:$T2309
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2309	DD	0ffffffffH
	DD	FLAT:$L2302
	DD	00H
	DD	FLAT:$L2304
	DD	01H
	DD	FLAT:$L2305
xdata$x	ENDS
;	COMDAT ??0bofstream@@QAE@HPADH@Z
_TEXT	SEGMENT
__filedesc$ = 8
_name$ = 12
__value$ = 16
_$initVBases$ = 20
_this$ = -16
$T2301 = -20
__$EHRec$ = -12
??0bofstream@@QAE@HPADH@Z PROC NEAR			; bofstream::bofstream, COMDAT

; 35   :                       bfstream() {

	push	-1
	push	$L2308
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	eax, DWORD PTR _$initVBases$[esp+16]
	push	esi
	mov	esi, ecx
	mov	DWORD PTR $T2301[esp+24], 0
	test	eax, eax
	mov	DWORD PTR _this$[esp+24], esi
	je	SHORT $L2120
	lea	ecx, DWORD PTR [esi+12]
	mov	DWORD PTR [esi], OFFSET FLAT:??_8bofstream@@7B@ ; bofstream::`vbtable'
	call	??0ios@@IAE@XZ				; ios::ios
	mov	DWORD PTR $T2301[esp+24], 1
	mov	DWORD PTR __$EHRec$[esp+32], 0
$L2120:
	mov	eax, DWORD PTR __value$[esp+20]
	mov	ecx, DWORD PTR _name$[esp+20]
	mov	edx, DWORD PTR __filedesc$[esp+20]
	push	0
	push	eax
	push	ecx
	push	edx
	mov	ecx, esi
	call	??0ofstream@@QAE@HPADH@Z		; ofstream::ofstream
	lea	ecx, DWORD PTR [esi+8]
	mov	DWORD PTR __$EHRec$[esp+32], 1
	call	??0bfstream@@QAE@XZ			; bfstream::bfstream
	mov	eax, DWORD PTR [esi]
	mov	BYTE PTR __$EHRec$[esp+32], 2
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+esi], OFFSET FLAT:??_7bofstream@@6B@ ; bofstream::`vftable'

; 36   :   B_Init();

	mov	ecx, esi
	call	?B_Init@bofstream@@QAEXXZ		; bofstream::B_Init

; 37   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+24]
	mov	eax, esi
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2302:
	mov	eax, DWORD PTR $T2301[ebp]
	and	eax, 1
	test	eax, eax
	je	$L2303
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	jmp	??1ios@@UAE@XZ				; ios::~ios
$L2303:
	ret	0
$L2304:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1ofstream@@UAE@XZ			; ofstream::~ofstream
$L2305:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	jmp	??1bfstream@@QAE@XZ			; bfstream::~bfstream
$L2308:
	mov	eax, OFFSET FLAT:$T2307
	jmp	___CxxFrameHandler
text$x	ENDS
??0bofstream@@QAE@HPADH@Z ENDP				; bofstream::bofstream
PUBLIC	?setmode@bofstream@@QAEHH@Z			; bofstream::setmode
;	COMDAT ?B_Init@bofstream@@QAEXXZ
_TEXT	SEGMENT
?B_Init@bofstream@@QAEXXZ PROC NEAR			; bofstream::B_Init, COMDAT

; 44   :   setmode( binary );

	push	128					; 00000080H
	call	?setmode@bofstream@@QAEHH@Z		; bofstream::setmode

; 45   : }

	ret	0
?B_Init@bofstream@@QAEXXZ ENDP				; bofstream::B_Init
_TEXT	ENDS
;	COMDAT xdata$x
; File E:\TMPro_SourceTree\Projects\Portable\bfstream\bofstream.cpp
xdata$x	SEGMENT
$T2325	DD	019930520H
	DD	01H
	DD	FLAT:$T2327
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T2327	DD	0ffffffffH
	DD	FLAT:$L2320
xdata$x	ENDS
;	COMDAT ??1bofstream@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1bofstream@@UAE@XZ PROC NEAR				; bofstream::~bofstream, COMDAT

; 49   : bofstream::~bofstream() {

	push	-1
	push	$L2326
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	esi, ecx
	mov	DWORD PTR _this$[esp+20], esi
	mov	eax, DWORD PTR [esi-12]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+esi-12], OFFSET FLAT:??_7bofstream@@6B@ ; bofstream::`vftable'

; 50   :   ;
; 51   : }

	lea	edx, DWORD PTR [esi-12]
	xor	ecx, ecx
	test	edx, edx
	mov	DWORD PTR __$EHRec$[esp+28], ecx
	je	SHORT $L2319
	lea	ecx, DWORD PTR [esi-4]
$L2319:
	call	??1bfstream@@QAE@XZ			; bfstream::~bfstream
	lea	ecx, DWORD PTR [esi-4]
	mov	DWORD PTR __$EHRec$[esp+28], -1
	call	??1ofstream@@UAE@XZ			; ofstream::~ofstream
	mov	ecx, DWORD PTR __$EHRec$[esp+20]
	pop	esi
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L2320:
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 4
	jmp	??1ofstream@@UAE@XZ			; ofstream::~ofstream
$L2326:
	mov	eax, OFFSET FLAT:$T2325
	jmp	___CxxFrameHandler
text$x	ENDS
??1bofstream@@UAE@XZ ENDP				; bofstream::~bofstream
EXTRN	?setmode@filebuf@@QAEHH@Z:NEAR			; filebuf::setmode
;	COMDAT ?setmode@bofstream@@QAEHH@Z
_TEXT	SEGMENT
?setmode@bofstream@@QAEHH@Z PROC NEAR			; bofstream::setmode, COMDAT

; 58   :   return( ofstream::setmode( binary ) );

	mov	eax, DWORD PTR [ecx]
	push	128					; 00000080H
	mov	edx, DWORD PTR [eax+4]
	mov	ecx, DWORD PTR [edx+ecx+4]
	call	?setmode@filebuf@@QAEHH@Z		; filebuf::setmode

; 59   : }

	ret	4
?setmode@bofstream@@QAEHH@Z ENDP			; bofstream::setmode
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@D@Z			; bofstream::operator<<
EXTRN	?put@ostream@@QAEAAV1@E@Z:NEAR			; ostream::put
;	COMDAT ??6bofstream@@QAEAAV0@D@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@D@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 68   :   (*this).put( var );

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	eax
	call	?put@ostream@@QAEAAV1@E@Z		; ostream::put

; 69   :   return *this;

	mov	eax, esi
	pop	esi

; 70   : }

	ret	4
??6bofstream@@QAEAAV0@D@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@E@Z			; bofstream::operator<<
;	COMDAT ??6bofstream@@QAEAAV0@E@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@E@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 75   :   (*this).put( var );

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	eax
	call	?put@ostream@@QAEAAV1@E@Z		; ostream::put

; 76   :   return *this;

	mov	eax, esi
	pop	esi

; 77   : }

	ret	4
??6bofstream@@QAEAAV0@E@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@C@Z			; bofstream::operator<<
;	COMDAT ??6bofstream@@QAEAAV0@C@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@C@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 82   :   (*this).put( var );

	mov	eax, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	eax
	call	?put@ostream@@QAEAAV1@E@Z		; ostream::put

; 83   :   return *this;

	mov	eax, esi
	pop	esi

; 84   : }

	ret	4
??6bofstream@@QAEAAV0@C@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@PBD@Z			; bofstream::operator<<
EXTRN	?write@ostream@@QAEAAV1@PBDH@Z:NEAR		; ostream::write
;	COMDAT ??6bofstream@@QAEAAV0@PBD@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@PBD@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 89   :   int len = strlen( var );

	mov	edx, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	edi
	mov	edi, edx
	or	ecx, -1
	xor	eax, eax
	repne scasb
	not	ecx
	dec	ecx

; 90   :   write( var, len );                   // write out the string

	push	ecx
	push	edx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 91   :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 92   : }

	ret	4
??6bofstream@@QAEAAV0@PBD@Z ENDP			; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@PBE@Z			; bofstream::operator<<
;	COMDAT ??6bofstream@@QAEAAV0@PBE@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@PBE@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 97   :   int len = strlen( (const char *)var );

	mov	edx, DWORD PTR _var$[esp-4]
	push	esi
	mov	esi, ecx
	push	edi
	mov	edi, edx
	or	ecx, -1
	xor	eax, eax
	repne scasb
	not	ecx
	dec	ecx

; 98   :   write( var, len );                   // write out the string

	push	ecx
	push	edx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 99   :   return *this;

	mov	eax, esi
	pop	edi
	pop	esi

; 100  : }

	ret	4
??6bofstream@@QAEAAV0@PBE@Z ENDP			; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@F@Z			; bofstream::operator<<
EXTRN	?ConvertEndian@bfstream@@IAEXAAF@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??6bofstream@@QAEAAV0@F@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@F@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 105  : bofstream & bofstream::operator<<( short var ) {

	push	esi
	mov	esi, ecx

; 106  :   ConvertEndian( var );                // Change the endian, if needed

	lea	eax, DWORD PTR _var$[esp]
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	call	?ConvertEndian@bfstream@@IAEXAAF@Z	; bfstream::ConvertEndian

; 107  :   write( (char *)&var, 2 );            // write out the value

	lea	ecx, DWORD PTR _var$[esp]
	push	2
	push	ecx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 108  :   return *this;

	mov	eax, esi
	pop	esi

; 109  : }

	ret	4
??6bofstream@@QAEAAV0@F@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@G@Z			; bofstream::operator<<
EXTRN	?ConvertEndian@bfstream@@IAEXAAG@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??6bofstream@@QAEAAV0@G@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@G@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 114  : bofstream & bofstream::operator<<( unsigned short var ) {

	push	esi
	mov	esi, ecx

; 115  :   ConvertEndian( var );                // Change the endian, if needed

	lea	eax, DWORD PTR _var$[esp]
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	call	?ConvertEndian@bfstream@@IAEXAAG@Z	; bfstream::ConvertEndian

; 116  :   write( (char *)&var, 2 );            // write out the value

	lea	ecx, DWORD PTR _var$[esp]
	push	2
	push	ecx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 117  :   return *this;

	mov	eax, esi
	pop	esi

; 118  : }

	ret	4
??6bofstream@@QAEAAV0@G@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@J@Z			; bofstream::operator<<
EXTRN	?ConvertEndian@bfstream@@IAEXAAJ@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??6bofstream@@QAEAAV0@J@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@J@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 123  : bofstream & bofstream::operator<<( long var ) {

	push	esi
	mov	esi, ecx

; 124  :   ConvertEndian( var );                // Change the endian, if needed

	lea	eax, DWORD PTR _var$[esp]
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	call	?ConvertEndian@bfstream@@IAEXAAJ@Z	; bfstream::ConvertEndian

; 125  :   write( (char *)&var, 4 );            // write out the value

	lea	ecx, DWORD PTR _var$[esp]
	push	4
	push	ecx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 126  :   return *this;

	mov	eax, esi
	pop	esi

; 127  : }

	ret	4
??6bofstream@@QAEAAV0@J@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@K@Z			; bofstream::operator<<
EXTRN	?ConvertEndian@bfstream@@IAEXAAK@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??6bofstream@@QAEAAV0@K@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@K@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 132  : bofstream & bofstream::operator<<( unsigned long var ) {

	push	esi
	mov	esi, ecx

; 133  :   ConvertEndian( var );                // Change the endian, if needed

	lea	eax, DWORD PTR _var$[esp]
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	call	?ConvertEndian@bfstream@@IAEXAAK@Z	; bfstream::ConvertEndian

; 134  :   write( (char *)&var, 4 );            // write out the value

	lea	ecx, DWORD PTR _var$[esp]
	push	4
	push	ecx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 135  :   return *this;

	mov	eax, esi
	pop	esi

; 136  : }

	ret	4
??6bofstream@@QAEAAV0@K@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@M@Z			; bofstream::operator<<
EXTRN	?ConvertEndian@bfstream@@IAEXAAM@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??6bofstream@@QAEAAV0@M@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@M@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 141  : bofstream & bofstream::operator<<( float var ) {

	push	esi
	mov	esi, ecx

; 142  :   ConvertEndian( var );                // Change the endian, if needed

	lea	eax, DWORD PTR _var$[esp]
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	call	?ConvertEndian@bfstream@@IAEXAAM@Z	; bfstream::ConvertEndian

; 143  :   write( (char *)&var, 4 );            // write out the value

	lea	ecx, DWORD PTR _var$[esp]
	push	4
	push	ecx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 144  :   return *this;

	mov	eax, esi
	pop	esi

; 145  : }

	ret	4
??6bofstream@@QAEAAV0@M@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@N@Z			; bofstream::operator<<
EXTRN	?ConvertEndian@bfstream@@IAEXAAN@Z:NEAR		; bfstream::ConvertEndian
;	COMDAT ??6bofstream@@QAEAAV0@N@Z
_TEXT	SEGMENT
_var$ = 8
??6bofstream@@QAEAAV0@N@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 150  : bofstream & bofstream::operator<<( double var ) {

	push	esi
	mov	esi, ecx

; 151  :   ConvertEndian( var );               // Change the endian, if needed

	lea	eax, DWORD PTR _var$[esp]
	push	eax
	lea	ecx, DWORD PTR [esi+8]
	call	?ConvertEndian@bfstream@@IAEXAAN@Z	; bfstream::ConvertEndian

; 152  :   write( (char *)&var, 8 );           // write out the value

	lea	ecx, DWORD PTR _var$[esp]
	push	8
	push	ecx
	mov	ecx, esi
	call	?write@ostream@@QAEAAV1@PBDH@Z		; ostream::write

; 153  :   return *this;

	mov	eax, esi
	pop	esi

; 154  : }

	ret	8
??6bofstream@@QAEAAV0@N@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@H@Z			; bofstream::operator<<
;	COMDAT ??6bofstream@@QAEAAV0@H@Z
_TEXT	SEGMENT
??6bofstream@@QAEAAV0@H@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 159  : bofstream & bofstream::operator<<( int var ) {

	mov	eax, ecx

; 160  :   // As int sizes are platform specific, they are not supported.  Cast ints to
; 161  :   //  longs or shorts instead.
; 162  :   assert( 0 ); // DON'T WRITE OUT INTS!!!
; 163  :   return *this;
; 164  : }

	ret	4
??6bofstream@@QAEAAV0@H@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
PUBLIC	??6bofstream@@QAEAAV0@I@Z			; bofstream::operator<<
;	COMDAT ??6bofstream@@QAEAAV0@I@Z
_TEXT	SEGMENT
??6bofstream@@QAEAAV0@I@Z PROC NEAR			; bofstream::operator<<, COMDAT

; 169  : bofstream & bofstream::operator<<( unsigned int var ) {

	mov	eax, ecx

; 170  :   // As int sizes are platform specific, they are not supported.  Cast ints to
; 171  :   //  longs or shorts instead.
; 172  :   assert( 0 ); // DON'T WRITE OUT UNSIGNED INTS!!!
; 173  :   return *this;
; 174  : }

	ret	4
??6bofstream@@QAEAAV0@I@Z ENDP				; bofstream::operator<<
_TEXT	ENDS
END
