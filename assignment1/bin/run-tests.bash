#!/bin/bash
# runs chosen tokeniser over test files or regenerates them
# it assumes this directory is SOMEHWERE/bin
# it assumes tests directory is SOMEWHERE/tests, ie ../tests
# it assumes that the executable is in the parent directory SOMEWHERE/

# tests / tokeniser to run
case $1 in
add)
	tokeniser="../bin/tokeniser"
	tokeniser_path="bin/tokeniser        "
	tokeniser_message="working tokeniser"
	tokeniser_context="../bin/tokeniser-context"
	tokeniser_context_path="bin/tokeniser-context"
	tokeniser_context_message="working tokeniser-context"
	regenerate="MISSING"
	;;
new)
	tokeniser="../bin/tokeniser"
	tokeniser_path="bin/tokeniser        "
	tokeniser_message="working tokeniser"
	tokeniser_context="../bin/tokeniser-context"
	tokeniser_context_path="bin/tokeniser-context"
	tokeniser_context_message="working tokeniser-context"
	regenerate="ALL"
	;;
working)
	tokeniser="../bin/tokeniser"
	tokeniser_path="bin/tokeniser        "
	tokeniser_message="working tokeniser"
	tokeniser_context="../bin/tokeniser-context"
	tokeniser_context_path="bin/tokeniser-context"
	tokeniser_context_message="working tokeniser-context"
	regenerate="NO"
	;;
tokeniser)
	tokeniser="../tokeniser"
	tokeniser_path="tokeniser        "
	tokeniser_message="student tokeniser"
	tokeniser_context="../tokeniser-context"
	tokeniser_context_path="tokeniser-context"
	tokeniser_context_message="student tokeniser-context"
	regenerate="NO"
	;;
*)
	tokeniser="../tokeniser"
	tokeniser_path="tokeniser        "
	tokeniser_message="student tokeniser"
	tokeniser_context="../tokeniser-context"
	tokeniser_context_path="tokeniser-context"
	tokeniser_context_message="student tokeniser-context"
	regenerate="NO"
	;;
esac

# display messages or hide them
case $2 in
quiet)
	loud="quiet"
	;;
loud)
	loud="loud"
	;;
*)
	loud="loud"
	;;
esac

# work out relative path to tests directory
THISPWD=`pwd`

# cd to the bin directory - where this script is
BINDIR=`dirname $0`
cd "$BINDIR"
BINPWD=`pwd`

# find the parent directory name
PARENTDIR=`dirname "${BINPWD}"`

# check we have a tests dir
if [ ! -d "../tests" ] ; then
	echo "Cannot find the tests directory"
	exit -1
fi

# cd to the tests dir
cd "../tests"
TESTPWD=`pwd`

# fix up pathnames to be displayed to the user
if [ "$THISPWD" == "$TESTPWD" ] ; then
	TESTDIR=""
	PARENTDIR="../"
elif [ "$THISPWD" == "$BINPWD" ] ; then
	TESTDIR="../tests/"
	PARENTDIR="../"
elif [ "$THISPWD" == "$PARENTDIR" ] ; then
	TESTDIR="tests/"
	PARENTDIR="./"
else
	echo "Execute the following command first if you want to cut-paste the test commands:"
	echo "cd ${PARENTDIR}"
	echo
	TESTDIR="tests/"
	PARENTDIR="./"
fi

# check we have a tokeniser first
if [ ! -x "${tokeniser}" ] ; then
	echo Cannot find executable ${PARENTDIR}${tokeniser_path}
	exit -1
fi

### PRETTY STUFF ###

# if using an xterm - use headings and colour coded test results
if [ "${TERM:0:5}" == "xterm" -o "${TERM:0:2}" == "vt" ] ; then
	colorboldblack="\x1b[1;7;30m"
	colorboldgreen="\x1b[1;7;32m"
	colorboldred="\x1b[1;7;31m"
	undocolor="\x1b[0m"
else
	colorboldblack=
	colorboldgreen=
	colorboldred=
	undocolor=
fi

