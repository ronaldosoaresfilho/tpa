#include "common.h"
#include "home.h"
#include "dictionary.h"
#include "translator.h"
#include "interface.h"

/*
 * Seção principal
 */

GtkWidget* create_home_page(void); 
GtkWidget* create_dictionary_page(void);
GtkWidget* create_translator_page(void);

// Início
static GtkWidget 
    *hmain_box, 
    *hvbox, 
    *hlabel;

// Dicionário
static GtkWidget 
    *dmain_box, 
    *dhbox, 
    *dentry, 
    *dbutton, 
    *dscrolled_window,
    *dinner_box;

// Tradutor
static GtkWidget
    *tmain_box, 
    *thbox, 
    *tentry, 
    *tsearch_button,
    *ttrain_button, 
    *tscrolled_window,
    *tinner_box,
    *tresult_label;

/* 
 * função responsável por alternar a visibilidade dos botões
 */
static void toggle_buttons(GtkButton *button, gpointer user_data) {
    if (button) {}
    GtkWidget *button_home = GTK_WIDGET(user_data);
    GtkWidget *button_dict = gtk_widget_get_next_sibling(button_home);
    GtkWidget *button_trans = gtk_widget_get_next_sibling(button_dict);
    
    gboolean visible = gtk_widget_get_visible(button_home);
    gtk_widget_set_visible(button_home, !visible);
    gtk_widget_set_visible(button_dict, !visible);
    gtk_widget_set_visible(button_trans, !visible);
}

/* 
 * Função que é chamada quando o botão Inico é clicado
 */
static void on_home_clicked(GtkButton *button, GtkStack *stack) {
    if (button) {} // sem uso
    gtk_stack_set_visible_child_name(stack, "home");
}

/*
 * Função que é chamada quando o botão Dicionário é clicado
 */
static void on_dictionary_clicked(GtkButton *button, GtkStack *stack) {
    if (button) {} // sem uso
    gtk_stack_set_visible_child_name(stack, "dictionary");
}

/*
 * Função que é chamada quando o botão Tradutor é clicado
 */
static void on_translator_clicked(GtkButton *button, GtkStack *stack) {
    if (button) {} // sem uso
    gtk_stack_set_visible_child_name(stack, "translator");
}

/* 
 * Função de ativação da aplicação
 */
static void activate(GtkApplication *app, gpointer user_data) {
    if (user_data) {}
    GtkWidget *window;
    GtkWidget *button_home, *button_dict, *button_trans, *toggle_button;
    GtkWidget *stack, *main_box, *button_box;

    // Janela principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tupi Antigo");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_decorated(GTK_WINDOW(window), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    //gtk_window_fullscreen(GTK_WINDOW(window));

    // Caixa principal que contém os botões e o conteúdo
    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Caixa para os botões
    button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(button_box, 10);
    gtk_widget_set_margin_top(button_box, 10);
    
    // Botões "Início", "Dicionário" e "Tradutor"
    button_home = gtk_button_new_with_label("Início");
    button_dict = gtk_button_new_with_label("Dicionário");
    button_trans = gtk_button_new_with_label("Tradutor");

    // Botão Tradutor desabilitado
    gtk_widget_set_sensitive(GTK_WIDGET(button_trans), FALSE); 

    // Botão de Toggle
    toggle_button = gtk_button_new_with_label("≡");
    
    // Adicionando botões à caixa de botões
    gtk_box_append(GTK_BOX(button_box), toggle_button);
    gtk_box_append(GTK_BOX(button_box), button_home);
    gtk_box_append(GTK_BOX(button_box), button_dict);
    gtk_box_append(GTK_BOX(button_box), button_trans);

    // Stack para as páginas
    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

    // Adicionar as páginas de Home, Dictionary e Tradutor
    gtk_stack_add_named(GTK_STACK(stack), create_home_page(), "home");
    gtk_stack_add_named(GTK_STACK(stack), create_dictionary_page(), "dictionary");
    gtk_stack_add_named(GTK_STACK(stack), create_translator_page(), "translator");

    // Adicionando a caixa de botões e a stack à caixa principal
    gtk_box_append(GTK_BOX(main_box), button_box);
    gtk_box_append(GTK_BOX(main_box), stack);

    // Definindo a expansão do stack para ocupar o espaço disponível
    gtk_widget_set_hexpand(stack, TRUE);
    gtk_widget_set_vexpand(stack, TRUE);

    // Sinais para alternar entre as páginas
    g_signal_connect(button_home, "clicked", G_CALLBACK(on_home_clicked), stack);
    g_signal_connect(button_dict, "clicked", G_CALLBACK(on_dictionary_clicked), stack);
    g_signal_connect(button_trans, "clicked", G_CALLBACK(on_translator_clicked), stack);
    
    // Sinal para alternar a visibilidade dos botões
    g_signal_connect(toggle_button, "clicked", G_CALLBACK(toggle_buttons), button_home);

    gtk_window_present(GTK_WINDOW(window));
}

