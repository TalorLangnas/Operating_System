#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/**
 * Compresses and encrypts a specified library using the tar, gzip, and gpg utilities.
 *
 * This function takes the name of the library as input, creates a tar archive of the
 * library, compresses it using gzip, and then encrypts the compressed archive using gpg.
 * The resulting encrypted archive is saved as "archive.tar.gz.gpg".
 *
 * @param libraryName The name of the library to compress and encrypt.
 * 
 * @note This function uses the fork-exec pattern to create separate processes for
 *       tar, gzip, and gpg, allowing for parallel execution of these operations.
 *       It uses pipes for inter-process communication.
 *
 * @warning Make sure the required utilities (tar, gzip, gpg) are installed on the system.
 * 
 * @param libraryName The name of the library to compress and encrypt.
 */
void myzip(char *libraryName) {
    int pipefd_gzip[2], pipefd_gpg[2];

    if (pipe(pipefd_gzip) == -1 || pipe(pipefd_gpg) == -1) {
        perror("pipe");
        exit(1);
    }

    if (!fork()) {
        // child 1 - tar
        close(pipefd_gzip[0]); // Close read end of the gzip pipe
        dup2(pipefd_gzip[1], 1); // Redirect stdout to the gzip pipe

        // create command line
        char *arglist[] = {"tar", "-czf", "-", libraryName, NULL};
        execvp("/bin/tar", arglist);
        perror("execvp tar");
        exit(EXIT_FAILURE);
    }

    if (!fork()) {
        // child 2 - gzip
        close(pipefd_gpg[0]); // Close read end of the gpg pipe
        close(pipefd_gzip[1]); // Close write end of the gzip pipe
        dup2(pipefd_gzip[0], 0); // Redirect stdin to the gzip pipe
        dup2(pipefd_gpg[1], 1); // Redirect stdout to the gpg pipe

        // create command line
        char *gzip_arglist[] = {"/bin/gzip", "-f", "-c", "--stdout", NULL};
        execvp("/bin/gzip", gzip_arglist);
        perror("execvp gzip");
        exit(EXIT_FAILURE);
    }

    if (!fork()) {
        // child 3 - gpg
        close(pipefd_gzip[0]); // Close read end of the gzip pipe
        close(pipefd_gzip[1]); // Close write end of the gzip pipe
        close(pipefd_gpg[1]); // Close write end of the gpg pipe
        dup2(pipefd_gpg[0], 0); // Redirect stdin to the gpg pipe

        // create command line
        char *gpg_arglist[] = {"gpg", "-c", "--output", "archive.tar.gz.gpg", NULL};
        execvp("/usr/bin/gpg", gpg_arglist);
        perror("execvp gpg");
        exit(EXIT_FAILURE);
    }

    // Close unnecessary pipe ends in the parent process
    close(pipefd_gzip[0]);
    close(pipefd_gzip[1]);
    close(pipefd_gpg[0]);
    close(pipefd_gpg[1]);

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("Compression and encryption completed. Terminating.\n");
}

/**
 * Decrypts and decompresses a specified encrypted and compressed archive using
 * the gpg, gzip, and tar utilities. The decrypted and decompressed contents
 * are extracted into the current working directory.
 *
 * This function takes the name of the input encrypted and compressed file,
 * decrypts it using gpg, decompresses it using gzip, and finally extracts the
 * contents using tar. The result is the original uncompressed data.
 *
 * @param inputFile The name of the encrypted and compressed input file.
 * 
 * @note This function uses the fork-exec pattern to create separate processes
 *       for gpg, gzip, and tar, allowing for parallel execution of these operations.
 *       It uses pipes for inter-process communication.
 *
 * @warning Make sure the required utilities (gpg, gzip, tar) are installed on the system.
 * 
 * @param inputFile The name of the encrypted and compressed input file.
 */
void myunzip(char *inputFile) {
    int pipefd_one[2], pipefd_two[2];

    if (pipe(pipefd_one) == -1 || pipe(pipefd_two) == -1) {
        perror("pipe");
        exit(1);
    }

    if (!fork()) {
        // Child 1 - gpg
        close(pipefd_one[0]); // Close read end of the pipefd_one
        dup2(pipefd_one[1], 1); // Redirect stdout to the pipefd_one

        // create command line
        char *gpg_arglist[] = {"gpg", "--decrypt", "--output", "-", inputFile, NULL};
        execvp("/usr/bin/gpg", gpg_arglist);
        perror("execvp gpg");
        exit(EXIT_FAILURE);
    }


    if (!fork()) {
        // Child 2 - gzip
        close(pipefd_two[0]); // Close read end of the pipefd_two
        close(pipefd_one[1]); // Close write end of the pipefd_one

        dup2(pipefd_one[0], 0); // Redirect stdin to the pipefd_one
        dup2(pipefd_two[1], 1); // Redirect stdout to the pipefd_two
        // create command line
        char *gzip_arglist[] = {"/bin/gzip", "-d", "--stdout", NULL};
        execvp("/bin/gzip", gzip_arglist);
        perror("execvp gzip");
        exit(EXIT_FAILURE);
    }

    if (!fork()) {
        // Child 3 - tar
        close(pipefd_one[0]); // Close read end of the pipefd_one 
        close(pipefd_one[1]); // Close write end of the pipefd_one 
        close(pipefd_two[1]); // Close write end of the pipefd_two
        
        dup2(pipefd_two[0], 0); // Redirect stdin to the pipefd_two

        // create command line
        char *tar_arglist[] = {"tar", "-xvzf", "-", NULL};
        execvp("/bin/tar", tar_arglist);
        perror("execvp tar");
        exit(EXIT_FAILURE);
    }

    // Close unnecessary pipe ends in the parent process
    close(pipefd_one[0]);
    close(pipefd_one[1]);
    close(pipefd_two[0]);
    close(pipefd_two[1]);

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("Decompression completed. Terminating.\n");
}

/**
 * Main function for executing either the 'myzip' or 'myunzip' functionality based on
 * command-line arguments.
 *
 * This program provides two operations: compressing a library using 'myzip' or
 * decompressing a previously compressed and encrypted archive using 'myunzip'.
 * The desired operation and the library name are specified as command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 *
 * @return 0 on successful execution, EXIT_FAILURE otherwise.
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <myzip or myunzip> <library_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get the desired function to be preforemed
    const char *function = argv[1];
    // Get the library name from the command line
    char *libraryName = argv[2];

    if(strcmp("myzip", function)== 0){
        //Compress the library
        myzip(libraryName);
    }else if(strcmp("myunzip", function)== 0){
        myunzip(libraryName);
    }else{
        fprintf(stderr, "Usage: %s <zip or unzip> <library_name>\n", argv[0]);
    }
    
    return 0;
}