#include <stdio.h>
#include <locale.h>
#include <jansson.h>
#include <gtk/gtk.h>

static void button_clicked (GtkWidget *widget, gpointer data)
{
	g_print ("Botão clicado.\n");
}

static void activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *button;

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "TPA");
	gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

	button = gtk_button_new_with_label ("Olá, tupi!");

	g_signal_connect (button, "clicked", G_CALLBACK (button_clicked), NULL);
	gtk_window_set_child (GTK_WINDOW (window), button);

	gtk_window_present (GTK_WINDOW (window));
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "pt_BR.UTF-8");

	GtkApplication *app;
	int status;

	app = gtk_application_new ("com.rontekhne.tpa", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
