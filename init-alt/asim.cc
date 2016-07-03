  class Simulation {
    const RadialDecayTable radial_absorption;
    Array2D<double> total_attempted_absorption;
    . . .
  public:
    Simulation(uint world_x, uint world_y)
    :
      radial_absorption(absorption_radius),
      total_attempted_absorption(world_x, world_y)
      . . . 
    { }

    void add_up_attempted_absorption() {
      for (auto it = organisms.begin(); it != organisms.end(); ++it) {
        Organism& o = it->second;
        radial_absorption.add_to(total_attempted_absorption, o.x, o.y);
      }
    }
    . . .
  };
