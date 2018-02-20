release: 
	g++ -Wall -Wextra -Werror -o memleakdiag.o -c memleakdiag.cpp
	g++ -Wall -Wextra -Werror -o overwritemem.o -c overwritemem.cpp
	g++ -Wall -Wextra -Werror -o NABasicObject.o -c NABasicObject.cpp
	g++ -Wall -Wextra -Werror memleakdiag.o overwritemem.o NABasicObject.o -o memleak main.cpp

debug: 
	g++ -g -Wall -Wextra -Werror -o memleakdiag.o -c memleakdiag.cpp
	g++ -g -Wall -Wextra -Werror -o overwritemem.o -c overwritemem.cpp
	g++ -g -Wall -Wextra -Werror -o NABasicObject.o -c NABasicObject.cpp
	g++ -g -Wall -Wextra -Werror memleakdiag.o overwritemem.o NABasicObject.o -o memleak main.cpp

clean:
	rm -rf *.o ./*~ ./\#*\# ./.\#* memleak
