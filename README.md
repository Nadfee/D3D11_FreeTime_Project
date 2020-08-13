# dx11_project
Personal project where I tinker with D3D11  
  
My main purpose with this project is to play around with DX11, DirectX Tool Kit and the organization of a larger project  
  
- Current focus is to create a game with minimal graphical techniques to get experience in organizing possible abstractions (e.g model, where to draw, etc.)  
  
Controls:  
  
WASD : Move  
T : Toggle between Absolute and Relative mouse mode (visible/invisible)  
Q : Descend  
E : Ascend  
O : Remove the static cube objects in order (WARNING: Intentionally asserting false once you try to remove when there are none of the static cubes left)  
P : Inserts a triangle infront of the camera  
G : Toggle render state for the floating and rotating cube object (Object and mesh still lives but user can choose whether to render it or not)  
GUI -> Options -> Restore Scene : Restores initial scene from startup  
  
Project should build (DirectXTK Desktop Win10 NuGet package is installed so make sure it is restored)  
Make sure that SubSystem is set to Windows and the dependencies "d3d11.lib", "dxgi.lib", "d3dcompiler.lib" are added  
  
Artifacting in the background is caused by the Gif recorder, pay no mind to it :)  
Progress preview as of August 13th 2020.
- Dynamically insert and remove objects (with meshes)  
- Dynamically insert and remove point lights (Space for 500 (can be user-specified) lights is reserved for the Structured Buffer during initialization)  
  - No code for handling exceeding amount of maximum point lights right now.  
 
![Alt text](/progresspreview.gif?raw=true "Gif")  
  
Example of "interface" to create an object, a point light, and how to modify them during Update! 
(Uses std::unordered_map)  
![Alt text](/ss3New.png?raw=true "Example code")