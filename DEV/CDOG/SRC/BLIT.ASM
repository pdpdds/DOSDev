
_TEXT        segment byte public 'CODE'
             assume CS:_TEXT

             public DrawPic_
             public DrawTPic_
             public DrawTTPic_
             public DrawBTPic_
             public CopyToScreen_
             public SetPalette_
             public SetClip_
             public SetDstScreen_
             public GetDstScreen_
             public AltScrCopy_


; Clip rectangle

clip_Left    dd    0
clip_Top     dd    0
clip_Right   dd    319
clip_Bottom  dd    199

dstScreen    dd    0a0000h

compiled     dd    0
rleData      dd    0
clipped      dw    0

spriteWidth  dw    0
spriteHeight dw    0


doClip:      mov [clipped],0
             mov ax,[esi]
             mov [spriteWidth],ax
             add esi,2
             mov ax,[esi]
             mov [spriteHeight],ax
             add esi,2
             xor edx,edx
             mov eax,[clip_Top]
             cmp eax,ecx
             jg clipUpper

testLower:   mov eax,ecx
             add ax,[spriteHeight]
             dec ax
             cmp eax,[clip_Bottom]
             jg clipLower

testLeft:    mov eax,[clip_Left]
             cmp eax,ebx
             jg clipLeft

testRight:   mov eax,ebx
             add ax,[spriteWidth]
             dec ax
             cmp eax,[clip_Right]
             jg clipRight

clipDone:    stc
             ret

done:        clc
             ret

clipUpper:   sub eax,ecx
             mov ecx,eax
             xor eax,eax
             mov ax,[spriteHeight]
             sub eax,ecx
             jle done
             mov [spriteHeight],ax
             mov ax,[spriteWidth]
             mul ecx
             add esi,eax
             mov ecx,[clip_Top]
             mov [clipped],1
             jmp testLower

clipLower:   cmp ecx,[clip_Bottom]
             jg done
             sub eax,[clip_Bottom]
             sub [spriteHeight],ax
             mov [clipped],1
             jmp testLeft

clipLeft:    sub eax,ebx
             mov edx,eax
             xor eax,eax
             mov ax,[spriteWidth]
             sub eax,edx
             jle done
             mov [spriteWidth],ax
             add esi,edx
             mov ebx,[clip_Left]
             jmp testRight

clipRight:   cmp ebx,[clip_Right]
             jg done
             sub eax,[clip_Right]
             sub [spriteWidth],ax
             add edx,eax
             jmp clipDone


; entryPoint...
; ebx == x; ecx == y; esi -> bitmap
; Modifies eax, edx and edi

DrawPic_   proc near
           mov [compiled],eax
           call doClip
           jnc done
           lea eax,[ecx + 4*ecx]
           ;mov eax,ecx
           ;shl eax,2
           ;add eax,ecx
           shl eax,6
           add eax,ebx
           add eax,dstScreen
           mov edi,eax
           xor eax,eax
           mov ax,[spriteWidth]
           xor ecx,ecx
           mov cx,[spriteHeight]

           and edx,edx
           jnz copyClip

;           mov dx,[clipped]
;           and edx,edx
;           jnz copy

;           mov edx,[compiled]
;           and edx,edx
;           jnz callCode

copy:      cmp ax,16
           je copy16w
           test ax,3
           je copyDW
           test ax,1
           je copyW
           jmp copyB

callCode:  call edx
           ret

; Blit 16 pixels wide, unclipped
; ecx == number of scanlines
; esi -> bitmap data
; edi -> destination screen address

copy16w:   mov ax,[edi]
           movsd
           movsd
           movsd
           movsd
           add edi,304
           dec cx
           jnz copy16w
           ret

; Blit (horz) unclipped sprite with width a multiple of 4
; ecx == number of scanlines
; esi -> bitmap data
; edi -> destination screen address
; eax == width

copyDW:    mov edx,320
           sub edx,eax
           shr eax,2
           mov ebx,ecx
dwLine:    mov ecx,eax
           rep movsd
           add edi,edx
           dec ebx
           jnz dwLine
           ret

; Blit (horz) unclipped sprite with width a multiple of 2
; ecx == number of scanlines
; esi -> bitmap data
; edi -> destination screen address
; eax == width

copyW:     mov edx,320
           sub edx,eax
           shr eax,1
           mov ebx,ecx
wLine:     mov ecx,eax
           rep movsw
           add edi,edx
           dec ebx
           jnz wLine
           ret

; Blit (horz) unclipped sprite
; ecx == number of scanlines
; esi -> bitmap data
; edi -> destination screen address
; eax == width

