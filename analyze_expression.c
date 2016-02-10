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
#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "analyze_expression.h"


//================================================

gdouble *nums;
gint numsSize = 0;
gint usedNumsSize = 0;

struct Operator *operator;
gint operatorSize = 0;
gint usedOperatorSize = 0;

//================================================

// TOKEN LIST.....................................

struct tokenList * initTokenlist( union Token tt, enum type tt2 )
{
	struct tokenList *ptr;

	ptr = ( struct tokenList * )calloc( 1, sizeof( struct tokenList ) );

	ptr->to = tt;
	ptr->ty = tt2;

	return ptr;
}

void addTokenlist( struct tokenList *new )
{
	if( head == NULL )
	{
		head = new;
		end = new;
	}

	end->next = new;
	new->next = NULL;
	end = new;
}

void deleteTokenlist()
{
	struct tokenList *ptr = head;
	struct tokenList *temp;

   if( head == NULL ) return;

   head = NULL;
   end = NULL;

   while( ptr != NULL ) {
      temp = ptr->next;
      free( ptr );
      ptr = temp;
   }
}

//================================================

// PARSER.........................................

gboolean parseNextOperator( gchar* string, struct Operator *o )
{
	gint i;
	for( i = 0; i < OPSIZE; i++ )
	{
		if( strncmp( string, op[i].name, strlen( op[i].name ) ) == 0 )
		{
			*o = op[i];
			return TRUE;
		}
	}

//	g_print( "ERROR!! \"%s\" is UNKNOWN TOKEN\n", string );
	return FALSE;

}

//================================================

// COT && ACOT FUNCTIONS..........................

gdouble cot( gdouble x )
{
	return ( 1.0/tan(x) );
}

gdouble acot( gdouble x )
{
	return ( atan(1.0/x) );
}

//================================================

// STACKS.........................................

void pushNum( gdouble i )
{
     if( usedNumsSize >= numsSize )
     {
    	 numsSize *= 2;
         nums = realloc( nums, numsSize*sizeof(gdouble) );
     }

     nums[usedNumsSize] = i;
     usedNumsSize++;
}/* end of function */

gdouble popNum()
{
	if( usedNumsSize != 0 )
	{
		usedNumsSize--;

		return nums[usedNumsSize];
	}

//	g_print( "ERROR!! you are popping no number!\n");
	return UNKNOWN_DOUBLE;

}/* end of function */

gdouble topNum()
{
	if( usedNumsSize != 0 )
		return nums[usedNumsSize-1];
	return -10000.0;
}/* end of function */


void pushOperator( struct Operator i )
{
     if( usedOperatorSize >= operatorSize )
     {
    	 operatorSize *= 2;
    	 operator = realloc( operator, operatorSize*sizeof(struct Operator) );
     }

     operator[usedOperatorSize] = i;
     usedOperatorSize++;

}/* end of function */

struct Operator popOperator()
{
	if( usedOperatorSize != 0 )
	{
		usedOperatorSize--;

		return operator[usedOperatorSize];
	}

	struct Operator o;
	o.type = NO_OPERATOR;

	return o;
}/* end of function */

struct Operator topOperator()
{
	if( usedOperatorSize != 0 )
		return operator[usedOperatorSize-1];

	struct Operator o;
	o.type = NO_OPERATOR;

	return o;

}/* end of function */

//================================================

// ANALYZING......................................

