//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#ifndef GAME_H
#define GAME_H

#include "network.h"
#include "structures.h"

#ifdef _WIN32
#define DLLEXPORT extern "C" __declspec(dllexport)

#ifdef ENABLE_THREADS
#include "pthread.h"
#endif

#else
#define DLLEXPORT

#ifdef ENABLE_THREADS
#include <pthread.h>
#endif

#endif

struct Connection
{
  int socket;
  
  #ifdef ENABLE_THREADS
  pthread_mutex_t mutex;
  #endif
  
  int mapWidth;
  int mapHeight;
  int turnNumber;
  int maxPlants;
  int playerID;
  int gameNumber;
  int bumbleweedSpeed;
  int poolDamage;
  int poolBuff;
  int sporeRate;
  int maxSpores;
  int uprootRange;

  _Player* Players;
  int PlayerCount;
  _Mappable* Mappables;
  int MappableCount;
  _Plant* Plants;
  int PlantCount;
  _Mutation* Mutations;
  int MutationCount;
};

#ifdef __cplusplus
extern "C"
{
#endif
  DLLEXPORT Connection* createConnection();
  DLLEXPORT void destroyConnection(Connection* c);
  DLLEXPORT int serverConnect(Connection* c, const char* host, const char* port);

  DLLEXPORT int serverLogin(Connection* c, const char* username, const char* password);
  DLLEXPORT int createGame(Connection* c);
  DLLEXPORT int joinGame(Connection* c, int id, const char* playerType);

  DLLEXPORT void endTurn(Connection* c);
  DLLEXPORT void getStatus(Connection* c);


//commands

  ///Allows a player to germinate a new Plant.
  DLLEXPORT int playerGerminate(_Player* object, int x, int y, int mutation);
  ///Allows a plant to display messages on the screen
  DLLEXPORT int plantTalk(_Plant* object, char* message);
  ///Command to radiate (heal, attack) another Plant.
  DLLEXPORT int plantRadiate(_Plant* object, int x, int y);
  ///Command to uproot a plant and move to a new location.
  DLLEXPORT int plantUproot(_Plant* object, int x, int y);

//derived properties



//accessors

DLLEXPORT int getMapWidth(Connection* c);
DLLEXPORT int getMapHeight(Connection* c);
DLLEXPORT int getTurnNumber(Connection* c);
DLLEXPORT int getMaxPlants(Connection* c);
DLLEXPORT int getPlayerID(Connection* c);
DLLEXPORT int getGameNumber(Connection* c);
DLLEXPORT int getBumbleweedSpeed(Connection* c);
DLLEXPORT int getPoolDamage(Connection* c);
DLLEXPORT int getPoolBuff(Connection* c);
DLLEXPORT int getSporeRate(Connection* c);
DLLEXPORT int getMaxSpores(Connection* c);
DLLEXPORT int getUprootRange(Connection* c);

DLLEXPORT _Player* getPlayer(Connection* c, int num);
DLLEXPORT int getPlayerCount(Connection* c);

DLLEXPORT _Mappable* getMappable(Connection* c, int num);
DLLEXPORT int getMappableCount(Connection* c);

DLLEXPORT _Plant* getPlant(Connection* c, int num);
DLLEXPORT int getPlantCount(Connection* c);

DLLEXPORT _Mutation* getMutation(Connection* c, int num);
DLLEXPORT int getMutationCount(Connection* c);



  DLLEXPORT int networkLoop(Connection* c);
#ifdef __cplusplus
}
#endif

#endif
