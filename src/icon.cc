#include "icon.hh"

#include <fstream>
#include <stdlib.h>

#include "log.hh"

Icon::Icon(
  std::string path,
  std::string name,
  int x,
  int y,
  bool inter,
  Menu* menu,
  Display* dis,
  Window win,
  GC gc
){
  nameId = name;
  destX = x;
  destY = y;
  interact = inter;
  drop = menu;
  width = -1;
  height = -1;
  focus = false;
  active = false;
  /* Read pixels out of the file */
  std::ifstream file(path);
  std::string fmt;
  int depth = -1;
  int z = 0;
  std::string line;
  while(std::getline(file, line)){
    int s = -1;
    int e = -1;
    if(!line.empty() && !fmt.empty() && width >= 0 && height >= 0 && depth >= 0){
      if(line.size() != width * height * 3){
        WARN("Bad data section detected");
      }
      for(int x = 0; x < line.size(); x += 3){
        /* NOTE: X11 stores BGR. */
        data[0][z++] = line[x + 2];
        data[0][z++] = line[x + 1];
        data[0][z++] = line[x    ];
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
          data.push_back((char*)malloc(width * height * 4));
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
  if(width <= 0 || height <= 0){
    WARN("Incorrect image dimensions, icon may be corrupt");
  }
  if(depth != 255){
    WARN("Incorrect image bit depth, icon may be corrupt");
  }
  /* Generate image */
  imgs.push_back(
    XCreateImage(
      dis,
      DefaultVisual(dis, DefaultScreen(dis)),
      DefaultDepth(dis, DefaultScreen(dis)),
      ZPixmap,
      0,
      data[0],
      width,
      height,
      32,
      0
    )
  );
}

Icon::~Icon(){
  for(int x = 0; x < imgs.size(); x++){
    free(data[x]);
    XFree(imgs[x]);
  }
  data.clear();
  imgs.clear();
}

void Icon::addModifier(std::string name, unsigned long mask, Display* dis){
  /* Check if this icon is even interactive */
  if(!interact){
    /* Point modifier to default location encase it gets called */
    mods[name] = 0;
    return;
  }
  /* Clone from original and apply mask */
  data.push_back((char*)malloc(width * height * 4));
  char mask0 = ~((mask >> 24) & 0xFF);
  char mask1 = ~((mask >> 16) & 0xFF);
  char mask2 = ~((mask >>  8) & 0xFF);
  char mask3 = ~((mask      ) & 0xFF);
  int off = data.size() - 1;
  mods[name] = off;
  int size = width * height * 4;
  for(int x = 0; x < size; x += 4){
    data[off][x    ] = ~(~data[0][x    ] & mask3);
    data[off][x + 1] = ~(~data[0][x + 1] & mask2);
    data[off][x + 2] = ~(~data[0][x + 2] & mask1);
    data[off][x + 3] = ~(~data[0][x + 3] & mask0);
  }
  /* Create image */
  imgs.push_back(
    XCreateImage(
      dis,
      DefaultVisual(dis, DefaultScreen(dis)),
      DefaultDepth(dis, DefaultScreen(dis)),
      ZPixmap,
      0,
      data[data.size() - 1],
      width,
      height,
      32,
      0
    )
  );
}

void Icon::setXY(int x, int y){
  destX = x;
  destY = y;
}

std::string Icon::getName(){
  return nameId;
}

Menu* Icon::getMenu(){
  return drop;
}

int Icon::getWidth(){
  return width;
}

int Icon::getHeight(){
  return height;
}

bool Icon::interactive(){
  return interact;
}

bool Icon::insideBounds(int x, int y){
  return x >= destX && x < destX + width;
}

void Icon::setFocused(bool state, int x, int y){
  active = false;
  focus = state;
  drop->setState(focus, active, x, y);
}

void Icon::setActive(bool state, int x, int y){
  focus = false;
  active = state;
  drop->setState(focus, active, x, y);
}

void Icon::draw(Display* dis, Window win, GC gc){
  if(focus || active){
    if(focus){
      XPutImage(dis, win, gc, imgs[mods["focus"]], 0, 0, destX, destY, width, height);
    }else{
      XPutImage(dis, win, gc, imgs[mods["active"]], 0, 0, destX, destY, width, height);
    }
  }else{
    XPutImage(dis, win, gc, imgs[0], 0, 0, destX, destY, width, height);
  }
  /* Allow menu to redraw too */
  drop->draw(dis);
}
