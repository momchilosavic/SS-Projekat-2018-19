#ifndef _WORD_H_
#define _WORD_H_
char *ltrim(char *str, const char *seps);
char *rtrim(char *str, const char *seps);
char *trim(char *str, const char *seps);

int match(const char *string, char *pattern);
#endif
