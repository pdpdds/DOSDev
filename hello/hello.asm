.model small
.stack

.data

    msg db "Hello World!", 13, 10, "$"

.code

start:
    mov ax, @data
    mov ds, ax              ; set DS to point to data segment
    mov dx, offset msg      ; point to the string
    mov ah, 09h             ; function to print the string
    int 21h                 ; execute the function

    mov ax, 4C00h           ; function to terminate the program
    int 21h                 ; execute
end start