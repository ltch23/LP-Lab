/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <iostream>
#include <ctype.h>

using namespace std;

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();


/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define LEFT_COMMENT 27
#define RIGHT_COMMENT 28
#define COMMENT 29


#define FOR_CODE 30
#define IF_CODE 31
#define ELSE_CODE 32
#define WHILE_CODE 33
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37




/******************************************************/
/* main driver */
main() {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;

    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;

    case '+':
        addChar();
        nextToken = ADD_OP;
        break;

    case '-':
        addChar();
        nextToken = SUB_OP;
        break;

    case '*':
        addChar();
        nextToken = MULT_OP;
        break;

    case '/':
        addChar();
        nextToken = DIV_OP;
        break;

    default:
        addChar();
        nextToken = EOF;
        break;
}
return nextToken;
}

    /*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {

        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
             nextToken = IDENT;
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            
            if(nextChar == '/' ){
                getChar();
                if(nextChar== '*'){
                    nextToken = LEFT_COMMENT;
                    lexeme[0] = '/';lexeme[1] = '*'; //lexeme[2] = 0;
                    // getChar();

                    int i=2;
                    while ((nextChar= getc(in_fp)) != EOF){
                        if(nextChar!= '*'){
                            // cout<<"nextChar: "<<nextChar<<endl;
                            lexeme[i]=nextChar;
                            i++;
                        }
                        
                        else{
                        //     char tmpChar=nextChar;
                        //     cout<<"nextChar: "<<nextChar<<endl;
                            
                        //     if((nextChar= getc(in_fp)) != EOF){
                        //         if(nextChar!= '/'){
                        //             cout<<"nextChar: "<<tmpChar<<endl;
                        //             lexeme[i]=tmpChar;
                        //             i++;
                        // //         lexeme[i]=nextChar;
                        // //         i++;
                        //         }
                        //      else{
                        //         lexeme[i]=tmpChar;
                        //         i++;
                        //         break;
                        //         }
                        //     }
                            break;
                        }
                    }
                }                
            }
            else if(nextChar == '*' ){
                getChar();
                if(nextChar== '/'){
                    nextToken = RIGHT_COMMENT; 
                    lexeme[0] = '*';lexeme[1] = '/';lexeme[2] = 0;
                    getChar();

                }                
            }
            else
                getChar();
            break;
            
        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E'; lexeme[1] = 'O'; lexeme[2] = 'F'; lexeme[3] = 0;
            break;
    } /* End of switch */

    // excersise 5 words reservers
    if (lexeme[0]=='f' and lexeme[1]=='o' and lexeme[2]=='r' and  lexeme[3]== 0)
        nextToken=FOR_CODE;
    if (lexeme[0]=='i' and lexeme[1]=='f' and lexeme[2]== 0)
        nextToken=IF_CODE;
    if (lexeme[0]=='e' and lexeme[1]=='l' and lexeme[2]=='s' and lexeme[3]=='e' and lexeme[4]== 0)
        nextToken=ELSE_CODE;
    if (lexeme[0]=='w' and lexeme[1]=='h' and lexeme[2]=='i' and lexeme[3]=='l' and lexeme[4]=='e' and lexeme[5] == 0)
        nextToken=WHILE_CODE;
    if (lexeme[0]=='d' and lexeme[1]=='o' and lexeme[2]== 0)
        nextToken=DO_CODE;
    if (lexeme[0]=='i' and lexeme[1]=='n' and lexeme[2]=='t' and lexeme[3]== 0)
        nextToken=INT_CODE;
    if (lexeme[0]=='f' and lexeme[1]=='l' and lexeme[2]=='o' and lexeme[3]=='a' and lexeme[4]=='t' and lexeme[5]== 0)
        nextToken=FLOAT_CODE;
    if (lexeme[0]=='s' and lexeme[1]=='w' and lexeme[2]=='i' and lexeme[3]=='t' and lexeme[4]=='c' and lexeme[5]=='h' and  lexeme[6]== 0)
        nextToken=SWITCH_CODE;




    printf("Next token is: %d, Next lexeme is %s\n",
           nextToken, lexeme);
    return nextToken;
} /* End of function lex */
