#include <gtk/gtk.h>
#include <jansson.h>
#include <string.h>
#include "dictionary.h"

/*GtkWidget* create_dictionary_page() {
    GtkWidget *label;
    label = gtk_label_new("Esta é a página do dicionário.");
    return label;
}*/

// Estrutura para armazenar entradas do dicionário
typedef struct {
    char *word;
    char *definition;
} DictionaryEntry;

// Variável global para armazenar o dicionário
static GList *dictionary = NULL;

// Função para carregar o dicionário a partir de um arquivo JSON
GList* load_dictionary(const char *path) {
    json_error_t error;
    json_t *root = json_load_file(path, 0, &error);
    
    if (!root) {
        g_print("Erro ao carregar JSON: %s\n", error.text);
        return NULL;
    }

    json_t *palavras = json_object_get(root, "palavras");
    if (!json_is_object(palavras)) {
        g_print("Erro: 'palavras' não é um objeto JSON.\n");
        json_decref(root);
        return NULL;
    }

    const char *key;
    json_t *value;
    
    // Itera sobre cada palavra no objeto JSON
    json_object_foreach(palavras, key, value) {
        DictionaryEntry *entry = g_malloc(sizeof(DictionaryEntry));
        entry->word = g_strdup(key);
        entry->definition = g_strdup(json_string_value(value));
        dictionary = g_list_append(dictionary, entry);
    }

    json_decref(root);
    return dictionary;
}

// Função para encontrar uma definição no dicionário
const char* find_definition(const char *word) {
    for (GList *l = dictionary; l != NULL; l = l->next) {
        DictionaryEntry *entry = (DictionaryEntry *)l->data;
        if (g_strcmp0(entry->word, word) == 0) {
            return entry->definition;
        }
    }
    return "Definição não encontrada.";
}


// Callback para o botão de busca
void on_search_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(user_data);
    
    // Obtém o texto da entrada
    const char *word = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Verifica se a palavra foi obtida com sucesso
    if (word == NULL || strlen(word) == 0) {
        g_print("Entrada em branco.\n");
        return;
    }

    g_print("Botão clicado! Buscando pela palavra: %s\n", word);

    // Busca pela definição
    const char *result = find_definition(word);
    
    // Atualiza o label com o resultado
    GtkWidget *result_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "label"));
    
    // Certifique-se de que result_label é um GtkLabel
    if (GTK_IS_LABEL(result_label)) {
        gtk_label_set_text(GTK_LABEL(result_label), result);
    } else {
        g_print("O objeto armazenado não é um GtkLabel.\n");
    }
}

// Função para criar a página do dicionário
GtkWidget* create_dictionary_page() {
    GtkWidget *main_box, *hbox, *entry, *button, *label;

    // Carrega o dicionário ao criar a página
    if (dictionary == NULL) {
        dictionary = load_dictionary("app/data/dictionary.json");
    }

    // Caixa principal (vertical)
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Caixa horizontal para entrada e botão
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    entry = gtk_entry_new();
    gtk_widget_set_margin_top(entry, 10);
    gtk_widget_set_size_request(entry, 300, -1);
    gtk_widget_set_hexpand(entry, TRUE);

    // Cria um botão com uma imagem com o ícone de lupa
    button = gtk_button_new();
    GtkWidget *search_icon_image = gtk_image_new_from_icon_name("system-search");
    // Adiciona a imagem ao botão e define a margem do topo
    gtk_button_set_child(GTK_BUTTON(button), search_icon_image);
    gtk_widget_set_margin_top(button, 10);

    
    // Adiciona a entrada e o botão à caixa horizontal
    gtk_box_append(GTK_BOX(hbox), entry);
    gtk_box_append(GTK_BOX(hbox), button);

    // Centraliza a caixa horizontal
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);

    // Adiciona a caixa horizontal
    gtk_box_append(GTK_BOX(main_box), hbox);

    // Centraliza o rótulo
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);

    // Sinal para o botão de busca
    g_signal_connect(button, "clicked", G_CALLBACK(on_search_button_clicked), entry);

    // Caixa de resultado

    // Cria uma GtkScrolledWindow para permitir rolagem
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(scrolled_window, -1, 200); // Define a altura da área de rolagem
    gtk_widget_set_vexpand(scrolled_window, TRUE); // expansão vertical
    gtk_widget_set_margin_bottom(scrolled_window, 20);

     // Cria uma outra GtkBox que ficará dentro da GtkScrolledWindow
    GtkWidget *inner_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(inner_box, 20);
    gtk_widget_set_margin_end(inner_box, 20);
    gtk_widget_set_margin_top(inner_box, 20);
    gtk_widget_set_margin_bottom(inner_box, 20);

     // Define a inner_box como o conteúdo da scrolled_window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), inner_box);

    // Adiciona a scrolled_window à main_box
    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    label = gtk_label_new("Digite uma palavra em Tupi Antigo.");
    gtk_widget_set_margin_start(label, 10);
    gtk_widget_set_margin_top(label, 10);

    // Ativar quebra de linha
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);

    // Adiciona o  rótulo à caixa principal
    gtk_box_append(GTK_BOX(inner_box), label);

    // Armazena o label no botão para acesso posterior
    g_object_set_data(G_OBJECT(button), "label", label);


    return main_box;
}
