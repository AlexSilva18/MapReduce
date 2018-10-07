all:mapreduce

mapreduce:mapreduce.cpp
	g++ -Wall -Werror -fsanitize=address -g -pthread main.cpp mapreduce.cpp framework.cpp -o mapred

clean:
	rm -rf mapred Makefile~ main.cpp~ mapreduce.cpp~ framework.cpp~ framework.h~ mapreduce.h~

#-Wall -Werror -fsanitize=address

#r --app wordcount --impl procs --maps 2 --reduces 2 --input word_input.txt --output word_output.txt

#r --app sort --impl procs --maps 2 --reduces 2 --input sort_input.csv --output sort_output.txt
