//Copyright (C) 2009 - Missouri S&T ACM AI Team
//Please do not modify this file while building your AI
//See AI.h & AI.cpp for that
#pragma warning(disable : 4996)

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>

#include "game.h"
#include "network.h"
#include "structures.h"

#include "sexp/sfcompat.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <cmath>

#ifdef _WIN32
//Doh, namespace collision.
namespace Windows
{
    #include <Windows.h>
};
#else
#include <unistd.h>
#endif

#ifdef ENABLE_THREADS
#define LOCK(X) pthread_mutex_lock(X)
#define UNLOCK(X) pthread_mutex_unlock(X)
#else
#define LOCK(X)
#define UNLOCK(X)
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::string;
using std::ofstream;


struct point
{
  int x,y;
  point(int x, int y):x(x),y(y){}
};

//stuff being spawned this turn
static std::vector<point> thisTurnPlants;
static int turnNo;

//distance helper function
int dist(int x1, int y1, int x2, int y2)
{
  return static_cast<int>(floor(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))));
}

DLLEXPORT Connection* createConnection()
{
  Connection* c = new Connection;
  c->socket = -1;
  #ifdef ENABLE_THREADS
  pthread_mutex_init(&c->mutex, NULL);
  #endif

  c->mapWidth = 0;
  c->mapHeight = 0;
  c->turnNumber = 0;
  c->maxPlants = 0;
  c->playerID = 0;
  c->gameNumber = 0;
  c->bumbleweedSpeed = 0;
  c->poolDamage = 0;
  c->poolBuff = 0;
  c->uprootRange = 0;
  c->Players = NULL;
  c->PlayerCount = 0;
  c->Mappables = NULL;
  c->MappableCount = 0;
  c->sporeRate = 0;
  c->maxSpores = 0;
  c->Plants = NULL;
  c->PlantCount = 0;
  c->Mutations = NULL;
  c->MutationCount = 0;
  return c;
}

DLLEXPORT void destroyConnection(Connection* c)
{
  #ifdef ENABLE_THREADS
  pthread_mutex_destroy(&c->mutex);
  #endif
  if(c->Players)
  {
    for(int i = 0; i < c->PlayerCount; i++)
    {
      delete[] c->Players[i].playerName;
    }
    delete[] c->Players;
  }
  if(c->Mappables)
  {
    for(int i = 0; i < c->MappableCount; i++)
    {
    }
    delete[] c->Mappables;
  }
  if(c->Plants)
  {
    for(int i = 0; i < c->PlantCount; i++)
    {
    }
    delete[] c->Plants;
  }
  if(c->Mutations)
  {
    for(int i = 0; i < c->MutationCount; i++)
    {
      delete[] c->Mutations[i].name;
    }
    delete[] c->Mutations;
  }
  delete c;
}

DLLEXPORT int serverConnect(Connection* c, const char* host, const char* port)
{
  c->socket = open_server_connection(host, port);
  return c->socket + 1; //false if socket == -1
}

DLLEXPORT int serverLogin(Connection* c, const char* username, const char* password)
{
  string expr = "(login \"";
  expr += username;
  expr += "\" \"";
  expr += password;
  expr +="\")";

  send_string(c->socket, expr.c_str());

  sexp_t* expression, *message;

  char* reply = rec_string(c->socket);
  expression = extract_sexpr(reply);
  delete[] reply;

  message = expression->list;
  if(message->val == NULL || strcmp(message->val, "login-accepted") != 0)
  {
    cerr << "Unable to login to server" << endl;
    destroy_sexp(expression);
    return 0;
  }
  destroy_sexp(expression);
  return 1;
}

DLLEXPORT int createGame(Connection* c)
{
  sexp_t* expression, *number;

  send_string(c->socket, "(create-game)");

  char* reply = rec_string(c->socket);
  expression = extract_sexpr(reply);
  delete[] reply;

  number = expression->list->next;
  c->gameNumber = atoi(number->val);
  destroy_sexp(expression);

  std::cout << "Creating game " << c->gameNumber << endl;

  c->playerID = 0;

  return c->gameNumber;
}

