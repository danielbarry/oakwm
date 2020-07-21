#include "manager.cc"

/**
 * main()
 *
 * The main entry point into the program. If an invalid command line option
 * found, exit, otherwise start the X11 window manager.
 *
 * @param arc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return The return code of the program.
 **/
int main(int argc, char **argv){
  int i;
  /* If we have command line arguments, display an appropriate error */
  if(argc > 1){
    for(i = strlen(argv[0])-1; i > 0 && argv[0][i] != '/'; --i);
    fprintf(stderr, "usage: %s\n", argv[0] + (i > 0) + i);
    exit(2);
  }
  /* Start window manager */
  WindowManager manager;
  /* Exit success */
  return 0;
}
