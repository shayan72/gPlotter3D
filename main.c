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

/* Headers */

#include <gtk/gtk.h>
#include <glib.h>
#include <glib-object.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Matrix.h"
#include "analyze_expression.h"

/* Global Variables */

gdouble scale, k, angz, angy, ly, lz, fromx, tox, fromy, toy;
gulong keyPressId;
gchar string[500];
gint mode ;
gint mode1 = 1, mode2 = 2;
gboolean block1 , block2 ;

/* Shared Objects */

GtkWindow* window = NULL;
GtkButton* generateButton = NULL;
GtkMenuBar* menubar = NULL;
GtkWidget* drawarea = NULL;

GtkEntry* textFormula = NULL;
GtkEntry* textFromX = NULL;
GtkEntry* textToX = NULL;
GtkEntry* textFromY = NULL;
GtkEntry* textToY = NULL;
GtkEntry* textK = NULL;

GtkLabel* labelFormula;
GtkLabel* labelFromX;
GtkLabel* labelToX;
GtkLabel* labelFromY;
GtkLabel* labelToY;
GtkLabel* labelK;

GtkHBox* hbox = NULL;
GtkVBox* vbox = NULL;

/* Init */

void initOperators();
void create_window();
void initMenuBar();
void initLabelsTexts();

/* Signal Handlers */

void changeMode( GtkWidget* widget, gpointer data );
void draw_plot  (GtkWidget *widget, GdkEventExpose *event, gpointer data);
void keyPressEvent( GtkWidget *widget, GdkEventKey *event, gpointer user_data );
void blockUnblock( GtkWidget *widget, GdkEventKey *event, gpointer user_data );

int main (int argc, char *argv[])
{
	initOperators();

	gtk_init( &argc, &argv );

	create_window();

    // vbox

	vbox = (GtkVBox*)gtk_vbox_new( FALSE, 10 );
	gtk_container_add( GTK_CONTAINER(window), (GtkWidget*)vbox );

	// Menu

	initMenuBar();

	// Draw Area

	drawarea = gtk_drawing_area_new();
	gtk_box_pack_start( (GtkBox*)vbox, (GtkWidget*)drawarea, TRUE, TRUE, 10);

	// hbox

	hbox = (GtkHBox*)gtk_hbox_new( FALSE, 0 );
	gtk_box_pack_start( (GtkBox*)vbox, (GtkWidget*)hbox, FALSE, FALSE, 10);

	// Labels and texts

	initLabelsTexts();

	// Generate Button

	generateButton = (GtkButton*)gtk_button_new_with_label("Generate");


    gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)generateButton, FALSE, FALSE, 10);

	g_signal_connect_swapped ( G_OBJECT(generateButton), "clicked", G_CALLBACK (draw_plot), drawarea );
	keyPressId = g_signal_connect(window , "key_press_event",G_CALLBACK(keyPressEvent),NULL);

	gchar str[8];

	gchar* doubleToString( gdouble d )
	{

        sprintf( str, "%.2lf", d );

		return str;
	}

    gtk_entry_set_text(textFormula, "x^2+y^2");
	gtk_entry_set_text(textFromX, doubleToString(fromx) );
	gtk_entry_set_text(textFromY, doubleToString(fromy) );
	gtk_entry_set_text(textToX,doubleToString(tox) );
	gtk_entry_set_text(textToY,doubleToString(toy) );
	gtk_entry_set_text(textK, doubleToString(k) );

    block1 = TRUE ;
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (textFormula, "focus-in-event", G_CALLBACK ( blockUnblock ), &block1);
    g_signal_connect (textFromX, "focus-in-event", G_CALLBACK (blockUnblock), &block1);
    g_signal_connect (textFromY, "focus-in-event", G_CALLBACK (blockUnblock), &block1);
    g_signal_connect (textToY, "focus-in-event", G_CALLBACK (blockUnblock), &block1);
    g_signal_connect (textToX, "focus-in-event", G_CALLBACK (blockUnblock), &block1);
    g_signal_connect (textK, "focus-in-event", G_CALLBACK (blockUnblock), &block1);

    block2 = FALSE;

    g_signal_connect (textFormula, "focus-out-event", G_CALLBACK (blockUnblock),&block2);
    g_signal_connect (textFromX, "focus-out-event", G_CALLBACK (blockUnblock), &block2);
    g_signal_connect (textFromY, "focus-out-event", G_CALLBACK (blockUnblock), &block2);
    g_signal_connect (textToY, "focus-out-event", G_CALLBACK (blockUnblock), &block2);
    g_signal_connect (textToX, "focus-out-event", G_CALLBACK (blockUnblock), &block2);
    g_signal_connect (textK, "focus-out-event", G_CALLBACK (blockUnblock), &block2);

	// show

	gtk_widget_show_all((GtkWidget*)window);

	// main

	gtk_main();

	return 0;
}

