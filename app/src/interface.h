#ifndef INTERFACE_H
#define INTERFACE_H

#include "dictionary.h"

#define LINESIZE     2048
#define MAXEXAMPLES    32

int initApp(int argc, char *argv[]);

void renderDictWord(json_t *word);
void renderDictPluriform(json_t *pluriform);
void renderDictClass(json_t *class);
void renderDictRemark(json_t *remark);
void renderDictEtimology(json_t *etimology);
void renderDictHVariation(json_t *variation);
void renderDictTitle(char *title);
void renderDictCaseTp(json_t *tp);
void renderDictCaseVariation(json_t *variation);
void renderDictCasePt(json_t *pt);
void renderDictCaseSrc(json_t *src);
void renderDictTp(json_t *tp);
void renderDictVariation(json_t *variation);
void renderDictPt(json_t *pt);
void renderDictSrc(json_t *src);
void renderDictNote(json_t *note);
void renderDictSources(json_t *sources);

#endif