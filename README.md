# GANN-Object-Avoidance

Neural-network pathfinding trained with a genetic algorithm.

## What this is

Eight small vehicles drive around a walled arena full of square obstacles that slowly drift. Each vehicle is steered by its own tiny neural network (a set of connected artificial neurons that turns inputs into outputs). Nobody tells the networks how to steer. A genetic algorithm (a search method copied from evolution) breeds new networks from the vehicles that survived longest, so later generations get better at avoiding the obstacles.

It is a Windows program written in C++ in 2019, with a Visual Studio 2013 project and a prebuilt copy of the program.

## What you can learn from it

- Biology: natural selection. Vehicles that survive longer are more likely to become parents.
- Biology: inheritance and crossover. A child's "genes" (its network weights) are cut from two parents and joined.
- Computer Science: how a neural network maps sensor readings to actions.
- Computer Science: ray casting (stepping along a straight line until it hits something) to give a vehicle a sense of distance.
- Computer Science: learning without a teacher. There is no right answer for each moment, only a score at the end.

## Run it

### On your own computer

**You will need:** a Windows PC. The program uses Windows-only code (`conio.h`), so it does not build or run on macOS or Linux. None of these steps has been tested for this guide.

The quickest way is the prebuilt program in the repo:

1. On the repo page, choose **Code**, then **Download ZIP**, and unzip it. Or, with [Git](https://git-scm.com/downloads):

   ```bash
   git clone https://github.com/godofecht/GANN-Object-Avoidance.git
   ```

2. Open the folder `Simulator\Release`.
3. Double-click **Simulator.exe**. It needs the files next to it (`arial.ttf` and the `sfml-*.dll` files), so run it from that folder.

A black window called "ADA Project" opens. You should see the arena walls, the square obstacles, and the vehicles with lines coming out of their fronts. Those lines are the sensor rays. The red number in the top-left is the generation. The yellow number below it is the average fitness of the last generation.

There are no keyboard or mouse controls. Close the window to stop.

If Windows reports that `MSVCR120.dll` is missing, install the Visual C++ Redistributable for Visual Studio 2013 (32-bit) from Microsoft.

#### Building from source

The source builds with Visual Studio. The project file was made with Visual Studio 2013 for 32-bit Windows, and it points to folders on the author's old `A:` drive, so it needs changes first:

1. Open `Simulator/Simulator.sln` in [Visual Studio](https://visualstudio.microsoft.com/) with the C++ workload. Accept the offer to upgrade the project.
2. In the project properties, set **Additional Include Directories** to the repo's `include` folder and **Additional Library Directories** to the repo's `lib` folder.
3. Build the **Release | Win32** configuration.
4. Copy `arial.ttf` and the `sfml-*.dll` files from `bin/` or `Simulator/Release/` next to the new `Simulator.exe`.

The SFML libraries in `lib/` were built for Visual Studio 2013. A newer Visual Studio may refuse to link them. If so, download SFML 2.x for your compiler from the [SFML website](https://www.sfml-dev.org/download.php) and point the project at that instead.

## How it works

1. **Sensing.** Each vehicle casts five rays: straight ahead, 10 degrees and 20 degrees to each side. A ray moves forward one pixel at a time, up to 200 steps, and stops when it enters a wall or obstacle. The number of steps is the distance. If it hits nothing, the reading is 0.
2. **Thinking.** The five distances go into a network with 5 inputs, 4 hidden neurons and 5 outputs. Each neuron adds up its inputs, each multiplied by a weight (a number that sets how much that input matters), and passes the total through tanh, which keeps the result between -1 and 1.
3. **Moving.** Output 1 minus output 2 sets how far the vehicle turns. Output 3 sets how fast it moves forward. Outputs 4 and 5 are not used.
4. **Scoring.** A vehicle's fitness goes up every frame it is alive. It dies when its centre enters an obstacle or a wall.
5. **Breeding.** A generation ends when every vehicle has died or after 20 seconds. The program then picks parents by roulette wheel selection: each vehicle's chance of being picked is in proportion to its fitness. Two parents' weights are cut at one random point and the pieces are swapped to make two children (single-point crossover). The children replace the whole population and the vehicles go back to the start.

| File or folder | What is in it |
| --- | --- |
| `Simulator/Simulator/Source.cpp` | the main loop: ray casting, movement, fitness, roulette wheel selection and drawing |
| `Simulator/Simulator/Source.h` | the arena, obstacles, vehicle class, speeds and network shape settings |
| `Simulator/Simulator/NN.h`, `NN.cpp` | the neuron, layer and network, with feed-forward and weight get/set |
| `Simulator/Simulator/GenAlg.h`, `GenAlg.cpp` | single-point crossover over the weight lists |
| `Simulator/Simulator/NeuralNetwork.cpp` | an empty early sketch of the network classes (not built) |
| `Simulator/Simulator.sln` | the Visual Studio solution |
| `Simulator/Release/` | a prebuilt `Simulator.exe` with the files it needs |
| `include/`, `lib/`, `bin/` | SFML headers, libraries and DLLs for Windows |
| `Box2D/` | a copy of the Box2D physics library (not used by the current code) |

## Ideas to try

1. In `Simulator/Simulator/Source.h`, change `positionSpeed` and `rotationSpeed`. Faster vehicles crash sooner. Does evolution still find a way through?
2. In `Simulator/Simulator/Source.cpp`, change the 8 in `for (int i = 0; i < 8; i++)` near the top of `main` to 20 for a bigger population.
3. Add mutation. After `Crossover` in `Source.cpp`, add a small random number to a few of each child's weights before `PutWeights`. Compare how fast the average fitness rises with and without it.
4. In `Simulator/Simulator/Source.h`, the commented-out `isGettingCloser` function rewards a vehicle for moving towards `destination`. Bring it back and see whether the vehicles learn to head for a goal.
5. In `rayTrace` in `Source.cpp`, the distance uses `pow(..., 1 / 2)`. In C++, `1 / 2` is integer division and equals 0, so each step adds 1. Work out what the line was meant to do and fix it.
6. Port the program to CMake so it builds on macOS and Linux. Replace `conio.h` and `getch()`, and fetch SFML with CMake.

## Technical details

There is no training signal per timestep. A vehicle drives until it crashes or the generation ends, its fitness is how well it did, and the next generation is bred from the survivors. The network is a plain feed-forward map from sensor readings to steering, so all the learning lives in how the weights are selected.

- Each layer has an extra bias neuron whose output is fixed at 1.0, so the network has (5 + 1) x 4 + (4 + 1) x 5 = 49 weights.
- Starting weights are random numbers between 0 and 1.
- `NN.cpp` also contains backpropagation (learning rate 0.15, momentum 0.5), but the simulator never calls it. All learning comes from the genetic algorithm.
- Vehicles are 50 by 30 pixel rectangles that start at (300, 500) facing 45 degrees. The arena is an 800 by 800 square of thin walls. There are nine 100 by 100 obstacles that move 0.1 pixels per frame in a repeating pattern and are put back in place at the start of each generation.
- A vehicle's fitness goes up by 2 each frame it is alive (the code adds 1 in two places).
- Collisions are checked with SFML bounding boxes. Box2D is in the repo and the Debug configuration links `Box2D.lib`, but the code does not call it.
- Rendering and input are SFML 2. The Visual Studio solution is `Simulator/Simulator.sln`.

## Project status

This is a finished 2019 student project and is not being developed. Known problems:

- Windows only, and the Visual Studio project points to absolute paths on an `A:` drive.
- The genetic algorithm has crossover and no mutation, so it can only remix the weights of the first generation.
- Large build files are committed: `Box2D/Build/Box2D.sdf` (44 MB), `Simulator/Simulator.sdf`, `.obj` files, `.pdb` files and `Simulator/Release.rar`.
- There is no screenshot or video of the simulator in the repo.

## Licence

You can use, copy, change and share this code, including in closed-source work, as long as you keep the copyright notice and the licence text, and do not use the author's name to promote what you make. See [LICENSE](LICENSE) (BSD 3-Clause).

Box2D, GLEW, GLFW and SFML are copied into this repo and keep their own licences. See [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md).

## Credits

- Written by Abhishek Shivakumar.
- [SFML](https://github.com/SFML/SFML) by Laurent Gomila, for windows and drawing (zlib/libpng licence).
- [Box2D](https://github.com/erincatto/box2d) by Erin Catto (MIT), [GLEW](https://github.com/nigels-com/glew) and [GLFW](https://github.com/glfw/glfw), included under `Box2D/`.
