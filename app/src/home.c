#include <gtk/gtk.h>
#include "home.h"


GtkWidget* create_home_page() {
    GtkWidget *main_box, *vbox, *label;

    // Caixa principal (vertical)
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    // Caixa horizontal para entrada e botão
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Adiciona a caixa horizontal
    gtk_box_append(GTK_BOX(main_box), vbox);

    label = gtk_label_new("Bem-vindo ao Tupi Antigo!");
    // Centraliza o rótulo
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_end(label, 100);
    gtk_widget_set_margin_top(label, 100);

    gtk_box_append(GTK_BOX(vbox), label);

    return main_box;
}