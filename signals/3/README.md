
gcc main.c -o main
gcc kill.c -o my_kill
./main

//первый способ
pgrep main | ./my_kill 


//второй способ
./my_kill $(pgrep main)


//третий способ
kill -SIGUSR1 $(pgrep main)
