#include <gtk/gtk.h>
#include <jansson.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"

#define FILESIZE   64 
#define LETTERSIZE 9
#define MAXEXAMPLES 32
#define LINESIZE 2048

static GtkWidget 
    *main_box, 
    *hbox, 
    *entry, 
    *button, 
    *scrolled_window,
    *inner_box, 
    *word_label, 
    *pluriform_label, 
    *class_label,
    *remark_label, 
    *etimology_label, 
    *variation_label,

    *meaning_title_label,

    *meaning_case_variation_label[MAXEXAMPLES], 
    *meaning_case_pt_label[MAXEXAMPLES], 
    *meaning_case_src_label[MAXEXAMPLES],

    *meaning_example_tp_label[MAXEXAMPLES], 
    *meaning_example_variation_label[MAXEXAMPLES], 
    *meaning_example_pt_label[MAXEXAMPLES], 
    *meaning_example_src_label[MAXEXAMPLES],
    
    *context_title_label,

    *context_case_tp_label[MAXEXAMPLES],
    *context_case_variation_label[MAXEXAMPLES], 
    *context_case_pt_label[MAXEXAMPLES],
    *context_case_src_label[MAXEXAMPLES],

    *context_example_tp_label[MAXEXAMPLES], 
    *context_example_variation_label[MAXEXAMPLES], 
    *context_example_pt_label[MAXEXAMPLES],
    *context_example_src_label[MAXEXAMPLES], 
    
    *note_title_label,
    *note_label, 

    *sources_title_label,
    *sources_label; 

// Função para verificar se o caractere inicial é uma vogal acentuada e normalizar
static char normalize_accented_vowel(const char *c) {
    // Normaliza vogais acentuadas para suas versões não acentuadas
    if (strncmp(c, "á", 2) == 0 || strncmp(c, "à", 2) == 0 || strncmp(c, "â", 2) == 0 || strncmp(c, "ã", 2) == 0) {
        return 'a';
    } else if (strncmp(c, "é", 2) == 0 || strncmp(c, "ê", 2) == 0) {
        return 'e';
    } else if (strncmp(c, "í", 2) == 0 || strncmp(c, "î", 2) == 0 || strncmp(c, "ì", 2) == 0) {
        return 'i';
    } else if (strncmp(c, "ó", 2) == 0 || strncmp(c, "ô", 2) == 0 || strncmp(c, "õ", 2) == 0) {
        return 'o';
    } else if (strncmp(c, "ú", 2) == 0 || strncmp(c, "û", 2) == 0 || strncmp(c, "ù", 2) == 0) {
        return 'u';
    }
    return '\0'; // Retorna '\0' se não for uma vogal acentuada
}

static char *get_word_letter(const char *word) {
    short index = 0;
    char *letter = malloc(LETTERSIZE * sizeof(char));
    if (!letter) return NULL; // Retorna NULL caso a alocação falhe

    // Ignora caracteres iniciais '-' e '\''
    while (word[index] == '-' || word[index] == '\'') {
        index++;
    }

    // Exceções para casos especiais
    if (strncmp(&word[index], "gû", 3) == 0) {
        letter[0] = 'g';
        letter[1] = '\0';
    } else if (strncmp(&word[index], "nh", 2) == 0) {
        letter[0] = 'n';
        letter[1] = 'h';
        letter[2] = '\0';
    }else {
        // Normaliza a primeira letra, se for vogal acentuada
        char normalized = normalize_accented_vowel(&word[index]);
        if (normalized) {
            letter[0] = normalized;
            letter[1] = '\0';
        } else if (isalpha((unsigned char)word[index])) {
            // Se for uma letra sem acento, copia a primeira letra normal
            letter[0] = word[index];
            letter[1] = '\0';
        } else {
            letter[0] = '\0'; // Retorna string vazia se não encontrar letra
        }
    }

    g_print("Letra retornada: %s\n", letter);

    return letter;
}


// Normaliza a palavra e retorna o nome do arquivo de dicionário
static char *get_dictionary_filename(const char *word)
{
    char *filename = malloc(FILESIZE * sizeof(char));
    char *letter = get_word_letter(word);

    sprintf(filename, "app/data/dictionary/dict_%s.json", letter);
    free(letter);

    return filename;
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

// Itera sobre todos os filhos para formatá-los
static void add_box_global_properties(void)
{
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(inner_box));
    while (child != NULL) {
        gtk_widget_set_margin_start(child, 10);
        gtk_widget_set_margin_end(child, 10);
        gtk_widget_set_halign(child, GTK_ALIGN_START);
        gtk_label_set_wrap(GTK_LABEL(child), TRUE);
        gtk_label_set_wrap_mode(GTK_LABEL(child), PANGO_WRAP_WORD);

        child = gtk_widget_get_next_sibling(child); // Armazena o próximo filho
    }
}

