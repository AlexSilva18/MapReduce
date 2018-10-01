all:mapreduce

mapreduce:mapreduce.cpp
	g++ main.cpp mapreduce.cpp -o mapreduce

clean:
	rm -rf mapreduce Makefile~ main.cpp~ mapreduce.cpp~
