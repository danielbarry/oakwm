#include "menu.hh"

Menu::Menu(int max){
  maxItems = max;
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

void Menu::focusItem(int x, int y){
  /* TODO: Select an element to be focused. */
}

void Menu::selectItem(int x, int y){
  /* TODO: Select a specific element. */
  system(items[0].cmd.c_str()); // TODO
}

void Menu::draw(Display* dis){
  /* TODO: Draw the window. */
}
