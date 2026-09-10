# GANN-Object-Avoidance

Vehicles that learn to avoid obstacles. The steering is a neural network, and
the weights are found by a genetic algorithm rather than by gradient descent.

There is no training signal per timestep. A vehicle drives until it crashes or
the generation ends, its fitness is how well it did, and the next generation is
bred from the survivors. The network is a plain feed-forward map from sensor
readings to steering, so all the learning lives in how the weights are selected.

| File | What it holds |
|---|---|
| `Simulator/Simulator/NN.{h,cpp}` | The neuron and layer types. |
| `Simulator/Simulator/NeuralNetwork.cpp` | Feed-forward evaluation. |
| `Simulator/Simulator/GenAlg.{h,cpp}` | Selection, crossover and mutation over the weight vectors. |
| `Simulator/Simulator/Source.cpp` | The simulation loop and the fitness function. |

Physics is Box2D. Rendering and input are SFML. The Visual Studio solution is
`Simulator/Simulator.sln`.

## Licensing

The simulator, the network and the genetic algorithm are mine, under
[LICENSE](LICENSE). Box2D, GLEW, GLFW and SFML are vendored in this tree and are
not. See [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md).