void initOperators()
{
	strcpy( string, "" );

	strcpy( op[0].name, "abs" );
	op[0].type = FUNCTION;
	op[0].func = fabs;

	strcpy( op[1].name, "sin" );
	op[1].type = FUNCTION;
	op[1].func = sin;

	strcpy( op[2].name, "cos" );
	op[2].type = FUNCTION;
	op[2].func = cos;

	strcpy( op[3].name, "tan" );
	op[3].type = FUNCTION;
	op[3].func = tan;

	strcpy( op[4].name, "cot" );
	op[4].type = FUNCTION;
	op[4].func = cot;

	strcpy( op[5].name, "asin" );
	op[5].type = FUNCTION;
	op[5].func = asin;

	strcpy( op[6].name, "acos" );
	op[6].type = FUNCTION;
	op[6].func = acos;

	strcpy( op[7].name, "atan" );
	op[7].type = FUNCTION;
	op[7].func = atan;

	strcpy( op[8].name, "acot" );
	op[8].type = FUNCTION;
	op[8].func = acot;

	strcpy( op[9].name, "floor" );
	op[9].type = FUNCTION;
	op[9].func = floor;

	strcpy( op[10].name, "ceil" );
	op[10].type = FUNCTION;
	op[10].func = ceil;

	strcpy( op[11].name, "sqrt" );
	op[11].type = FUNCTION;
	op[11].func = sqrt;

	strcpy( op[12].name, "+" );
	op[12].type = UNARY;

	strcpy( op[13].name, "-" );
	op[13].type = UNARY;

	strcpy( op[14].name, "+" );
	op[14].type = BINARY;
	op[14].priority = 100;

	strcpy( op[15].name, "-" );
	op[15].type = BINARY;
	op[15].priority = 100;

	strcpy( op[16].name, "*" );
	op[16].type = BINARY;
	op[16].priority = 101;

	strcpy( op[17].name, "/" );
	op[17].type = BINARY;
	op[17].priority = 101;

	strcpy( op[18].name, "^" );
	op[18].type = BINARY;
	op[18].priority = 102;

	strcpy( op[19].name, "(" );
	op[19].type = PARENTHESES;

	strcpy( op[20].name, ")" );
	op[20].type = PARENTHESES;
}

void create_window()
{
	scale  = 35; // initial scale
    angz = 30*3.1415/180; // inital rotation z value
    angy = 60*3.1415/180;  // initial rotation y value
    k = 0.15;  // inital step

    fromx = -2.0 ;
    tox = 2.0 ;

    fromy = -2.0 ;
    toy = 2.0 ;

    mode = 1 ; /// initial mode !

    window = (GtkWindow*)gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "gPlotter3D");
    gtk_window_set_position( window , GTK_WIN_POS_CENTER );
    gtk_window_set_default_size(window , 800 , 600 );

    return;
}


void initMenuBar()
{
	menubar = (GtkMenuBar*)gtk_menu_bar_new();
	gtk_box_pack_start((GtkBox*)vbox,(GtkWidget*)menubar,FALSE,FALSE,2);
	GtkMenuItem* changeModeMenuItem = (GtkMenuItem*)gtk_menu_item_new_with_mnemonic("_Change Mode");
	gtk_menu_shell_append((GtkMenuShell*)menubar,(GtkWidget*)changeModeMenuItem);
	GtkMenu* changeModeMenu = (GtkMenu*)gtk_menu_new();
	gtk_menu_item_set_submenu((GtkMenuItem*)changeModeMenuItem,(GtkWidget*)changeModeMenu);
    GtkImageMenuItem* changeModeSingleMenuItem = (GtkImageMenuItem*)gtk_image_menu_item_new_with_label("Single drawing");
	gtk_menu_shell_append((GtkMenuShell*)changeModeMenu,(GtkWidget*)changeModeSingleMenuItem);
    g_signal_connect(G_OBJECT(changeModeSingleMenuItem),"activate",G_CALLBACK(changeMode),&mode1);
	GtkImageMenuItem* changeModeMultipleMenuItem = (GtkImageMenuItem*)gtk_image_menu_item_new_with_label("Multiple drawing");
	gtk_menu_shell_append((GtkMenuShell*)changeModeMenu,(GtkWidget*)changeModeMultipleMenuItem);
	g_signal_connect(G_OBJECT(changeModeMultipleMenuItem),"activate",G_CALLBACK(changeMode),&mode2);

}


