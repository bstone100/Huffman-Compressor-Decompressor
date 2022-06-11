all: zap unzap

zap: zap.cc huffman.h
	g++ -Wall -Werror -std=c++11 -o zap zap.cc

unzap: unzap.cc huffman.h
	g++ -Wall -Werror -std=c++11 -o unzap unzap.cc

test_pqueue: test_pqueue.cc pqueue.h
	g++ -Wall -Werror -std=c++11 -o test_pqueue test_pqueue.cc -pthread -lgtest

test_bstream: test_bstream.cc bstream.h
	g++ -Wall -Werror -std=c++11 -o test_bstream test_bstream.cc -pthread -lgtest

clean: 
	rm -rf test_pqueue test_bstream zap unzap *.zap *.unzap