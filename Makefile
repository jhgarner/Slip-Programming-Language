lisp: main.o context.o language.o
	g++ main.o context.o language.o -ledit -o lisp

main.o: main.cpp context.hpp
	g++ -g -I /usr/include -c main.cpp

context.o: context.hpp context.cpp language.hpp
	g++ -g -c context.cpp

language.o: language.hpp language.cpp
	g++ -g -c language.cpp

# scope.o: scope.hpp scope.cpp
# 	g++ -g -c scope.cpp

clean:
	rm language.o context.o main.o lisp
