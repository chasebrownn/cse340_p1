/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !!&%!!\n";
    exit(1);
}

// To run: 
//
// g++ -g -Wall inputbuf.cc lexer.cc parser.cc
// ./syntax.sh

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;
    
    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;
        token = lexer.peek(i);
        token.Print();
    }
   
    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }

}

void Parser::parse_input()
{
    Token token;

    parse_program();
    parse_inputs();

    token = lexer.peek(1);

    if (token.token_type == END_OF_FILE)
    {
        return;
    }
    else
    {
        syntax_error();
    }
}

void Parser::parse_program()
{
    Token token;

    parse_poly_decl_sect();

    token = expect(START);

    //token = lexer.peek(1);
    //
    //if (token.token_type == START) return;
    //else
    //    syntax_error();
}

void Parser::parse_poly_decl_sect()
{
    Token token;

    parse_poly_decl();

    token = lexer.peek(1);

    if (token.token_type == POLY) { parse_poly_decl_sect(); }
    else
        return; // syntax_error();
}

void Parser::parse_poly_decl()
{
    Token token;

    token = expect(POLY);
    parse_poly_header();
    token = expect(EQUAL);
    parse_poly_body();
    token = expect(SEMICOLON);

    //token = lexer.peek(1);
    //
    //if (token.token_type == SEMICOLON) return;
    //else
    //    syntax_error();
}

void Parser::parse_poly_header()
{
    Token token;

    parse_poly_name();

    token = lexer.peek(1);

    if (token.token_type == LPAREN)
    {
        token = expect(LPAREN);
        parse_id_list();
        token = expect(RPAREN);
        return;
    }
    else
        return;
}

void Parser::parse_id_list()
{
    Token token;

    token = expect(ID);

    token = lexer.peek(1);
 
    if (token.token_type == COMMA)
    {
        token = expect(COMMA);
        parse_id_list();
        return;
    }
    else
        return;
}

void Parser::parse_poly_name()
{
    Token token;

    token = expect(ID);
}

void Parser::parse_poly_body()
{
    Token token;

    parse_term_list();
}

void Parser::parse_term_list()
{
    Token token;

    parse_term();

    token = lexer.peek(1);

    if (token.token_type == PLUS || token.token_type == MINUS)
    {
        parse_add_operator();
        parse_term_list();
        return;
    }
    else
        return;
}

void Parser::parse_term() {

    Token token;

    token = lexer.peek(1);

    if (token.token_type == ID)
    {
        parse_monomial_list();
        return;
    }
    else if (token.token_type == NUM)
    {
        parse_coefficient();

        token = lexer.peek(1);

        if (token.token_type == ID)
        {
            parse_monomial_list();
        }
        else
            return;

    }
    else
        syntax_error();

}

/*

void Parser::parse_term()
{
    Token token;

    token = lexer.peek(1);

    if (token.token_type == NUM)
    {
        parse_coefficient();

        token = lexer.peek(1);
        if (token.token_type != END_OF_FILE)
        {
            parse_monomial_list();
            return;
        }

        return;
    }
    else if (token.token_type != END_OF_FILE)
    {
        parse_monomial_list();
    }
    else
        return;
}

*/

void Parser::parse_monomial_list()
{
    Token token;

    parse_monomial();

    token = lexer.peek(1);
    if (token.token_type == ID)
    {
        parse_monomial_list();
        return;
    }
    else
        return;
}

void Parser::parse_monomial()
{
    Token token;

    token = expect(ID);

    token = lexer.peek(1);
    if (token.token_type == POWER)
    {
        parse_exponent();
        return;
    }
    else
        return;
}

void Parser::parse_exponent()
{
    Token token;

    token = expect(POWER);
    token = expect(NUM);

    //token = lexer.peek(1);
    //if (token.token_type == NUM)
    //{
    //    return;
    //}
    //else
    //    syntax_error();
}

void Parser::parse_add_operator()
{
    Token token;

    token = lexer.peek(1);
    if (token.token_type == PLUS)
    {
        token = expect(PLUS);
        return;
    }
    else if (token.token_type == MINUS)
    {
        token = expect(MINUS);
        return;
    }
    else
        return;
}

void Parser::parse_coefficient()
{
    Token token;

    token = expect(NUM);
}

void Parser::parse_start()
{
    Token token;

    token = expect(START);
    parse_statement_list();
}

void Parser::parse_inputs()
{
    Token token;

    token = expect(NUM);

    token = lexer.peek(1);
    if (token.token_type == NUM)
    {
        parse_inputs();
        return;
    }
    else
        return;
}

void Parser::parse_statement_list()
{
    Token token;

    parse_statement();

    token = lexer.peek(1);
    if (token.token_type == INPUT || token.token_type == ID)
    {
        parse_statement_list();
        return;
    }
    else
        return;
}

void Parser::parse_statement()
{
    Token token;

    token = lexer.peek(1);
    if (token.token_type == INPUT)
    {
        parse_input_statement();
        return;
    }
    else if (token.token_type == ID)
    {
        parse_poly_eval_statement();
        return;
    }
    else
        return; // syntax_error();
}

void Parser::parse_poly_eval_statement()
{
    Token token;

    parse_poly_eval();

    token = expect(SEMICOLON);

    //token = lexer.peek(1);
    //if (token.token_type == SEMICOLON) return;
    //else
    //    syntax_error();
}

void Parser::parse_input_statement()
{
    Token token;

    token = expect(INPUT);
    token = expect(ID);
    token = expect(SEMICOLON);
}

void Parser::parse_poly_eval()
{
    Token token;

    parse_poly_name();
    token = expect(LPAREN);
    parse_argument_list();
    token = expect(RPAREN);
}

void Parser::parse_argument_list()
{
    Token token;

    parse_argument();

    token = lexer.peek(1);
    if (token.token_type == COMMA)
    {
        token = expect(COMMA);
        parse_argument_list();
        return;
    }
    else
        return;
}

void Parser::parse_argument()
{
    Token token;

    token = lexer.peek(2);
    if (token.token_type == LPAREN)
    {
        parse_poly_eval();
        return;
    }

    token = lexer.peek(1);
    if (token.token_type == ID)
    {
        token = expect(ID);
        return;
    }
    else if (token.token_type == NUM)
    {
        token = expect(NUM);
        return;
    }
    else
        return;
}

//void Parser::parse_argument()
//{
//    Token token;
//
//    token = lexer.peek(1);
//    if (token.token_type == ID)
//    {
//        token = expect(ID);
//
//        token = lexer.peek(1);
//        if (token.token_type == LPAREN)
//        {
//            parse_poly_eval();
//            return;
//        }
//
//        return;
//    }
//    else if (token.token_type == NUM)
//    {
//        token = expect(NUM);
//        return;
//    }
//    else
//        return; // syntax_error();
//}

int main()
{
	// note: the parser class has a lexer object instantiated in it. You should not be declaring
    // a separate lexer object. You can access the lexer object in the parser functions as shown in the
    // example method Parser::ConsumeAllInput
    // If you declare another lexer object, lexical analysis will not work correctly
    Parser parser;

    parser.parse_input();

	parser.ConsumeAllInput();


    //changes start here


}
