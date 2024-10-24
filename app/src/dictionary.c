#include <gtk/gtk.h>
#include <jansson.h>
#include <string.h>
#include "dictionary.h"

GtkWidget* create_dictionary_page() {
    GtkWidget *label;
    label = gtk_label_new("Esta é a página do dicionário.");
    return label;
}