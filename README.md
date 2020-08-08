# dx11_project
Personal project where I tinker with DX11

My main purpose with this project is to play around with DX11, DirectX Tool Kit and the object-oriented organization of a project.

- Current focus is to create a game with minimal graphical techniques to get experience in organizing possible abstractions (e.g model, where to draw, etc.)

Main top-level logic resides in Application.cpp

Controls:
A - Toggle between Absolute and Relative mouse mode (visible/invisible)

Project should build (DirectXTK Desktop Win10 is installed so make sure it is restored)
Otherwise, make sure that SubSystem is set to Windows and the dependencies "d3d11.lib", "dxgi.lib", "d3dcompiler.lib" is added.
