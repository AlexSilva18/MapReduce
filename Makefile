all:mapreduce

mapreduce:mapreduce.cpp
	g++ -Wall -Werror -fsanitize=address main.cpp mapreduce.cpp framework.cpp -o mapred

clean:
	rm -rf mapred Makefile~ main.cpp~ mapreduce.cpp~ framework.cpp~ framework.h~ mapreduce.h~
