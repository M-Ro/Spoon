#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string &TrimLeft(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &TrimRight(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &Trim(std::string &s) {
    return TrimLeft(TrimRight(s));
}

// laziness for char arrays

static inline char* cltrim(char* s)
{
    while (isspace(*s)) s++;
    return s;
}

static inline char* crtrim(char* s)
{
    char* back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}

static inline char* ctrim(char* s)
{
    return crtrim(cltrim(s));
}

/**
 * Radiant exports angles in the range of -360 to 360,
 * often slightly offset (e.g 89.99995 or -179.99995)
 * Returns a correct int between 0-360
 */
static inline int fixed_angle(float angle)
{
    if (angle < 0) {
        angle = 360 - angle;
    }

    return (int)round(angle);
}