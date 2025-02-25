// file: src/assg.c
// name- rakshit singh
// used resources(links{only for concept clarity}) - https://eclass.srv.ualberta.ca/mod/page/view.php?id=8249101
//                                                   https://stackoverflow.com/questions/2018730/fprintf-with-string-argument
//                                                   https://stackoverflow.com/questions/3351809/reading-file-using-fscanf-in-c
//                                                   https://developer.arm.com/documentation/dui0041/c/arm-compiler-reference/limits-for-floating-point-numbers
//                                                   https://stackoverflow.com/questions/22156494/getting-a-memory-leak-from-dynamically-allocated-arrays
//                                                   CMPUT 201 Discord server for general help with funtions and many more concepts like shadow,lighting,ray generation etc.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "assg.h" // include the header file

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// initialize camera and viewport
void Initialize_camera(float focal_length, float viewport_height, int image_width, int image_height) {
    // initialize camera 
    Camera.position = (Vec3){0, 0, 0};
    // initialize viewport
    Camera.focal_length = focal_length;
    // calculate viewport height
    Camera.viewport_height = viewport_height;
    // calculate viewport width
    Camera.viewport_width = viewport_height * ((float)image_width / image_height);
    // calculate pixel values
    Camera.x_value_pixel = Camera.viewport_width / image_width;
    Camera.y_value_pixel = Camera.viewport_height / image_height;
    // calculate first pixel
    Camera.first_pixel = (Vec3){
        // x and y values of the first pixel in the viewport 
        -Camera.viewport_width / 2 + Camera.x_value_pixel / 2, //left edge djusts the position to the center of the first pixel.
        Camera.viewport_height / 2 - Camera.y_value_pixel / 2, //top edge adjusts the position to the center of the first pixel.
        -Camera.focal_length
    };
}

// generate ray for a pixel sample FS 
Ray produce_ray(Vec3 pixel_position) {
    Ray ray;
    ray.origin = (Vec3){0,0,0};
    ray.direction = normalize(subtract(pixel_position, ray.origin));
    return ray;
}

// generate ray for a pixel sample ms-2
Ray ray_generation(int x, int y, int image_width, int image_height) {
    // initialize ray
    Ray ray;
    // calculate ray origin
    ray.origin = Camera.position;
    Vec3 pixel_position = {
        // calculate pixel position
        Camera.first_pixel.x + x * Camera.x_value_pixel, // adding the x value of the first pixel and x value of the pixel multiplied by x value of the pixel
        Camera.first_pixel.y - y * Camera.y_value_pixel, // subtracting the y value of the first pixel and y value of the pixel multiplied by y value of the pixel
        Camera.first_pixel.z
    };
    // calculate ray direction
    ray.direction = normalize(subtract(pixel_position, ray.origin)); // normalize the vector
    return ray;
}

// calculate the color for a given ray
Vec3 calculateColor(Ray ray, World *world, Light light_source, Vec3 backgroundcolor) {
    //set the smallest t to the maximum value of float flt_max
    float largest_t = INFINITY;
    //initialize the color to the background color
    Vec3 color = backgroundcolor; 
    //iterate through all the spheres in the world
    for (int i = 0; i < world->size; i++) {
        float t;
        //check if the ray intersects with the sphere
        if (doesIntersect(world->spheres[i], ray.origin, ray.direction, &t) && t < largest_t) {
            largest_t = t;
            //calculate the intersection point and normal vector at the intersection point 
            Vec3 intersectionPoint = add(ray.origin, scalarMultiply(t, ray.direction));
            Vec3 normal = normalize(subtract(intersectionPoint, world->spheres[i]->pos));
            //calculate the color of the sphere at the intersection point with lighting and shadows
            color = calculateLightingAndShadows(intersectionPoint, normal, world->spheres[i]->color, light_source, world->spheres, world->size);
        }
    }
    return color;
}

