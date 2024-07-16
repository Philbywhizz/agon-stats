; stats.inc
;
; Phil Howlett <phil.g.howlett@gmail.com>
; https://github.com/Philbywhizz/agon-stats
;
; Last updated: July 2024
;
; This routine will dump all the registers to the display
;
; To use simply use the .STATS macro anywhere in your code and 
; a display of the curret registers will be displayed to the console

; Example:
;
;       include 'stats.inc'
;
;       ...
;       nop
;       .STATS
;       nop
;       ...

; Will output the the VDP:
; PC=xxxxxx A=xx HL=xxxxxx BC=xxxxxx DE=xxxxxx IX=xxxxxx IY=xxxxxx SP=xxxxxx


ST_STATS:

    macro .STATS
        push af     ; Lets save all the registers to the stack
        push hl
        push bc
        push de
        push ix
        push iy

        call ST_STATS

        pop iy      ; Restore all the registers back to what they were
        pop ix
        pop de
        pop bc
        pop hl
        pop af
    endmacro

    ; current return stack mapping after the macro is called
    ;
    ; 02 01 00 - Calling return location
    ; 05 04 03 - IY
    ; 08 07 06 - IX
    ; 0B 0A 09 - DE
    ; 0E 0D 0C - BC
    ; 11 10 0F - HL
    ; 12 13    - AF (F=12, A=13)

    ; use IX as the register pointing to the stack
    ld ix, 0
    add ix, sp
    ld (ST_org_SP), sp

    ; Calculate PC from the return stack
    ld hl, (ix)             ; load hl with the context of ix
    ld bc, $fffff4          ; Sub eleven ($0b) from current PC to 
    add hl, bc              ; get the calling PC at DUMPSTACK position
    ld (ST_org_PC), hl      ; Save this value for later

       ; Display 'PC' Label
    ld hl, ST_txt_PC
    ld bc, $00
    ld a, $00
    rst.lil $18

    ld a, (ST_org_PC+2)
    call @printHexByte
    ld a, (ST_org_PC+1)
    call @printHexByte
    ld a, (ST_org_PC)
    call @printHexByte

    ; Display 'A' label
    ld hl, ST_txt_A
    ld bc, $00
    ld a, $00
    rst.lil $18

    ; Display 'A' register value
    ld a, (ix+$13)
    call @printHexByte

    ; Display 'HL' label
    ld hl, ST_txt_HL
    ld bc, $00
    ld a, $00
    rst.lil $18

    ; Display 'HL' register value
    ld a, (ix+$11)
    call @printHexByte
    ld a, (ix+$10)
    call @printHexByte
    ld a, (ix+$0F)
    call @printHexByte

    ; Display 'BC' label
    ld hl, ST_txt_BC
    ld bc, $00
    ld a, $00
    rst.lil $18

    ; Display 'BC' register value
    ld a, (ix+$0E)
    call @printHexByte
    ld a, (ix+$0D)
    call @printHexByte
    ld a, (ix+$0C)
    call @printHexByte

    ; Display 'DE' label
    ld hl, ST_txt_DE
    ld bc, $00
    ld a, $00
    rst.lil $18

    ; Display 'DE' register value
    ld a, (ix+$0B)
    call @printHexByte
    ld a, (ix+$0A)
    call @printHexByte
    ld a, (ix+$09)
    call @printHexByte
 
    ; Display 'IX' label
    ld hl, ST_txt_IX
    ld bc, $00
    ld a, $00
    rst.lil $18

    ; Display 'IX' register value
    ld a, (ix+$08)
    call @printHexByte
    ld a, (ix+$07)
    call @printHexByte
    ld a, (ix+$06)
    call @printHexByte
 
    ; Display 'IY' label
    ld hl, ST_txt_IY
    ld bc, $00
    ld a, $00
    rst.lil $18

    ; Display 'IY' register value
    ld a, (ix+$05)
    call @printHexByte
    ld a, (ix+$04)
    call @printHexByte
    ld a, (ix+$03)
    call @printHexByte

    ; Display 'SP' label
    ld hl, ST_txt_SP
    ld bc, $00
    ld a, $00
    rst.lil $18
   
    ; Display 'SP' register value
    ld a, (ST_org_SP+2)
    call @printHexByte
    ld a, (ST_org_SP+1)
    call @printHexByte
    ld a, (ST_org_SP)
    call @printHexByte

    ; Done
    ld hl, ST_txt_Done
    ld bc, $00
    ld a, $00
    rst.lil $18

    ret

;
; printByte
;
@printHexByte:  ; print the contents of A to the VDP as a HEX byte (2 chars)

    ; MSB
    ld (@tmp_byte), a   ; Make a backup of A
    and $f0             ; Get the higher nibble
    rra
    rra
    rra
    rra
    add a, $30          ; increase value to ascii code range 0-9
    cp $3a
    jr c, @printMSB
    add a, $07          ; add 7 to get A-F characters
@printMSB:
    rst.lil $10

    ; LSB
    ld a, (@tmp_byte)   
    and $0f             ; Get the lower nibble
    add a, $30          ; increase value to ascii code range 0-9
    cp $3a
    jr c, @printLSB
    add a, $07          ; add 7 to get A-F characters
@printLSB:
    rst.lil $10
    
    ret

@tmp_byte:
    byte $00

ST_org_PC:    dw24 $000000
ST_org_SP:    dw24 $000000

ST_txt_PC:    asciz "PC="
ST_txt_A:     asciz " A="
ST_txt_HL:    asciz " HL="
ST_txt_BC:    asciz " BC="
ST_txt_DE:    asciz " DE="
ST_txt_IX:    asciz " IX="
ST_txt_IY:    asciz " IY="
ST_txt_SP:    asciz " SP="
ST_txt_Done:  asciz "\r\n"
