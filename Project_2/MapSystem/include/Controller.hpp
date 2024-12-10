#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Model.hpp"
#include "View.hpp"
#include "GlobalFlags.hpp"

class Controller{
private:
  Model model;
  View view;
public:
  Controller(string path = "./resources/edge.txt"):model(path),view(model){}

  void showGUI(){
    view.show();
  }
};


#endif 