// Função que faz parse no dicionário e encontra o resultado imprimindo-o 
// Na GtkWidget inner_box como um label (futuramente cada parte do resultado
// será impresso em um label separado e formatado)
static bool parse_dictionary(json_t *root, const char *word) {
    // Navega até o objeto `root`
    json_t *root_obj = json_object_get(root, "root");
    if (!root_obj) {
        fprintf(stderr, "Objeto 'root' não encontrado.\n");
        return false;
    }

    const char *key;
    json_t *entry_array;

    int case_index = 0;
    int example_index = 0;

    // Limpa a inner_box para exibir novos rótulos para a pesquisa.
    clear_box(inner_box);

    // Itera sobre cada chave em `root` (como "a", "b", etc.)
    json_object_foreach(root_obj, key, entry_array) {
        json_t *entries = json_object_get(entry_array, "entries");
        if (!entries || !json_is_array(entries)) {
            fprintf(stderr, "Array 'entries' não encontrado.\n");
            continue;
        }

        // Itera sobre cada entrada em `entries`
        size_t i;
        json_t *entry;
        json_array_foreach(entries, i, entry) {
            // Obtém o campo "word" da entrada
            json_t *json_word = json_object_get(entry, "word");
            if (!json_word || !json_is_string(json_word)) {
                continue;
            }

            // Compara a palavra atual com a palavra de entrada
            if (strcmp(json_string_value(json_word), word) == 0) {
                // Imprime a entrada encontrada
                json_t *id = json_object_get(entry, "id");
                json_t *pluriform = json_object_get(entry, "pluriform");
                json_t *class = json_object_get(entry, "class");
                json_t *remark = json_object_get(entry, "remark");
                json_t *etimology = json_object_get(entry, "etimology");
                json_t *variation = json_object_get(entry, "variation");
                json_t *meaning = json_object_get(entry, "meaning");
                json_t *context = json_object_get(entry, "context");
                json_t *note = json_object_get(entry, "note");
                json_t *sources = json_object_get(entry, "sources");

                char *markup = malloc(LINESIZE *sizeof(char));

                // Cria e adiciona os labels dentro de inner_box
                word_label = gtk_label_new(NULL);
                sprintf(markup, "<span font='20' weight='bold' line-height='1.5'>%s</span>", json_string_value(json_word));
                gtk_label_set_markup(GTK_LABEL(word_label), markup);
                gtk_box_append(GTK_BOX(inner_box), word_label);

                if (pluriform) {
                    pluriform_label = gtk_label_new(json_string_value(NULL));
                    sprintf(markup, "<span font='12'>Pluriforme: </span><span font='11' style='italic'>%s</span>", json_string_value(pluriform));
                    gtk_label_set_markup(GTK_LABEL(pluriform_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), pluriform_label);
                }

                if (class) {
                    class_label = gtk_label_new(json_string_value(class));
                    sprintf(markup, "<span font='12'>Classe: </span><span font='11' style='italic'>%s</span>", json_string_value(class));
                    gtk_label_set_markup(GTK_LABEL(class_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), class_label);
                }

                if (remark) {
                    remark_label = gtk_label_new(json_string_value(remark));
                    sprintf(markup, "<span font='12'>Observação: </span><span font='11' style='italic'>%s</span>", json_string_value(remark));
                    gtk_label_set_markup(GTK_LABEL(remark_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), remark_label);
                }

                if (etimology) {
                    etimology_label = gtk_label_new(json_string_value(etimology));
                    sprintf(markup, "<span font='12'>Etimologia: </span><span font='11' style='italic'>%s</span>", json_string_value(etimology));
                    gtk_label_set_markup(GTK_LABEL(etimology_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), etimology_label);
                }

                if (variation) {
                    variation_label = gtk_label_new(json_string_value(variation));
                    sprintf(markup, "<span font='12'>Variação: </span><span font='11' style='italic'>%s</span>", json_string_value(variation));
                    gtk_label_set_markup(GTK_LABEL(variation_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), variation_label);
                }

                // DEBUG
                printf("ID: %s\n", json_string_value(id));
                printf("Palavra: %s\n", json_string_value(json_word));
                printf("Pluriform: %s\n", json_string_value(pluriform));
                printf("Classe: %s\n", json_string_value(class));
                printf("Remark: %s\n", json_string_value(remark));
                printf("Etimology: %s\n", json_string_value(etimology));
                printf("Variation: %s\n", json_string_value(variation));

                // Itera sobre `meaning` para imprimir os significados e exemplos
                if (meaning && json_is_array(meaning)) {
                    meaning_title_label = gtk_label_new(NULL);
                    sprintf(markup, "<span font='14' weight='bold' line-height='1.5'>Definições: </span>");
                    gtk_label_set_markup(GTK_LABEL(meaning_title_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), meaning_title_label);

                    size_t j;
                    json_t *meaning_entry;
                    json_array_foreach(meaning, j, meaning_entry) {
                        json_t *meaning_cases = json_object_get(meaning_entry, "cases");
                        if (meaning_cases && json_is_array(meaning_cases)) {
                            size_t meaning_k;
                            json_t *meaning_case_entry;
                            json_array_foreach(meaning_cases, meaning_k, meaning_case_entry) {
                                json_t *meaning_case_variation = json_object_get(meaning_case_entry, "variation");
                                json_t *meaning_case_pt = json_object_get(meaning_case_entry, "pt");
                                json_t *meaning_case_src = json_object_get(meaning_case_entry, "src");
                                
                                if (meaning_case_variation) { // não sei se há variação no case, mas pode haver
                                    meaning_case_variation_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12'>%s</span>", json_string_value(meaning_case_variation));
                                    gtk_label_set_markup(GTK_LABEL(meaning_case_variation_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), meaning_case_variation_label[case_index]);
                                }
                                
                                if (meaning_case_pt) {
                                    meaning_case_pt_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12' line-height='1.5'>* %s</span>", json_string_value(meaning_case_pt));
                                    gtk_label_set_markup(GTK_LABEL(meaning_case_pt_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), meaning_case_pt_label[case_index]);
                                }

                                if (meaning_case_src) {
                                    meaning_case_src_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12' style='italic'>%s</span>", json_string_value(meaning_case_src));
                                    gtk_label_set_markup(GTK_LABEL(meaning_case_src_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), meaning_case_src_label[case_index]);
                                }

                                case_index++;

                                // DEBUG
                                printf("Variação: %s\n", json_string_value(meaning_case_variation));
                                printf("Significado: %s\n", json_string_value(meaning_case_pt));
                                printf("Fonte: %s\n", json_string_value(meaning_case_src));

                                json_t *meaning_examples = json_object_get(meaning_case_entry, "examples");
                                if (meaning_examples && json_is_array(meaning_examples)) {
                                    size_t meaning_l;
                                    json_t *meaning_example;
                                    json_array_foreach(meaning_examples, meaning_l, meaning_example) {
                                        json_t *meaning_tp = json_object_get(meaning_example, "tp");
                                        json_t *meaning_variation = json_object_get(meaning_example, ("variation"));
                                        json_t *meaning_pt = json_object_get(meaning_example, "pt");
                                        json_t *meaning_src = json_object_get(meaning_example, "src");
                                        
                                        if (meaning_tp) {
                                            meaning_example_tp_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='12' weight='bold'>%s</span>", json_string_value(meaning_tp));
                                            gtk_label_set_markup(GTK_LABEL(meaning_example_tp_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), meaning_example_tp_label[example_index]);
                                        }

                                        if (meaning_variation) {
                                            meaning_example_variation_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='12' weight='bold' line-height='0.5'>[%s]</span>", json_string_value(meaning_variation));
                                            gtk_label_set_markup(GTK_LABEL(meaning_example_variation_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), meaning_example_variation_label[example_index]);
                                        }

                                        if (meaning_pt) {
                                            meaning_example_pt_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='12' line-height='0.5'>%s</span>", json_string_value(meaning_pt));
                                            gtk_label_set_markup(GTK_LABEL(meaning_example_pt_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), meaning_example_pt_label[example_index]);
                                        }

                                        if (meaning_src) {
                                            meaning_example_src_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='10' style='italic' line-height='0.5'>%s</span>", json_string_value(meaning_src));
                                            gtk_label_set_markup(GTK_LABEL(meaning_example_src_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), meaning_example_src_label[example_index]);
                                        }

                                        example_index++;

                                        // DEBUG
                                        printf("Exemplo: %s\nVariação: %s\nTradução: %s\n(Fonte: %s)\n", 
                                               json_string_value(meaning_tp),
                                               json_string_value(meaning_variation), 
                                               json_string_value(meaning_pt), 
                                               json_string_value(meaning_src));
                                    }
                                }
                            }
                        }
                    }
                }

                // reseta os índices para iterar sobre o context
                case_index = 0;
                example_index = 0;

                // // Itera sobre `context` para imprimir os significados e exemplos
                if (context && json_is_array(context)) {
                    context_title_label = gtk_label_new(NULL);
                    sprintf(markup, "<span font='14' weight='bold' line-height='1.5'>Contexto: </span>");
                    gtk_label_set_markup(GTK_LABEL(context_title_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), context_title_label);

                    size_t context_j;
                    json_t *context_entry;
                    json_array_foreach(context, context_j, context_entry) {
                        json_t *context_cases = json_object_get(context_entry, "cases");

                        if (context_cases && json_is_array(context_cases)) {
                            size_t context_k;
                            json_t *context_case_entry;
                            json_array_foreach(context_cases, context_k, context_case_entry) {
                                json_t *context_case_tp = json_object_get(context_case_entry, "tp");
                                json_t *context_case_variation = json_object_get(context_case_entry, "variation");
                                json_t *context_case_pt = json_object_get(context_case_entry, "pt");
                                json_t *context_case_src = json_object_get(context_case_entry, "src");

                                if (context_case_tp) { 
                                    context_case_tp_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12' weight='bold'>%s</span>", json_string_value(context_case_tp));
                                    gtk_label_set_markup(GTK_LABEL(context_case_tp_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), context_case_tp_label[case_index]);
                                }

                                if (context_case_variation) { 
                                    context_case_variation_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12'>%s</span>", json_string_value(context_case_variation));
                                    gtk_label_set_markup(GTK_LABEL(context_case_variation_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), context_case_variation_label[case_index]);
                                }
                                
                                if (context_case_pt) {
                                    context_case_pt_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12' line-height='1.5'>* %s</span>", json_string_value(context_case_pt));
                                    gtk_label_set_markup(GTK_LABEL(context_case_pt_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), context_case_pt_label[case_index]);
                                }

                                if (context_case_src) {
                                    context_case_src_label[case_index] = gtk_label_new(NULL);
                                    sprintf(markup, "<span font='12' style='italic'>%s</span>", json_string_value(context_case_src));
                                    gtk_label_set_markup(GTK_LABEL(context_case_src_label[case_index]), markup);
                                    gtk_box_append(GTK_BOX(inner_box), context_case_src_label[case_index]);
                                }

                                case_index++;

                                // DEBUG
                                printf(" - Exemplo: %s\n", json_string_value(context_case_tp));
                                if (context_case_variation) {
                                    printf("   Variação: %s\n", json_string_value(context_case_variation));
                                }
                                printf("   Tradução: %s\n", json_string_value(context_case_pt));
                                printf("   Fonte: %s\n", json_string_value(context_case_src));

                                json_t *context_examples = json_object_get(context_case_entry, "examples");
                                if (context_examples && json_is_array(context_examples)) {
                                    size_t context_l;
                                    json_t *context_example;
                                    json_array_foreach(context_examples, context_l, context_example) {
                                        json_t *context_tp = json_object_get(context_example, "tp");
                                        json_t *context_variation = json_object_get(context_example, ("variation"));
                                        json_t *context_pt = json_object_get(context_example, "pt");
                                        json_t *context_src = json_object_get(context_example, "src");
                                        
                                        if (context_tp) {
                                            context_example_tp_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='12' weight='bold'>%s</span>", json_string_value(context_tp));
                                            gtk_label_set_markup(GTK_LABEL(context_example_tp_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), context_example_tp_label[example_index]);
                                        }

                                        if (context_variation) {
                                            context_example_variation_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='12' weight='bold' line-height='0.5'>[%s]</span>", json_string_value(context_variation));
                                            gtk_label_set_markup(GTK_LABEL(context_example_variation_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), context_example_variation_label[example_index]);
                                        }

                                        if (context_pt) {
                                            context_example_pt_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='12' line-height='0.5'>%s</span>", json_string_value(context_pt));
                                            gtk_label_set_markup(GTK_LABEL(context_example_pt_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), context_example_pt_label[example_index]);
                                        }

                                        if (context_src) {
                                            context_example_src_label[example_index] = gtk_label_new(NULL);
                                            sprintf(markup, "<span font='10' style='italic' line-height='0.5'>%s</span>", json_string_value(context_src));
                                            gtk_label_set_markup(GTK_LABEL(context_example_src_label[example_index]), markup);
                                            gtk_box_append(GTK_BOX(inner_box), context_example_src_label[example_index]);
                                        }

                                        example_index++;

                                        // DEBUG
                                        printf("Exemplo: %s\nVariação: %s\nTradução: %s\n(Fonte: %s)\n", 
                                               json_string_value(context_tp),
                                               json_string_value(context_variation), 
                                               json_string_value(context_pt), 
                                               json_string_value(context_src));
                                    }
                                }
                            }
                        }
                    }
                }

                // Imprime `note`, se presente
                if (note && json_is_string(note)) {
                    note_title_label = gtk_label_new(NULL);
                    sprintf(markup, "<span font='14' weight='bold' line-height='1.5'>Nota: </span>");
                    gtk_label_set_markup(GTK_LABEL(note_title_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), note_title_label);

                    note_label = gtk_label_new(NULL);
                    sprintf(markup, "<span font='11'>%s</span>", json_string_value(note));
                    gtk_label_set_markup(GTK_LABEL(note_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), note_label);
                
                    // DEBUG
                    printf("Nota: %s\n", json_string_value(note));
                }

                // Imprime `sources`
                if (sources && json_is_string(sources)) {
                    sources_title_label = gtk_label_new(NULL);
                    sprintf(markup, "<span font='14' weight='bold' line-height='1.5'>Fontes: </span>");
                    gtk_label_set_markup(GTK_LABEL(sources_title_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), sources_title_label);

                    sources_label = gtk_label_new(NULL);
                    sprintf(markup, "<span font='10' style='italic'>%s</span>", json_string_value(sources));
                    gtk_label_set_markup(GTK_LABEL(sources_label), markup);
                    gtk_box_append(GTK_BOX(inner_box), sources_label);

                    // DEBUG
                    printf("Fonte: %s\n", json_string_value(sources));
                }

                // Propriedades globais adicionadas aos filhos (labels) do inner_box
                add_box_global_properties();

                // Retorna true pois a palavra foi encontrada
                return true;
            }
        }
    }

    // Retorna false se a palavra não foi encontrada
    return false;
}

