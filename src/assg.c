// File: src/assg.c
// Name- Rakshit Singh
// Student ID- 1824303
// Used resources - Given Course Book,Lecture Slides and Lab-3,4,5 concept with Basic C programming concepts 
// Used resources(links{only for concept clarity}) - https://eclass.srv.ualberta.ca/mod/page/view.php?id=8249101
//                                                   https://stackoverflow.com/questions/2018730/fprintf-with-string-argument
//                                                   https://stackoverflow.com/questions/3351809/reading-file-using-fscanf-in-c
//                                                   https://stackoverflow.com/questions/22156494/getting-a-memory-leak-from-dynamically-allocated-arrays
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "spheres.h"
#include "vector.h"

// Structures for vectors and rays
typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

// Light source structure
typedef struct {
    Vec3 position;
    float brightness;
} Light;

// Global camera and viewport parameters
struct {
    Vec3 position;
    float focal_length;
    float viewport_height;
    float viewport_width;
    float x_value_pixel;  // x between pixel
    float y_value_pixel;  // y between pixel
    Vec3 first_pixel;
} Camera;

// Global scene variables
Light globallight;
Vec3 backgroundcolor = {0, 0, 0}; // Initialize backgroundcolor

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize camera and viewport
void Initialize_camera(float focal_length, float viewport_height, int image_width, int image_height) {
    // set camera position to 0,0,0
    Camera.position.x = 0;
    Camera.position.y = 0;
    Camera.position.z = 0;

    // set focal length and viewport wigth and height
    Camera.focal_length = focal_length;
    Camera.viewport_height = viewport_height;
    Camera.viewport_width = viewport_height * ((float)image_width / image_height);

    // calculate pixel size in x and y direction
    Camera.x_value_pixel = Camera.viewport_width / image_width;
    Camera.y_value_pixel = Camera.viewport_height / image_height;

    // calculate viewport first pixel position
    Camera.first_pixel.x = -Camera.viewport_width / 2;
    Camera.first_pixel.y = Camera.viewport_height / 2;
    Camera.first_pixel.z = -Camera.focal_length;
}

// generate ray for a pixel
Ray Ray_generation(int pixel_x_cords, int pixel_y_cords, int image_width, int image_height) {
    Ray ray;
    ray.origin = Camera.position;
    
    // calculate pixel center in viewport coordinates
    float pixel_center_x = Camera.first_pixel.x + (pixel_x_cords + 0.5) * Camera.x_value_pixel;
    float pixel_center_y = Camera.first_pixel.y - (pixel_y_cords + 0.5) * Camera.y_value_pixel;
    float pixel_center_z = Camera.first_pixel.z;
    
    // calculate ray direction (from camera to pixel center)
    Vec3 pixel_center;
    pixel_center.x = pixel_center_x;
    pixel_center.y = pixel_center_y;
    pixel_center.z = pixel_center_z;
    
    // subtract camera position from pixel center to get direction
    ray.direction = subtract(pixel_center, ray.origin);
    
    // normalize the direction vector
    ray.direction = normalize(ray.direction);
    
    // return the generated ray
    return ray;
}

// scene initialization
void init_scene(Vec3 light_vector, float light_intensity) {
    // set global light position
    globallight.position = light_vector;
    
    // set global light brightness
    globallight.brightness = light_intensity;
}

// light intensity at a point
float calculate_light_intensity(Vec3 point) {
    // calculate direction from point to light
    Vec3 light_dir = subtract(globallight.position, point);
    
    // calculate distance to light
    float distance = length(light_dir);
    
    // normalize the light direction
    light_dir = normalize(light_dir);
    
    // calculate intensity
    return globallight.brightness / (distance * distance);
}

// function to read colors from file
void readColors(FILE *input_file, unsigned int *colors, int total_colors) {
    for (int i = 0; i < total_colors; i++) {
        // read color value from file
        fscanf(input_file, "%x", &colors[i]);
    }
}

// function to read spheres from file and add to the world
void readSpheres(FILE *input_file, int total_spheres, World *world) {
    for (int i = 0; i < total_spheres; i++) {
        Vec3 position;
        float radius;
        int color_index;
        fscanf(input_file, "%f %f %f %f %d", &position.x, &position.y, &position.z, &radius, &color_index);

        // use white color (1, 1, 1) we dont need antything for milestone 1 
        Sphere *sphere = createSphere(radius, position, (Vec3){1, 1, 1});
        addSphere(world, sphere);
    }
}

void printVectorOperations(FILE *output_file, Vec3 backgroundcolor, Vec3 globallight_position, float viewport_width) {
    Vec3 add_result = add(backgroundcolor, globallight_position);
    Vec3 sub_result = subtract(backgroundcolor, globallight_position);
    Vec3 scalar_result = scalarMultiply(viewport_width, globallight_position);
    Vec3 norm_result = normalize(globallight_position);

    // Print vector operations
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

void printSphereOperations(FILE *output_file, Vec3 globallight_position, World world) {
    for (int i = 0; i < world.size; i++) {
        Sphere *sphere = world.spheres[i];
        Vec3 div_result = scalarDivide(sphere->color, sphere->r);
        float dot_result = dot(globallight_position, sphere->pos);
        float dist_result = distance(globallight_position, sphere->pos);
        float length_result = length(sphere->pos);

        // Print sphere operations
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }

    // Read input and output file paths
    const char *input_path = argv[1];
    const char *output_path = argv[2];

    // Open input file
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        printf("Error! Could not open file\n");
        return 1;
    }

// Read input file
    int image_width;
    int image_height;
    int total_colors;
    int background_index;
    int total_spheres;

    float viewport_height;
    float focal_length;

    Light light_source;
    World world;    
    worldInit(&world);

    // Read input file
    fscanf(input_file, "%d %d", &image_width, &image_height);
    fscanf(input_file, "%f", &viewport_height);
    fscanf(input_file, "%f", &focal_length);
    fscanf(input_file, "%f %f %f %f", &light_source.position.x, &light_source.position.y, &light_source.position.z, &light_source.brightness);
    fscanf(input_file, "%d", &total_colors);

    // Array for colors in the input file
    unsigned int colors[total_colors];
    readColors(input_file, colors, total_colors);

    // Read background color
    fscanf(input_file, "%d", &background_index);
    fscanf(input_file, "%d", &total_spheres);

    // Read spheres
    readSpheres(input_file, total_spheres, &world);

    fclose(input_file);

    // Initialize camera and scene
    Initialize_camera(focal_length, viewport_height, image_width, image_height);
    init_scene(light_source.position, light_source.brightness);
    
    // Open output file
    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        printf("Failed to open output file: %s\n", output_path);
        return 1;
    }

    // Output for Milestone 1
    #ifdef MS1
    printVectorOperations(output_file, backgroundcolor, globallight.position, Camera.viewport_width);
    printSphereOperations(output_file, globallight.position, world);
    #endif

    fclose(output_file);

    // Free the allocated memory for spheres
    freeWorld(&world);

    return 0;
}