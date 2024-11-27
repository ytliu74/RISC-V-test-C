.PHONY: all clean

all: gen
	./gen | tee data.txt

gen: test_mat_gen.cpp
	clang++ -std=c++11 -o gen test_mat_gen.cpp

clean:
	rm -f gen data.txt