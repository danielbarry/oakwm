#pragma once

/**
 * util.hh
 *
 * Some utility functions.
 **/
class Util{
  public:
    /**
     * strToLong()
     *
     * Convert a string to long with a given base. NOTE: This function has only
     * been tested up to base 16.
     *
     * @param s The string to be converted.
     * @param b The base to be converted.
     * @return The converted value.
     **/
    static long strToLong(const char* s, int b){
      long v = 0;
      int x = -1;
      /* Check for minus number on front */
      bool m = s[x + 1] == '-';
      if(m){
        ++x;
      }
      /* Add numbers together */
      while(s[++x] != '\0'){
        v *= b;
        if(s[x] <= '9'){
          v += s[x] - '0';
        }else{
          v += (0b1011111 & s[x]) - 'A' + 10;
        }
      }
      return m ? -v : v;
    }
};