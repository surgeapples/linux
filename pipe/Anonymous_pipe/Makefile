.PHONY:all
all:pipe demo

pipe:pipe.cpp
	g++ -o $@ $^ -std=c++11
demo:demo.cpp
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f pipe demo
