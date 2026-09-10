# Third-party notices

This repository vendors libraries that are not mine. They keep their own
copyright and licence and are not covered by this project's LICENSE.

| Path | Component | Licence |
|:--|:--|:--|
| `Box2D/Box2D/` | Box2D, Erin Catto | MIT |
| `Box2D/glew/` | GLEW | Modified BSD, Mesa 3-D, Khronos |
| `Box2D/glfw/` | GLFW | zlib/libpng |
| `bin/`, `lib/`, `include/` (SFML) | SFML, Laurent Gomila | zlib/libpng |

The `bin/` and `lib/` directories contain prebuilt SFML and OpenAL binaries for
Windows. Rebuild them from upstream rather than trusting the checked-in copies.

Upstream:

- Box2D: https://github.com/erincatto/box2d
- GLEW: https://github.com/nigels-com/glew
- GLFW: https://github.com/glfw/glfw
- SFML: https://github.com/SFML/SFML
