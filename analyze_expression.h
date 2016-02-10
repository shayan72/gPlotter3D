/*
    Copyright 2012 Mohammad Razeghi , Shayan Salehian

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#ifndef ANALYZE_EXPRESSION_H_
#define ANALYZE_EXPRESSION_H_

#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>

#define OPSIZE 21
#define UNKNOWN_DOUBLE -100000.0

enum type
{
	NUMBER,
	OPERATOR
};

enum operatorType
{
	FUNCTION,
	UNARY,
	BINARY,
	PARENTHESES,
	NO_OPERATOR
};

enum numberType
{
	X,
	Y,
	NORMAL
};

struct Operator
{
	gchar name[5];
	enum operatorType type;
	gint priority; // for binary operators. priority of '+' is 100
	gdouble (*func)(gdouble);
}op[OPSIZE];

struct Number
{
	gdouble number;
	enum numberType type;
};

union Token
{
	struct Operator OP;
	struct Number NUM;
};

struct tokenList // linked list of tokens
{
	union Token to;
	enum type ty; // type of union
	struct tokenList *next;
}*head,*end;

// prototype of functions

struct tokenList * initTokenlist( union Token tt, enum type tt2 );
void addTokenlist( struct tokenList *new );
void deleteTokenlist();

gdouble cot( gdouble x );
gdouble acot( gdouble x );

void pushNum( gdouble i );
gdouble popNum();
gdouble topNum();

void pushOperator( struct Operator i );
struct Operator popOperator();
struct Operator topOperator();

gdouble analyzeFormula( gchar* , gdouble, gdouble );

#endif /* ANALYZE_EXPRESSION_H_ */
