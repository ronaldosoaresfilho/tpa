#include "common.h"
#include "dictionary.h"
#include "interface.h"

/*
 * Função para verificar se o caractere inicsial é uma vogal acentuada e normalizar
 */
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

/*
 * Retorna a letra inicial do arquivo de dicionário
 */
static char *get_word_letter(const char *word) {
    short index = 0;
    char *letter = malloc(LETTERSIZE * sizeof(char));
    if (!letter) return NULL; // Retorna NULL caso a alocação falhe

    // Ignora caracteres inicsiais '-' e '\''
    while (word[index] == '-' || word[index] == '\'') {
        index++;
    }

    // Exceções para casos especsiais
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

    return letter;
}

/* 
 * Normaliza a palavra e retorna o nome do arquivo de dicsionário
 */
char *get_dictionary_filename(const char *word)
{
    char *filename = malloc(FILESIZE * sizeof(char));
    char *letter = get_word_letter(word);

    sprintf(filename, "app/data/dictionary/dict_%s.json", letter);
    free(letter);

    return filename;
}

/* 
 * Função que faz parse no dicsionário e encontra o resultado imprimindo-o 
 * Na GtkWidget inner_box como um label 
 */
bool parse_dictionary(json_t *root, const char *word) {
    // Navega até o objeto `root`
    json_t *jroot = json_object_get(root, "root");
    if (!jroot) {
        fprintf(stderr, "Objeto 'root' não encontrado.\n");
        return false;
    }

    bool found = false;
    const char *key;
    json_t *entry_array;

    // Itera sobre cada chave em `root` (como "a", "b", etc.)
    json_object_foreach(jroot, key, entry_array) {
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
                found = true;

                render_dict_separator();

                // Imprime a entrada encontrada
                //json_t *id = json_object_get(entry, "id");
                json_t *pluriform = json_object_get(entry, "pluriform");
                json_t *class = json_object_get(entry, "class");
                json_t *remark = json_object_get(entry, "remark");
                json_t *etimology = json_object_get(entry, "etimology");
                json_t *variation = json_object_get(entry, "variation");
                json_t *meaning = json_object_get(entry, "meaning");
                json_t *context = json_object_get(entry, "context");
                json_t *note = json_object_get(entry, "note");
                json_t *sources = json_object_get(entry, "sources");

                // Adiociona labels
                render_dict_word(json_word);
                render_dict_pluriform(pluriform);              
                render_dict_class(class);
                render_dict_remark(remark);
                render_dict_etimology(etimology);
                render_dict_h_variation(variation);

                // Itera sobre `meaning` para imprimir os significados e exemplos
                if (meaning && json_is_array(meaning)) {
                    
                    render_dict_title("Definições");
                    
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
                                
                                render_dict_case_variation(meaning_case_variation);
                                render_dict_case_pt(meaning_case_pt);
                                render_dict_case_src(meaning_case_src);

                                json_t *meaning_examples = json_object_get(meaning_case_entry, "examples");
                                if (meaning_examples && json_is_array(meaning_examples)) {
                                    size_t meaning_l;
                                    json_t *meaning_example;
                                    json_array_foreach(meaning_examples, meaning_l, meaning_example) {
                                        json_t *meaning_tp = json_object_get(meaning_example, "tp");
                                        json_t *meaning_variation = json_object_get(meaning_example, ("variation"));
                                        json_t *meaning_pt = json_object_get(meaning_example, "pt");
                                        json_t *meaning_src = json_object_get(meaning_example, "src");
                                        
                                        render_dict_tp(meaning_tp);
                                        render_dict_variation(meaning_variation);
                                        render_dict_pt(meaning_pt);
                                        render_dict_src(meaning_src);
                                    }
                                }
                            }
                        }
                    }
                }

                // Itera sobre `context` para imprimir os significados e exemplos
                if (context && json_is_array(context)) {
                    render_dict_title("Contexto");

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

                                render_dict_case_tp(context_case_tp);
                                render_dict_case_variation(context_case_variation);
                                render_dict_case_pt(context_case_pt);
                                render_dict_case_src(context_case_src);

                                json_t *context_examples = json_object_get(context_case_entry, "examples");
                                if (context_examples && json_is_array(context_examples)) {
                                    size_t context_l;
                                    json_t *context_example;
                                    json_array_foreach(context_examples, context_l, context_example) {
                                        json_t *context_tp = json_object_get(context_example, "tp");
                                        json_t *context_variation = json_object_get(context_example, ("variation"));
                                        json_t *context_pt = json_object_get(context_example, "pt");
                                        json_t *context_src = json_object_get(context_example, "src");
                                        
                                        render_dict_tp(context_tp);
                                        render_dict_variation(context_variation);
                                        render_dict_pt(context_pt);
                                        render_dict_src(context_src);
                                    }
                                }
                            }
                        }
                    }
                }

                // Imprime `note`, se presente
                if (note && json_is_string(note)) {
                    render_dict_title("Nota");
                    render_dict_note(note);
                }

                // Imprime `sources`
                if (sources && json_is_string(sources)) {
                    render_dict_title("Fontes");
                    render_dict_sources(sources);
                }
            }else if (found) {
                // Retorna true pois todas as ocorrências da palavra foi encontrada
                return true;        
            }
        }
    }

    // Retorna false se a palavra não foi encontrada
    return false;
}
