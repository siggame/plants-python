# -*- python -*-

from library import library

class BaseAI:
  """@brief A basic AI interface.

  This class implements most the code an AI would need to interface with the lower-level game code.
  AIs should extend this class to get a lot of builer-plate code out of the way
  The provided AI class does just that.
  """
  #\cond
  initialized = False
  iteration = 0
  runGenerator = None
  connection = None
  #\endcond
  players = []
  mappables = []
  plants = []
  mutations = []
  #\cond
  def startTurn(self):
    from GameObject import Player
    from GameObject import Mappable
    from GameObject import Plant
    from GameObject import Mutation

    BaseAI.players = [Player(library.getPlayer(self.connection, i)) for i in xrange(library.getPlayerCount(self.connection))]
    BaseAI.mappables = [Mappable(library.getMappable(self.connection, i)) for i in xrange(library.getMappableCount(self.connection))]
    BaseAI.plants = [Plant(library.getPlant(self.connection, i)) for i in xrange(library.getPlantCount(self.connection))]
    BaseAI.mutations = [Mutation(library.getMutation(self.connection, i)) for i in xrange(library.getMutationCount(self.connection))]

    if not self.initialized:
      self.initialized = True
      self.init()
    BaseAI.iteration += 1;
    if self.runGenerator:
      try:
        return self.runGenerator.next()
      except StopIteration:
        self.runGenerator = None
    r = self.run()
    if hasattr(r, '__iter__'):
      self.runGenerator = r
      return r.next()
    return r
  #\endcond
  #\cond
  def getMapWidth(self):
    return library.getMapWidth(self.connection)
  #\endcond
  mapWidth = property(getMapWidth)
  #\cond
  def getMapHeight(self):
    return library.getMapHeight(self.connection)
  #\endcond
  mapHeight = property(getMapHeight)
  #\cond
  def getTurnNumber(self):
    return library.getTurnNumber(self.connection)
  #\endcond
  turnNumber = property(getTurnNumber)
  #\cond
  def getMaxPlants(self):
    return library.getMaxPlants(self.connection)
  #\endcond
  maxPlants = property(getMaxPlants)
  #\cond
  def getPlayerID(self):
    return library.getPlayerID(self.connection)
  #\endcond
  playerID = property(getPlayerID)
  #\cond
  def getGameNumber(self):
    return library.getGameNumber(self.connection)
  #\endcond
  gameNumber = property(getGameNumber)
  #\cond
  def getBumbleweedSpeed(self):
    return library.getBumbleweedSpeed(self.connection)
  #\endcond
  bumbleweedSpeed = property(getBumbleweedSpeed)
  #\cond
  def getPoolDamage(self):
    return library.getPoolDamage(self.connection)
  #\endcond
  poolDamage = property(getPoolDamage)
  #\cond
  def getPoolBuff(self):
    return library.getPoolBuff(self.connection)
  #\endcond
  poolBuff = property(getPoolBuff)
  #\cond
  def getTitanDebuff(self):
    return library.getTitanDebuff(self.connection)
  #\endcond
  titanDebuff = property(getTitanDebuff)
  def __init__(self, connection):
    self.connection = connection
