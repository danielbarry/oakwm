#include "icon.hh"

#include <fstream>
#include <stdlib.h>

#include "log.hh"

Icon::Icon(std::string path, int x, int y, Display* dis, Window win, GC gc){
  destX = x;
  destY = y;
  width = -1;
  height = -1;
  img = NULL;
  /* Read pixels out of the file */
  std::ifstream file(path);
  std::string fmt;
  int depth = -1;
  int z = 0;
  std::string line;
  while(std::getline(file, line)){
    int s = -1;
    int e = -1;
    if(!line.empty() && !fmt.empty() && width >= 0 && height >=0 && depth >= 0){
      if(line.size() != width * height * 3){
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
        if(width < 0){
          width = num;
          continue;
        }
        if(height < 0){
          height = num;
          continue;
        }
        if(depth < 0){
          depth = num;
          data = (char*)malloc(width * height * 4);
          continue;
        }
      }
    }
  }
  /* Sanity check input */
  if(fmt.compare("P6") != 0){
    WARN("Incorrect image format, icon may be corrupt");
  }
  if(width <= 0 || height <= 0){
    WARN("Incorrect image dimensions, icon may be corrupt");
  }
  if(depth != 255){
    WARN("Incorrect image bit depth, icon may be corrupt");
  }
  /* Generate image */
  img = XCreateImage(
    dis,
    DefaultVisual(dis, DefaultScreen(dis)),
    DefaultDepth(dis, DefaultScreen(dis)),
    ZPixmap,
    0,
    data,
    width,
    height,
    32,
    0
  );
}

Icon::~Icon(){
  free(data);
  XFree(img);
}

void Icon::draw(Display* dis, Window win, GC gc){
  XPutImage(dis, win, gc, img, 0, 0, destX, destY, width, height);
}
