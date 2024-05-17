#include <stdio.h>

/**
 * Demonstrates an example of infinite recursion.
 *
 * This program contains a function, infinite_recursion, which recursively calls itself
 * indefinitely. The main function initiates this recursion, leading to a stack overflow
 * and likely causing the program to terminate due to a stack exhaustion error.
 *
 * @note Infinite recursion without a base case can result in a stack overflow and
 *       unintended program termination.
 *
 * @warning Running this program may lead to undefined behavior, as it lacks a base case
 *          for terminating the recursion.
 */
void infinite_recursion() {
   infinite_recursion();
}
/**
 * Entry point of the program.
 *
 * Initiates the infinite recursion by calling the infinite_recursion function.
 * The program is likely to terminate due to a stack overflow.
 *
 * @return 0 on successful execution (although this may not be reached).
 */
int main() {
    infinite_recursion();
    return 0;
}