# make a string of whitespace matching the longest string parameter
padding=
setpadding()
{
	padding=
	while [ "${#@}" -gt 0 ] ; do
		while [ "${#padding}" -lt "${#1}" ] ; do
			padding="${padding} "
		done
		shift 1
	done
}

# column1 - blank
setpadding "Checking"
col1="${padding}"

# column2 - test input file
setpadding "cat ${TESTDIR}ExplsSquare-SquareGame.final"
input_padding="${padding}"

col2="          Test Input"
col2="${col2}${input_padding:${#col2}}"

# column3 - the tokeniser being run
setpadding "${PARENTDIR}${tokeniser_context_path}"
command_padding1="${padding}"

col3="  Tokeniser"
col3="${col3}${command_padding1:${#col3}}"

# column 4 - test expected output file
setpadding "diff - ${TESTDIR}ExplsSquare-SquareGame.final.context"
output_padding="${padding}"

col4="             Expected Test Output"
col4="${col4}${output_padding:${#col4}}"

# column 5 - test result
setpadding " test passed " " test failed "
result_padding="${padding}"
col5=" Test Result"
col5="${col5}${result_padding:${#col5}}"

# nullglob is set so nothing runs if there are no test files
shopt -s nullglob

banner="NO"
header=
columns=
display_banner()
{
	if [ "${banner}" == "YES" ] ; then return ; fi

	banner="YES"
	if [ ${#header} -gt 0 ] ; then echo "${header}" ; fi
	if [ ${#columns} -gt 0 ] ; then echo -e "${columns}" ; fi
}

if [ "${regenerate}" == "ALL" ] ; then
	echo "Regenerating all .tokens files"

	# run working tokeniser with all test files to generate matching .tokens file
	# ignore files that are named *.tokens
	for i in * ; do
		input="${i%%.tokens}"
		badinput="${i%%.context}"
		if [ "${input}" == "${i}" -a "${badinput}" == "${i}" ] ; then

			testmessage="Regenerating \"${TESTDIR}${i}.tokens\" - "

			if cat ${input} | ${tokeniser} > ${i}.tokens 2> ${i}.tokens.err ; then
				if [ -s ${i}.tokens.err ] ; then
					echo -e "${testmessage} ${colorboldred}failed${undocolor}"
					rm -f ${i}.tokens.err
					exit -1
				else
					if [ "${loud}" != "quiet" ] ; then echo -e "${testmessage} ${colorboldred}ok${undocolor}" ; fi
				fi
			else
				echo -e "${testmessage} ${colorboldred}failed${undocolor}"
			fi
			rm -f ${i}.tokens.err
		fi
	done
elif [ "${regenerate}" == "MISSING" ] ; then
	echo "Generating missing .tokens files"

	# run working tokeniser generate missing .tokens files
	# ignore files that are named *.tokens or have a matching .tokens file
	for i in * ; do
		input="${i%%.tokens}"
		badinput="${i%%.context}"
		if [ "${input}" == "${i}" -a "${badinput}" == "${i}" -a ! -f "${input}.tokens" ] ; then

			testmessage="Generating \"${TESTDIR}${i}.tokens\" - "

			if cat ${input} | ${tokeniser} > ${i}.tokens 2> ${i}.tokens.err ; then
				if [ -s ${i}.tokens.err ] ; then
					echo -e "${testmessage} ${colorboldred}failed${undocolor}"
					rm -f ${i}.tokens.err
					exit -1
				else
					if [ "${loud}" != "quiet" ] ; then echo -e "${testmessage} ${colorboldred}ok${undocolor}" ; fi
				fi
			else
				echo -e "${testmessage} ${colorboldred}failed${undocolor}"
			fi
			rm -f ${i}.tokens.err
		fi
	done
else
	# test tokeniser with all test files that have a matching .tokens file
	header="Testing ${tokeniser_message} against .tokens files"
	columns="${colorboldblack}${col1}| ${col2} | ${col3} | ${col4}  | ${col5}${undocolor}"
	for i in *.tokens ; do
		input="${i%%.tokens}"
		if [ -f "${input}" ] ; then
			display_input="cat ${TESTDIR}${input}"
			display_input="${display_input}${input_padding:${#display_input}}"

			display_output="diff - ${TESTDIR}${i}"
			display_output="${display_output}${output_padding:${#display_output}}"

			testmessage="Checking \"${display_input} | ${PARENTDIR}${tokeniser_path} | ${display_output}\" - "
			if cat ${input} | ${tokeniser} 2> /dev/null | diff -q - ${i} > /dev/null 2> /dev/null ; then
				if [ "${loud}" != "quiet" ] ; then
					display_banner
					echo -e "${testmessage} ${colorboldgreen}test passed${undocolor}" ;
				fi
			else
				display_banner
				echo -e "${testmessage} ${colorboldred}test failed${undocolor}"
			fi
		fi
	done
fi

if [ "${regenerate}" == "ALL" ] ; then
	echo "Regenerating all .context files"

	# run working tokeniser with all test files to generate matching .context file
	# ignore files that are named *.context
	for i in *.tokens ; do
		input="${i%%.tokens}"
		if [ -f "${input}" ] ; then

			testmessage="Regenerating \"${TESTDIR}${input}.context\" - "

			if cat ${input} | ${tokeniser_context} > ${input}.context 2> ${input}.context.err ; then
				if [ -s ${input}.context.err ] ; then
					echo -e "${testmessage} ${colorboldred}failed${undocolor}"
					rm -f ${input}.context.err
					exit -1
				else
					if [ "${loud}" != "quiet" ] ; then echo -e "${testmessage} ${colorboldred}ok${undocolor}" ; fi
				fi
			else
				echo -e "${testmessage} ${colorboldred}failed${undocolor}"
			fi
			rm -f ${input}.context.err
			if /usr/bin/cmp ${i} ${input}.context > /dev/null 2> /dev/null ; then
				rm -f ${input}.context
			fi
		fi
	done
elif [ "${regenerate}" == "MISSING" ] ; then
	echo "Generating missing .context files"

	# run working tokeniser generate missing .context files
	# ignore files that are named *.context or have a matching .context file
	for i in *.tokens ; do
		input="${i%%.tokens}"
		if [ -f "${input}" -a ! -f "${input}.context" ] ; then

			testmessage="Generating \"${TESTDIR}${i}.context\" - "

			if cat ${input} | ${tokeniser_context} > ${input}.context 2> ${input}.context.err ; then
				if [ -s ${input}.context.err ] ; then
					echo -e "${testmessage} ${colorboldred}failed${undocolor}"
					rm -f ${input}.context.err
					exit -1
				else
					if [ "${loud}" != "quiet" ] ; then echo -e "${testmessage} ${colorboldred}ok${undocolor}" ; fi
				fi
			else
				echo -e "${testmessage} ${colorboldred}failed${undocolor}"
			fi
			rm -f ${input}.context.err
			if /usr/bin/cmp ${input} ${input}.context > /dev/null 2> /dev/null ; then
				rm -f ${input}.context
			fi
		fi
	done
else
	# test tokeniser with all test files that have a matching .context file
	header="Testing ${tokeniser_context_message} against .context files"
	columns="${colorboldblack}${col1}| ${col2} | ${col3} | ${col4}  | ${col5}${undocolor}"
	for i in *.context ; do
		input="${i%%.context}"
		if [ -f "${input}" ] ; then
			display_input="cat ${TESTDIR}${input}"
			display_input="${display_input}${input_padding:${#display_input}}"

			display_output="diff - ${TESTDIR}${i}"
			display_output="${display_output}${output_padding:${#display_output}}"

			testmessage="Checking \"${display_input} | ${PARENTDIR}${tokeniser_context_path} | ${display_output}\" - "
			if cat ${input} | ${tokeniser_context} 2> /dev/null | diff -q - ${i} > /dev/null 2> /dev/null ; then
				if [ "${loud}" != "quiet" ] ; then
					display_banner
					echo -e "${testmessage} ${colorboldgreen}test passed${undocolor}" ;
				fi
			else
				display_banner
				echo -e "${testmessage} ${colorboldred}test failed${undocolor}"
			fi
		fi
	done
fi
