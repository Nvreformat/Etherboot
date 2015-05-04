#ifndef STRING_H
#define STRING_H

typedef const char* String;

size_t StringGetLength(const char* String);
int StringEqual(const char* String1, const char* String2);
const char* StringConcat(const char* String1, const char* String2);
void StringCopy(const char* SourceString, void* Destination);

#endif
