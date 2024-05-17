#include <stdio.h>
#include <math.h>
#include "pythagorean_triangle.h"
// Define PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/**
 * Checks if the given side lengths form a Pythagorean triangle and calculates
 * the corresponding angles in radians if it is valid.
 *
 * This function takes three side lengths (a, b, and c) as input and checks if
 * they form a Pythagorean triangle. If the triangle is valid, it calculates the
 * angles in radians using the atan2 function and prints them. If the input is
 * invalid (e.g., non-positive side lengths), appropriate error messages are displayed.
 *
 * @param a The length of side 'a'.
 * @param b The length of side 'b'.
 * @param c The length of side 'c'.
 *
 * @note This function assumes that the inputs represent the sides of a triangle.
 *       It uses the atan2 function to calculate angles in radians.
 *
 * @warning The function may produce inaccurate results for very large values of side lengths.
 *
 * @see atan2
 * @see M_PI
 */
void pythagorean_triangle(int a, int b, int c) {
    if (a > 0 && b > 0 && c > 0) {
        if ((a * a + b * b == c * c) || (a * a + c * c == b * b) || (b * b + c * c == a * a)) {
            // Calculate angles in radians using atan2 function
            double pi_value = M_PI;

            double angleA = atan2(a, b);
            double angleB = atan2(b, a);
            // Since this is a right-angled triangle, angle CC is 90 degrees.
            double angleC = (M_PI * 90)/180;

            // Print angles in radians
            printf("Angles in radians: %lf %lf %lf\n", angleA, angleB, angleC);
        } else {
            // Not a Pythagorean triangle
            printf("Error\n");
            return;
        }
    } else {
        // Invalid input, side lengths should be positive
        printf("Error: Side lengths should be positive\n");
        return;
    }
}

