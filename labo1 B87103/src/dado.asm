;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (MINGW64)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"dado.c"
	list	p=12f683
	radix dec
	include "p12f683.inc"
;--------------------------------------------------------
; config word(s)
;--------------------------------------------------------
	__config 0x3ff7
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_super_seed
	global	_delay
	global	_generate_random_number

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0070
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
UD_dado_0	udata
_super_seed	res	2

;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_dado_0	udata
r0x100E	res	1
r0x100F	res	1
r0x1007	res	1
r0x1006	res	1
r0x1008	res	1
r0x1009	res	1
r0x100A	res	1
r0x100B	res	1
r0x100C	res	1
r0x100D	res	1
r0x1002	res	1
r0x1003	res	1
r0x1004	res	1
r0x1005	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------
;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_dado	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _generate_random_number
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _generate_random_number
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;3 compiler assigned registers:
;   r0x100E
;   r0x100F
;   STK00
;; Starting pCode block
S_dado__main	code
_main:
; 2 exit points
;	.line	14; "dado.c"	TRISIO = 0b00100000;		// Configura los pines GPIO como salidas excepto el pin 5
	MOVLW	0x20
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	15; "dado.c"	GPIO = 0x00;				// Establece todos los pines GPIO en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
;	.line	16; "dado.c"	super_seed = 0xA3;			// Valor inicial arbitrario del LFSR
	MOVLW	0xa3
	BANKSEL	_super_seed
	MOVWF	_super_seed
	CLRF	(_super_seed + 1)
_00121_DS_:
;	.line	23; "dado.c"	if (GP5 == 1) // Si se detecta una entrada alta en el pin GP5
	BANKSEL	r0x100E
	CLRF	r0x100E
	BANKSEL	_GPIObits
	BTFSS	_GPIObits,5
	GOTO	_00001_DS_
	BANKSEL	r0x100E
	INCF	r0x100E,F
_00001_DS_:
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00118_DS_
;	.line	25; "dado.c"	num_aleatorio = generate_random_number(); // Genera un número aleatorio
	PAGESEL	_generate_random_number
	CALL	_generate_random_number
	PAGESEL	$
	BANKSEL	r0x100F
	MOVWF	r0x100F
	MOVF	STK00,W
;	.line	28; "dado.c"	if (num_aleatorio == 1)
	MOVWF	r0x100E
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00106_DS_
	MOVF	r0x100F,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00106_DS_
;	.line	30; "dado.c"	GPIO = 0b00000100;
	MOVLW	0x04
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	31; "dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	32; "dado.c"	break;
	GOTO	_00123_DS_
_00106_DS_:
;	.line	34; "dado.c"	if (num_aleatorio == 2)
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	XORLW	0x02
	BTFSS	STATUS,2
	GOTO	_00108_DS_
	MOVF	r0x100F,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00108_DS_
;	.line	36; "dado.c"	GPIO = 0b00010000;
	MOVLW	0x10
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	37; "dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	38; "dado.c"	break;
	GOTO	_00123_DS_
_00108_DS_:
;	.line	40; "dado.c"	if (num_aleatorio == 3)
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	XORLW	0x03
	BTFSS	STATUS,2
	GOTO	_00110_DS_
	MOVF	r0x100F,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00110_DS_
;	.line	42; "dado.c"	GPIO = 0b00010100;
	MOVLW	0x14
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	43; "dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	44; "dado.c"	break;
	GOTO	_00123_DS_
_00110_DS_:
;	.line	46; "dado.c"	if (num_aleatorio == 4)
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	XORLW	0x04
	BTFSS	STATUS,2
	GOTO	_00112_DS_
	MOVF	r0x100F,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00112_DS_
;	.line	48; "dado.c"	GPIO = 0b00010001;
	MOVLW	0x11
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	49; "dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	50; "dado.c"	break;
	GOTO	_00123_DS_
_00112_DS_:
;	.line	52; "dado.c"	if (num_aleatorio == 5)
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	XORLW	0x05
	BTFSS	STATUS,2
	GOTO	_00114_DS_
	MOVF	r0x100F,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00114_DS_
;	.line	54; "dado.c"	GPIO = 0b00010101;
	MOVLW	0x15
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	55; "dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	56; "dado.c"	break;
	GOTO	_00123_DS_
_00114_DS_:
;	.line	58; "dado.c"	if (num_aleatorio == 6)
	BANKSEL	r0x100E
	MOVF	r0x100E,W
	XORLW	0x06
	BTFSS	STATUS,2
	GOTO	_00119_DS_
	MOVF	r0x100F,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00119_DS_
;	.line	60; "dado.c"	GPIO = 0b00010011;
	MOVLW	0x13
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	61; "dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	62; "dado.c"	break;
	GOTO	_00123_DS_
_00118_DS_:
;	.line	67; "dado.c"	GPIO = 0b00000000; // Si no se detecta una entrada alta en el pin GP5, establece todos los pines GPIO en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
_00119_DS_:
;	.line	69; "dado.c"	super_seed++; // Incrementa el estado del generador de números aleatorios
	BANKSEL	_super_seed
	INCF	_super_seed,F
	BTFSC	STATUS,2
	INCF	(_super_seed + 1),F
	GOTO	_00121_DS_