copyB:     mov edx,320
           sub edx,eax
           mov ebx,ecx
bLine:     mov ecx,eax
           rep movsb
           add edi,edx
           dec ebx
           jnz bLine
           ret


; Blit (horz) clipped sprite
; ecx == number of scanlines
; esi -> start of bitmap data to copy
; edi -> destination screen address
; eax == width
; edx == width clipped (== offset to add to esi)


copyClip:  mov ebx,ecx
bcLine:    mov ecx,eax
           rep movsb
           add edi,320
           sub edi,eax
           add esi,edx
           dec ebx
           jnz bcLine
           ret

DrawPic_   endp

DrawTPic_  proc near
           mov [compiled],eax
           call doClip
           jnc done
           lea eax,[ecx + 4*ecx]
           ;mov eax,ecx
           ;shl eax,2
           ;add eax,ecx
           shl eax,6
           add eax,ebx
           add eax,dstScreen
           mov edi,eax
           xor eax,eax
           mov ax,[spriteWidth]
           mov cx,[spriteHeight]

           and edx,edx
           jnz copyTClip

           mov dx,[clipped]
           and edx,edx
           jnz copyTB

           mov edx,[compiled]
           and edx,edx
           jnz callCode

; Blit transparent (horz) unclipped sprite
; ecx == number of scanlines
; esi -> start of bitmap data to copy
; edi -> destination screen address
; eax == width

copyTB:    mov edx,eax
           mov ebx,ecx
tbLine:    mov ecx,edx
tbPixels:  mov al,[esi]
           inc esi
           cmp al,0
           jz tbNoSet
           mov [edi],al
tbNoSet:   inc edi
           dec cx
           jnz tbPixels
           ;loop tbPixels
           add edi,320
           sub edi,edx
           dec ebx
           jnz tbLine
           ret


; Blit transparent (horz) clipped sprite
; ecx == number of scanlines
; esi -> start of bitmap data to copy
; edi -> destination screen address
; eax == width
; edx == width clipped (== offset to add to esi)


copyTClip: mov ebx,ecx
           mov ecx,eax
           shl eax,8
tcbPixels: mov al,[esi]
           inc esi
           cmp al,0
           jz tcbNoSet
           mov [edi],al
tcbNoSet:  inc edi
           dec cx
           jnz tcbPixels
           ;loop tcbPixels
           add esi,edx
           mov ecx,eax
           shr ecx,8
           add edi,320
           sub edi,ecx
           dec ebx
           jnz tcbPixels
           ret

DrawTPic_  endp


; CopyToScreen
; no parameters

CopyToScreen_ proc near
              mov ecx,16000
              mov edi,0a0000h
              mov esi,[dstScreen]
              rep movsd
              ret
CopyToScreen_ endp


; AltScrCopy
; no parameters

AltScrCopy_   proc near
              mov ecx,16000
              mov edi,0a0000h
              mov esi,[dstScreen]
altLoop:      mov eax,[esi]
              add esi,4
              mov [edi],eax
              add edi,4
              dec ecx
              jnz altLoop
              ret
AltScrCopy_   endp


; SetPalette
; esi -> palette to set

SetPalette_   proc near
              mov dx,03c8h
              mov al,0
              out dx,al
              inc dx
              mov cx,256
palLoop:      outsb
              outsb
              outsb
              dec cx
              jnz palLoop
              ret
SetPalette_   endp


; DrawTTPic
; Draw pic through color translation table

DrawTTPic_ proc near
           push ebp
           mov [rleData],eax
           mov ebp,edi
           call doClip
           jnc doneBP
           lea eax,[ecx + 4*ecx]
           shl eax,6
           add eax,ebx
           add eax,dstScreen
           mov edi,eax
           xor eax,eax
           mov ax,[spriteWidth]
           mov cx,[spriteHeight]

           and edx,edx
           jnz copyTTClip

           mov dx,[clipped]
           and edx,edx
           jnz copyTTB

           mov edx,[rleData]
           and edx,edx
           jnz drawTRLE

; Blit transparent (horz) unclipped sprite
; ecx == number of scanlines
; esi -> start of bitmap data to copy
; edi -> destination screen address
; eax == width

copyTTB:   mov edx,eax
           xor eax,eax
           mov ebx,ecx
ttbLine:   mov ecx,edx
ttbPixels: mov al,[esi]
           inc esi
           cmp al,0
           jz ttbNoSet
           mov al,[ebp + eax]
           mov [edi],al

ttbNoSet:  inc edi
           dec cx
           jnz ttbPixels
           ;loop ttbPixels
           add edi,320
           sub edi,edx
           dec ebx
           jnz ttbLine
           pop ebp
           ret