gdouble analyzeFormula( gchar *string, gdouble x, gdouble y )
{
	gboolean effectBinaryOperator()
	{
		if( usedNumsSize <= 1 )
		{
//			g_print("ERROR!\n");
			return FALSE;
		}

		gdouble num2 = popNum();
		gdouble num1 = popNum();

		gdouble temp;

		switch( popOperator().name[0] )
		{
			case '+':
				temp = num1 + num2;
				break;
			case '-':
				temp = num1 - num2;
				break;
			case '*':
				temp = num1 * num2;
				break;
			case '/':
				if( num2 == 0.0 )
				{
//					g_print("ERROR! There is a DIVISION_BY_ZERO error ");
					return FALSE;
				}
				temp = num1 / num2;
				break;
			case '^':
				temp = pow( num1, num2 );
				break;
		}
		pushNum(temp);

		return TRUE;
	}


	numsSize = 0;
	operatorSize = 0;

	usedNumsSize = 0;
	usedOperatorSize = 0;

	deleteTokenlist();

	struct tokenList *ptr;
	struct tokenList *previous_ptr;

	union Token firstT;
	enum type firstT_type;

	firstT.OP.type = NO_OPERATOR;
	firstT_type = OPERATOR;

	ptr = initTokenlist( firstT, firstT_type );
	addTokenlist( ptr );

	previous_ptr = ptr;

	// add tokens to token list

	while( 1 )
	{
		union Token t;
		enum type t2;

		if( string[0] == '\0' )
			break;

		if( string[0] == ' ' || string[0] == '\t'  )
		{
			string++;
			continue;
		}

		if( string[0] == 'x' )
		{
			t.NUM.type = X;
			t2 = NUMBER;
			numsSize++;
			string++;
		}
		else if( string[0] == 'y' )
		{
			t.NUM.type = Y;
			t2 = NUMBER;
			numsSize++;
			string++;
		}
		else if( string[0] >= '0' && string[0] <= '9' )
		{
			t.NUM.number = strtod(string, &string);
			t.NUM.type = NORMAL;
			t2 = NUMBER;
			numsSize++;
		}
		else
		{
			if( !parseNextOperator(string, &t.OP ) )
			{
				return UNKNOWN_DOUBLE;
			}

			t2 = OPERATOR;
			operatorSize++;
			string += strlen( t.OP.name );
		}

		ptr = initTokenlist( t, t2 );
		addTokenlist( ptr );
	}

	nums = calloc( numsSize, sizeof(gdouble) );
	operator = (struct Operator*)calloc( operatorSize, sizeof(struct Operator) );

	// Calculate Z

	previous_ptr = head; // head is no operator


	if( previous_ptr->next != NULL )
	{
		ptr = previous_ptr->next;

		while( 1 )
		{
			if( ptr->ty == NUMBER )
			{
				// PUSH NUMBER TO NUM STACK

				gdouble num;

				if( ptr->to.NUM.type == NORMAL )
				{
					num = ptr->to.NUM.number;
				}
				else if( ptr->to.NUM.type == X )
				{
					num = x;
				}
				else if( ptr->to.NUM.type == Y )
				{
					num = y;
				}

				if( previous_ptr->ty == NUMBER )
				{
//					g_print( "ERROR!! There is a number after number!!\n" );
					return UNKNOWN_DOUBLE;
				}
				if( previous_ptr->ty == OPERATOR && previous_ptr->to.OP.name[0] == ')' )
				{
//					g_print( "ERROR!! There is a number after close parentheses!!\n" );
					return UNKNOWN_DOUBLE;
				}

				pushNum( num );

				while( topOperator().type == FUNCTION || topOperator().type == UNARY )
				{
					if( topOperator().type == FUNCTION )
					{
						if( usedNumsSize > 0 && usedOperatorSize > 0 )
						{
							if( topOperator().func == sqrt && topNum() < 0.0 )
							{
//								g_print("Semantic ERROR!\n");
								return UNKNOWN_DOUBLE;
							}
							pushNum( popOperator().func( popNum() ) );
						}
						else
						{
//							g_print("ERROR!\n");
							return UNKNOWN_DOUBLE;
						}
					}
					else
					{
						switch( popOperator().name[0] )
						{
							case '+':
								// do nothing
								break;
							case '-':
								if( usedNumsSize > 0 )
									pushNum( -popNum() );
								else
								{
//									g_print("ERROR!\n");
									return UNKNOWN_DOUBLE;
								}
								break;
						}
					}
				}
			}
			else if( ptr->ty == OPERATOR )
			{
				// PUSH OPERATOR TO OPERATOR STACK

				//..............

				if( ptr->to.OP.type == UNARY /*&&   // by default all of '+' && '-' are unary !
					( ptr->to.OP.name[0] == '+' || ptr->to.OP.name[0] == '-' )*/ )
				{
					if( previous_ptr->ty == NUMBER ||
						( previous_ptr->ty == OPERATOR && previous_ptr->to.OP.name[0] == ')' ) )
					{
						ptr->to.OP.type = BINARY;
					}
				}

				//..............

				if( ptr->to.OP.type == FUNCTION || ptr->to.OP.type == UNARY )
				{
					if( ptr->to.OP.type == FUNCTION )
					{
						if( previous_ptr->ty == NUMBER )
						{
//							g_print( "ERROR!! There is a function after number!!" );
							return UNKNOWN_DOUBLE;
						}
						if( previous_ptr->ty == OPERATOR && previous_ptr->to.OP.name[0] == ')' )
						{
//							g_print( "ERROR!! There is a function after close parentheses!!\n" );
							return UNKNOWN_DOUBLE;
						}
					}

					pushOperator( ptr->to.OP );
				}
				else if( ptr->to.OP.type == BINARY )
				{
					if( ( previous_ptr->ty != NUMBER ) &&
						!( previous_ptr->ty == OPERATOR && previous_ptr->to.OP.name[0] == ')' ) )
					{
//						g_print( "ERROR!! There isn't any number or close parentheses before binary operation!!\n" );
						return UNKNOWN_DOUBLE;
					}

					while( topOperator().type == BINARY &&
						   ( topOperator().priority > ptr->to.OP.priority ||
						   ( topOperator().priority == ptr->to.OP.priority && topOperator().name[0] != '^' ) ) )
					{
						if( !effectBinaryOperator() )
							return UNKNOWN_DOUBLE;
					}

					pushOperator( ptr->to.OP );
				}
				else if( ptr->to.OP.type == PARENTHESES )
				{
					if( ptr->to.OP.name[0] == '(' )
					{
						if( previous_ptr->ty == NUMBER )
						{
//							g_print( "ERROR!! There is a open parentheses after number!!\n" );
							return UNKNOWN_DOUBLE;
						}
						if( previous_ptr->ty == OPERATOR && previous_ptr->to.OP.name[0] == ')' )
						{
//							g_print( "ERROR!! There is a open parentheses after close parentheses!!\n" );
							return UNKNOWN_DOUBLE;
						}

						pushOperator(ptr->to.OP);
					}
					else if( ptr->to.OP.name[0] == ')' )
					{
						if( previous_ptr->ty != NUMBER &&
							!( previous_ptr->ty == OPERATOR && previous_ptr->to.OP.type == PARENTHESES ) )
						{
//							g_print( "ERROR!! Wrong close parentheses location\n" );
							return UNKNOWN_DOUBLE;
						}
						while( topOperator().type != PARENTHESES || topOperator().name[0] != '(' )
						{
							if( topOperator().type == NO_OPERATOR )
							{
//								g_print( "ERROR!! parentheses does not match\n" );
								return UNKNOWN_DOUBLE;
							}
							if( topOperator().type == BINARY )
							{
								if( !effectBinaryOperator() )
									return UNKNOWN_DOUBLE;
							}
							else
							{
								return UNKNOWN_DOUBLE;
							}
						}

						popOperator(); // pop '('

						// after calculating inside of parentheses, it's just like we enter a number
						// so we should check functions and unaries for that number

						while( topOperator().type == FUNCTION || topOperator().type == UNARY )
						{
							if( topOperator().type == FUNCTION )
							{
								if( usedNumsSize > 0 && usedOperatorSize > 0 )
								{
									if( topOperator().func == sqrt && topNum() < 0.0 )
									{
//										g_print("Semantic ERROR!\n");
										return UNKNOWN_DOUBLE;
									}
									pushNum( popOperator().func( popNum() ) );
								}
								else
								{
//									g_print( "ERROR!\n");
									return UNKNOWN_DOUBLE;
								}
							}
							else
							{
								switch( popOperator().name[0] )
								{
									case '+':
										// do nothing
										break;
									case '-':
										if( usedNumsSize > 0 )
											pushNum( -popNum() );
										else
										{
//											g_print("ERROR!\n");
											return UNKNOWN_DOUBLE;
										}
										break;
								}
							}
						}
					}
				}
			}

			if( ptr->next == NULL )
				break;

			previous_ptr = ptr;
			ptr = ptr->next;
		}
	}

	while( topOperator().type == BINARY )
	{
		if( !effectBinaryOperator() )
			return UNKNOWN_DOUBLE;
	}

	if( usedNumsSize == 1 && usedOperatorSize == 0 )
		return popNum();
	else
	{
//		g_print("ERROR! syntax error\n");
		return UNKNOWN_DOUBLE;
	}
}

//================================================
