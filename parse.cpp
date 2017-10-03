/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/

/* CSC 254 A2 Syntax Error Recovery
 * Author: Po-Chun Chiu & Yujie Liu
 */

// including & namespace using
#include <iostream>
#include <ios>
#include "scan.h"
#include <cstdlib>
#include <string>

//using this instead of namespace std to avoid allocating for those unused
using std::string;
using std::cin;
using std::cout;
using std::endl;
//using std::noskipws;

const char* names[] = {"check", "read", "write", "id", "literal", "gets", "if",
"fi", "do", "od", "equal", "notequal", "smaller",
"greater", "smallerequal","greaterequal",
"add", "sub", "mul", "div", "lparen", "rparen", "eof", "eps"};

//token names for exact token to be printed
const string token_names[] = {"\"check\"", "\"read\"", "\"write\"", "id", "literal", "\"gets\"", "\"if\"",
"\"fi\"", "\"do\"", "\"od\"", "\"==\"", "\"<>\"", "\"<\"","\">\"", "\"<=\"","\">=\"","\"+\"", "\"-\"", "\"*\"", "\"/\"", 
				"\"(\"", "\")\"", "\"eof\"", "\"eps\""};

static token input_token;
static int tabNum = 0;  //to check for tabing spaces
static int hasError = 0;
static string image = "";
// check for the follow sets of statement, relation and expression to handle exceptions catched
static token s_follow[] = {t_id, t_read, t_write, t_if, t_do, t_check, t_eof};
static token r_follow[] = {t_id, t_read, t_write, t_if, t_do, t_check, t_eof, t_fi, t_rparen};
static token e_follow[] = {t_id, t_read, t_write, t_if, t_do, t_check, t_eof, t_fi, t_rparen
                        , t_equal, t_notequal, t_smaller, t_greater, t_smallerequal, t_greaterequal};


//Functions to help print the syntax trees including preIndent, postIndent, and prefix
string postIndent(string str, int tab){
  for(int i = 0; i <= tab; i++){
    str += " ";
  }
  return str;
}

string preIndent(string str, int tab){
  for(int i = 0; i <= tab; i++){
    str = " " + str;
  }
  return str;
}

string prefix(string str, string tail){
  if(tail == "") return str;
  for (int i = 0; i < tail.length(); ++i){
    if(tail[i] == ' '){
      return tail.substr(0,i)+" "+ str +" "+ tail.substr(i+1, tail.length() - i);
    }
  }
  return "prefix error";
}
//check if t is in the set[]
int contains(token t, token set[]){
  int i = 0;
  while(set[i]){
    if (t == set[i++]) {
      return 1;
    }
  }
  return 0;
}
//error function 
void error () {
    cout << "Syntax error" << endl;
    exit (1);
}
//check if the expected token is matched
string match (token expected) {
    if (input_token == expected) {
        image = getImage();
        input_token = scan ();
        // cout << "Matched "<<names[expected] << " image: "<<image<< endl;
        //cout << "match next: "<<names[input_token] << endl;
    }
    else{
      image = getImage();
      cout << "Token " << names[input_token] << ": do you mean " << token_names[expected] << "?"<< endl;
      throw string("match");
  }
  return "";
}

string program ();
string stmt_list ();
string stmt ();
string expr ();
string expr_tail();
string term_tail ();
string term ();
string factor_tail ();
string factor ();
string relation_op();
string add_op ();
string mul_op ();
string relation();

