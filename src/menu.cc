#include "menu.hh"

Menu::Menu(int max, Display* display, int disScreen){
  maxItems = max;
  Display* dis = display;
  int screen = disScreen;
  Window win = NULL;
  GC gc = NULL;
}

Menu::~Menu(){
  /* Do nothing */
}

bool Menu::addItem(std::string text, std::string cmd){
  if(items.size() + 1 >= maxItems){
    return false;
  }
  items.emplace_back(Item());
  items[items.size() - 1].text = text;
  items[items.size() - 1].cmd = cmd;
  return true;
}

void Menu::setState(bool fState, bool aState, int x, int y){
  focus = fState;
  active = aState;
}

void Menu::draw(Display* dis){
  /* TODO: Draw the window. */
}
