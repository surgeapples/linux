.PHONY: all clean
all: signal killpid abort raise alarm exception sigset sigaction
signal: signal.cc
	g++ -o $@ $^ -std=c++11
sigaction: sigaction.cc
	g++ -o $@ $^ -std=c++11
killpid: kill.cc
	g++ -o $@ $^ -std=c++11
abort: abort.cc
	g++ -o $@ $^ -std=c++11
raise: raise.cc
	g++ -o $@ $^ -std=c++11
alarm: alarm.cc
	g++ -o $@ $^ -std=c++11
exception: exception.cc
	g++ -o $@ $^ -std=c++11 -g
sigset: sigset.cc
	g++ -o $@ $^ -std=c++11
clean:
	rm -f signal sigaction killpid abort raise alarm exception sigset