//program function with try/catch exception
//each switch cases are the first set of the current production 
string program () {
    try{
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_if:
            case t_do:
            case t_check:
            case t_eof:{
              tabNum++;
                string str1 = "(program \n" ;
                str1 = postIndent(str1, tabNum);
                str1 += "[";

                str1 += stmt_list ();
                match (t_eof);
                str1 = postIndent(str1, tabNum);
                str1 += "]\n";
                if(hasError) return "";
                return str1+")\n";
            }
            default:
            throw string("program");
            return "";
        }
    }catch(string e){
        cout << " Not expecting " << names[input_token] << " in Program"<< endl;
        return "";
    }
}
//stament list function
//each switch cases are the first set of the current production 
string stmt_list () {
  switch (input_token) {
      case t_id:
      case t_check:
      case t_write:
      case t_read:
      case t_if:
      case t_do:{
        string str1 = "";
        str1 = postIndent(str1, tabNum);
        str1 += "("+stmt();
        str1 += stmt_list();
        str1 = postIndent(str1, tabNum);
        str1 += ")\n";
        tabNum--;
        return str1;
      }
      case t_eof:
        tabNum--;
        return "\n";          /*  epsilon production */
    default:
      tabNum--;
      return "\n";
    }
}
//stament function
//each switch cases are the first set of the current production 
string stmt () {
  tabNum++;
  try{
    switch (input_token) {
        case t_id:{
            match (t_id);
            match (t_gets);
            string str1 = "( := (id "+ image+")" + relation();//Used to be expr()
	    str1 = postIndent(str1, tabNum);
	    str1 += ")";
            tabNum--;
          return str1;
      }
        case t_read:
        match (t_read);
        match (t_id);
        tabNum--;
        return "read (id"+image+" )\n";
      case t_write:{
        match (t_write);
            string str1 = relation();//Used to be expr()
            str1 = postIndent(str1, tabNum);
          tabNum--;
          return "(write " + str1 + ")\n";
        }
        case t_if:{
          match(t_if);
          string str1 = "(if \n";
          str1 = postIndent(str1, tabNum);
          str1 += relation();
          str1 = postIndent(str1, tabNum);
          string str2  = stmt_list();
          str2 = postIndent(str2, tabNum);
          match(t_fi);
          tabNum--;
          return str1 +"[\n"+ str2 + "])\n";
        }
        case t_do:{
          match(t_do);
          string str1 = "(do\n";
          str1 += stmt_list();
          str1 = postIndent(str1, tabNum);
          match(t_od);
          tabNum--;
          return "["+ str1 + "])\n";
        }
        case t_check:{
          match(t_check);
          string str1 = "";
          str1 = postIndent(str1, tabNum);
          str1 += relation();
          str1 = postIndent(str1, tabNum);
          tabNum--;
          return "(check\n"+str1+")\n";
        }
        default: 
        error();
        tabNum--;
        return "";
      }
    }catch(string e){
	hasError = 1;
      if(e == "match") cout <<" Not expecting " << token_names[input_token] << " in Statement" <<endl;
	//print the error messages in in catch exceptions
      else cout << " Not expecting " << token_names[input_token] << " in " << e << endl;
	//print the error messages in in catch exceptions
      cout << " Skipped: " << token_names[input_token] << endl;
      input_token = scan();
      while(!contains(input_token, s_follow)
            &&input_token != t_eof){
            cout << " Skipped: " << token_names[input_token] << endl;
            input_token = scan();

    }
    if(contains(input_token, s_follow)){
        hasError = 1;
        return "(error)\n";
    }else{} //If having reached eof
            return "";
        }
}

//each switch cases are the first set of the current production 
string expr () {
  tabNum++;
  try{
    string str1 = term ();
    string str2 = term_tail ();
    tabNum--;
    return prefix(str1, str2);
    }catch(string e){
	hasError = 1;
      if(e == "match") cout <<" Not expecting " << token_names[input_token] << " in expression" <<endl;
	//print the error messages in in catch exceptions
      else cout << " Not expecting " << token_names[input_token] << " in " << e << endl;
	//print the error messages in in catch exceptions
      cout << " Skipped: " << token_names[input_token] << endl;
	input_token = scan();
            while(!contains(input_token, e_follow)
            &&input_token != t_eof){
            cout << " Skipped: " << token_names[input_token] << endl;
            input_token = scan();

       }
        if(contains(input_token, e_follow)){
            hasError = 1;
            tabNum--;
            //cout << "follow token "<< names[input_token]<<" found" << endl;
            return "(error)\n";
        }else{

            } //If having reached eof
            tabNum--;
            return "";
        }
  error ();
  tabNum--;
  return "";

}

//each switch cases are the first set of the current production 
string expr_tail(){
  tabNum++;
  switch (input_token) {
    case t_equal:
    case t_notequal:
    case t_smaller:
    case t_greater:
    case t_smallerequal:
    case t_greaterequal:{
        string str1 = relation_op();
        string str2 = expr();
        tabNum--;
        return str1+" "+str2;
    }
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
    tabNum--;
    return "";
    default:
    tabNum--;
    return "";
}
}

