#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Gameboard.h"

GtkWidget *button[100];
GtkWidget *window;
GtkWidget *percent;
Gameboard *board;

int lose;
int win;
int width;
int height;
int mines;
int seed;

void show_lose();
void show_win();
void button_clicked(GtkWidget *widget, gpointer data);

int main( int argc, char *argv[])
{
    height = 10;
    width = 10;
    mines = 10;
    seed = -1;
    
    GtkWidget *fixed;
    board = new Gameboard(width, height, mines);
    //board->generate();
    lose = false;
    win = false;
    
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");
    gtk_window_set_default_size(GTK_WINDOW(window), width*25, height*25+40);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    
    percent = gtk_label_new("% Clear: 0");
    gtk_fixed_put(GTK_FIXED(fixed), percent, 0, 0);
    
    for (int i=0; i<height; i++)
        for (int j=0; j<width; j++)
        {
            button[(i*width)+j] = gtk_button_new_with_label("#");
            
            gtk_fixed_put(GTK_FIXED(fixed), button[(i*width)+j], (j)*25, (i)*25+40);
            gtk_widget_set_size_request(button[(i*width)+j], 25, 25);
            
            g_signal_connect(G_OBJECT(button[(i*width)+j]), "clicked", 
                G_CALLBACK(button_clicked), GINT_TO_POINTER((i*width)+j));
                //G_CALLBACK(gtk_main_quit), G_OBJECT(window));
        }
    g_signal_connect_swapped(G_OBJECT(window), "destroy", 
        G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    return 0;
}

void button_clicked(GtkWidget *widget, gpointer data)
{
    if(!lose && !win)
    {
        int i = GPOINTER_TO_INT(data);
        char buffer [10];
        g_ascii_dtostr (buffer,10,i);
        //g_print(buffer);
        //button[i] = gtk_button_new_with_label(buffer);
        board->pickSquare(i/width, i%width);
        board->getCoordinate(i/width, i%width);
        
        for (int j=0; j<height; j++)
            for (int k=0; k<width; k++)
            {
                int val = board->getCoordinate(j, k, 'p');
                if (val == TYPE_MINE){
                    gtk_object_set(GTK_OBJECT(button[(j*width)+k]), "label", "M", NULL);
                    lose = true;
                }
                else if (val == TYPE_HIDDEN)
                    gtk_object_set(GTK_OBJECT(button[(j*width)+k]), "label", "#", NULL);
                else if (val == TYPE_FLAG)
                    gtk_object_set(GTK_OBJECT(button[(j*width)+k]), "label", "!", NULL);
                else if (val == 0)
                {
                    gtk_button_set_relief(GTK_BUTTON(button[(j*width)+k]),GTK_RELIEF_NONE);
                    gtk_object_set(GTK_OBJECT(button[(j*width)+k]), "label", " ", NULL);
                }
                else
                    gtk_object_set(GTK_OBJECT(button[(j*width)+k]), "label",
                                   g_ascii_dtostr (buffer,10,val), NULL);
            }
            
        g_ascii_dtostr (buffer,10,(int)(board->percentComplete()));
        char tmp[20];
        strcat(tmp, "% Clear: ");
        strcat(tmp, buffer);
        gtk_label_set_label(GTK_LABEL(percent), tmp);
        
        if(lose)
            show_lose();
        if(board->percentComplete() == 100.0)
        {
            win = true;
            show_win();
        }
    }
}
void show_lose()
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "You Lose!");
    gtk_window_set_title(GTK_WINDOW(dialog), "Loser");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
void show_win()
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "You Win!");
    gtk_window_set_title(GTK_WINDOW(dialog), "Winner");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
