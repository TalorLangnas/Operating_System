#include "pythagorean_triangle.h"
#include <stdio.h>

int main() {
    int a, b, c;

    // Input side lengths from the user
    printf("Enter the three side lengths of the triangle: ");
    scanf("%d %d %d", &a, &b, &c);

    // Call the pythagorean_triangle function with user input
    pythagorean_triangle(a, b, c);

    return 0;
}