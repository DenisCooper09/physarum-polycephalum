#include "PP_FileReader.h"

void PP_ReadFile(const char *restrict path, char **restrict output, size_t *sz)
{
    FILE *file = fopen(path, "rb");

    if (!file)
    {
        PP_ERROR("Unable to open file - Path: %s", path);
        return;
    }

    fseek(file, 0L, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    *output     = malloc((size + 1) * sizeof(char));
    if (sz) *sz = size + 1;

    if (!*output)
    {
        PP_ERROR("Failed to allocate memory.");
        return;
    }

    fread(*output, sizeof(char), size, file);
    *(*output + size) = '\0';
}
