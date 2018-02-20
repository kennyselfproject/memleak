#ifndef OVERWRITEMICRO_H
#define OVERWRITEMICRO_H

using namespace std;

// var is heap or nothrow
#define new(var) new((var), (char*)__FILE__, __LINE__)

#endif // OVERWRITEMICRO_H
