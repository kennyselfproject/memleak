#ifndef OVERWRITEMICRO_H
#define OVERWRITEMICRO_H

using namespace std;

// var is heap or nothrow
#define new(...) new(__VA_ARGS__, (char*)__FILE__, __LINE__)

#endif // OVERWRITEMICRO_H
