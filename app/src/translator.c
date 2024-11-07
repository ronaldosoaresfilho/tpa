#include <gtk/gtk.h>
#include <jansson.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "translator.h"

#define LINESIZE 2048

static GtkWidget 
    *main_box, 
    *hbox, 
    *entry, 
    *search_button,
    *train_button, 
    *scrolled_window,
    *inner_box,
    *result_label;

// Função tradutora
char *translate(const char *text)
{
    char *t = (char*)text;
    return t;
}

// Função para limpar todos os widgets da inner_box
static void clear_box(GtkWidget *box) 
{
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(box));

    // Itera sobre todos os filhos
    while (child != NULL) {
        GtkWidget *next_child = gtk_widget_get_next_sibling(child); // Armazena o próximo filho
        gtk_box_remove(GTK_BOX(box), child); // Remove o filho da box
        child = next_child; // Move para o próximo filho
    }
}

static void on_train_button_clicked(GtkButton *button, gpointer user_data)
{
    if (button) {}
    if (user_data) {}

    // treina modelo

}

static void on_search_button_clicked(GtkButton *button, gpointer user_data) 
{
    if (button) {}
    GtkWidget *entry = GTK_WIDGET(user_data);
    //json_t *root;
    //json_error_t error;
    
    // Interface e resultado da tradução
    clear_box(inner_box);

    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Verifica se o texto foi obtido com sucesso
    if (text == NULL || strlen(text) == 0) {
        g_print("Entrada em branco.\n");
        return;
    }

    // Debug
    g_print("Botão clicado! Texto de entrada: %s\n", text);

    // Tradução
    const char *translation = translate(text);
    
    result_label = gtk_label_new(translation);
    gtk_widget_set_margin_start(result_label, 10);
    gtk_widget_set_margin_end(result_label, 10);
    gtk_widget_set_halign(result_label, GTK_ALIGN_START);
    gtk_label_set_wrap(GTK_LABEL(result_label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(result_label), PANGO_WRAP_WORD);
    gtk_box_append(GTK_BOX(inner_box), result_label);

    // Debug
    g_print("Tradução: %s\n", translation);
}

// Função para criar a página do dicionário
GtkWidget* create_translator_page(void) {
    // Caixa principal (vertical)
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Caixa horizontal para entrada e botão
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite um texto em Tupi Antigo ou Portuguẽs...");
    gtk_widget_set_margin_start(entry, 90);
    gtk_widget_set_margin_top(entry, 10);
    gtk_widget_set_size_request(entry, 600, -1);
    gtk_widget_set_hexpand(entry, TRUE);

    // Cria o botão de pesquisa com uma imagem com o ícone de lupa
    search_button = gtk_button_new();
    GtkWidget *search_icon_image = gtk_image_new_from_icon_name("system-search");
    // Adiciona a imagem ao botão e define a margem do topo
    gtk_button_set_child(GTK_BUTTON(search_button), search_icon_image);
    gtk_widget_set_margin_top(search_button, 10);

    // Cria o botão de treino com uma imagem com o ícone de ?
    train_button = gtk_button_new();
    GtkWidget *train_icon_image = gtk_image_new_from_icon_name("applications-science");
    // Adiciona a imagem ao botão e define a margem do topo
    gtk_button_set_child(GTK_BUTTON(train_button), train_icon_image);
    gtk_widget_set_margin_top(train_button, 10);

    // Adiciona a entrada e o botão à caixa horizontal
    gtk_box_append(GTK_BOX(hbox), entry);
    gtk_box_append(GTK_BOX(hbox), search_button);
    gtk_box_append(GTK_BOX(hbox), train_button);
    gtk_widget_set_margin_end(hbox, 100);

    // Centraliza a caixa horizontal
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);

    // Adiciona a caixa horizontal
    gtk_box_append(GTK_BOX(main_box), hbox);

    // Imagem de manutenção
    GtkWidget *image_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(main_box), image_box);
    GtkWidget *image = gtk_image_new_from_file("app/images/development.png");
    gtk_widget_set_size_request(image, 256, 256); 
    gtk_widget_set_hexpand(image, TRUE);
    gtk_widget_set_vexpand(image, TRUE);
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER); 
    gtk_widget_set_valign(image, GTK_ALIGN_CENTER); 
    gtk_box_append(GTK_BOX(image_box), image);

    // Caixa de resultado
    // Cria uma GtkScrolledWindow para permitir rolagem
    scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(scrolled_window, -1, 200); // Define a altura da área de rolagem
    gtk_widget_set_vexpand(scrolled_window, TRUE); // expansão vertical
    gtk_widget_set_margin_bottom(scrolled_window, 20);

     // Cria uma outra GtkBox que ficará dentro da GtkScrolledWindow
    inner_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(inner_box, 20);
    gtk_widget_set_margin_end(inner_box, 20);
    gtk_widget_set_margin_top(inner_box, 20);
    gtk_widget_set_margin_bottom(inner_box, 20);

     // Define a inner_box como o conteúdo da scrolled_window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), inner_box);

    // Adiciona a scrolled_window à main_box
    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    // Sinal para o botão de busca e de treino
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), entry);
    g_signal_connect(train_button, "clicked", G_CALLBACK(on_train_button_clicked), NULL);

    return main_box;
}
