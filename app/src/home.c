#include <gtk/gtk.h>
#include "home.h"


GtkWidget* create_home_page() {
    GtkWidget *label;
    label = gtk_label_new("Bem-vindo ao Tupi Antigo!");
    return label;
}