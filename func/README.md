
//1)в gdb находим адрес инструкции "Access granted!"

Нужные нам строчки

0x00000000004011bd <+39>:    jne    0x4011d8 <main+66>
0x00000000004011d8 <+66>:    lea    0xe43(%rip),%rax  # "Access granted!"


//2)Нужно найти адрес возврата функции

Dump of assembler code for function main:
   0x0000000000401196 <+0>:     endbr64
   0x000000000040119a <+4>:     push   %rbp
   0x000000000040119b <+5>:     mov    %rsp,%rbp
   0x000000000040119e <+8>:     sub    $0x10,%rsp
=> 0x00000000004011a2 <+12>:    lea    0xe5b(%rip),%rax        # 0x402004
   0x00000000004011a9 <+19>:    mov    %rax,%rdi
   0x00000000004011ac <+22>:    call   0x401070 <puts@plt>
   0x00000000004011b1 <+27>:    call   0x4011ee <IsPassOk>
   0x00000000004011b6 <+32>:    mov    %eax,-0x4(%rbp)
   0x00000000004011b9 <+35>:    cmpl   $0x0,-0x4(%rbp)
   0x00000000004011bd <+39>:    jne    0x4011d8 <main+66>
   0x00000000004011bf <+41>:    lea    0xe4e(%rip),%rax        # 0x402014
   0x00000000004011c6 <+48>:    mov    %rax,%rdi
   0x00000000004011c9 <+51>:    call   0x401070 <puts@plt>
   0x00000000004011ce <+56>:    mov    $0x1,%edi
   0x00000000004011d3 <+61>:    call   0x4010a0 <exit@plt>
   0x00000000004011d8 <+66>:    lea    0xe43(%rip),%rax        # 0x402022
   0x00000000004011df <+73>:    mov    %rax,%rdi
   0x00000000004011e2 <+76>:    call   0x401070 <puts@plt>
   0x00000000004011e7 <+81>:    mov    $0x0,%eax
   0x00000000004011ec <+86>:    leave
   0x00000000004011ed <+87>:    ret
End of assembler dump.



//Вывели 16 байт регистра rb

(gdb) x/16xb $rbp
0x7fffffffda90: 0xb0 0xda 0xff 0xff 0xff 0x7f 0x00 0x00  # RBP
0x7fffffffda98: 0xb6 0x11 0x40 0x00 0x00 0x00 0x00 0x00  # Оригинальный Ret Address

rbp + 8    # Нужный нам адресс 0x4011b6 
rbp        # RBP
rbp - 12   # Массив Pass <- вот тут



0x4011b6 нужно заменить на 0x4011d8 который мы нашли, при этом дописать еще 20 байт:

AAAAAAAAAAAAAAAAAAAA - 20 байт мусора (A = \x41..) 00 00 00 00 00 40 11 b6 - перезаписываем первые 4 байта (\xda\x11\x40\x00)

Итоговая строка: AAAAAAAAAAAAAAAAAAAA\xd8\x11\x40\x00\x00\x00\x00\x00


//3)Результат работы

make payload 
./main < payload.bin  


grogu@localhost:~/eltex/embedded_c/eltex_course_embedded/func$ make payload 
/usr/bin/printf "AAAAAAAAAAAAAAAAAAAA\xd8\x11\x40\x00\x00\x00\x00\x00" > payload.bin
grogu@localhost:~/eltex/embedded_c/eltex_course_embedded/func$ ./main < payload.bin 
Enter password:
Access granted!
Ошибка шины (образ памяти сброшен на диск)
grogu@localhost:~/eltex/embedded_c/eltex_course_embedded/func$
