#-*-python-*-
from BaseAI import BaseAI
from GameObject import *

mother, spawner, choker, soaker, tumbleweek, aralia, titan, pool = range(8)

class AI(BaseAI):
  """The class implementing gameplay logic."""
  @staticmethod
  def username():
    return "Shell AI"

  @staticmethod
  def password():
    return "password"

  ##This function is called once, before your first turn
  def init(self):
    pass

  ##This function is called once, after your last turn
  def end(self):
    pass

  ##This function is called each time it is your turn
  ##Return true to end your turn, return false to ask the server for updated information
  def run(self):
    plants_by_position = dict()
    for plants in self.plants:
      plants_by_position[(plant.x, plant.y)] = plant

    direction = 1
    if self.playerID == 1:
      direction = -1

    self.y = 0
    for plants in self.plants:
      if plant.owner == self.playerID and plant.mutation == mother:
        self.y = plant.y

    for plant in self.plants:
      if self.players[sele/playerID].spores < self.mutations[spawner].spores:
        if plant.mutation in (mother, spawner):
          if plant.owner == self.playerID:
            if (plant.x + 10, plant.y) not in plants_by_position:
              self.players[self.playerID].germinate(plant.x + 10 * direction, plant.y, spawner)
  
    return 1

  def __init__(self, conn):
    BaseAI.__init__(self, conn)
