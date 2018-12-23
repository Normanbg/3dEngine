
# 3dEngine
This 3D engine is created for the subject "3D Engines" of [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/) by [Norman Benet](https://github.com/Normanbg) & [Carles Margelí](https://github.com/Margeli).

## Installation:
Decompress the .zip file in any directory and execute the executable inside the decompressed folder.

### Camera Controls on Scene:

**Mouse Wheel** to zoom in and out.

**Mouse Right click** for free look around.

**Mouse Right click + A or D** to move around the X-axis.

**Mouse Right click + R or T** to move around the Y-axis.

**Mouse Right click + W or S** to move around the Z-axis.

**Holding SHIFT** duplicates movement speed.

**Alt + Mouse Right click** to orbit an object.

**F** focus the camera around the geometry.

## Controls Guizmo:
**W** to translate.

**E** to rotate.

**R** to scale.

### Controls on panels:

**Mouse Left click** for selecting.

**Double Mouse Left Click** inside hierarchy or materials to deselect a game object.

**Mouse Drag** for changing components like Transform.

### Load Objects:
On assets panel there are all the files to load, click on one and select "Load file". Also it is possible to drag & drop files.

### Link to the repository:
[https://github.com/Normanbg/3dEngine](https://github.com/Normanbg/3dEngine)

### Latest release:
[https://github.com/Normanbg/3dEngine/releases](https://github.com/Normanbg/3dEngine/releases)

### Innovation:
For this delivery we are able to render directly the scene on a window, using a Frame Buffer Object to get the texture and let ImGui Image to render it.

Also, we have our fyle system that uses the library physfs.


## Video

<iframe width="560" height="315" src="https://youtu.be/2DGAjaIH5W8" frameborder="0" allowfullscreen></iframe>

### Members work:
Game Objects->Both

Bounding box-> Both

Components-> Both

QuadTree-> Norman

Frustum Culling-> Norman

Import files-> Carles

Own file format-> Carles

Mouse picking-> Norman

Time manager-> Carles

Resources & Fyle System-> Carles

.meta-> Carles

Guizmos-> Norman

**Panels:**

Assets-> Carles

Hierarchy-> Both

Inspector-> Both

Optimization-> Norman

Scene-> Norman

Scene Info-> Both

UI Draw -> Both

Button, Checkbox, Window -> Norman

Image, Text, InputText -> Carles

Scene preparation -> Both

### License:
MIT License

Copyright (c) 2018 Normanbg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

