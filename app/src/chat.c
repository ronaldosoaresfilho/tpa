#include "common.h"
#include "chat.h"

char *chat(const char *text)
{
    char *t = (char *) malloc(TEXTSIZE * sizeof(char));
    
    sprintf(t, "Endé nhe'eng: %s", text);

    return t;
}

char *train(void)
{
    return "ok";
}