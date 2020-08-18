#include "icon.hh"

#include <fstream>
#include <stdlib.h>

#include "log.hh"
#include "util.hh"

Icon::Icon(
  JSON* cfg,
  JSON* iCfg,
  XFontStruct* font,
  Display* dis,
  int screen,
  Window win,
  GC gc
){
  nameId = iCfg->get("name")->value("");
  destX = 0;
  destY = 0;
  interact = iCfg->get("interactive")->value("false").compare("true") == 0;
  drop = new Menu(cfg, iCfg, font, dis, screen);
  width = -1;
  height = -1;
  focus = false;
  active = false;
  /* Read pixels out of the file */
  data.push_back(Util::readPPM(iCfg->get("image")->value(""), &width, &height));
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
  /* Apply icon modifiers */
  for(int x = 0; x < cfg->get("modifiers")->length(); x++){
    JSON* mCfg = cfg->get("modifiers")->get(x);
    std::string name = mCfg->get("name")->value("");
    unsigned long mask = Util::strToLong(mCfg->get("mask")->value("").c_str(), 16);
    /* Update our icon with modifiers */
    addModifier(name, mask, dis);
  }
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

bool Icon::setFocused(bool state, int x, int y){
  bool change = false;
  /* Update active state */
  bool prev = active;
  active = false;
  change |= prev != active;
  /* Update focus state */
  prev = focus;
  focus = state;
  change |= prev != focus;
  /* Update icon with new states */
  change |= drop->setState(focus, active, x, y);
  /* Inform the caller whether a redraw is required */
  return change;
}

bool Icon::setActive(bool state, int x, int y){
  bool change = false;
  /* Update focus state */
  bool prev = focus;
  focus = true;
  change |= prev != focus;
  /* Update active state */
  prev = active;
  active = state;
  change |= prev != active;
  /* Update icon with new states */
  change |= drop->setState(focus, active, x, y);
  /* Inform the caller whether a redraw is required */
  return change;
}

void Icon::draw(Display* dis, Window win, GC gc){
  if(focus || active){
    if(active){
      XPutImage(dis, win, gc, imgs[mods["active"]], 0, 0, destX, destY, width, height);
    }else{
      XPutImage(dis, win, gc, imgs[mods["focus"]], 0, 0, destX, destY, width, height);
    }
  }else{
    XPutImage(dis, win, gc, imgs[0], 0, 0, destX, destY, width, height);
  }
  /* Allow menu to redraw too */
  drop->draw(dis);
}
