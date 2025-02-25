// File: src/assg.c
// Name- Rakshit Singh

#include "spheres.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// initializes the world structure with capacity of 1
void worldInit(World *world) {
    // initial size to 0
    world->size = 0;
    // initial capacity to 1
    world->capacity = 1;
    // allocate memory 
    world->spheres = (Sphere **)malloc(sizeof(Sphere *) * world->capacity);
    // memory allocation failed
    if (!world->spheres) {
        printf("failed to allocate memory for spheres\n");
        // return on error
        return;
    }
}

// frees all allocated memory for the spheres in the world 
void freeWorld(World *world) {
    // if spheres array is not null
    if (world->spheres != NULL) {
        // loop through each sphere
        for (int i = 0; i < world->size; i++) {
            // if sphere is not null
            if (world->spheres[i] != NULL) {
                //free sphere
                free(world->spheres[i]);
                // pointer to null
                world->spheres[i] = NULL;
            }
        }
        // free the array of pointers
        free(world->spheres);
        // set spheres array to nuull pointer
        world->spheres = NULL;
    }
    // size to 0
    world->size = 0;
    // capacity to 0
    world->capacity = 0;
}

// adds a sphere to the world
void addSphere(World *world, Sphere *sphere) {
    // check if we need to resize the array
    if (world->size >= world->capacity) {
        // capacity x 2
        int newCapacity = world->capacity * 2;
        // reallocate memory
        Sphere **newSpheres = (Sphere **)realloc(world->spheres, sizeof(Sphere *) * newCapacity);
        // memory reallocation failed
        if (!newSpheres) {
            // print error message
            printf("failed to reallocate memory for spheres\n");
            // return on error
            return;
        }
        // array to new memory location
        world->spheres = newSpheres;
        // update capacity
        world->capacity = newCapacity;
    }
    // add sphere to array and increment size
    world->spheres[world->size++] = sphere;
}

// creates a new sphere
Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    // memory for new sphere
    Sphere *sphere = (Sphere *)malloc(sizeof(Sphere));
    // check if memory allocation failed
    if (!sphere) {
        // print error message
        printf("failed to allocate memory for sphere\n");
        // return null on failure
        return NULL;
    }
    // set sphere radius
    sphere->r = radius;
    // set sphere position
    sphere->pos = position;
    // set sphere color
    sphere->color = color;
    // return the created sphere
    return sphere;
}

// Function to check if a ray intersects with a sphere
int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
    // vector from the ray position to the sphere center
    Vec3 V = subtract(rayPos, sphere->pos);

    // coefficients of the quadratic equation
    float a = dot(rayDir, rayDir); // rayDir . rayDir
    float b = 2 * dot(rayDir, V);  // 2 * (rayDir . V)
    float c = dot(V, V) - sphere->r * sphere->r; // V . V - r^2

    // discriminant value
    float discriminant = b * b - 4 * a * c;

    // discriminant is negative, no intersection
    if (discriminant < 0) {
        return 0; 
    }

    // calculate the two possible solutions for t
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2 * a);
    float t2 = (-b + sqrtDiscriminant) / (2 * a);

    // intersections are in front of the ray
    if (t1 >= 0 && t2 >= 0) {
        *t = (t1 < t2) ? t1 : t2; // closer intersection comparision
        return 1; // both front of the ray
    } else if (t1 >= 0) {
        *t = t1; // t1 is in front
        return 1;
    } else if (t2 >= 0) {
        *t = t2; // t2 is in front 
        return 1;
    }

    // both behind the ray
    return 0;
}