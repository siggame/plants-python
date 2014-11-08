#-*-python-*-
from BaseAI import BaseAI
from GameObject import *
import math
import random

class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "Shell AI"

  @staticmethod
  def password():
    return "password"
  
  MOTHER = 0
  SPAWNER = 1
  CHOKER = 2
  SOAKER = 3
  BUMBLEWEED = 4
  ARALIA = 5
  TITAN = 6
  POOL = 7

  ##This function is called once, before your first turn
  def init(self):
    #set up me field
    self.me = self.players[self.playerID]
    #set up mother field
    self.mother = self.getMyPlants()[0]
    
    #set up directionOfEnemy field
    #if our mother is on the left side of the map, the enemy must be on the right side
    #and vice versa of course
    if self.mother.x < self.mapWidth/2:
      self.directionOfEnemy = 1
    else:
      self.directionOfEnemy = -1
    pass

  ##This function is called once, after your last turn
  def end(self):
    pass

  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):
    myPlants = self.getMyPlants()
    #for every plant we own, move them forward and attack if it finds an enemy
    for plant in myPlants:
      #only try radiating if it's possible
      if plant.radiatesLeft > 0:
        #only heal or buff allies and attack enemies
        targetOwner = 1 - self.playerID
        if plant.mutation == self.BUMBLEWEED or plant.mutation == self.SOAKER:
          targetOwner = self.playerID

        for foe in self.plants:
          #if it's dead skip it
          if foe.rads >= foe.maxRads:
            continue

          #don't mess with pools
          if foe.mutation == self.POOL:
            continue

          #if it's not the right target
          if foe.owner != targetOwner:
            continue

          #if a healer or soaker can't effect the mother weed
          if targetOwner == self.playerID and foe.mutation == self.MOTHER:
            continue

          #if a soaker can't effect other soakers
          if plant.mutation == self.SOAKER and foe.mutation == self.SOAKER:
            continue

          #if we're within range...
          if self.distance(plant.x, plant.y, foe.x, foe.y) < plant.range:
            #get 'im!
            plant.radiate(foe.x, foe.y)
            break

      #move them straight to the other side. no regrets.
      #move as far as possible, as long as it's not off the map
      wantedX = plant.x
      if plant.mutation == self.BUMBLEWEED:
        wantedX += self.directionOfEnemy * self.bumbleweedSpeed
      else:
        wantedX += self.directionOfEnemy * self.uprootRange
      if plant.uprootsLeft > 0 and self.getPlantAt(wantedX, plant.y) is None and  0 <= wantedX < self.mapWidth:
        plant.uproot(wantedX, plant.y)

    #make a new plant every turn, because why not?
    #first, check if we can actually do that
    if len(myPlants) >= self.maxPlants:
      #end turn
      return True

    spawnX = -1
    spawnY = -1
    angle = 0
    loc = 0
    for plant in myPlants:
      #remove all plants in our list except for mothers and spawners
      if not (plant.mutation == self.MOTHER or plant.mutation == self.SPAWNER):
        myPlants.remove(plant)

    #get a random spawner or mother plant
    spawnerPlant = myPlants[random.randint(0, len(myPlants) - 1)]

    #get a new position centered around that spawner within its range
    #also, keep generating new coordinates until they become valid ones
    #Remember from trig:
    #(random x inside a circle) = centerX + rand(0,1)*radius*cos(angle)
    spawnCheckLimit = 0
    while not self.withinBounds(spawnX, spawnY) or self.getPlantAt(spawnX, spawnY) is not None:
      angle = random.random() * 2 * math.pi
      while spawnX < 0 or spawnX >= self.mapWidth:
        spawnX = spawnerPlant.x + int(random.random() * spawnerPlant.range * math.cos(angle))
      while spawnY < 0 or spawnY >= self.mapHeight:
        spawnY = spawnerPlant.y + int(random.random() * spawnerPlant.range * math.sin(angle))
      spawnCheckLimit += 1
      #if we try to spawn too many times, just give up and end the turn
      if spawnCheckLimit > 10:
        return True
    #spawn a random type of plant that isn't a mother or a pool at the coordinates we made
    #of course, make sure we have enough spores to do the job!
    mutationType = random.randint(1, 6)
    if self.me.spores >= self.mutations[mutationType].spores and self.withinSpawnerRange(spawnX, spawnY):
      self.me.germinate(spawnX, spawnY, mutationType)
    return 1

  #Helper function to get all of the plants owned
  def getMyPlants(self):
    myPlants = []
    for plant in self.plants:
      if plant.owner == self.playerID:
        myPlants.append(plant)
    return myPlants

  #Helper function to get distance as a whole number
  def distance(self, x1,  y1,  x2,  y2):
    return int((math.sqrt(math.pow(x1-x2,2)+math.pow(y1-y2,2))))
  
  #Helper function to get a Plant at a point
  #Returns None if no plant found
  def getPlantAt(self, x, y):
    #if it's out of bounds, we don't need to check anything
    if not self.withinBounds(x, y):
      return None
      
    #for every plant, if a plant is at the position we want, return it
    for plant in self.plants:
      if plant.x == x and plant.y == y:
        return plant

    return None

  #Helper function for bounds checking
  def withinBounds(self, x, y):
    if x < 0 or x >= self.mapWidth or y < 0 or y >= self.mapHeight:
      return False
    return True

  #Helper function to check if we're within range of a Spawner or Mother
  def withinSpawnerRange(self, x, y):
    #No need to check if we're not within the bounds of the map
    if not self.withinBounds(x, y):
      return False

    #for every plant
    for plant in self.plants:
      #check for ownership and correct mutation
      if plant.owner == self.me.id and (plant.mutation == self.SPAWNER or plant.mutation == self.MOTHER):
        #if we're within range, we're good
        if self.distance(x, y, plant.x, plant.y) < plant.range:
          return True

    #if we found none, nope
    return False

  def __init__(self, conn):
    BaseAI.__init__(self, conn)
