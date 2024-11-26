#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Model.hpp"
#include "View.hpp"

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


#endif /* D5C014FA_110A_46B2_A627_E0FB82341F86 */
