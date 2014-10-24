#include "getters.h"

DLLEXPORT int playerGetId(_Player* ptr)
{
  return ptr->id;
}
DLLEXPORT char* playerGetPlayerName(_Player* ptr)
{
  return ptr->playerName;
}
DLLEXPORT float playerGetTime(_Player* ptr)
{
  return ptr->time;
}
DLLEXPORT int playerGetSpores(_Player* ptr)
{
  return ptr->spores;
}
DLLEXPORT int mappableGetId(_Mappable* ptr)
{
  return ptr->id;
}
DLLEXPORT int mappableGetX(_Mappable* ptr)
{
  return ptr->x;
}
DLLEXPORT int mappableGetY(_Mappable* ptr)
{
  return ptr->y;
}
DLLEXPORT int plantGetId(_Plant* ptr)
{
  return ptr->id;
}
DLLEXPORT int plantGetX(_Plant* ptr)
{
  return ptr->x;
}
DLLEXPORT int plantGetY(_Plant* ptr)
{
  return ptr->y;
}
DLLEXPORT int plantGetOwner(_Plant* ptr)
{
  return ptr->owner;
}
DLLEXPORT int plantGetMutation(_Plant* ptr)
{
  return ptr->mutation;
}
DLLEXPORT int plantGetRads(_Plant* ptr)
{
  return ptr->rads;
}
DLLEXPORT int plantGetMaxRads(_Plant* ptr)
{
  return ptr->maxRads;
}
DLLEXPORT int plantGetRadiatesLeft(_Plant* ptr)
{
  return ptr->radiatesLeft;
}
DLLEXPORT int plantGetMaxRadiates(_Plant* ptr)
{
  return ptr->maxRadiates;
}
DLLEXPORT int plantGetRange(_Plant* ptr)
{
  return ptr->range;
}
DLLEXPORT int plantGetUprootsLeft(_Plant* ptr)
{
  return ptr->uprootsLeft;
}
DLLEXPORT int plantGetMaxUproots(_Plant* ptr)
{
  return ptr->maxUproots;
}
DLLEXPORT int plantGetStrength(_Plant* ptr)
{
  return ptr->strength;
}
DLLEXPORT int plantGetMinStrength(_Plant* ptr)
{
  return ptr->minStrength;
}
DLLEXPORT int plantGetBaseStrength(_Plant* ptr)
{
  return ptr->baseStrength;
}
DLLEXPORT int plantGetMaxStrength(_Plant* ptr)
{
  return ptr->maxStrength;
}
DLLEXPORT int mutationGetId(_Mutation* ptr)
{
  return ptr->id;
}
DLLEXPORT char* mutationGetName(_Mutation* ptr)
{
  return ptr->name;
}
DLLEXPORT int mutationGetType(_Mutation* ptr)
{
  return ptr->type;
}
DLLEXPORT int mutationGetSpores(_Mutation* ptr)
{
  return ptr->spores;
}
DLLEXPORT int mutationGetMaxRadiates(_Mutation* ptr)
{
  return ptr->maxRadiates;
}
DLLEXPORT int mutationGetMaxRads(_Mutation* ptr)
{
  return ptr->maxRads;
}
DLLEXPORT int mutationGetRange(_Mutation* ptr)
{
  return ptr->range;
}
DLLEXPORT int mutationGetMaxUproots(_Mutation* ptr)
{
  return ptr->maxUproots;
}
DLLEXPORT int mutationGetMinStrength(_Mutation* ptr)
{
  return ptr->minStrength;
}
DLLEXPORT int mutationGetBaseStrength(_Mutation* ptr)
{
  return ptr->baseStrength;
}
DLLEXPORT int mutationGetMaxStrength(_Mutation* ptr)
{
  return ptr->maxStrength;
}

