# D3DEngine
Experimenting with D3D11 API by making a simple 3d model loader.

## TODO:
- [ ] separate camera movments into CameraMover class.
- [ ] separate vertex and pixel shaders from each other and create PipeLine class.
- [ ] abstract render target.
- [ ] abstract SetShaderResource(...).
- [ ] remove m_buffers, m_impl, m_handle from the header file.
- [ ] [Code Style] get rid of the init() and cleanup() for all classes and just use the constructor.
- [ ] add ENGINE_WRN macro.
- [ ] replace macros with functions.
- [x] add frame limiter.
- [x] shader can load from text rather than filepath.