copyTTClip:mov ebx,ecx
           mov ecx,eax
           push eax
           xor eax,eax
ttcbPixels:mov al,[esi]
           inc esi
           cmp al,0
           jz ttcbNoSet
           mov al,[ebp+eax]
           mov [edi],al
ttcbNoSet: inc edi
           dec cx
           jnz ttcbPixels
           ;loop ttcbPixels
           add esi,edx
           pop ecx
           push ecx
           add edi,320
           sub edi,ecx
           dec ebx
           jnz ttcbPixels
           pop ecx
           pop ebp
           ret

doneBP:    pop ebp
           ret
DrawTTPic_ endp


; DrawBTPic
; Draw background under pic through color translation table

DrawBTPic_ proc near
           push ebp
           mov [rleData],eax
           mov ebp,edi
           call doClip
           jnc doneBP
           lea eax,[ecx + 4*ecx]
           shl eax,6
           add eax,ebx
           add eax,dstScreen
           mov edi,eax
           xor eax,eax
           mov ax,[spriteWidth]
           mov cx,[spriteHeight]

           and edx,edx
           jnz copyBTClip

           mov dx,[clipped]
           and edx,edx
           jnz copyBTB

           mov edx,[rleData]
           and edx,edx
           jnz drawBTRLE

copyBTB:   mov edx,eax
           xor eax,eax
           mov ebx,ecx
btbLine:   mov ecx,edx
btbPixels: mov al,[esi]
           inc esi
           cmp al,0
           jz btbNoSet
           mov al,[edi]
           mov al,[ebp + eax]
           mov [edi],al

btbNoSet:  inc edi
           dec cx
           jnz btbPixels
           add edi,320
           sub edi,edx
           dec ebx
           jnz btbLine
           pop ebp
           ret

copyBTClip:mov ebx,ecx
           mov ecx,eax
           push eax
           xor eax,eax
btcbPixels:mov al,[esi]
           inc esi
           cmp al,0
           jz btcbNoSet
           mov al,[edi]
           mov al,[ebp+eax]
           mov [edi],al
btcbNoSet: inc edi
           dec cx
           jnz btcbPixels
           add esi,edx
           pop ecx
           push ecx
           add edi,320
           sub edi,ecx
           dec ebx
           jnz btcbPixels
           pop ecx
           pop ebp
           ret

DrawBTPic_ endp


; SetClip
; eax -> left, ebx -> top, ecx -> right, edx -> bottom
; set clipping rectangle used

SetClip_   proc near
           mov [clip_Left],eax
           mov [clip_Top],ebx
           mov [clip_Right],ecx
           mov [clip_Bottom],edx
           ret
SetClip_   endp


; SetDstScreen
;   esi -> memory buffer
; set screen buffer to draw to

SetDstScreen_  proc near
               mov [dstScreen],esi
               ret
SetDstScreen_  endp


; GetDstScreen
;   memory buffer -> eax
; get address of current offscreen buffer

GetDstScreen_  proc near
               mov eax,[dstScreen]
               ret
GetDstScreen_  endp



drawRLE:   mov esi,edx
           mov eax,0
           mov al,[esi]
           inc esi
           cmp al,0
           jz  rleSkip

rleCopy:   mov ecx,eax
           rep movsb

rleSkip:   mov ax,[esi]
           cmp eax,0
           jz  rleDone
           add esi,2
           add edi,eax
           mov al,[esi]
           inc esi
           jmp rleCopy

rleDone:   ret


drawTRLE:  mov esi,edx
           mov eax,0
           mov ecx,0
           mov al,[esi]
           inc esi
           cmp al,0
           jz  rleTSkip

rleTCopy:  mov cl,[esi]
           inc esi
           mov cl,[ebp + ecx]
           mov [edi],cl
           inc edi
           dec al
           jnz rleTCopy

rleTSkip:  mov ax,[esi]
           cmp eax,0
           jz  doneBP
           add esi,2
           add edi,eax
           mov al,[esi]
           inc esi
           jmp rleTCopy

drawBTRLE: mov esi,edx
           mov eax,0
           mov ecx,0
           mov al,[esi]
           inc esi
           cmp al,0
           jz  rleBTSkip

rleBTCopy: mov cl,[edi]
           mov cl,[ebp + ecx]
           mov [edi],cl
           inc esi
           inc edi
           dec al
           jnz rleBTCopy

rleBTSkip: mov ax,[esi]
           cmp eax,0
           jz  doneBP
           add esi,2
           add edi,eax
           mov al,[esi]
           inc esi
           jmp rleBTCopy


_TEXT      ends
           end
