# D3DEngine
Experimenting with D3D11 API by making a simple 3d model loader.

## TODO:
- [ ] separate camera movments into CameraMover class.
- [ ] separate vertex and pixel shaders from each other and create PipeLine class.
- [ ] abstract render target.
- [ ] abstract SetShaderResource(...).
- [ ] use Desc instead of parameters.
- [ ] remove m_buffers, m_impl, m_handle from the header file.
- [ ] [Code Style] get rid of the init() and cleanup() for all classes and just use the constructor.
- [x] replace macros with functions.
- [x] add ENGINE_WRN macro.
