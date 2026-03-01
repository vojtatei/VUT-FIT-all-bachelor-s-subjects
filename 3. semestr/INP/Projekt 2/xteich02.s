; Autor reseni: Vojtěch Teichmann xteich02
; Pocet cyklu k serazeni puvodniho retezce: 6919
; Pocet cyklu razeni sestupne serazeneho retezce: 7207
; Pocet cyklu razeni vzestupne serazeneho retezce: 6337
; Pocet cyklu razeni retezce s vasim loginem: 1371
; Implementovany radici algoritmus:bubble sort
; ------------------------------------------------

; DATA SEGMENT
                .data
;login:          .asciiz "vitejte-v-inp-2023"    ; puvodni uvitaci retezec
;login:          .asciiz "vvttpnjiiee3220---"  ; sestupne serazeny retezec
 ;login:          .asciiz "---0223eeiijnpttvv"  ; vzestupne serazeny retezec
 login:          .asciiz "xteich02"            ; SEM DOPLNTE VLASTNI LOGIN
                                                ; A POUZE S TIMTO ODEVZDEJTE

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize - "funkce" print_string)

; CODE SEGMENT
                .text
main:
    daddi   r4, r0, login            ; Nastavení adresy řetězce do r4
    jal     bubble_sort               ; Zavolání bubble sort
    jal     print_string              ; Vypsání seřazeného řetězce
    syscall 0                        ; Halt syscall

bubble_sort:

    ; Hlavní smyčka bubble sortu
    daddi   r1, r0, 0        ; 0 pro kontrolu zda jsme na konci řetězce
    loop_i:
        daddi   r2, r0, 0            ; j = 0
        daddi   r3, r0, 1            
        daddi   r8, r0, 0            
        loop_j:
            lb      r5, 0(r4)         ; Načtení znaku
            lb      r6, 1(r4)         ; Načtení následujícího znaku

            ; Porovnání r5 (aktuální znak) a r6 (následující znak)
            dsub    r7, r5, r6       ; r7 = r5 - r6

            beqz    r7, no_swap       ; Pokud r5 == r6, pokračujeme
            bgez    r7, swap          ; Pokud r5 >= r6, přeskočíme prohození

        no_swap:
            addi    r2, r2, 1        ; Zvýšení hodnoty pro kontrolu průchodu polem
            addi    r3, r3, 1        ; Zvýšení hodnoty pro kontrolu druhého znaku ve porovnání
            addi    r4, r4, 1        ; Posunutí na další znak v řetězci
            lb      r9, 1(r4)        ; Načtení znaku
            beqz    r9, compare_done ; Pokud jsme prošli celé pole, ukončíme vnitřní smyčku

            j       loop_j            ; Opakování vnitřní smyčky

        swap:
            sb      r6, 0(r4)        ; Uložení r6 na pozici aktuálního znaku
            sb      r5, 1(r4)        ; Uložení r5 na pozici následujícího znaku
            addi    r8, r8, 1        ; Zvýšení hodnoty pro kontrolu celého pole
            j       no_swap

    compare_done:
        addi    r1, r1, 1            ; Zvýšení hodnoty pro kontrolu celého pole
        daddi   r4, r0, login         ; Nastavení ukazatele na začátek řetězce
        sub     r7, r3, r1            ; Pokud r3 > r1, tak skok
        beqz    r7, end
        bgez    r7, loop_i
        beqz    r8, end
        addi    r8, r0, 0

    end:
        jr      r31                    ; Návrat              ; Return


print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