void initLabelsTexts()
{
	labelFormula = (GtkLabel*)gtk_label_new("    Z = ");
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)labelFormula, FALSE, FALSE, 1);

	textFormula = (GtkEntry*)gtk_entry_new();
	gtk_entry_set_width_chars( textFormula, 25 );
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)textFormula, FALSE, FALSE, 10);

	//-----------

	labelFromX = (GtkLabel*)gtk_label_new("From X :");
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)labelFromX, FALSE, FALSE, 3);

	textFromX = (GtkEntry*)gtk_entry_new();
	gtk_entry_set_width_chars( textFromX, 4 );
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)textFromX, FALSE, FALSE, 10);

	//-----------

	labelToX = (GtkLabel*)gtk_label_new("To X :");
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)labelToX, FALSE, FALSE, 3);

	textToX = (GtkEntry*)gtk_entry_new();
	gtk_entry_set_width_chars( textToX, 4 );
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)textToX, FALSE, FALSE, 10);

	//-----------

	labelFromY = (GtkLabel*)gtk_label_new("From Y :");
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)labelFromY, FALSE, FALSE, 3);

	textFromY = (GtkEntry*)gtk_entry_new();
	gtk_entry_set_width_chars( textFromY, 4 );
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)textFromY, FALSE, FALSE, 10);

	//-----------

	labelToY = (GtkLabel*)gtk_label_new("To Y :");
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)labelToY, FALSE, FALSE, 3);

	textToY = (GtkEntry*)gtk_entry_new();
	gtk_entry_set_width_chars( textToY, 4 );
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)textToY, FALSE, FALSE, 10);

	//-----------

	labelK = (GtkLabel*)gtk_label_new("Step :");
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)labelK, FALSE, FALSE, 3);

	textK = (GtkEntry*)gtk_entry_new();
	gtk_entry_set_width_chars( textK, 4 );
	gtk_box_pack_start( (GtkBox*)hbox, (GtkWidget*)textK, FALSE, FALSE, 10);

	//-----------
}


void changeMode( GtkWidget* widget, gpointer data )
{
    mode = *((int*)data);
}


void blockUnblock (GtkWidget *widget ,
                    GdkEventKey *event ,
                    gpointer data  )
{

    if ( *((gboolean*)data) == FALSE )
        g_signal_handler_unblock( window, keyPressId );
    else
        g_signal_handler_block( window, keyPressId );
}

void keyPressEvent (GtkWidget *widget ,
                    GdkEventKey *event ,
                    gpointer user_data  )
{
    if ( mode == 2 ) return ;
    gint emit = 1;
    if ( event->keyval == GDK_Up ) // ("up");
        angy += 1*3.14/180;
    else if ( event->keyval == GDK_Down) // ("down");
        angy -= 1*3.14/180;
    else if ( event->keyval == GDK_Right ) // ("right");
        angz += 1*3.14/180;
    else if ( event->keyval == GDK_Left) // ("left");
        angz -= 1*3.14/180;
    else if ( event->keyval == 65451 || event->keyval == 43   ) // ("+");
        scale += 0.5;
    else if ( (event->keyval == 65453 || event->keyval == 95) && scale > 10  ) // ("-");
        scale -= 0.5;

    else
    {
    	emit = 0;
    }
    if( emit )
    {
    	gtk_signal_emit_by_name( (GtkObject*)generateButton, "clicked" );
    }
    return;
}



