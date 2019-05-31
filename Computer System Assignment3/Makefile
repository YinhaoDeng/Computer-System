# all the executables
EXECS= parser pretty codegen optimiser-r
TESTS=$(patsubst %,test-%,${EXECS})
MYALL:=${wildcard shared-*.cpp}
MYPARSER:=parser.cpp ${wildcard parser-*.cpp}
MYCODEGEN:=codegen.cpp ${wildcard codegen-*.cpp}
MYPRETTY:=pretty.cpp ${wildcard pretty-*.cpp}
MYOPTIMISER_R:=optimiser-r.cpp ${wildcard optimiser-r-*.cpp}

# C++ 11 just in case
CXXFLAGS=--std=c++11 -I. -Iincludes

all: ${EXECS}

test: ${TESTS}

help:
	-@echo make options are one of:
	-@echo " all, parser, codegen, pretty or optimiser-r"
	-@echo testing options are one of:
	-@echo " test-all, test-parser, test-codegen, test-pretty or test-optimiser-r"
	-@echo


# to test student versions of the programs
test-new:
	-@(bash bin/run-tests new loud)

test-parser: parser
	-@(bash bin/run-tests parser loud)

test-codegen: codegen
	-@(bash bin/run-tests codegen loud)

test-pretty: pretty
	-@(bash bin/run-tests pretty loud)

test-optimiser-r: optimiser-r
	-@(bash bin/run-tests optimiser-r loud)

# to run tests with working versions of the programs
test-working: test-working-parser test-working-codegen test-working-pretty test-working-optimiser-r

test-working-parser:
	-@(bash bin/run-tests working-parser loud)

test-working-codegen:
	-@(bash bin/run-tests working-codegen loud)

test-working-pretty:
	-@(bash bin/run-tests working-pretty loud)

test-working-optimiser-r:
	-@(bash bin/run-tests working-optimiser-r loud)

clean:
	-rm -rf ${EXECS} *.o

%.o: %.cpp %.h
	g++ ${CXXFLAGS} -c $<

parser: ${MYPARSER} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

codegen: ${MYCODEGEN} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

pretty: ${MYPRETTY} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

optimiser-r: ${MYOPTIMISER_R} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