// Callback para o botão de busca
static void on_search_button_clicked(GtkButton *button, gpointer user_data) {
    if (button) {}

    GtkWidget *entry = GTK_WIDGET(user_data);
    json_t *root;
    json_error_t error;

    // Obtém o texto da entrada
    const char *word = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Verifica se a palavra foi obtida com sucesso
    if (word == NULL || strlen(word) == 0) {
        clear_box(inner_box);
        GtkWidget *white = gtk_label_new("Entre com uma palavra...");
        gtk_widget_set_margin_start(white, 10);
        gtk_widget_set_halign(white, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(inner_box), white);
        g_print("Entrada em branco.\n");
        return;
    }

    // Debug
    g_print("Botão clicado! Buscando pela palavra: %s\n", word);

    // Pega o nome do arquivo
    char *filename = get_dictionary_filename(word);

    g_print("Nome do arquivo aberto: %s\n", filename);

    if (filename == NULL) {
        g_print("Arquivo não encontrado.");
        free(filename);
        return;
    }

    // Carregar o arquivo JSON
    root = json_load_file(filename, 0, &error);
    if (!root) {
        g_print("Erro ao carregar JSON: %s\n", error.text);
        free(filename);
        return;
    }

    if (!parse_dictionary(root, word)) {
        GtkWidget *not_found = gtk_label_new("Palavra não encontrada!");
        gtk_widget_set_margin_start(not_found, 10);
        gtk_widget_set_halign(not_found, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(inner_box), not_found);
        g_print("Palavra não encontrada!");
        json_decref(root);  // Libera a memória do objeto JSON
        free(filename);    
        return;
    }

    // Libera a memória do objeto JSON
    json_decref(root);
    
    free(filename);
}

// Função para criar a página do dicionário
GtkWidget* create_dictionary_page(void) {
    // Caixa principal (vertical)
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Caixa horizontal para entrada e botão
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    entry = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Digite uma palavra em Tupi Antigo...");
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
    gtk_widget_set_margin_end(hbox, 100);

    // Centraliza a caixa horizontal
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);

    // Adiciona a caixa horizontal
    gtk_box_append(GTK_BOX(main_box), hbox);

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

   // Sinal para o botão de busca
    g_signal_connect(button, "clicked", G_CALLBACK(on_search_button_clicked), entry);

    return main_box;
}