DLLEXPORT int joinGame(Connection* c, int gameNum, const char* playerType)
{
  sexp_t* expression;
  stringstream expr;

  c->gameNumber = gameNum;

  expr << "(join-game " << c->gameNumber << " "<< playerType << ")";
  send_string(c->socket, expr.str().c_str());

  char* reply = rec_string(c->socket);
  expression = extract_sexpr(reply);
  delete[] reply;

  if(strcmp(expression->list->val, "join-accepted") == 0)
  {
    destroy_sexp(expression);
    c->playerID = 1;
    send_string(c->socket, "(game-start)");
    return 1;
  }
  else if(strcmp(expression->list->val, "create-game") == 0)
  {
    std::cout << "Game did not exist, creating game " << c->gameNumber << endl;
    destroy_sexp(expression);
    c->playerID = 0;
    return 1;
  }
  else
  {
    cerr << "Cannot join game "<< c->gameNumber << ": " << expression->list->next->val << endl;
    destroy_sexp(expression);
    return 0;
  }
}

DLLEXPORT void endTurn(Connection* c)
{
  LOCK( &c->mutex );
  send_string(c->socket, "(end-turn)");
  UNLOCK( &c->mutex );
}

DLLEXPORT void getStatus(Connection* c)
{
  LOCK( &c->mutex );
  send_string(c->socket, "(game-status)");
  UNLOCK( &c->mutex );
}


DLLEXPORT int playerGerminate(_Player* object, int x, int y, int mutation)
{
  const int spawnerNo = 1;
  const int motherNo = 0;

  stringstream expr;
  expr << "(game-germinate " << object->id
       << " " << x
       << " " << y
       << " " << mutation
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);

  Connection* c = object->_c;

  if(turnNo != c->turnNumber)
  {
    thisTurnPlants.clear();
    turnNo = c->turnNumber;
  }

  //Check for invalid mutation ID
  if (mutation <= 0 || mutation >= 7)
    return 0;

  //Get Mutation object
  _Mutation* mut_obj = getMutation(c,mutation);

  if (mut_obj == NULL)
    return 0;
  else if (mut_obj->spores == 0)
    return 0;
  else if (object->spores < mut_obj->spores)
    return 0;
  else if (x < 0 || x >= getMapWidth(c) || y < 0 || y >= getMapHeight(c))
    return 0;

  //Check Plants Owned
  int plantsOwned = 0;
  for (int i = 0; i < getPlantCount(c); i++)
  {
    plantsOwned += (getPlant(c,i)->owner == getPlayerID(c));
  }
  if (plantsOwned+thisTurnPlants.size() >= getMaxPlants(c))
    return 0;
  
  // Check to make sure there is not a plant on that tile already.
  // Make sure that plant is still alive
  for (int i = 0; i < getPlantCount(c); i++)
  {
    _Plant* checking = getPlant(c,i);
    if (checking->x == x && checking->y == y && checking->rads < checking->maxRads)
      return 0;
  }

  //Check range
  bool inRange = false;
  for (int i = 0; i < getPlantCount(c); i++)
  {
    _Plant* checking = getPlant(c,i);
    // Check to make sure the target is in the range of the spawner
    if ((checking->mutation == spawnerNo || checking->mutation == motherNo) &&
        checking->owner == object->id)
    {
      if (dist(x, y , checking->x, checking->y) <= checking->range)
      {
        inRange = true;
        break;       
      }
    }
  }

  if (!inRange)
    return 0;

  for(int i = 0; i < thisTurnPlants.size(); i++)
  {
    if(thisTurnPlants[i].x == x && thisTurnPlants[i].y == y)
    {
      return 0;
    }
  }

  thisTurnPlants.push_back(point(x, y));

  //TODO: Do some spawning-on-turn stuff with a list

  object->spores -= mut_obj->spores;

  return 1;
}



DLLEXPORT int plantTalk(_Plant* object, char* message)
{
  if(object -> hasSpoken == 0)
  {
    if(strlen(message) > 140)
    {
      message[140] = '\0';               //sneakily truncating message
      object -> hasSpoken = 1;
    }
  }
      
  stringstream expr;
  expr << "(game-talk " << object->id
      << " \"" << escape_string(message) << "\""
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);
  return 1;
}

