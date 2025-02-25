# Ray Tracing Engine

## Project Overview

This project focuses on building a 3D renderer from scratch. The final version of the program will output an image of spheres in 3D space, rendered using semi-realistic lighting and ray tracing techniques. The goal is to develop a fully customizable rendering engine in C, implementing dynamic memory allocation, file I/O, low-level programming, and optimized sorting algorithms.

The project is structured into multiple stages, each focusing on different aspects of the rendering pipeline. The suggested workflow includes developing core mathematical operations, constructing a scene, implementing a ray tracing algorithm, and progressively refining the rendering output with lighting effects and anti-aliasing.

## How to Use This

1. Clone this repository to your local machine:
   ```sh
   git clone <repository_url>
   cd <repository_directory>
   ```
2. Follow the provided test cases to verify functionality.
3. Compile and run the program using the provided Makefile.
4. View the generated PPM files using an image viewer or the provided `viewppm` tool.

## Milestone Breakdown

### Stage 1: Core Development
1. **Vector Mathematics**: Implement basic vector operations for 3D space, including addition, subtraction, scalar multiplication, normalization, and dot product.
2. **Camera and Viewport**: Define a viewport and implement a virtual camera that projects rays into the scene.
3. **Basic Scene Representation**: Implement a data structure to represent spheres in the world, including their position, radius, and default color.
4. **Input Handling**: Read input values for scene configuration, including image size, camera settings, and object definitions.
5. **Basic Output Handling**: Generate a simple text-based representation of the scene for debugging purposes.

### Stage 2: Rendering Pipeline
1. **Ray-Sphere Intersection**: Implement an algorithm to determine whether a ray intersects a sphere using quadratic solutions.
2. **Basic Image Rendering**: Generate an image where each pixel represents whether it hit a sphere or the background.
3. **Color Representation**: Convert colors from hexadecimal format to RGB and apply them to spheres.
4. **File Output**: Implement the ability to write images to PPM format for visualization.

### Stage 3: Advanced Features
1. **Lighting Calculation**: Implement point light sources and diffuse shading to simulate realistic lighting effects.
2. **Shadows**: Cast shadow rays to determine whether an intersection point is blocked by another object.
3. **Sorting and Optimization**: Sort sphere colors using `qsort` for efficient rendering.
4. **Anti-Aliasing**: Implement multi-sample anti-aliasing using a 3x3 grid sampling technique to smooth edges.
5. **Performance Improvements**: Optimize calculations and memory usage for better efficiency.

## Technical Details

### Multi-File Structure
The project is organized into multiple files for modularity and maintainability:

```
src/
|-- vector.h
|-- vector.c
|-- spheres.h
|-- spheres.c
|-- color.h
|-- color.c
|-- renderer.h
|-- renderer.c
|-- main.c
|-- Makefile
```

- `vector.h/.c`: Implements 3D vector mathematics.
- `spheres.h/.c`: Defines and manages sphere objects.
- `color.h/.c`: Handles color conversion and output.
- `renderer.h/.c`: Manages the ray tracing pipeline.
- `main.c`: The entry point that initializes and runs the rendering engine.
- `Makefile`: Automates compilation and execution.

### Makefile
A `Makefile` is included to compile and run the program efficiently. It builds separate executables for different configurations and ensures all dependencies are correctly linked.

### Ray-Sphere Intersection
Using the mathematical representation of a sphere and a parametric ray equation, the intersection points are computed using the quadratic formula. The closest positive intersection determines the pixel color.

### Lighting Model
The lighting model incorporates:
1. **Diffuse Shading**: Determines brightness based on the angle between the light direction and the surface normal.
2. **Shadows**: Determines whether an object is occluded by another before receiving direct light.
3. **Ambient Lighting**: A base lighting intensity to ensure objects are visible even in shadow.

### Anti-Aliasing
To reduce jagged edges, each pixel is sampled at nine points in a 3x3 grid, and the final color is determined by averaging these samples.

## Output Format
The generated images are saved in PPM format, which can be easily visualized using various tools.

## Future Improvements
1. **Reflections and Refractions**: Extend ray tracing to support reflective and transparent surfaces.
2. **Acceleration Structures**: Implement bounding volume hierarchies (BVH) for efficient ray-object intersection tests.
3. **Parallel Processing**: Utilize multi-threading or GPU acceleration for performance improvements.

This project serves as a foundation for understanding and implementing ray tracing techniques in C, providing insights into graphics programming and computational geometry.

