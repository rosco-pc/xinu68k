void *memchr(const void *p, int v, size_t size);
int memcmp(const void *a, const void *b, size_t size);
void *memcpy(void *dst, const void *src, size_t size);
void *memset(void *dst, int c, size_t size);
void *memmove(void *dst, const void *src, size_t size);
int sprintf(char *dst, const char *fmt, ...);
char *strcat(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dst, const char *src);
size_t strlcat(char *dst, const char *src, size_t size);
size_t strlen(const char *s);
size_t strlcpy(char *dst, const char *src, size_t size);
char *strncat(char *s1, const char *s2, size_t size);
int strncmp(const char *s1, const char *s2, size_t size);
char *strncpy(char *s1, const char *s2, size_t size);