DLLEXPORT int plantRadiate(_Plant* object, int x, int y)
{
  stringstream expr;
  expr << "(game-radiate " << object->id
       << " " << x
       << " " << y
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);

  Connection* c = object->_c;

  //Check ownership
  if (object->owner != getPlayerID(c))
    return 0;

  //Check radiates left
  if (object->radiatesLeft <= 0)
    return 0;

  //Check radiation
  if (object->rads > object->maxRads)
    return 0;

  //Check bounds
  if (x < 0 || x >= getMapWidth(c) || y < 0 || y >= getMapHeight(c))
    return 0;

  // Check range
  if (dist(x, y, object->x, object->y) > object->range)
    return 0;

  //Target plant
  _Plant* target = NULL;
  for (int i = 0; i < getPlantCount(c); ++i)
  {
    _Plant* candidate = getPlant(c,i);
    if (candidate->x == x && candidate->y == y && candidate->mutation != 7 && candidate->rads < candidate->maxRads) 
    {
      if (candidate->mutation != 7) //if pool mutation
      {
        target = candidate;
        break;
      }
    }
  }

  //If no target, return
  if (target == NULL)
    return 0;


  if (object->mutation == 2 || object->mutation == 5)
  {
    if (target->owner != (1 - getPlayerID(c)))
      return 0;

    int damage = object->strength + int(object->strength * ((float)object->rads / (float)object->maxRads));
    target->rads += damage;
    object->uprootsLeft = 0;
  }
  else if (object->mutation == 3 || object->mutation == 4)
  {
    if (target->owner != getPlayerID(c))
      return 0;
    else if (target->mutation == 0)
      return 0;
    else if (object->mutation == 3 && target->mutation == 3)
      return 0;


    if (object->mutation == 4)  //heal if tumbleweed
      target->rads = std::max(target->rads - object->strength, 0);
    else if (object->mutation == 3) { //buff if soaker
      int buff = static_cast<int>(1 + object->strength/4.0);
      target->strength = std::min(target->strength + buff, target->maxStrength);
    }
  }

  object->radiatesLeft--;

  return 1;
}

DLLEXPORT int plantUproot(_Plant* object, int x, int y)
{
  const int spawnerNo = 1;
  const int tumbleNo = 4;
  const int motherNo = 0;

  //Clear thisTurnPlants if it is from the last turn.
  if(turnNo != object->_c->turnNumber)
  {
    thisTurnPlants.clear();
    turnNo = object->_c->turnNumber;
  }

  stringstream expr;
  expr << "(game-uproot " << object->id
       << " " << x
       << " " << y
       << ")";
  LOCK( &object->_c->mutex);
  send_string(object->_c->socket, expr.str().c_str());
  UNLOCK( &object->_c->mutex);

  Connection* c = object->_c;

  if (object->owner != getPlayerID(c))
    return 0;
  else if (object->uprootsLeft <= 0)
    return 0;
  else if (!(x >= 0 && x < getMapWidth(c)) || !(y >= 0 || y < getMapHeight(c)))
    return 0;

  //Make sure we're not spawning on top of another plant
  _Plant* a_plant;
  for (int i = 0; i < getPlantCount(c); i++)
  {
    a_plant = getPlant(c,i);
    if (a_plant->x == x && a_plant->y == y && a_plant->rads < a_plant->maxRads)
      return 0;
  }

  //Make sure we aren't moving on to a plant about to spawn:
  for(int i = 0; i < thisTurnPlants.size(); i++)
  {
    if(thisTurnPlants[i].x == x && thisTurnPlants[i].y == y)
    {
      return 0;
    }
  }

  //Find a spawner to move with
  bool inRange;
  if (object->mutation != tumbleNo)
  {
    if (dist(object->x, object->y, x, y) > getUprootRange(c))
      return 0;
  }
  else if (dist(object->x, object->y, x, y) > getBumbleweedSpeed(c))
    return 0;

  //update position
  object->x = x;
  object->y = y;
  object->uprootsLeft--;

  return 1;

}



