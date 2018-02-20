#ifndef OVERWRITEMICRO_H
#define OVERWRITEMICRO_H

using namespace std;

#define new(heap) new((heap), (char*)__FILE__, __LINE__)

#endif // OVERWRITEMICRO_H
