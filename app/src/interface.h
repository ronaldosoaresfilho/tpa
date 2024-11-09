#ifndef INTERFACE_H
#define INTERFACE_H

#include <jansson.h>

#define LINESIZE     2048
#define MAXEXAMPLES    32

int initApp(int argc, char *argv[]);

void render_dict_word(json_t *word);
void render_dict_pluriform(json_t *pluriform);
void render_dict_class(json_t *class);
void render_dict_remark(json_t *remark);
void render_dict_etimology(json_t *etimology);
void render_dict_h_variation(json_t *variation);
void render_dict_title(char *title);
void render_dict_case_tp(json_t *tp);
void render_dict_case_variation(json_t *variation);
void render_dict_case_pt(json_t *pt);
void render_dict_case_src(json_t *src);
void render_dict_tp(json_t *tp);
void render_dict_variation(json_t *variation);
void render_dict_pt(json_t *pt);
void render_dict_src(json_t *src);
void render_dict_note(json_t *note);
void render_dict_sources(json_t *sources);
void render_dict_separator(void);

#endif