//Utility functions for parsing data
void parsePlayer(Connection* c, _Player* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->playerName = new char[strlen(sub->val)+1];
  strncpy(object->playerName, sub->val, strlen(sub->val));
  object->playerName[strlen(sub->val)] = 0;
  sub = sub->next;
  object->time = atof(sub->val);
  sub = sub->next;
  object->spores = atoi(sub->val);
  sub = sub->next;

}
void parseMappable(Connection* c, _Mappable* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->x = atoi(sub->val);
  sub = sub->next;
  object->y = atoi(sub->val);
  sub = sub->next;

}
void parsePlant(Connection* c, _Plant* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->x = atoi(sub->val);
  sub = sub->next;
  object->y = atoi(sub->val);
  sub = sub->next;
  object->owner = atoi(sub->val);
  sub = sub->next;
  object->mutation = atoi(sub->val);
  sub = sub->next;
  object->rads = atoi(sub->val);
  sub = sub->next;
  object->maxRads = atoi(sub->val);
  sub = sub->next;
  object->radiatesLeft = atoi(sub->val);
  sub = sub->next;
  object->maxRadiates = atoi(sub->val);
  sub = sub->next;
  object->range = atoi(sub->val);
  sub = sub->next;
  object->uprootsLeft = atoi(sub->val);
  sub = sub->next;
  object->maxUproots = atoi(sub->val);
  sub = sub->next;
  object->strength = atoi(sub->val);
  sub = sub->next;
  object->minStrength = atoi(sub->val);
  sub = sub->next;
  object->baseStrength = atoi(sub->val);
  sub = sub->next;
  object->maxStrength = atoi(sub->val);
  sub = sub->next;
  object -> hasSpoken = false;

}
void parseMutation(Connection* c, _Mutation* object, sexp_t* expression)
{
  sexp_t* sub;
  sub = expression->list;

  object->_c = c;

  object->id = atoi(sub->val);
  sub = sub->next;
  object->name = new char[strlen(sub->val)+1];
  strncpy(object->name, sub->val, strlen(sub->val));
  object->name[strlen(sub->val)] = 0;
  sub = sub->next;
  object->type = atoi(sub->val);
  sub = sub->next;
  object->spores = atoi(sub->val);
  sub = sub->next;
  object->maxRadiates = atoi(sub->val);
  sub = sub->next;
  object->maxRads = atoi(sub->val);
  sub = sub->next;
  object->range = atoi(sub->val);
  sub = sub->next;
  object->maxUproots = atoi(sub->val);
  sub = sub->next;
  object->minStrength = atoi(sub->val);
  sub = sub->next;
  object->baseStrength = atoi(sub->val);
  sub = sub->next;
  object->maxStrength = atoi(sub->val);
  sub = sub->next;

}

DLLEXPORT int networkLoop(Connection* c)
{
  while(true)
  {
    sexp_t* base, *expression, *sub, *subsub;

    char* message = rec_string(c->socket);
    string text = message;
    base = extract_sexpr(message);
    delete[] message;
    expression = base->list;
    if(expression->val != NULL && strcmp(expression->val, "game-winner") == 0)
    {
      expression = expression->next->next->next;
      int winnerID = atoi(expression->val);
      if(winnerID == c->playerID)
      {
        cout << "You win!" << endl << expression->next->val << endl;
      }
      else
      {
        cout << "You lose. :(" << endl << expression->next->val << endl;
      }
      stringstream expr;
      expr << "(request-log " << c->gameNumber << ")";
      send_string(c->socket, expr.str().c_str());
      destroy_sexp(base);
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "log") == 0)
    {
      ofstream out;
      stringstream filename;
      expression = expression->next;
      filename << expression->val;
      filename << ".gamelog";
      expression = expression->next;
      out.open(filename.str().c_str());
      if (out.good())
        out.write(expression->val, strlen(expression->val));
      else
        cerr << "Error : Could not create log." << endl;
      out.close();
      destroy_sexp(base);
      return 0;
    }
    else if(expression->val != NULL && strcmp(expression->val, "game-accepted")==0)
    {
      char gameID[30];

      expression = expression->next;
      strcpy(gameID, expression->val);
      cout << "Created game " << gameID << endl;
    }
    else if(expression->val != NULL && strstr(expression->val, "denied"))
    {
      cout << expression->val << endl;
      cout << expression->next->val << endl;
    }
    else if(expression->val != NULL && strcmp(expression->val, "status") == 0)
    {
      while(expression->next != NULL)
      {
        expression = expression->next;
        sub = expression->list;
        if(string(sub->val) == "game")
        {
          sub = sub->next;
          c->mapWidth = atoi(sub->val);
          sub = sub->next;

          c->mapHeight = atoi(sub->val);
          sub = sub->next;

          c->turnNumber = atoi(sub->val);
          sub = sub->next;

          c->maxPlants = atoi(sub->val);
          sub = sub->next;

          c->playerID = atoi(sub->val);
          sub = sub->next;

          c->gameNumber = atoi(sub->val);
          sub = sub->next;

          c->bumbleweedSpeed = atoi(sub->val);
          sub = sub->next;

          c->poolDamage = atoi(sub->val);
          sub = sub->next;

          c->poolBuff = atoi(sub->val);
          sub = sub->next;

          c->sporeRate = atoi(sub->val);
          sub = sub->next;

          c->maxSpores = atoi(sub->val);
          sub = sub->next;

          c->uprootRange = atoi(sub->val);
          sub = sub->next;

        }
        else if(string(sub->val) == "Player")
        {
          if(c->Players)
          {
            for(int i = 0; i < c->PlayerCount; i++)
            {
              delete[] c->Players[i].playerName;
            }
            delete[] c->Players;
          }
          c->PlayerCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Players = new _Player[c->PlayerCount];
          for(int i = 0; i < c->PlayerCount; i++)
          {
            sub = sub->next;
            parsePlayer(c, c->Players+i, sub);
          }
        }
        else if(string(sub->val) == "Mappable")
        {
          if(c->Mappables)
          {
            for(int i = 0; i < c->MappableCount; i++)
            {
            }
            delete[] c->Mappables;
          }
          c->MappableCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Mappables = new _Mappable[c->MappableCount];
          for(int i = 0; i < c->MappableCount; i++)
          {
            sub = sub->next;
            parseMappable(c, c->Mappables+i, sub);
          }
        }
        else if(string(sub->val) == "Plant")
        {
          if(c->Plants)
          {
            for(int i = 0; i < c->PlantCount; i++)
            {
            }
            delete[] c->Plants;
          }
          c->PlantCount =  sexp_list_length(expression)-1; //-1 for the header
          c->Plants = new _Plant[c->PlantCount];
          for(int i = 0; i < c->PlantCount; i++)
          {
            sub = sub->next;
            parsePlant(c, c->Plants+i, sub);
          }
        }
        else if(string(sub->val) == "Mutation")
        {
          if(c->Mutations)
          {
            sub = sub->next;
            for(int i = 0; i < c->MutationCount; i++)
            {
              if(!sub)
              {
                break;
              }
              int id = atoi(sub->list->val);
              if(id == c->Mutations[i].id)
              {
                delete[] c->Mutations[i].name;
                parseMutation(c, c->Mutations+i, sub);
                sub = sub->next;
              }
            }
          }
          else
          {
            c->MutationCount =  sexp_list_length(expression)-1; //-1 for the header
            c->Mutations = new _Mutation[c->MutationCount];
            for(int i = 0; i < c->MutationCount; i++)
            {
              sub = sub->next;
              parseMutation(c, c->Mutations+i, sub);
            }
          }
        }
      }
      destroy_sexp(base);
      return 1;
    }
    else
    {
#ifdef SHOW_WARNINGS
      cerr << "Unrecognized message: " << text << endl;
#endif
    }
    destroy_sexp(base);
  }
}

