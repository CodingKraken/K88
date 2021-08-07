org 0x8000           ; Indicates to the compiler to write the program starting from 0x8000

main:
    mov idy, 1       ; save 1 to y register
    jsr fibloop      ; jump to the main program loop
    hlt              ; Once exited from the loop, halt the processor

fibloop:
    mov idz, idy     ; move the value in y to z
    add idz, idx     ; add the contents of x to z
    mov idx, idy     ; move the value in y to x
    mov idy, idz     ; move the value in z to y

    cmp fgc, 1       ; Check if carry is set high
    jfs fgz, fibloop ; If not, return to the top of the loop
    rts              ; If so, exit from the loop

