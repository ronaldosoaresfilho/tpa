#include <stdio.h>
#include <gtk/gtk.h>
#include "home.h"
#include "dictionary.h"

// Função responsável por alternar a visibilidade dos botões
static void toggle_buttons(GtkButton *button, gpointer user_data) {
    GtkWidget *button_home = GTK_WIDGET(user_data);
    GtkWidget *button_dict = gtk_widget_get_next_sibling(button_home);
    
    gboolean visible = gtk_widget_get_visible(button_home);
    gtk_widget_set_visible(button_home, !visible);
    gtk_widget_set_visible(button_dict, !visible);
}

// Função que é chamada quando o botão Home é clicado
static void on_home_clicked(GtkButton *button, GtkStack *stack) {
    gtk_stack_set_visible_child_name(stack, "home");
}

// Função que é chamada quando o botão Dictionary é clicado
static void on_dictionary_clicked(GtkButton *button, GtkStack *stack) {
    gtk_stack_set_visible_child_name(stack, "dictionary");
}

// Função de ativação da aplicação
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button_home, *button_dict, *toggle_button;
    GtkWidget *stack, *main_box, *button_box;

    // Janela principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "TPA");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Caixa principal que contém os botões e o conteúdo
    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Caixa para os botões
    button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(button_box, 10);
    gtk_widget_set_margin_top(button_box, 10);
    
    // Botões "Home" e "Dictionary"
    button_home = gtk_button_new_with_label("Início");
    button_dict = gtk_button_new_with_label("Dicionário");

    // Botão de Toggle
    toggle_button = gtk_button_new_with_label("≡");
    
    // Adicionando botões à caixa de botões
    gtk_box_append(GTK_BOX(button_box), toggle_button);
    gtk_box_append(GTK_BOX(button_box), button_home);
    gtk_box_append(GTK_BOX(button_box), button_dict);

    // Stack para as páginas
    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

    // Adicionar as páginas de Home e Dictionary
    gtk_stack_add_named(GTK_STACK(stack), create_home_page(), "home");
    gtk_stack_add_named(GTK_STACK(stack), create_dictionary_page(), "dictionary");

    // Adicionando a caixa de botões e a stack à caixa principal
    gtk_box_append(GTK_BOX(main_box), button_box);
    gtk_box_append(GTK_BOX(main_box), stack);

    // Sinais para alternar as páginas
    g_signal_connect(button_home, "clicked", G_CALLBACK(on_home_clicked), stack);
    g_signal_connect(button_dict, "clicked", G_CALLBACK(on_dictionary_clicked), stack);

    // Sinal para alternar a visibilidade dos botões
    g_signal_connect(toggle_button, "clicked", G_CALLBACK(toggle_buttons), button_home);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.rontekhne.tpa", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

