# D3DEngine
Experimenting with D3D11 API by making a simple 3d model loader.

## TODO:
- [ ] separate vertex and pixel shaders from each other and create PipeLine class.
- [ ] abstract render target.
- [ ] make shader system propably using JSON.
- [ ] separate camera movments into CameraMover class.
- [x] create DepthStencilState.
- [x] remove m_buffers, m_impl, m_handle from the header file.
- [x] add depth stencil to texture.
- [x] [Code Style] get rid of the init() and cleanup() for all classes and just use the constructor.
- [x] replace macros with functions.
- [x] add ENGINE_WRN macro.
