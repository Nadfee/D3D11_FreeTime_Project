# d3d11_experimental
Playing around
  
My main purpose with this project is to play around with DX11, DirectX Tool Kit and just try out some ideas (good or bad) that I have in my mind.


Controls:  
  
WASD : Move  
T : Toggle between Absolute and Relative mouse mode (visible/invisible)  
Q : Descend  
Space : Ascend  
O : Remove the static cube objects in order (WARNING: Intentionally asserting false once you try to remove when there are none of the static cubes left)  
P : Inserts a triangle infront of the camera  
G : Toggle render state for the floating and rotating cube object (Object and mesh still lives but user can choose whether to render it or not)  
  
Preview as of September 6th 2020.  
- Dynamically insert and remove objects (with meshes)  
- Dynamically insert and remove point lights (Space for 500 (can be user-specified) lights is reserved for the Structured Buffer during initialization)  
  - No code for handling exceeding amount of maximum point lights right now.  
- Toggle Render State for existing Objects (Graphics will skip drawing that objects mesh)
- Toggle Update State for existing Point Lights (Ensures an identifier is set for the light (Constant attenuation set to -1) so that HLSL can skip the calculation for that specific light)
- Gamma correction (inverse square law for attenuation now works well!)
- Basic GPU simulated particle system (Ping Pong buffers implementation explained by Zink in Practical Rendering for D3D11)
- Skybox
- Model loading (Sponza)
 
![Alt text](/pic.png?raw=true "Picture")
  
Example of "interface" to create an object, a point light, and how to modify them during Update! (Uses hash map)   
![Alt text](/codeSnippet.png?raw=true "Example code")

Yea I got tired of this project and not updating it anymore. Simply using this project as reference for future projects