/* 
 * Inicializa a aplicação gráfica GTK. 
 */
int initApp(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.rontekhne.tpa", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

/*
 * Início
*/

GtkWidget* create_home_page(void) {
    // Caixa principal (vertical)
    hmain_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    // Caixa horizontal para entrada e botão
    hvbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Adiciona a caixa horizontal
    gtk_box_append(GTK_BOX(hmain_box), hvbox);

    hlabel = gtk_label_new("Bem-vindo ao Tupi Antigo!");
    // Centraliza o rótulo
    gtk_widget_set_halign(hlabel, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_end(hlabel, 100);
    gtk_widget_set_margin_top(hlabel, 100);

    gtk_box_append(GTK_BOX(hvbox), hlabel);

    return hmain_box;
}

/* 
 * Dicionário
 */

/* 
 * Função para limpar todos os widgets de uma inner_box
 */
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

/*
 * Itera sobre todos os filhos para formatá-los
 */
static void add_box_global_properties(GtkWidget *inner_box)
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

/*
 * Renderiza a palavra do dicionário
*/
void render_dict_word(json_t *word)
{
    GtkWidget *word_label = gtk_label_new(NULL);
    char *markup = malloc(LINESIZE * sizeof(char));
    sprintf(markup, "<span font='20' weight='bold' line-height='1.5'>%s</span>", json_string_value(word));
    gtk_label_set_markup(GTK_LABEL(word_label), markup);
    gtk_box_append(GTK_BOX(dinner_box), word_label);
    free(markup);
}

/*
 * Renderiza o pluriforme se houver
*/
void render_dict_pluriform(json_t *pluriform)
{
    if (pluriform) {
        GtkWidget *pluriform_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12'>Pluriforme: </span><span font='11' style='italic'>%s</span>", json_string_value(pluriform));
        gtk_label_set_markup(GTK_LABEL(pluriform_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), pluriform_label);
        free(markup);
    }
}

/*
 * Renderiza a classe se houver
*/
void render_dict_class(json_t *class)
{
    if (class) {
        GtkWidget *class_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12'>Classe: </span><span font='11' style='italic'>%s</span>", json_string_value(class));
        gtk_label_set_markup(GTK_LABEL(class_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), class_label);
        free(markup);
    }
}

/*
 * Renderiza a observação se houver
*/
void render_dict_remark(json_t *remark)
{
    if (remark) {
        GtkWidget *remark_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12'>Observação: </span><span font='11' style='italic'>%s</span>", json_string_value(remark));
        gtk_label_set_markup(GTK_LABEL(remark_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), remark_label);
        free(markup);
    }
}

/*
 * Renderiza a etimologia se houver
*/
void render_dict_etimology(json_t *etimology)
{
    if (etimology) {
        GtkWidget *etimology_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12'>Etimologia: </span><span font='11' style='italic'>%s</span>", json_string_value(etimology));
        gtk_label_set_markup(GTK_LABEL(etimology_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), etimology_label);
        free(markup);
    }
}

/*
 * Renderiza a variante da palavra se houver
*/
void render_dict_h_variation(json_t *variation)
{
    if (variation) {
        GtkWidget *variation_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12'>Variação: </span><span font='11' style='italic'>%s</span>", json_string_value(variation));
        gtk_label_set_markup(GTK_LABEL(variation_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), variation_label);
        free(markup);
    }
}

/*
 * Renderiza o título de definições
*/
void render_dict_title(char *title)
{
    GtkWidget *title_label = gtk_label_new(NULL);
    char *markup = malloc(LINESIZE * sizeof(char));
    sprintf(markup, "<span font='14' weight='bold' line-height='1.5'>%s: </span>", title);
    gtk_label_set_markup(GTK_LABEL(title_label), markup);
    gtk_box_append(GTK_BOX(dinner_box), title_label);
    free(markup);
}

/*
 * Renderiza o tupi de um caso se houver (geralmente em contexto)
*/
void render_dict_case_tp(json_t *tp)
{
    if (tp) { 
        GtkWidget *tp_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12' weight='bold'>%s</span>", json_string_value(tp));
        gtk_label_set_markup(GTK_LABEL(tp_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), tp_label);
        free(markup);
    }
}

/*
 * Renderiza a variante de um caso se houver
*/
void render_dict_case_variation(json_t *variation)
{
    if (variation) { // incerto
        GtkWidget *variation_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12'>%s</span>", json_string_value(variation));
        gtk_label_set_markup(GTK_LABEL(variation_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), variation_label);
        free(markup);
    }
}

/*
 * Renderiza o português de um caso se houver
*/
void render_dict_case_pt(json_t *pt)
{
    if (pt) {
        GtkWidget *pt_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12' line-height='1.5'>* %s</span>", json_string_value(pt));
        gtk_label_set_markup(GTK_LABEL(pt_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), pt_label);
        free(markup);
    }
}

/*
 * Renderiza a fonte de um caso se houver
*/
void render_dict_case_src(json_t *src)
{
    if (src) {
        GtkWidget *src_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12' style='italic'>%s</span>", json_string_value(src));
        gtk_label_set_markup(GTK_LABEL(src_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), src_label);
        free(markup);
    }
}

/*
 * Renderiza exemplo em Tupi
*/
void render_dict_tp(json_t *tp)
{
    if (tp) {
        GtkWidget *tp_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12' weight='bold'>%s</span>", json_string_value(tp));
        gtk_label_set_markup(GTK_LABEL(tp_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), tp_label);
        free(markup);
    }
}

/*
 * Renderiza variante em tupi
*/
void render_dict_variation(json_t *variation)
{
    if (variation) {
        GtkWidget *variation_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12' weight='bold' line-height='0.5'>[%s]</span>", json_string_value(variation));
        gtk_label_set_markup(GTK_LABEL(variation_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), variation_label);
        free(markup);
    }
}

/*
 * Renderiza a tradução do exemplo em português
*/
void render_dict_pt(json_t *pt)
{
    if (pt) {
        GtkWidget *pt_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='12' line-height='0.5'>%s</span>", json_string_value(pt));
        gtk_label_set_markup(GTK_LABEL(pt_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), pt_label);
        free(markup);
    }
}

/*
 * Renderiza a fonte do exemplo
*/
void render_dict_src(json_t *src)
{
    if (src) {
        GtkWidget *src_label = gtk_label_new(NULL);
        char *markup = malloc(LINESIZE * sizeof(char));
        sprintf(markup, "<span font='10' style='italic' line-height='0.5'>%s</span>", json_string_value(src));
        gtk_label_set_markup(GTK_LABEL(src_label), markup);
        gtk_box_append(GTK_BOX(dinner_box), src_label);
        free(markup);
    }
}

/*
 * Renderiza a nota
*/
void render_dict_note(json_t *note)
{
    GtkWidget *note_label = gtk_label_new(NULL);
    char *markup = malloc(LINESIZE * sizeof(char));
    sprintf(markup, "<span font='11'>%s</span>", json_string_value(note));
    gtk_label_set_markup(GTK_LABEL(note_label), markup);
    gtk_box_append(GTK_BOX(dinner_box), note_label);
    free(markup);
}

/*
 * Renderiza a nota
*/
void render_dict_sources(json_t *sources)
{
    GtkWidget *sources_label = gtk_label_new(NULL);
    char *markup = malloc(LINESIZE * sizeof(char));
    sprintf(markup, "<span font='10' style='italic'>%s</span>", json_string_value(sources));
    gtk_label_set_markup(GTK_LABEL(sources_label), markup);
    gtk_box_append(GTK_BOX(dinner_box), sources_label);
    free(markup);
}

/* 
 * Callback para o botão de busca do dicionário
 */
static void on_dict_search_button_clicked(GtkButton *button, gpointer user_data) {
    if (button) {}

    GtkWidget *entry = GTK_WIDGET(user_data);
    json_t *root;
    json_error_t error;

    // Obtém o texto da entrada
    const char *word = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Limpa a inner_box para eibir novos rótulos para a pesquisa.
    clear_box(dinner_box);

    // Verifica se a palavra foi obtida com sucesso
    if (word == NULL || strlen(word) == 0) {
        GtkWidget *white = gtk_label_new("Entre com uma palavra...");
        gtk_widget_set_margin_start(white, 10);
        gtk_widget_set_halign(white, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(dinner_box), white);
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
        gtk_box_append(GTK_BOX(dinner_box), not_found);
        g_print("Palavra não encontrada!");
        json_decref(root);  // Libera a memória do objeto JSON
        free(filename);    
        return;
    }

    // Propriedades globais adicsionadas aos filhos (labels) do inner_box
    add_box_global_properties(dinner_box);

    // Libera a memória do objeto JSON
    json_decref(root);
    
    free(filename);
}

/* 
 * Função para criar a página do dicionário
 */
GtkWidget* create_dictionary_page(void) {
    // Caixa princsipal (vertical)
    dmain_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Caixa horizontal para entrada e botão
    dhbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    dentry = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(dentry), "Digite uma palavra em Tupi Antigo...");
    gtk_widget_set_margin_top(dentry, 10);
    gtk_widget_set_size_request(dentry, 300, -1);
    gtk_widget_set_hexpand(dentry, TRUE);

    // Cria um botão com uma imagem com o ícone de lupa
    dbutton = gtk_button_new();
    GtkWidget *search_icon_image = gtk_image_new_from_icon_name("system-search");
    // Adicsiona a imagem ao botão e define a margem do topo
    gtk_button_set_child(GTK_BUTTON(dbutton), search_icon_image);
    gtk_widget_set_margin_top(dbutton, 10);

    
    // Adicsiona a entrada e o botão à caixa horizontal
    gtk_box_append(GTK_BOX(dhbox), dentry);
    gtk_box_append(GTK_BOX(dhbox), dbutton);
    gtk_widget_set_margin_end(dhbox, 100);

    // Centraliza a caixa horizontal
    gtk_widget_set_halign(dhbox, GTK_ALIGN_CENTER);

    // Adicsiona a caixa horizontal
    gtk_box_append(GTK_BOX(dmain_box), dhbox);

    // Caixa de resultado

    // Cria uma GtkScrolledWindow para permitir rolagem
    dscrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(dscrolled_window, -1, 200); // Define a altura da área de rolagem
    gtk_widget_set_vexpand(dscrolled_window, TRUE); // expansão vertical
    gtk_widget_set_margin_bottom(dscrolled_window, 20);

     // Cria uma outra GtkBox que ficará dentro da GtkScrolledWindow
    dinner_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(dinner_box, 20);
    gtk_widget_set_margin_end(dinner_box, 20);
    gtk_widget_set_margin_top(dinner_box, 20);
    gtk_widget_set_margin_bottom(dinner_box, 20);

     // Define a inner_box como o conteúdo da scrolled_window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(dscrolled_window), dinner_box);

    // Adicsiona a scrolled_window à main_box
    gtk_box_append(GTK_BOX(dmain_box), dscrolled_window);

   // Sinal para o botão de busca
    g_signal_connect(dbutton, "clicked", G_CALLBACK(on_dict_search_button_clicked), dentry);

    return dmain_box;
}

/*
 * Tradutor
*/

static void on_trans_train_button_clicked(GtkButton *button, gpointer user_data)
{
    if (button) {}
    if (user_data) {}

    // treina modelo

}

static void on_trans_search_button_clicked(GtkButton *button, gpointer user_data) 
{
    if (button) {}
    GtkWidget *entry = GTK_WIDGET(user_data);
    //json_t *root;
    //json_error_t error;
    
    // Interface e resultado da tradução
    clear_box(tinner_box);

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
    
    tresult_label = gtk_label_new(translation);
    gtk_widget_set_margin_start(tresult_label, 10);
    gtk_widget_set_margin_end(tresult_label, 10);
    gtk_widget_set_halign(tresult_label, GTK_ALIGN_START);
    gtk_label_set_wrap(GTK_LABEL(tresult_label), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(tresult_label), PANGO_WRAP_WORD);
    gtk_box_append(GTK_BOX(tinner_box), tresult_label);

    // Debug
    g_print("Tradução: %s\n", translation);
}

// Função para criar a página do dicionário
GtkWidget* create_translator_page(void) {
    // Caixa principal (vertical)
    tmain_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Caixa horizontal para entrada e botão
    thbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    tentry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(tentry), "Digite um texto em Tupi Antigo ou Portuguẽs...");
    gtk_widget_set_margin_start(tentry, 90);
    gtk_widget_set_margin_top(tentry, 10);
    gtk_widget_set_size_request(tentry, 600, -1);
    gtk_widget_set_hexpand(tentry, TRUE);

    // Cria o botão de pesquisa com uma imagem com o ícone de lupa
    tsearch_button = gtk_button_new();
    GtkWidget *search_icon_image = gtk_image_new_from_icon_name("system-search");
    // Adiciona a imagem ao botão e define a margem do topo
    gtk_button_set_child(GTK_BUTTON(tsearch_button), search_icon_image);
    gtk_widget_set_margin_top(tsearch_button, 10);

    // Cria o botão de treino com uma imagem com o ícone de ?
    ttrain_button = gtk_button_new();
    GtkWidget *train_icon_image = gtk_image_new_from_icon_name("applications-science");
    // Adiciona a imagem ao botão e define a margem do topo
    gtk_button_set_child(GTK_BUTTON(ttrain_button), train_icon_image);
    gtk_widget_set_margin_top(ttrain_button, 10);

    // Adiciona a entrada e o botão à caixa horizontal
    gtk_box_append(GTK_BOX(thbox), tentry);
    gtk_box_append(GTK_BOX(thbox), tsearch_button);
    gtk_box_append(GTK_BOX(thbox), ttrain_button);
    gtk_widget_set_margin_end(thbox, 100);

    // Centraliza a caixa horizontal
    gtk_widget_set_halign(thbox, GTK_ALIGN_CENTER);

    // Adiciona a caixa horizontal
    gtk_box_append(GTK_BOX(tmain_box), thbox);

    // Caixa de resultado
    // Cria uma GtkScrolledWindow para permitir rolagem
    tscrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_size_request(tscrolled_window, -1, 200); // Define a altura da área de rolagem
    gtk_widget_set_vexpand(tscrolled_window, TRUE); // expansão vertical
    gtk_widget_set_margin_bottom(tscrolled_window, 20);

    // Cria uma outra GtkBox que ficará dentro da GtkScrolledWindow
    tinner_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(tinner_box, 20);
    gtk_widget_set_margin_end(tinner_box, 20);
    gtk_widget_set_margin_top(tinner_box, 20);
    gtk_widget_set_margin_bottom(tinner_box, 20);

     // Define a inner_box como o conteúdo da scrolled_window
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(tscrolled_window), tinner_box);

    // Adiciona a scrolled_window à main_box
    gtk_box_append(GTK_BOX(tmain_box), tscrolled_window);

    // Sinal para o botão de busca e de treino
    g_signal_connect(tsearch_button, "clicked", G_CALLBACK(on_trans_search_button_clicked), tentry);
    g_signal_connect(ttrain_button, "clicked", G_CALLBACK(on_trans_train_button_clicked), NULL);

    return tmain_box;
}