_00123_DS_:
;	.line	71; "dado.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;5 compiler assigned registers:
;   r0x1002
;   r0x1003
;   r0x1004
;   r0x1005
;   STK00
;; Starting pCode block
S_dado__generate_random_number	code
_generate_random_number:
; 2 exit points
;	.line	91; "dado.c"	uint16_t lsb = super_seed & 1;
	MOVLW	0x01
	BANKSEL	_super_seed
	ANDWF	_super_seed,W
	BANKSEL	r0x1004
	MOVWF	r0x1004
	MOVWF	r0x1002
	CLRF	r0x1003
;;99	MOVF	r0x1002,W
	MOVLW	0x00
	MOVWF	r0x1005
;;shiftRight_Left2ResultLit:5474: shCount=1, size=2, sign=1, same=1, offr=0
;	.line	92; "dado.c"	super_seed >>= 1;
	BCF	STATUS,0
	BANKSEL	(_super_seed + 1)
	BTFSC	(_super_seed + 1),7
	BSF	STATUS,0
	RRF	(_super_seed + 1),F
	RRF	_super_seed,F
;	.line	93; "dado.c"	if (lsb)
	BANKSEL	r0x1005
	MOVF	r0x1005,W
	IORWF	r0x1004,W
	BTFSC	STATUS,2
	GOTO	_00161_DS_
;	.line	95; "dado.c"	super_seed ^= LFSR_TAPS;
	MOVLW	0x05
	BANKSEL	_super_seed
	XORWF	_super_seed,F
	MOVLW	0x80
	XORWF	(_super_seed + 1),F
;;shiftRight_Left2ResultLit:5474: shCount=1, size=1, sign=1, same=0, offr=1
_00161_DS_:
;	.line	99; "dado.c"	int rand_num = ((super_seed >> 10) & 0x0007) + 1; // Toma 3 bits del estado del LFSR y suma 1
	BCF	STATUS,0
	BANKSEL	(_super_seed + 1)
	BTFSC	(_super_seed + 1),7
	BSF	STATUS,0
	RRF	(_super_seed + 1),W
	BANKSEL	r0x1002
	MOVWF	r0x1002
	CLRF	r0x1003
	BTFSC	r0x1002,7
	DECF	r0x1003,F
;;shiftRight_Left2ResultLit:5474: shCount=1, size=2, sign=1, same=1, offr=0
	BCF	STATUS,0
	BTFSC	r0x1003,7
	BSF	STATUS,0
	RRF	r0x1003,F
	RRF	r0x1002,F
	MOVLW	0x07
	ANDWF	r0x1002,F
	CLRF	r0x1003
	INCF	r0x1002,F
	BTFSC	STATUS,2
	INCF	r0x1003,F
;	.line	100; "dado.c"	return rand_num;								  // Retorna el número aleatorio generado
	MOVF	r0x1002,W
	MOVWF	STK00
	MOVF	r0x1003,W
;	.line	101; "dado.c"	}
	RETURN	
; exit point of _generate_random_number

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x1006
;   STK00
;   r0x1007
;   r0x1008
;   r0x1009
;   r0x100A
;   r0x100B
;   r0x100C
;   r0x100D
;; Starting pCode block
S_dado__delay	code
_delay:
; 2 exit points
;	.line	73; "dado.c"	void delay(unsigned int tiempo)
	BANKSEL	r0x1006
	MOVWF	r0x1006
	MOVF	STK00,W
	MOVWF	r0x1007
;	.line	79; "dado.c"	for (i = 0; i < tiempo; i++)
	CLRF	r0x1008
	CLRF	r0x1009
_00134_DS_:
	BANKSEL	r0x1006
	MOVF	r0x1006,W
	SUBWF	r0x1009,W
	BTFSS	STATUS,2
	GOTO	_00155_DS_
	MOVF	r0x1007,W
	SUBWF	r0x1008,W
_00155_DS_:
	BTFSC	STATUS,0
	GOTO	_00136_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;	.line	80; "dado.c"	for (j = 0; j < 1275; j++)
	MOVLW	0xfb
	BANKSEL	r0x100A
	MOVWF	r0x100A
	MOVLW	0x04
	MOVWF	r0x100B
_00132_DS_:
	MOVLW	0xff
	BANKSEL	r0x100A
	ADDWF	r0x100A,W
	MOVWF	r0x100C
	MOVLW	0xff
	MOVWF	r0x100D
	MOVF	r0x100B,W
	BTFSC	STATUS,0
	INCFSZ	r0x100B,W
	ADDWF	r0x100D,F
	MOVF	r0x100C,W
	MOVWF	r0x100A
	MOVF	r0x100D,W
	MOVWF	r0x100B
	MOVF	r0x100D,W
	IORWF	r0x100C,W
	BTFSS	STATUS,2
	GOTO	_00132_DS_
;	.line	79; "dado.c"	for (i = 0; i < tiempo; i++)
	INCF	r0x1008,F
	BTFSC	STATUS,2
	INCF	r0x1009,F
	GOTO	_00134_DS_
_00136_DS_:
;	.line	82; "dado.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  195+   46 =   241 instructions (  574 byte)

	end