DLLEXPORT _Player* getPlayer(Connection* c, int num)
{
  return c->Players + num;
}
DLLEXPORT int getPlayerCount(Connection* c)
{
  return c->PlayerCount;
}

DLLEXPORT _Mappable* getMappable(Connection* c, int num)
{
  return c->Mappables + num;
}
DLLEXPORT int getMappableCount(Connection* c)
{
  return c->MappableCount;
}

DLLEXPORT _Plant* getPlant(Connection* c, int num)
{
  return c->Plants + num;
}
DLLEXPORT int getPlantCount(Connection* c)
{
  return c->PlantCount;
}

DLLEXPORT _Mutation* getMutation(Connection* c, int num)
{
  return c->Mutations + num;
}
DLLEXPORT int getMutationCount(Connection* c)
{
  return c->MutationCount;
}


DLLEXPORT int getMapWidth(Connection* c)
{
  return c->mapWidth;
}
DLLEXPORT int getMapHeight(Connection* c)
{
  return c->mapHeight;
}
DLLEXPORT int getTurnNumber(Connection* c)
{
  return c->turnNumber;
}
DLLEXPORT int getMaxPlants(Connection* c)
{
  return c->maxPlants;
}
DLLEXPORT int getPlayerID(Connection* c)
{
  return c->playerID;
}
DLLEXPORT int getGameNumber(Connection* c)
{
  return c->gameNumber;
}
DLLEXPORT int getBumbleweedSpeed(Connection* c)
{
  return c->bumbleweedSpeed;
}
DLLEXPORT int getPoolDamage(Connection* c)
{
  return c->poolDamage;
}
DLLEXPORT int getPoolBuff(Connection* c)
{
  return c->poolBuff;
}
DLLEXPORT int getSporeRate(Connection* c)
{
  return c->sporeRate;
}
DLLEXPORT int getMaxSpores(Connection* c)
{
  return c->maxSpores;
}
DLLEXPORT int getUprootRange(Connection* c)
{
  return c->uprootRange;
}