//each switch cases are the first set of the current production 
string term_tail () {
  tabNum++;
  switch (input_token) {
    case t_add:
    case t_sub:{
        string str1 = add_op ();
        str1 += " ";
        str1 += term ();
        string str2 = term_tail ();
        tabNum--;
        return prefix(str1,str2);
    }
    case t_rparen:
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
      tabNum--;
      return "";          /*  epsilon production */
    default:
      return "";
}
}

//each switch cases are the first set of the current production 
string term () {
  try{
  tabNum++;
  string str1 = factor ();
  string str2 = factor_tail ();
  tabNum--;
  return prefix(str1, str2);
}catch(string e){
  throw string("Term");
  tabNum--;
}
  return "";

}

//each switch cases are the first set of the current production 
string factor_tail () {
  tabNum++;
  switch (input_token) {
    case t_mul:
    case t_div:{
        string str1 = mul_op ();
        string str2 = factor ();


        str1 += str2;
        str1 += factor_tail ();
        tabNum--;
        return str1+"";
    }
    case t_add:
    case t_sub:
    case t_rparen:
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
      tabNum--;
      return "";          /*  epsilon production */
    default:
      return "";
    }
}

//each switch cases are the first set of the current production 
string factor () {
  tabNum++;
  switch (input_token) {
    case t_id :{
      match (t_id);
      tabNum--;
      string str1 = "(id"+image+")";
      return str1;
    }
    case t_literal:{
      match (t_literal);
      tabNum--;
      string str1 = "(lit"+image+")";
      return str1;
    }
    case t_lparen:{
    match (t_lparen);
    string str1 = relation ();
    match (t_rparen);
    tabNum--;
    return "("+str1+")";
  }
    default:
    throw string("Factor");
    tabNum--;
    return "";
}
}

// the new built one
//each switch cases are the first set of the current production 
string relation_op(){
  tabNum++;
  switch(input_token){
    case t_equal:
    match(t_equal);
    tabNum--;
    return "== ";
    case t_notequal:
    match(t_notequal);
    tabNum--;
    return "<> ";
    case t_smaller:
    match(t_smaller);
    tabNum--;
    return "< ";
    case t_greater:
    match(t_greater);
    tabNum--;
    return "> ";
    case t_smallerequal:
    match(t_smallerequal);
    tabNum--;
    return "<= ";
    case t_greaterequal:
    match(t_greaterequal);
    tabNum--;
    return ">= ";
    default:
    throw string("Relation Operation");
    tabNum--;
    return "";
}
}

//each switch cases are the first set of the current production 
string add_op () {
  tabNum++;
  switch (input_token) {
    case t_add:
    match (t_add);
    tabNum--;
    return "+ ";
    case t_sub:
    match (t_sub);
    tabNum--;
    return "- ";
    default:
    throw string("Arithmetic Operator");
    tabNum--;
    return "";
}
}

//each switch cases are the first set of the current production 
string mul_op () {
  tabNum++;
  switch (input_token) {
    case t_mul:
    match (t_mul);
    tabNum--;
    return "* ";
    case t_div:
    match (t_div);
    tabNum--;
    return "/ ";
    default:
    tabNum--;
    throw string("Multiplication Operator");
    return "";
}
}

//the relation() is the one we added and modified from professor's code
//each switch cases are the first set of the current production 
string relation(){
    try{
      tabNum++;
      string str2 = expr();
      string str1 = expr_tail();
      tabNum--;
      return "("+prefix(str2, str1)+ ")\n";
    }catch(string e){
	hasError = 1;
      if(e == "match") cout <<" Not expecting " << token_names[input_token] << " in Relation" <<endl;
	//print the error messages in in catch exceptions
      else cout << " Not expecting " << token_names[input_token] << " in " << e << endl;
	//print the error messages in in catch exceptions
      
      cout << " Skipped: " << token_names[input_token] << endl;
	input_token = scan();
            while(!contains(input_token, r_follow)&&input_token != t_eof){
            cout << " Skipped: " << token_names[input_token] << endl;
            input_token = scan();
            cout << input_token<<endl;
        }
        if(contains(input_token, r_follow)){
            hasError = 1;
            tabNum--;
            //cout << "follow token "<<names[input_token]<<" found" << endl;
            return "(error)\n";
        }else{} //If having reached eof
        tabNum--;
        return " eof";
      }
}

//main function to call the program function
int main () {
    input_token = scan ();
    cout << program ();
    return 0;
}
