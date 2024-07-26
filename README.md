# Night Scape Engine - C++ / DX11 Game Engine

Currently W.I.P., planning a partial refactoring/rework after an upcoming playtesting event

## Features
- Simple servers infrastructure for Input, Rendering, Time, Scene, etc... handling
- Double precision for the game entities positioning
- Rendering abstractions (GraphicsBuffer, Shader, Material, Texture, RenderTexture)*
- obj_ptr<> wrapper with Destroy() Create() functionality to handle entities' lifetime*
- CPU particle systems with effecient rendering
- Simple drawing batcher
- Simple UI with Pixel-perfect rendering
- Basic forward lighting
- .obj / .dds support

\* \- Somewhat similar to how it works in the Unity game engine

![In-engine screenshot](Showcase/Screenshot.png?raw=true "In-Engine Screenshot")
