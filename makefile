release: 
	g++ -Wall -Wextra -Werror -o NAMemoryDiag.o -c NAMemoryDiag.cpp
	g++ -Wall -Wextra -Werror -o NAOverWriteMemory.o -c NAOverWriteMemory.cpp
	g++ -Wall -Wextra -Werror -o NABasicObject.o -c NABasicObject.cpp
	g++ -Wall -Wextra -Werror NAMemoryDiag.o NAOverWriteMemory.o NABasicObject.o -o memleak main.cpp

debug: 
	g++ -g -Wall -Wextra -Werror -o NAMemoryDiag.o -c NAMemoryDiag.cpp
	g++ -g -Wall -Wextra -Werror -o NAOverWriteMemory.o -c NAOverWriteMemory.cpp
	g++ -g -Wall -Wextra -Werror -o NABasicObject.o -c NABasicObject.cpp
	g++ -g -Wall -Wextra -Werror NAMemoryDiag.o NAOverWriteMemory.o NABasicObject.o -o memleak main.cpp

clean:
	rm -rf *.o ./*~ ./\#*\# ./.\#* memleak
