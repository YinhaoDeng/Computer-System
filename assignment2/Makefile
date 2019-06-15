# must specify make target
all: parser translator

# C++ 11
CXXFLAGS=--std=c++11 -I. -Iincludes

clean:
	rm -rf parser translator *.o

PMYCPP:=parser.cpp
TMYCPP:=translator.cpp
MYCPP:=${wildcard my*.cpp}
PMYO:=${PMYCPP:.cpp=.o}
TMYO:=${TMYCPP:.cpp=.o}
MYO:=${MYCPP:.cpp=.o}

parser: ${PMYO} ${MYO} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

translator: ${TMYO} ${MYO} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

test: test-parser test-translator test-parser-translator

test-parser: parser
	-@bash bin/run-tests parser loud

test-translator: translator
	-@bash bin/run-tests translator loud

test-parser-translator: parser translator
	-@bash bin/run-tests parser-translator loud

test-working: test-working-parser test-working-translator

test-working-parser:
	-@bash bin/run-tests working-parser loud

test-working-translator: 
	-@bash bin/run-tests working-translator loud

test-new: test-new-Pxml test-new-Pxml-hack

test-new-Pxml:
	-@bash bin/run-tests new-Pxml quiet

test-new-Pxml-hack:
	-@bash bin/run-tests new-Pxml-hack quiet

test-add: test-add-Pxml test-add-Pxml-hack

test-add-Pxml:
	-@bash bin/run-tests add-Pxml quiet

test-add-Pxml-hack:
	-@bash bin/run-tests add-Pxml-hack quiet

