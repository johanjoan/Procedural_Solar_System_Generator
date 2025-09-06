# Procedural Solar System Generator

This project provides an implementation for procedurally generating entire solar systems in **Unreal Engine 5**.  
It is designed to help developers quickly create dynamic and varied space environments for videogames, such as **space exploration games**.  

The system is **fully customizable** from the Unreal Engine editor, allowing you to configure parameters such as:
- Number of planets
- Planet size and appearance
- Star size and luminosity
- Orbital distances
- Planet decorations  

A more detailed explanation about the customizable parameters can be found here: https://johanjoan.gitlab.io/solar_system_generator/

---

## 🧩 Source Code Classes

All the classes of the source code can be found in the [Source/Procedural_Solar_System_Generator](Source/Procedural_Solar_System_Generator/) folder.  
The main classes include:

| Class | Description |
|-------|-------------|
| `MyActorSolarSystemGenerator` | Controls the generation and organization of the entire solar system. |
| `Planet` | Represents a planet. Inherits from `CelestialBody`. |
| `Star` | Represents the solar system's star. Inherits from `CelestialBody`. |
| `CelestialBody` | Parent class of `Planet` and `Star`, which manages common properties of both classes. |
| `LODManager` | Manages the Level Of Detail (LOD) for each celestial body in the solar system. |
| `PerlinNoise` | Generates Perlin noise for creating planet surfaces. Implementation taken from [daniilsjb/perlin-noise](https://github.com/daniilsjb/perlin-noise). |
| `SpeedManager` | Handles changes in the player’s movement speed. |
| `WaterSphere` | Represents a water sphere. Used to add water to planets. |
| `SphereSection` | Represents a section of a sphere, used to build celestial bodies and water spheres. |

---

## 🎮 Demo

A playable demo featuring a solar system with 8 procedurally generated planets is available by downloading [Demo.zip](https://github.com/johanjoan/Procedural_Solar_System_Generator/blob/master/Demo.zip?raw=true) and executing `Procedural_Solar_System_Generator.exe`.

### Controls

| Action | Key | Value / Notes |
|--------|-----|---------------|
| Player Movement | WASD | — |
| Increase Max Velocity, Acceleration, and Deceleration | J | +10% |
| Decrease Max Velocity, Acceleration, and Deceleration | K | -10% |
| Set "Walking" Velocity | C | `MaxVelocity=4800 cm/s, Accel=2048 cm/s, Deaccel=9600 cm/s` |
| Reset to initial values | R | `MaxVelocity=300000 cm/s, Accel=300000 cm/s, Deaccel=600000 cm/s` |

---

## 🖼️ Example Screenshots

<p align="center">
  <img src="Images/Image_1.png" alt="Solar System Overview" width="600"/>
</p>

<p align="center">
  <img src="Images/Image_2.png" alt="Planet Surface Example" width="600"/>
</p>

<p align="center">
  <img src="Images/Image_3.png" alt="Star and Orbit View" width="600"/>
</p>

<p align="center">
  <img src="Images/Image_4.png" alt="Planet Decorations" width="600"/>
</p>

<p align="center">
  <img src="Images/Image_5.png" alt="Multiple Planets View" width="600"/>
</p>

<p align="center">
  <img src="Images/Image_6.png" alt="Close-up Planet" width="600"/>
</p>


