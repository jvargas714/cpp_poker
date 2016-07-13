clear
clean.sh
cmake CMakeLists.txt
make
./main2 10 &> run_log.txt
gedit run_log.txt &

#figure out how to pass vars to sh scripts 

