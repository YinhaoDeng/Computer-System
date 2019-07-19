#ifndef LIB_SYMBOLS_H
#define LIB_SYMBOLS_H

#include <string>
#include <map>

// an abstract symbol table class, these are created using the create_symbol_table_xxx functions below
template <class T> class symbol_table_a
{
public:
    virtual bool insert(std::string,T value) =0 ;             // insert a new symbol, value pair into the table
                                                              // the table is not changed if the symbol was already in the table
                                                              // the result is true if the table is modified, or false if it is not

    virtual bool update(std::string,T value) =0 ;             // update a symbol, value pair in the table
                                                              // the table is not changed if the symbol is not in the table
                                                              // the result is true if the table is modified, of false if it is not

    virtual T lookup(std::string) =0 ;                        // search the table for a symbol and return its associated value
} ;
    

// a symbol table recording int values,
// lookup returns -1 if a symbol cannot be found
typedef symbol_table_a<int> *symbol_table_int ;                // type of a pointer to an int symbol table
extern symbol_table_int create_symbol_table_int() ;            // returns a pointer to a newly created table

// a symbol table recording string values,
// lookup returns "" if a symbol cannot be found
typedef symbol_table_a<std::string> *symbol_table_string ;     // type of a pointer to a string symbol table
extern symbol_table_string create_symbol_table_string() ;      // returns a pointer to a newly created table

// class used to represent an arbitrary value in a symbol table
class symbol_value {} ;

// a symbol table recording pointer to symbol_value objects
// lookup returns nullptr if a symbol cannot be found
typedef symbol_table_a<symbol_value*> *symbol_table_values ;   // type of a pointer to a symbol_value object
extern symbol_table_values create_symbol_table_values() ;      // returns a pointer to a newly created table


// ***************   EXAMPLES OF HOW TO USE THE SYMBOL TABLES   *********************

// ***************  Symbol table mapping strings to int values  *********************

// Example of how to use an int symbol table
//
// Create a new table
//     symbol_table_int mytable = create_symbol_table_int() ;
//
// Now add a mapping from "bob" to 73 and report an error if "bob" is already in the mytable
//     if ( !mytable->insert("bob",73) ) cout << "Error!" << endl ;
//
// Now change "bob"'s 73 to 17 and report an error if "bob" is not in the mytable
//     if ( !mytable->update("bob",17) ) cout << "Error!" << endl ;
//
// Now lookup and print "bob"'s value, this will print -1, the missing value, if "bob" is not in mytable
//     cout << mytable->lookup("bob") << endl ;
//

// ***************  Symbol table mapping strings to string values  *********************

// Example of how to use a string symbol table
//
// Create a new table
//     symbol_table_string mytable = create_symbol_table_string() ;
//
// Now add a mapping from "bob" to "hello mum" and report an error if "bob" is already in the mytable
//     if ( !mytable->insert("bob","hello mum") ) cout << "Error!" << endl ;
//
// Now change "bob"'s "hello mum" to "bye bye mum" and report an error if "bob" is not in the mytable
//     if ( !mytable->update("bob","bye bye mum") ) cout << "Error!" << endl ;
//
// Now lookup and print "bob"'s value, this will print "", the missing value, if "bob" is not in mytable
//     cout << mytable->lookup("bob") << endl ;
//

// ***********  Symbol table mapping strings to pointers to variable objects  *************

// Example of how to use a variable symbol table
//
// Create a new class to represent a variable that inherits from symbol_value
//     class variable : public symbol_value { public: string name,kind,segment; int offset ; } ;
//
// Create an object for variable bob of type int in segment local at offset 2
//     variable bob = new variable() ;
//     bob->name = "bob" ;
//     bob->kind = "int" ;
//     bob->segment = "local" ;
//     bob->offset = 2 ;
//
// Create a new table
//     symbol_table_values mytable = create_symbol_table_values() ;
//
// Now add a mapping from "bob" to our variable bob and report an error if "bob" is already in the mytable
//     if ( !mytable->insert("bob",bob) ) cout << "Error!" << endl ;
//
// Now lookup "bob"'s variable, this will return nullptr if "bob" is not in mytable
// Type cast the result from symbol_value pointer to variable pointer
//     variable *result = (variable*) mytable->lookup("bob") ;
//

#endif //LIB_SYMBOLS_H

