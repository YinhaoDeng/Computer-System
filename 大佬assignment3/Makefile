# all the executables
EXECS= parser pretty codegen opt-r
JEXECS=$(patsubst %,j%,${EXECS})
TESTS=$(patsubst %,test-j%,${EXECS})
MYALL:=${wildcard shared-*.cpp}
MYPARSER:=parser.cpp ${wildcard parser-*.cpp}
MYCODEGEN:=codegen.cpp ${wildcard codegen-*.cpp}
MYPRETTY:=pretty-jack.cpp ${wildcard pretty-jack-*.cpp}
MYOPTIMISER_R:=optimiser-r.cpp ${wildcard optimiser-r-*.cpp}

# C++ 11 just in case
CXXFLAGS=--std=c++11 -I. -Iincludes

all: ${JEXECS}

test: ${TESTS}

help:
	-@echo make options are one of:
	-@echo " all, jparser, jcodegen, jpretty or jopt-r"
	-@echo testing options are one of:
	-@echo " test-all, test-parser, test-codegen, test-pretty or test-opt-r"
	-@echo


# to test student versions of the programs
test-new:
	-@(bash bin/run-tests new loud)

test-jparser: jparser
	-@(bash bin/run-tests jparser loud)

test-jcodegen: jcodegen
	-@(bash bin/run-tests jcodegen loud)

test-jpretty: jpretty
	-@(bash bin/run-tests jpretty loud)

test-jopt-r: jopt-r
	-@(bash bin/run-tests jopt-r loud)

# to run tests with working versions of the programs
test-working: test-working-jparser test-working-jcodegen test-working-jpretty test-working-jopt-r

test-working-jparser:
	-@(bash bin/run-tests working-jparser loud)

test-working-jcodegen:
	-@(bash bin/run-tests working-jcodegen loud)

test-working-jpretty:
	-@(bash bin/run-tests working-jpretty loud)

test-working-jopt-r:
	-@(bash bin/run-tests working-jopt-r loud)

clean:
	-rm -rf ${JEXECS} *.o

%.o: %.cpp %.h
	g++ ${CXXFLAGS} -c $<

jparser: ${MYPARSER} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

jcodegen: ${MYCODEGEN} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

jpretty: ${MYPRETTY} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

jopt-r: ${MYOPTIMISER_R} ${MYALL} lib/lib.a
	g++ ${CXXFLAGS} -o $@ $^