// calculate lighting
Vec3 calculateLighting(Vec3 p, Vec3 n, Vec3 sphereColor, Light light) {
    //calculate the direction vector from the intersection point to the light source
    Vec3 d = normalize(subtract(light.position, p));
    //calculate the dot product of the direction vector and the normal vector
    float dotProduct = fmax(dot(d, n), 0.0f);
    //calculate the square of the distance between the intersection point and the light source
    float distSq = distance2(light.position, p);
    //calculate the intensity of the light at the intersection point
    float I0 = light.brightness * dotProduct / distSq;
    // cap intensity to 1
    float I = fmin(1.0f, I0);

    //calculate the surface lighting color at the intersection point using the intensity of the light source
    Vec3 surfaceLightingColor = {
        sphereColor.x * I,
        sphereColor.y * I,
        sphereColor.z * I
    };

    return surfaceLightingColor;
}

// calculate shadows
Vec3 CalculateShadow(Vec3 p, Vec3 n, Vec3 surfaceLightingColor, Vec3 d, Sphere **spheres, int sphereCount, Vec3 lightPosition) {
    // create a shadow ray from the intersection point to the light source
    Ray shadowRay = { p, d };

    // iterate through all the spheres in the world
    for (int i = 0; i < sphereCount; i++) {
        float t;
        // check if the shadow ray intersects with the sphere
        if (doesIntersect(spheres[i], shadowRay.origin, shadowRay.direction, &t)) {
            // Ignore intersections that are very close to the origin of the shadow ray
            if (t > 0.001f) {
                // Check if the intersection point is between the original point and the light source
                Vec3 intersectionPoint = add(shadowRay.origin, scalarMultiply(t, shadowRay.direction));
                float distanceToLight = length(subtract(lightPosition, p));
                float distanceToIntersection = length(subtract(intersectionPoint, p));
                if (distanceToIntersection < distanceToLight && dot(subtract(intersectionPoint, p), d) > 0) {
                    // If the intersection point is between the original point and the light source, it casts a shadow
                    surfaceLightingColor.x *= 0.1f;
                    surfaceLightingColor.y *= 0.1f;
                    surfaceLightingColor.z *= 0.1f;
                    break;
                }
            }
        }
    }

    return surfaceLightingColor;
}

// combine lighting and shadows
Vec3 calculateLightingAndShadows(Vec3 p, Vec3 n, Vec3 sphereColor, Light light, Sphere **spheres, int sphereCount) {
    // calculate the direction vector from the intersection point to the light source 
    Vec3 d = normalize(subtract(light.position, p));
    Vec3 surfaceLightingColor = calculateLighting(p, n, sphereColor, light);

    // calculate the surface lighting color at the intersection point with shadows
    surfaceLightingColor = CalculateShadow(p, n, surfaceLightingColor, d, spheres, sphereCount, light.position);
    return surfaceLightingColor;
}

// scene initialization
void init_scene(Vec3 light_vector, float light_intensity, Light light_source) {
    // set the light source position and brightness 

    light_source.position = light_vector;
    light_source.brightness = light_intensity;
}

// light intensity at a point
float calculate_light_intensity(Vec3 point, Light light_source) {
    // calculate the direction vector from the intersection point to the light source
    Vec3 light_dir = subtract(light_source.position, point);
    // calculate the distance between the intersection point and the light source
    float distance = length(light_dir);
    // normalize the direction vector
    light_dir = normalize(light_dir);
    // calculate the intensity of the light at the intersection point
    return light_source.brightness / (distance * distance);
}

// function to read colors from file
void readColors(FILE *input_file, unsigned int *colors, int total_colors) {
    // read colors from file and store in array of colors
    for (int i = 0; i < total_colors; i++) {
        if (fscanf(input_file, "%x", &colors[i]) != 1) {
            fprintf(stderr, "error reading color %d\n", i);
            exit(EXIT_FAILURE);
        }
    }
}

