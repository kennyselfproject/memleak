release: memleak
	g++ -Wall -Wextra -Werror -o memleak memleak.cpp

debug: memleak
	g++ -g -Wall -Wextra -Werror -o memleak memleak.cpp

clean:
	rm -rf memleak.o memleak