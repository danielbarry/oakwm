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

    /**
     * readPPM()
     *
     * Read the PPM file from disk into an array.
     *
     * @param path The path of the image PPM file.
     * @param width The width of the icon.
     * @param height The height of the icon.
     * @return A pointer to the pixel buffer.
     **/
    static char* readPPM(std::string path, int *width, int *height){
      char* data;
      std::ifstream file(path);
      std::string fmt;
      int depth = -1;
      int z = 0;
      std::string line;
      while(std::getline(file, line)){
        int s = -1;
        int e = -1;
        if(!line.empty() && !fmt.empty() && *width >= 0 && *height >= 0 && depth >= 0){
          if(line.size() != *width * *height * 3){
            WARN("Bad data section detected");
          }
          for(int x = 0; x < line.size(); x += 3){
            /* NOTE: X11 stores BGR. */
            data[z++] = line[x + 2];
            data[z++] = line[x + 1];
            data[z++] = line[x    ];
            z++;
          }
          break;
        }
        for(int x = 0; x < line.size(); x++){
          char c = line[x];
          /* Check for comment marker */
          if(c == '#'){
            break;
          }
          /* Find next markers */
          if(s < 0){
            if(c != ' ' && c != '\t'){
              s = x;
            }
          }else{
            if(c == ' ' || c == '\t'){
              e = x;
            }
          }
          /* If we have values, use them */
          if(s >= 0 && (e >= 0 || x + 1 == line.size())){
            /* Pull out word */
            std::string word = line.substr(s, e - s);
            /* Reset markers */
            s = -1;
            e = -1;
            /* Find place */
            if(fmt.empty()){
              fmt = word;
              continue;
            }
            int num = std::atoi(word.c_str());
            if(*width < 0){
              *width = num;
              continue;
            }
            if(*height < 0){
              *height = num;
              continue;
            }
            if(depth < 0){
              depth = num;
              data = (char*)malloc(*width * *height * 4);
              continue;
            }
          }
        }
      }
      /* Close the file to reserve resources */
      file.close();
      /* Sanity check input */
      if(fmt.compare("P6") != 0){
        WARN("Incorrect image format, icon may be corrupt");
      }
      if(*width <= 0 || *height <= 0){
        WARN("Incorrect image dimensions, icon may be corrupt");
      }
      if(depth != 255){
        WARN("Incorrect image bit depth, icon may be corrupt");
      }
      return data;
    }

    /**
     * getPowerState()
     *
     * Get the current state of the system power.
     *
     * @param batt The location of the battery status.
     * @param dc The location of the DC status.
     * @return A positive integer between 0 and 100 if on battery, otherwise a
     * negative integer indicating the battery status on DC (assumed to be
     * charging).
     **/
    static int getPowerState(std::string batt, std::string dc){
      int power = 0;
      std::string read;
      /* Read battery value */
      std::ifstream fBatt(batt);
      std::getline(fBatt, read);
      fBatt.close();
      power = std::atoi(read.c_str());
      /* Read DC value */
      std::ifstream fDC(dc);
      std::getline(fDC, read);
      fDC.close();
      return read.compare("0") == 0 ? power : -power;
    }
};
