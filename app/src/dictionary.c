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
        g_print("Não deixe em branco. Digite uma palavra em Tupi Antigo.\n");
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

    button = gtk_button_new_with_label("Pesquisar");
    gtk_widget_set_margin_top(button, 10);

    // Rótulo
    label = gtk_label_new("Digite uma palavra em Tupi Antigo.");
    gtk_widget_set_margin_start(label, 10);
    gtk_widget_set_margin_top(label, 10);
    
    // Adiciona a entrada e o botão à caixa horizontal
    gtk_box_append(GTK_BOX(hbox), entry);
    gtk_box_append(GTK_BOX(hbox), button);

    // Centraliza a caixa horizontal
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);

    // Adiciona a caixa horizontal e o rótulo à caixa principal
    gtk_box_append(GTK_BOX(main_box), hbox);
    gtk_box_append(GTK_BOX(main_box), label);

    // Armazena o label no botão para acesso posterior
    g_object_set_data(G_OBJECT(button), "label", label);

    // Centraliza o rótulo
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);

    // Sinal para o botão de busca
    g_signal_connect(button, "clicked", G_CALLBACK(on_search_button_clicked), entry);

    return main_box;
}


/*
// Callback para o botão de busca
void on_search_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(user_data);
    
    // Obtém o texto da entrada
    const char *word = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Verifica se a palavra foi obtida com sucesso
    if (word == NULL || strlen(word) == 0) {
        g_print("Não deixe em branco. Digite um apalavra em Tupi Antigo.\n");
        return;
    }

    g_print("Botão clicado! Buscando pela palavra: %s\n", word);

    // Busca pela definição
    const char *result = find_definition(word);
    
    // Atualiza o label com o resultado
    GtkWidget *result_label = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "label"));
    gtk_label_set_text(GTK_LABEL(result_label), result);
}

// Função para criar a página do dicionário
GtkWidget* create_dictionary_page() {
    GtkWidget *box, *entry, *button, *label;

    // Carrega o dicionário ao criar a página
    if (dictionary == NULL) {
        dictionary = load_dictionary("app/data/dictionary.json");
    }

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    entry = gtk_entry_new();
    button = gtk_button_new_with_label("Pesquisar");
    label = gtk_label_new("Digite uma palavra em Tupi Antigo.");

    // Sinal para o botão de busca
    g_signal_connect(button, "clicked", G_CALLBACK(on_search_button_clicked), entry);

    // Armazena o label no botão para acesso posterior
    g_object_set_data(G_OBJECT(button), "label", label);

    gtk_box_append(GTK_BOX(box), entry);
    gtk_box_append(GTK_BOX(box), button);
    gtk_box_append(GTK_BOX(box), label);

    return box;
}
*/
