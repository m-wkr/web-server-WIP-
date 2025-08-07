#include <string>
#include <algorithm>

void turnHeaderToLowercase(char (&header)[],u_int8_t size) {

    for (int i = 0; i < size; i++) {
        if (header[i] == '\0') break;

        header[i] = std::tolower(header[i]);
    }
}