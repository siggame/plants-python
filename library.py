# -*-python-*-

import os

from ctypes import *

try:
  if os.name == 'posix':
    library = CDLL("./libclient.so")
  elif os.name == 'nt':
    library = CDLL("./client.dll")
  else:
    raise Exception("Unrecognized OS: "+os.name)
except OSError:
  raise Exception("It looks like you didn't build libclient. Run 'make' and try again.")

# commands

library.createConnection.restype = c_void_p
library.createConnection.argtypes = []

library.serverConnect.restype = c_int
library.serverConnect.argtypes = [c_void_p, c_char_p, c_char_p]

library.serverLogin.restype = c_int
library.serverLogin.argtypes = [c_void_p, c_char_p, c_char_p]

library.createGame.restype = c_int
library.createGame.argtypes = [c_void_p]

library.joinGame.restype = c_int
library.joinGame.argtypes = [c_void_p, c_int, c_char_p]

library.endTurn.restype = None
library.endTurn.argtypes = [c_void_p]

library.getStatus.restype = None
library.getStatus.argtypes = [c_void_p]

library.networkLoop.restype = c_int
library.networkLoop.argtypes = [c_void_p]

#Functions
library.playerGerminate.restype = c_int
library.playerGerminate.argtypes = [c_void_p, c_int, c_int, c_int]

library.plantTalk.restype = c_int
library.plantTalk.argtypes = [c_void_p, c_char_p]

library.plantRadiate.restype = c_int
library.plantRadiate.argtypes = [c_void_p, c_int, c_int]

library.plantUproot.restype = c_int
library.plantUproot.argtypes = [c_void_p, c_int, c_int]

# accessors

#Globals
library.getMapWidth.restype = c_int
library.getMapWidth.argtypes = [c_void_p]

library.getMapHeight.restype = c_int
library.getMapHeight.argtypes = [c_void_p]

library.getTurnNumber.restype = c_int
library.getTurnNumber.argtypes = [c_void_p]

library.getMaxPlants.restype = c_int
library.getMaxPlants.argtypes = [c_void_p]

library.getPlayerID.restype = c_int
library.getPlayerID.argtypes = [c_void_p]

library.getGameNumber.restype = c_int
library.getGameNumber.argtypes = [c_void_p]

library.getBumbleweedSpeed.restype = c_int
library.getBumbleweedSpeed.argtypes = [c_void_p]

library.getPoolDamage.restype = c_int
library.getPoolDamage.argtypes = [c_void_p]

library.getPoolBuff.restype = c_int
library.getPoolBuff.argtypes = [c_void_p]

library.getTitanDebuff.restype = c_int
library.getTitanDebuff.argtypes = [c_void_p]

library.getPlayer.restype = c_void_p
library.getPlayer.argtypes = [c_void_p, c_int]

library.getPlayerCount.restype = c_int
library.getPlayerCount.argtypes = [c_void_p]

library.getMappable.restype = c_void_p
library.getMappable.argtypes = [c_void_p, c_int]

library.getMappableCount.restype = c_int
library.getMappableCount.argtypes = [c_void_p]

library.getPlant.restype = c_void_p
library.getPlant.argtypes = [c_void_p, c_int]

library.getPlantCount.restype = c_int
library.getPlantCount.argtypes = [c_void_p]

library.getMutation.restype = c_void_p
library.getMutation.argtypes = [c_void_p, c_int]

library.getMutationCount.restype = c_int
library.getMutationCount.argtypes = [c_void_p]

# getters

#Data
library.playerGetId.restype = c_int
library.playerGetId.argtypes = [c_void_p]

library.playerGetPlayerName.restype = c_char_p
library.playerGetPlayerName.argtypes = [c_void_p]

library.playerGetTime.restype = c_float
library.playerGetTime.argtypes = [c_void_p]

library.playerGetSpores.restype = c_int
library.playerGetSpores.argtypes = [c_void_p]

library.mappableGetId.restype = c_int
library.mappableGetId.argtypes = [c_void_p]

library.mappableGetX.restype = c_int
library.mappableGetX.argtypes = [c_void_p]

library.mappableGetY.restype = c_int
library.mappableGetY.argtypes = [c_void_p]

library.plantGetId.restype = c_int
library.plantGetId.argtypes = [c_void_p]

library.plantGetX.restype = c_int
library.plantGetX.argtypes = [c_void_p]

library.plantGetY.restype = c_int
library.plantGetY.argtypes = [c_void_p]

library.plantGetOwner.restype = c_int
library.plantGetOwner.argtypes = [c_void_p]

library.plantGetMutation.restype = c_int
library.plantGetMutation.argtypes = [c_void_p]

library.plantGetRads.restype = c_int
library.plantGetRads.argtypes = [c_void_p]

library.plantGetMaxRads.restype = c_int
library.plantGetMaxRads.argtypes = [c_void_p]

library.plantGetRadiatesLeft.restype = c_int
library.plantGetRadiatesLeft.argtypes = [c_void_p]

library.plantGetMaxRadiates.restype = c_int
library.plantGetMaxRadiates.argtypes = [c_void_p]

library.plantGetRange.restype = c_int
library.plantGetRange.argtypes = [c_void_p]

library.plantGetUprootsLeft.restype = c_int
library.plantGetUprootsLeft.argtypes = [c_void_p]

library.plantGetMaxUproots.restype = c_int
library.plantGetMaxUproots.argtypes = [c_void_p]

library.plantGetStrength.restype = c_int
library.plantGetStrength.argtypes = [c_void_p]

library.plantGetMinStrength.restype = c_int
library.plantGetMinStrength.argtypes = [c_void_p]

library.plantGetBaseStrength.restype = c_int
library.plantGetBaseStrength.argtypes = [c_void_p]

library.plantGetMaxStrength.restype = c_int
library.plantGetMaxStrength.argtypes = [c_void_p]

library.mutationGetId.restype = c_int
library.mutationGetId.argtypes = [c_void_p]

library.mutationGetName.restype = c_char_p
library.mutationGetName.argtypes = [c_void_p]

library.mutationGetType.restype = c_int
library.mutationGetType.argtypes = [c_void_p]

library.mutationGetSpores.restype = c_int
library.mutationGetSpores.argtypes = [c_void_p]

library.mutationGetMaxRadiates.restype = c_int
library.mutationGetMaxRadiates.argtypes = [c_void_p]

library.mutationGetMaxRads.restype = c_int
library.mutationGetMaxRads.argtypes = [c_void_p]

library.mutationGetRange.restype = c_int
library.mutationGetRange.argtypes = [c_void_p]

library.mutationGetMaxUproots.restype = c_int
library.mutationGetMaxUproots.argtypes = [c_void_p]

library.mutationGetMinStrength.restype = c_int
library.mutationGetMinStrength.argtypes = [c_void_p]

library.mutationGetBaseStrength.restype = c_int
library.mutationGetBaseStrength.argtypes = [c_void_p]

library.mutationGetMaxStrength.restype = c_int
library.mutationGetMaxStrength.argtypes = [c_void_p]


#Properties
