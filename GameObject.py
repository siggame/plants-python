# -*- python -*-

from library import library

from ExistentialError import ExistentialError

class GameObject(object):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration


##
class Player(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.playerGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.players:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  ##Allows a player to germinate a new Plant.
  def germinate(self, x, y, mutation):
    self.validify()
    return library.playerGerminate(self._ptr, x, y, mutation)

  #\cond
  def getId(self):
    self.validify()
    return library.playerGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getPlayerName(self):
    self.validify()
    return library.playerGetPlayerName(self._ptr)
  #\endcond
  ##Player's Name
  playerName = property(getPlayerName)

  #\cond
  def getTime(self):
    self.validify()
    return library.playerGetTime(self._ptr)
  #\endcond
  ##Time remaining, updated at start of turn
  time = property(getTime)

  #\cond
  def getSpores(self):
    self.validify()
    return library.playerGetSpores(self._ptr)
  #\endcond
  ##Number of spores this player has to spawn a new unit
  spores = property(getSpores)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "playerName: %s\n" % self.getPlayerName()
    ret += "time: %s\n" % self.getTime()
    ret += "spores: %s\n" % self.getSpores()
    return ret

##A mappable object!
class Mappable(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.mappableGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.mappables:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  #\cond
  def getId(self):
    self.validify()
    return library.mappableGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getX(self):
    self.validify()
    return library.mappableGetX(self._ptr)
  #\endcond
  ##X position of the object
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.mappableGetY(self._ptr)
  #\endcond
  ##Y position of the object
  y = property(getY)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    return ret

##Represents a single Plant on the map.
class Plant(Mappable):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.plantGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.plants:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  ##Allows a plant to display messages on the screen
  def talk(self, message):
    self.validify()
    return library.plantTalk(self._ptr, message)

  ##Command to radiate (heal, attack) another Plant.
  def radiate(self, x, y):
    self.validify()
    return library.plantRadiate(self._ptr, x, y)

  ##Command to uproot a plant and move to a new location.
  def uproot(self, x, y):
    self.validify()
    return library.plantUproot(self._ptr, x, y)

  #\cond
  def getId(self):
    self.validify()
    return library.plantGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getX(self):
    self.validify()
    return library.plantGetX(self._ptr)
  #\endcond
  ##X position of the object
  x = property(getX)

  #\cond
  def getY(self):
    self.validify()
    return library.plantGetY(self._ptr)
  #\endcond
  ##Y position of the object
  y = property(getY)

  #\cond
  def getOwner(self):
    self.validify()
    return library.plantGetOwner(self._ptr)
  #\endcond
  ##The owner of this Plant.
  owner = property(getOwner)

  #\cond
  def getMutation(self):
    self.validify()
    return library.plantGetMutation(self._ptr)
  #\endcond
  ##The mutation of this Plant. This mutation refers to list of MutationSpecifications.
  mutation = property(getMutation)

  #\cond
  def getRads(self):
    self.validify()
    return library.plantGetRads(self._ptr)
  #\endcond
  ##The current amount of radiation this Plant has.
  rads = property(getRads)

  #\cond
  def getMaxRads(self):
    self.validify()
    return library.plantGetMaxRads(self._ptr)
  #\endcond
  ##The maximum amount of radiation this Plant can have before dying.
  maxRads = property(getMaxRads)

  #\cond
  def getRadiatesLeft(self):
    self.validify()
    return library.plantGetRadiatesLeft(self._ptr)
  #\endcond
  ##The remaining number of times the plant can radiate.
  radiatesLeft = property(getRadiatesLeft)

  #\cond
  def getMaxRadiates(self):
    self.validify()
    return library.plantGetMaxRadiates(self._ptr)
  #\endcond
  ##The maximum number of times the plant can radiate.
  maxRadiates = property(getMaxRadiates)

  #\cond
  def getRange(self):
    self.validify()
    return library.plantGetRange(self._ptr)
  #\endcond
  ##The maximum range this plant can radiate.
  range = property(getRange)

  #\cond
  def getUprootsLeft(self):
    self.validify()
    return library.plantGetUprootsLeft(self._ptr)
  #\endcond
  ##The remaining number of times this plant can be uprooted.
  uprootsLeft = property(getUprootsLeft)

  #\cond
  def getMaxUproots(self):
    self.validify()
    return library.plantGetMaxUproots(self._ptr)
  #\endcond
  ##The maximum number of times this plant can be uprooted.
  maxUproots = property(getMaxUproots)

  #\cond
  def getStrength(self):
    self.validify()
    return library.plantGetStrength(self._ptr)
  #\endcond
  ##The current power of this plant.
  strength = property(getStrength)

  #\cond
  def getMinStrength(self):
    self.validify()
    return library.plantGetMinStrength(self._ptr)
  #\endcond
  ##The minimum power of this plant.
  minStrength = property(getMinStrength)

  #\cond
  def getBaseStrength(self):
    self.validify()
    return library.plantGetBaseStrength(self._ptr)
  #\endcond
  ##The base power of this plant.
  baseStrength = property(getBaseStrength)

  #\cond
  def getMaxStrength(self):
    self.validify()
    return library.plantGetMaxStrength(self._ptr)
  #\endcond
  ##The maximum power of this plant.
  maxStrength = property(getMaxStrength)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "x: %s\n" % self.getX()
    ret += "y: %s\n" % self.getY()
    ret += "owner: %s\n" % self.getOwner()
    ret += "mutation: %s\n" % self.getMutation()
    ret += "rads: %s\n" % self.getRads()
    ret += "maxRads: %s\n" % self.getMaxRads()
    ret += "radiatesLeft: %s\n" % self.getRadiatesLeft()
    ret += "maxRadiates: %s\n" % self.getMaxRadiates()
    ret += "range: %s\n" % self.getRange()
    ret += "uprootsLeft: %s\n" % self.getUprootsLeft()
    ret += "maxUproots: %s\n" % self.getMaxUproots()
    ret += "strength: %s\n" % self.getStrength()
    ret += "minStrength: %s\n" % self.getMinStrength()
    ret += "baseStrength: %s\n" % self.getBaseStrength()
    ret += "maxStrength: %s\n" % self.getMaxStrength()
    return ret

##Represents a mutation of Plant.
class Mutation(GameObject):
  def __init__(self, ptr):
    from BaseAI import BaseAI
    self._ptr = ptr
    self._iteration = BaseAI.iteration
    self._id = library.mutationGetId(ptr)

  #\cond
  def validify(self):
    from BaseAI import BaseAI
    #if this class is pointing to an object from before the current turn it's probably
    #somewhere else in memory now
    if self._iteration == BaseAI.iteration:
      return True
    for i in BaseAI.mutations:
      if i._id == self._id:
        self._ptr = i._ptr
        self._iteration = BaseAI.iteration
        return True
    raise ExistentialError()
  #\endcond
  #\cond
  def getId(self):
    self.validify()
    return library.mutationGetId(self._ptr)
  #\endcond
  ##Unique Identifier
  id = property(getId)

  #\cond
  def getName(self):
    self.validify()
    return library.mutationGetName(self._ptr)
  #\endcond
  ##The name of this mutation of Plant.
  name = property(getName)

  #\cond
  def getType(self):
    self.validify()
    return library.mutationGetType(self._ptr)
  #\endcond
  ##The mutation of this Plant. This value is unique for all Mutations.
  type = property(getType)

  #\cond
  def getSpores(self):
    self.validify()
    return library.mutationGetSpores(self._ptr)
  #\endcond
  ##The current amount of radiation this Plant has.
  spores = property(getSpores)

  #\cond
  def getMaxRadiates(self):
    self.validify()
    return library.mutationGetMaxRadiates(self._ptr)
  #\endcond
  ##The maximum number of times the Plant can radiate.
  maxRadiates = property(getMaxRadiates)

  #\cond
  def getMaxRads(self):
    self.validify()
    return library.mutationGetMaxRads(self._ptr)
  #\endcond
  ##The maximum amount of radiation this Plant can have before dying.
  maxRads = property(getMaxRads)

  #\cond
  def getRange(self):
    self.validify()
    return library.mutationGetRange(self._ptr)
  #\endcond
  ##The maximum range this plant can radiate.
  range = property(getRange)

  #\cond
  def getMaxUproots(self):
    self.validify()
    return library.mutationGetMaxUproots(self._ptr)
  #\endcond
  ##The maximum number of times this plant can be uprooted.
  maxUproots = property(getMaxUproots)

  #\cond
  def getMinStrength(self):
    self.validify()
    return library.mutationGetMinStrength(self._ptr)
  #\endcond
  ##The minimum power of this plant.
  minStrength = property(getMinStrength)

  #\cond
  def getBaseStrength(self):
    self.validify()
    return library.mutationGetBaseStrength(self._ptr)
  #\endcond
  ##The base power of this plant.
  baseStrength = property(getBaseStrength)

  #\cond
  def getMaxStrength(self):
    self.validify()
    return library.mutationGetMaxStrength(self._ptr)
  #\endcond
  ##The maximum power of this plant.
  maxStrength = property(getMaxStrength)


  def __str__(self):
    self.validify()
    ret = ""
    ret += "id: %s\n" % self.getId()
    ret += "name: %s\n" % self.getName()
    ret += "type: %s\n" % self.getType()
    ret += "spores: %s\n" % self.getSpores()
    ret += "maxRadiates: %s\n" % self.getMaxRadiates()
    ret += "maxRads: %s\n" % self.getMaxRads()
    ret += "range: %s\n" % self.getRange()
    ret += "maxUproots: %s\n" % self.getMaxUproots()
    ret += "minStrength: %s\n" % self.getMinStrength()
    ret += "baseStrength: %s\n" % self.getBaseStrength()
    ret += "maxStrength: %s\n" % self.getMaxStrength()
    return ret
