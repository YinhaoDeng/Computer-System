#ifndef CSTOOLS_IOBUFFER_H
#define CSTOOLS_IOBUFFER_H

#include <iostream>

// the cstools iobuffer buffers data and errors to be written until explicitly erased or printed
// trace writes can be optionally included in the normal output
// log messages can be optionally included in the error output
// printing buffers following a fatal error is optional

// buffer configurations
enum iob_option
{
    iob_enable,         // future write_to_* calls are performed
    iob_disable,        // future write_to_* calls are ignored
    iob_not_permitted,  // future write_to_* calls result in a fatal_error, this cannot be changed

                        // the following can only be applied to output and errors
    iob_buffer,         // future write_to_* calls are buffered, erase_* is called by fatal error
    iob_loud_buffer,    // future write_to_* calls are buffered, print_* is called by fatal error
    iob_immediate,      // print buffer, future write_to_* calls are immmediately followed by a print_* call

                        // this option only applies to errors
    iob_buffer_why      // future write_to_errors calls are buffered, fatal_error calls erase_errors but prints it's message
} ;

// use the output buffer for all output normally written to std::cout
extern void write_to_output(std::string s) ;        // adds s to the end of the output buffer
extern void erase_output() ;                        // erase the output buffer contents, nothing is printed
extern void print_output() ;                        // print the output buffer contents, then call erase_output()
extern void config_output(iob_option opt) ;         // configure future behaviour, default is iob_buffer

// use the following for trace prints - these messages are written to the output buffer
extern void write_to_traces(std::string s) ;        // adds s to the end of the traces buffer
extern void config_traces(iob_option opt) ;         // configure future behaviour, default is iob_ignore

// use the following for logging - these messages are written to the errors buffer
extern void write_to_logs(std::string s) ;          // adds s to the end of the logs buffer
extern void config_logs(iob_option opt) ;           // configure future behaviour, default is iob_ignore

// use the errors buffer for all output normally written to std::cerr
extern void write_to_errors(std::string s) ;        // adds s to the end of the error buffer
extern void write_to_errors(int n) ;                // converts n to a string, then adds it to the end of the error buffer
extern void erase_errors() ;                        // erase the error buffer contents, nothing is printed
extern void print_errors() ;                        // print the error buffer contents, then call erase_errors()
extern void config_errors(iob_option opt) ;         // configure future behaviour, default is iob_immediate

// to exit due to a fatal error
extern void fatal_error(int status,std::string s) ; // write_to_errors(s), performs any configured ouput, then exit(status)


// ***************   EXAMPLES OF HOW TO USE THE IO BUFFER   *********************

// Example of how to add a line containing "hello" and 45 to the output buffer:
//     write_to_output("hello " + std::to_string(45) + "\n") ;
//
// To throw away everything written to the output buffer so far:
//     erase_output() ;
//
// To write the current contents of the output buffer to cout - and erase the contents:
//     print_output() ;
//
// Example of how to write out an error containing "elephants are big":
//     write_to_errors("elephants are big\n") ;
//
// To allow trace writes to appear in the output buffer
//     config_traces(iob_enable) ;
//
// To allow log writes to appear in the error buffer
//     config_logs(iob_enable) ;
//
// To cause output and trace writes to be printed immediately:
//     config_output(iob_immediate) ;
//
// To stop all output including the final error message printed after a fatal error:
//     config_output(iob_buffer) ;
//     config_errors(iob_buffer) ;
//
// Notes:
//  .  if you have written too much data to the buffers, an error message will
//     be printed and your program will exit. This is intended to catch programs
//     stuck in infinite loops, a correct program should never be affected.
//
// If an error occurs and you wish to exit the program immediately and without explanation:
//      fatal_error(0,"") ;
//


#endif //CSTOOLS_IOBUFFER_H
