/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2017.
*/
/* CSC 254 A2 Syntax Error Recovery
 * Author: Po-Chun Chiu & Yujie Liu
 */

#include<iostream>
#include<ios>
#include <cstdlib> //Only for exit(1)
#include <string>
#include <cctype>
#include "scan.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::noskipws;


char token_image[100];
string image;

std::string getImage(){
  return " \""+image+"\"";
}

token scan() {
    static char c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    /* skip white space */
    /*if (!cin.get(c)) return t_eof;
    while (isspace(c)) {
        if (!cin.get(c)) return t_eof;
    }*/
	do{
	if (!cin.get(c)) return t_eof;
	}while(isspace(c));
    if (isalpha(c)) { //TODO
        do {
            token_image[i++] = c;
            if(!cin.get(c)) break;
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if(token_image==string("read")) return t_read;
        else if (token_image==string("write")) return t_write;
        else if (token_image==string("do")) return t_do;
        else if (token_image==string("od")) return t_od;
        else if (token_image==string("if")) return t_if;
        else if (token_image==string("fi")) return t_fi;
        else if (token_image==string("check")) return t_check;
        else{
          image = token_image;
          return t_id;
        }
          // literal = token_image;
    }
    else if (isdigit(c)) { //TODO
        do {
            token_image[i++] = c;
            cin.get(c);
        } while (isdigit(c));
        token_image[i] = '\0';
        image = token_image;
        return t_literal;
    }
    else switch (c) {
        case '+': cin.get(c); return t_add;
        case '-': cin.get(c); return t_sub;
        case '*': cin.get(c); return t_mul;
        case '/': cin.get(c); return t_div;
        case '(': cin.get(c); return t_lparen;
        case ')': cin.get(c); return t_rparen;
        //Relation operators added
        case ':':
          cin.get(c);
          if (c == '='){
              cin.get(c);
              return t_gets;
            }
          cout << "error\n";
        case '<':
            cin.get(c);
            if (c == '>'){
                cin.get(c);
                return t_notequal;
            }else if(c == '='){
                cin.get(c);
                return t_smallerequal;
            }else return t_smaller;
        case '>':
            cin.get(c);
            if(c == '='){
              cin.get(c);
                return t_greaterequal;
            }else return t_greater;
        case '=':
            cin.get(c);
            if(c == '='){
              cin.get(c);
                return t_equal;
            }//remove double equal
            cout << "error\n";
        default:
            cout << "error\n";
            exit(1);
    }
}
