#ifndef GETTERS_H 
#define GETTERS_H
#include "structures.h"

#ifdef _WIN32
#define DLLEXPORT extern "C" __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT int playerGetId(_Player* ptr);
DLLEXPORT char* playerGetPlayerName(_Player* ptr);
DLLEXPORT float playerGetTime(_Player* ptr);
DLLEXPORT int playerGetSpores(_Player* ptr);


DLLEXPORT int mappableGetId(_Mappable* ptr);
DLLEXPORT int mappableGetX(_Mappable* ptr);
DLLEXPORT int mappableGetY(_Mappable* ptr);


DLLEXPORT int plantGetId(_Plant* ptr);
DLLEXPORT int plantGetX(_Plant* ptr);
DLLEXPORT int plantGetY(_Plant* ptr);
DLLEXPORT int plantGetOwner(_Plant* ptr);
DLLEXPORT int plantGetMutation(_Plant* ptr);
DLLEXPORT int plantGetRads(_Plant* ptr);
DLLEXPORT int plantGetMaxRads(_Plant* ptr);
DLLEXPORT int plantGetRadiatesLeft(_Plant* ptr);
DLLEXPORT int plantGetMaxRadiates(_Plant* ptr);
DLLEXPORT int plantGetRange(_Plant* ptr);
DLLEXPORT int plantGetUprootsLeft(_Plant* ptr);
DLLEXPORT int plantGetMaxUproots(_Plant* ptr);
DLLEXPORT int plantGetStrength(_Plant* ptr);
DLLEXPORT int plantGetMinStrength(_Plant* ptr);
DLLEXPORT int plantGetBaseStrength(_Plant* ptr);
DLLEXPORT int plantGetMaxStrength(_Plant* ptr);


DLLEXPORT int mutationGetId(_Mutation* ptr);
DLLEXPORT char* mutationGetName(_Mutation* ptr);
DLLEXPORT int mutationGetType(_Mutation* ptr);
DLLEXPORT int mutationGetSpores(_Mutation* ptr);
DLLEXPORT int mutationGetMaxRadiates(_Mutation* ptr);
DLLEXPORT int mutationGetMaxRads(_Mutation* ptr);
DLLEXPORT int mutationGetRange(_Mutation* ptr);
DLLEXPORT int mutationGetMaxUproots(_Mutation* ptr);
DLLEXPORT int mutationGetMinStrength(_Mutation* ptr);
DLLEXPORT int mutationGetBaseStrength(_Mutation* ptr);
DLLEXPORT int mutationGetMaxStrength(_Mutation* ptr);



#ifdef __cplusplus
}
#endif

#endif