void
draw_plot  (GtkWidget *widget,
            GdkEventExpose *event,
            gpointer data)
{

    strcpy(string, gtk_entry_get_text(textFormula) );
    fromx = atof( gtk_entry_get_text( textFromX ) );
    tox = atof( gtk_entry_get_text( textToX ) );
    fromy = atof( gtk_entry_get_text( textFromY ) );
    toy = atof( gtk_entry_get_text( textToY ) );
    k = atof( gtk_entry_get_text( textK ) );


	if( mode != 2 )
		gtk_widget_draw(widget , NULL);

	gdouble mat[4][4];
    gdouble mat2[4][4];

    gint nx = ( tox - fromx  ) / k +1  ;
    gint ny = ( toy - fromy  ) / k +1 ;

    struct Vector *points  = malloc((nx*ny + 1 )*sizeof(struct Vector));
    struct Vector *points_orig  = malloc((nx*ny + 1 )*sizeof(struct Vector));

    gdouble i , j ;
    gint c1 = 0, c2 = 0, l = 0, lc1 = 0, lc2 = 0;
    struct Vector vec , last , last_orig;

    RotationY (mat2 , angy);
    RotationZ (mat , angz );
    mat_mult(mat,mat,mat2);

    gint cx , cy ;
    gtk_window_get_size ( window , &cx , &cy ) ;

    cx/=2 ;
    cy/=2 ;

    gdouble zmax = -99999 ;
    gdouble zmin = 99999 ;

    for ( c1 = 0 , i = fromx ; c1 < nx  ; i+=k , c1 ++)
    {
        for ( c2 = 0 , j = fromy ; c2 < ny ; j+=k , c2 ++)
        {
        	gdouble zzz = analyzeFormula( string, i, j );

        	if( zzz == UNKNOWN_DOUBLE )
        	{
        		g_print ( "Error in x,y = (%lf, %lf )\n", i, j );
        	}
        	else if ( zzz > zmax )
            {
                zmax = zzz;
            }
        	else if ( zzz < zmin )
            {
            	zmin = zzz;
            }

            vec.x = i ;
            vec.y = j ;
            vec.z = zzz ;

            points_orig [ c2 *nx   +  c1 ] = vec;

            vec = Transform( vec , mat );
            vec.draw=FALSE;
            if ( zzz != UNKNOWN_DOUBLE )
            	vec.draw=TRUE;

            points[ c2 *nx   +  c1 ] = vec;

            // ZBuffer (Depth Buffer) Implementation ....

            for ( l = 0 ; l < (c2 *nx   +  c1 -1)  ; l++ )
            {
               if ( points[l].draw == FALSE ) continue;
                if (  (  (gint)(scale*points[ l ].z) == (gint)(scale*points[ c2 *nx   +  c1  ].z) ) &&
                		(  (gint)(scale*points[ l ].y) ==  (gint)(scale*points[ c2 *nx   +  c1  ].y)))
                {
                    if  ( points[l].x <  points[c2*nx+c1].x )
                        points[l].draw= FALSE;
                    else
                        points[c2*nx+c1].draw= FALSE;
                }
            }

            // ...
        }
    }

    gint red = 0;
    gint green = 0;
    gint blue = 0;
    gint percent = 0;

    for ( c1 = 0 , i = fromx ; c1 < nx; i+=k , c1 ++)
    {
        for ( c2= 0 , j = fromy ; c2 < ny ; j+=k , c2 ++)
        {
            if ( points[c2*nx+c1].draw == FALSE ) continue;  /// Point Should'nt be drawn

            //percent =   (j-(fromy))*100 / (toy-fromy);
            percent =  ((points_orig[c2*nx+c1].z-zmin)*100)/(zmax-zmin) ;  /// maximum height / mimimum

            if(percent<25)
            {
                blue=255;
                green=(percent)*255/25;
                red=0;
            }
            else if(percent<50)
            {
                blue=(50-percent)*255/25;
                green=255;
                red=0;
            }
            else if(percent<75)
            {
                blue=0;
                green=255;
                red=(percent-50)*255/25;
            }
            else
            {
                blue=0;
                green=(100-percent)*255/25;
                red=255;
            }

            gdk_rgb_gc_set_foreground(widget->style->black_gc , 256*256*red + 256*green + blue );
            gdk_draw_point( widget->window , widget->style->black_gc  , cx - scale*points[c2*nx+c1].y , cy-scale*points[c2*nx+c1].z);

            if (  c2 == lc2+1 & c1==lc1  )
                gdk_draw_line( widget->window , widget->style->black_gc  , cx - scale*points[c2*nx+c1].y , cy-scale*points[c2*nx+c1].z ,  cx - last.y , cy-last.z);

            lc1 = c1 ;
            lc2 = c2 ;

            last.y = scale*points[c2*nx+c1].y;
            last.z = scale*points[c2*nx+c1].z;

            last_orig.y = points_orig[c2*nx+c1].y;
            last_orig.z = points_orig[c2*nx+c1].z;
        }
    }

    free(points);
    free(points_orig);

    return;
}