// function to read spheres from file and add to the world
void readSpheres(FILE *input_file, int total_spheres, World *world, unsigned int *colors) {
    // read spheres from file and add to the world
    for (int i = 0; i < total_spheres; i++) {
        // read sphere parameters from file and create sphere object and add to world 
        Vec3 position;
        float radius;
        int color_index;
        // read sphere from file
        if (fscanf(input_file, "%f %f %f %f %d", &position.x, &position.y, &position.z, &radius, &color_index) != 5) {
            fprintf(stderr, "errror reading sphere %d\n", i);
            exit(EXIT_FAILURE);
        }
        #ifndef FS
        Sphere *sphere = createSphere(radius, position, (Vec3){1, 1, 1});
        #else 
        Sphere *sphere = createSphere(radius, position, unpackRGB(colors[color_index]));
        #endif
        addSphere(world, sphere);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    MS1                                                                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// print vector operations
void printVectorOperations(FILE *output_file, Vec3 backgroundcolor, Vec3 globallight_position, float viewport_width) {
    Vec3 add_result = add(backgroundcolor, globallight_position);
    Vec3 sub_result = subtract(backgroundcolor, globallight_position);
    Vec3 scalar_result = scalarMultiply(viewport_width, globallight_position);
    Vec3 norm_result = normalize(globallight_position);

    fprintf(output_file, "(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
        backgroundcolor.x, backgroundcolor.y, backgroundcolor.z,
        globallight_position.x, globallight_position.y, globallight_position.z,
        add_result.x, add_result.y, add_result.z);
    fprintf(output_file, "(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
        backgroundcolor.x, backgroundcolor.y, backgroundcolor.z,
        globallight_position.x, globallight_position.y, globallight_position.z,
        sub_result.x, sub_result.y, sub_result.z);
    fprintf(output_file, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
        viewport_width,
        globallight_position.x, globallight_position.y, globallight_position.z,
        scalar_result.x, scalar_result.y, scalar_result.z);
    fprintf(output_file, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
        globallight_position.x, globallight_position.y, globallight_position.z,
        norm_result.x, norm_result.y, norm_result.z);
}

// print sphere operations
void printSphereOperations(FILE *output_file, Vec3 globallight_position, World world) {
    for (int i = 0; i < world.size; i++) {
        Sphere *sphere = world.spheres[i];
        Vec3 div_result = scalarDivide(sphere->color, sphere->r);
        float dot_result = dot(globallight_position, sphere->pos);
        float dist_result = distance(globallight_position, sphere->pos);
        float length_result = length(sphere->pos);

        fprintf(output_file, "\n(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n",
            sphere->color.x, sphere->color.y, sphere->color.z, sphere->r,
            div_result.x, div_result.y, div_result.z);
        fprintf(output_file, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
            globallight_position.x, globallight_position.y, globallight_position.z,
            sphere->pos.x, sphere->pos.y, sphere->pos.z, dot_result);
        fprintf(output_file, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
            globallight_position.x, globallight_position.y, globallight_position.z,
            sphere->pos.x, sphere->pos.y, sphere->pos.z, dist_result);
        fprintf(output_file, "length(%.1f, %.1f, %.1f) = %.1f\n",
            sphere->pos.x, sphere->pos.y, sphere->pos.z, length_result);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    MS2                                                                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// render the scene and write to ppm file
void renderScene(World *world, int image_width, int image_height, FILE *output_file, Light light_source, Vec3 backgroundcolor) {
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            Ray ray = ray_generation(x, y, image_width, image_height); 
            Vec3 color = calculateColor(ray, world, light_source, backgroundcolor);
            writeColour(output_file, color);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    FS                                                                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void pixel_color(World *world, int x, int y, int image_width, int image_height, Light light_source, Vec3 backgroundcolor, Vec3 *final_pixel_colour) {
    float offsets[3] = {-1.0/3, 0.0, 1.0/3};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float u = ((x + 0.5 + offsets[i]) - (image_width / 2.0)) * (Camera.viewport_width / image_width);
            float v = ((y + 0.5 + offsets[j]) - (image_height / 2.0)) * (Camera.viewport_height / image_height);
            Ray ray = produce_ray((Vec3){u, v, -Camera.focal_length});
            Vec3 sample_color = calculateColor(ray, world, light_source, backgroundcolor);
            *final_pixel_colour = add(*final_pixel_colour, sample_color);
        }
    }
}

void anti_aliasing(World *world, int image_width, int image_height, FILE *output_file, Light light_source, Vec3 backgroundcolor) {
    for (int y = image_height-1; y >= 0; y--) {
        for (int x = 0; x < image_width; x++) {
            Vec3 final_pixel_colour = {0.0f, 0.0f, 0.0f};
            pixel_color(world, x, y, image_width, image_height, light_source, backgroundcolor, &final_pixel_colour);
            Vec3 color = scalarDivide(final_pixel_colour, 9.0f);
            writeColour(output_file, color);
        }
    }
}

// Function to read and sort colors
void readAndSortColors(unsigned int *colors, int numColors) {
    qsort(colors, numColors, sizeof(unsigned int), compareColor);
}

// Function to assign colors based on indices
void assignColors(unsigned int *colors, int numColors, int *indices, int numIndices, Vec3 *assignedColors) {
    for (int i = 0; i < numIndices; i++) {
        assignedColors[i] = unpackRGB(colors[indices[i]]);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// main function


int main(int argc, char const *argv[]) {
    if (argc != 3) {
        printf("incorrect number of arguments.\n");
        return 1;
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];

    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("could not open file\n");
        return 1;
    }

    int image_width, image_height, total_colors, background_index, total_spheres; // background_index is unused
    float viewport_height, focal_length;
    Light light_source;
    World world;    
    worldInit(&world);

    if (fscanf(input_file, "%d %d", &image_width, &image_height) != 2) {
        fprintf(stderr, "error reading image dimensions\n");
        return 1;
    }
    if (fscanf(input_file, "%f", &viewport_height) != 1) {
        fprintf(stderr, "error reading viewport height\n");
        return 1;
    }
    if (fscanf(input_file, "%f", &focal_length) != 1) {
        fprintf(stderr, "error reading focal length\n");
        return 1;
    }
    if (fscanf(input_file, "%f %f %f %f", &light_source.position.x, &light_source.position.y, &light_source.position.z, &light_source.brightness) != 4) {
        fprintf(stderr, "error reading light source parameters\n");
        return 1;
    }
    if (fscanf(input_file, "%d", &total_colors) != 1) {
        fprintf(stderr, "error reading total colors\n");
        return 1;
    }

    unsigned int colors[total_colors]; // colors is unused
    readColors(input_file, colors, total_colors);

    #ifdef FS
    readAndSortColors(colors, total_colors);
    #endif

    if (fscanf(input_file, "%d", &background_index) != 1) {
        fprintf(stderr, "error reading background index\n");
        return 1;
    }

    #ifndef FS
    Vec3 backgroundcolor = {0.0,0.0,0.0};

    #elif FS
    Vec3 backgroundcolor = unpackRGB(colors[background_index]);
    #endif

    if (fscanf(input_file, "%d", &total_spheres) != 1) {
        fprintf(stderr, "error reading total spheres\n");
        return 1;
    }

    readSpheres(input_file, total_spheres, &world, colors);
    fclose(input_file);

    Initialize_camera(focal_length, viewport_height, image_width, image_height);
    init_scene(light_source.position, light_source.brightness, light_source);

    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        printf("fail to open file: %s\n", output_path);
        return 1;
    }

    #ifdef MS1
    printVectorOperations(output_file, backgroundcolor, light_source.position, Camera.viewport_width);
    printSphereOperations(output_file, light_source.position, world);

    #elif MS2
    fprintf(output_file, "P3\n%d %d\n255\n", image_width, image_height);
    renderScene(&world, image_width, image_height, output_file, light_source, backgroundcolor);

    #else 
    fprintf(output_file, "P3\n%d %d\n255\n", image_width, image_height);
    anti_aliasing(&world, image_width, image_height, output_file, light_source, backgroundcolor);
    #endif

    fclose(output_file);
    freeWorld(&world);

    return 0;
}