CSC 254 Assignment 2 README
  Syntax Errpr Revovery
  Po-Chun Chiu(pchiu4) & Yujie Liu(yliu134)
	Student ID: 29433254 & 29421244
	Email: pchiu4@u.rochester.edu, yliu134@u.rochester.edu

Assignment Directory: A2
  TAs should be able to find every files under the A2 directory include parse.cpp, scan.cpp, scan.h, Makefile, README.txt, and a directory named tests including different test files.
  The code are all tested using the school cycle1 CSUG machine.
  The default shell of mine is cshell(csh).
  Use Makefile to compile our program.

Extra Credit on test mechanisms:
We used text files as input and we modify that in make file. In that case, we add several different test targets so that we can test a bunch of different inputs. The user only have to take a look on what test they are going to test in tests directory. All the input files are named corresponding to the test files. Like input3 refers to test3, and the user can run the test by typing make test3. We have 10 test files from input.txt to input9.txt which respond to test to test9.

Compile Steps:
make
make test
make test1
make test2
make test3
make test4
make test5
make test6
make test7
make test8
make test9

Note: test1 to test5 should return error messages that tell the user what is skipped/consumed for error recovery. test6 to test9 and test should return a valid syntax tree in linear form.
More Note: Our code doesn't take in specifically $$ for eof.
Therefore, it's why we use text files to test our program because by just taking standard in from command line, we need to use Ctrl+C to explicitly give the program EOF, which we consider not convenient.

Calculator Grammar:
	P	→ 	SL $$
	SL	→ 	S SL  |  ε
	S	→ 	id := R  |  read id  |  write R  |  if R SL fi  |  do SL od  |  check R
	R	→ 	E ET
	E	→ 	T TT
	T	→ 	F FT
	F	→ 	( R )  |  id  |  lit
	ET	→ 	ro E  |  ε
	TT	→ 	ao T TT  |  ε
	FT	→ 	mo F FT  |  ε
	ro	→ 	==  |  <>  |  <  |  >  |  <=  |  >=
	ao	→ 	+  |  -
	mo	→ 	*  |  /

1. We take in the standard input, from test files into the program, and expected to provide either a linearized syntax tree or error messages on where's wrong and what is expected.
2. We implement the Exception-Based Algorithm and used the try/catch exception syntax in C++ to help find the exceptions throw by the other part of the grammar.
3. We have implemented error recovery using follow sets of the grammar in order to predict what's expected to be seen in the grammar as the token is scanned.
4. If the token doesn't match what we are looking for, print the error messages, and make decisions on actions to deleting specific amounts of tokens until it see the tokens that's in the follow set of current production.
5. We modified a lot from the original C source code to C++ version with eliminating the library specific to C and not in C++.

P.S. We did include cstdlib library only for exit(1), and cctype for checking isalpha, isspace and isspace because there aren't good functions to use within just C++ libraries.

Detailed Specifics

Translate the source code from C to C++
1. We eliminated every printf statement with using cout. We replace the c=getchar() method with cin.get(c) method.
2. IMPORTANT concept: our program needs white spaces to separate every token, or otherwise the program might not be able to distinguish whether ab should be a identifier or in fact, two identifiers: a and b. Therefore, our program needs all the input tokens to be separated by white space.

Extend the grammar
1. We add if/do/check statement into the code, add relation and modify the original scan file for it to match the calculator grammar listed above.
2. We add a getImage() function in scan.h in order for us to get the image from scanner to parser such as the exact value in id and lit.
3. We modify the scan.cpp to fit the grammar. We adjust cases for t_eof to handle extra the issues of EOF. We use the string object to compare two strings like (token_image==string("write")) instead of original
strcmp() method.

Implement the Exception-Based Syntax Error Recoevery algorithm.
1. In our understanding, we attach exception handlers to different productions such as relations, expressions, and statements. In the program, we used the try/catch of strings to handle and find exceptions. E.g. catch(string e) so that we can find things unexpected for the other production passed.
2. In this recovery algorithm, we then put throw string() in order to find the specified problem.
3. Also, we wrote the follow sets for relation, expression, and statement, because we think that's the productions that we want to deal with the error messages.
4. The follow sets mean that: If the program sees something unexpected at specific production, it will consumed the current token, and keep scanning until it find something that's in the follow set of itself. For example, the follow set of R(relation) would be:{id, read, write, if ,do ,check, fi, ), eof} and the program will try to find the possible token in the follow sets. Those tokens that are not in the follow set will be deleted until it find a token that's in the follow set.
5. We consider that ET, TT,FT, and SL not possible of having errors. Because all these productions have ε(epsilon) transition, and that means the productions can just choose to use epsilon and left the error problem to the following productions.
6. The error message will tell the users what they might be expecting or looking for. These cases only happened if the error is thrown by match() function. Only the match function will know what tokens they are expecting and determine that the incoming token matched or not. If the error is thrown by deeper part in the parser, the program will prompt to user stating that the token is not expected, but will not specify explicitly which token to use since that the program will return a big amount of tokens in the possible follow sets.

Syntax Tree and Linearized Form
1. We did a big modification in order to print the syntax tree once the program is correct. We used tabNum to keep track of the number of tabs we are going to need to print. We also used image and getImage() to keep track of the exact tokens in identifier or literal(such as "n" for id and "5" for lit) pass it the parse.cpp.
2. We also have the prefix, postIndent, preIndent functions to help print the syntax tree in correct format. prefix is used to convert the operator to the front of the operands. preIndent and postIndent is going to take the tabNum and use a loop to print as many spaces as specified by tabNum.
3. In each recursive parser we used the idea of first set to determine which production to use. For each token in the first set we choose the corresponding production; if the input token is not in the first set, we throw an error and wait for relation, expression or statement parsers to catch it, while the current parser procedure is stopped.
4. The whole syntax tree is stored in a string which is constantly appended and at the end printed out.
