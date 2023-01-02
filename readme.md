<img align="left" width="64" height="64" src="images/convexHull2D_image.png">

# GPU Path Tracer 

## Introduction:
	A physically based render engine running on an OpenGL compute shader. 

## Controls:
### Camera:
The Camera Object casts rays out into the 3D scene and captures the image by evaluating 
the amount of light arriving at each pixel. The camera is free to move around within the 
3D world space.

1. Camera pan: `Middle Mouse Button`
2. Camera rotate: `Alt + Middle Mouse Button`
3. Camera zoom: `Mouse Scroll`

Other parameters: `Field of View`, `Focus Distance`, `Aperture` (Described below)




### Ray Depth
	The Ray Depth parameter controls the number of bounces the light ray emitted from the camera makes. 
<p float="left">
	<p>
  	<img src="output/Depth/depth1.jpg" width="256" />
	<p> depth 1 </p>
	</p>
  <img src="output/Depth/depth2.jpg" width="256" />
<p> depth 1 </p>
  <img src="output/Depth/depth3.jpg" width="256" />
  <img src="output/Depth/depth4.jpg" width="256" />
  <img src="output/Depth/depth5.jpg" width="256" />
  <img src="output/Depth/depth6.jpg" width="256" />
</p>

depth = 1 | depth = 2 | depth = 3
--- | --- | ---
![](output/Depth/depth1.jpg) | ![](output/Depth/depth2.jpg)| ![](output/Depth/depth3.jpg)
depth = 4 | depth = 5 | depth = 6
--- | --- | ---
![](output/Depth/depth4.jpg) | ![](output/Depth/depth5.jpg)| ![](output/Depth/depth6.jpg)

![image](output/chull_def.png)

## How to use the plugin:
	
The Convex Hull 2D object can be found under the "Extensions" tab. 

The Convex Hull is computed on it's children object's data. The data is interpreted differently depending
on the selected mode in the object properties.There are currently two convex hull modes:
**Pivot point based** and **Geometry based**.

### Pivot point based
	By default the Convex Hull 2D object snaps it's childern objects to it's local XZ plane and
	computes convex hull on it's child objects' pivot points. The child objects are free to move around
	in Convex Hull 2D object's XZ plane.


### Geometry based
	To use Geometry based mode we need to enable it first in the Object settings by checking on
	"Geometry based" option. This mode computes the convex hull on the points produced by intersecting
	the world X-Z plane with the childern objects. Note that the children objects must be in editable
	mode to be considered for computation.
	
<p float="left">
  <img src="images/editable.png" width="201" />
  <img src="images/geometry_based.png" width="784" /> 
</p>

### Define custom plane of intersection
	We can also overwrite the default "world X-Z" plane of intersection and define a custom plane
 	of intersection for our convex hull calculations. We do so by linking any object to the "Plane
	of intersection target" linker in the Object properties. Simply create a new object
	(for example a quad polygon), drag and drop it into the link area. Once the object has been linked
	it should have a green bounding box surrounding it. The orientation and the position of the linked 
	object now defines the new orientation and center of the plane of intersection for geometry based mode.
<p float="left">
  <img src="images/linkedobj.png" width="414" />
</p>

<p float="left">
  <img src="images/linked1.png" width="314" />
  <img src="images/linked2.png" width="314" />
  <img src="images/linked3.png" width="314" />
</p>

### Bounding plane
	For a better visualization we can turn on the "Show Bounding Plane" option. This will display
	a green bounding plane of the convex hull points.

<p float="left">
  <img src="images/linked1b.png" width="314" />
  <img src="images/linked2b.png" width="314" />
  <img src="images/linked3b.png" width="314" />
</p>

### Expansion
	The expansion slider allows us to expand/contract the convex hull, similar but not the same as
	to scaling it up or down. More precisely, the expansion direction is based on the sum of a point's
	tangent vectors, which are pointing to the next and previous points within the convex hull. From my
	testing this type of expansion gives the most uniform and stable results, especially when dealing with
	closed geometry. Currently the "Expansion mode" contains "Tangent Based" mode only. I am planning on
	including more modes in the future.
<p float="left">
  <img src="images/expansionMode.png" width="314" />
 
</p>
<p float="left">
  <img src="images/expand0.png" width="314" />
  <img src="images/expand100.png" width="314" />
</p>

### Precision
	Precision parameter can be thought of as a distance threshold between two points. If the distance
	between two given points is smaller than the value of the precision parameter, then the two points
	will be considered as a single point for the convex Hull computation. By default it is set to 0.001.
	You can adjust the precision parameter based on the topology of your geometry and the respective distances
	between child objects. 
	
	Sometimes when dealing with dense geometry it could be helpful to increase the value of the precision
	parameter to increase performance of the algorithm. But as always, it is a trade off between performance and accuracy.

### Future updates
	1) Update current gift wrapping algorithm to a more efficient Graham's Scan Algorithm.
	2) Add a feature to create multiple convex hull splines along geometry by offseting current plane of intersection.
	3) Add bezier and bspline support.
	