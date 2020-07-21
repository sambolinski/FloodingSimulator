# Final Year Project: OpenGL Ship Flooding Simulation
![flooding](https://i.imgur.com/bhoK9Rt.png)
## About
This project Final year project for my Computer Science degree. The goal was to create a physics simulation which can emulate a ship taking on water and seeing the effects of the ships list and trim as well as what combination of longitudinal/transverse bulkheads would be best in preventing sinking, and whether longitudinal bulkheads are safe to include. 

## Features

### Graphics
- OpenGL graphics
- Ships drawn as nodes, while springs are drawn as lines connecting nodes.
- Ocean drawn as plane.
- Flooding node coloured red, steel nodes coloured grey, air nodes coloured white.

### Simulation
- Applying forces (Drag, Gravity, Buoyancy).
- Verlet integration for velocity.
- Spring-Lattice physics
- Water flooding between nodes and blocked by steel nodes.

### Custom Simulation
- Can set custom ship design
- Hull material
- Hull thickness
- Water density
- Starting flooding node
- Flooding coefficient
- Flooding hole area

### Output
- Current simulation data
- Total Ship volume
- Total Floodable volume
- Maximum water intake before foundering
- Flooding node
- Water density
#### 5 second intervals
- Current time
- List/Trim
- Flooded volume
- Percentage flooded
