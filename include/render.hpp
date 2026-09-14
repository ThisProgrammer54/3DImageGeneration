#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "screen.hpp"

class Render {
  cv::Mat img;
  double width;
  double height;

public:
  Render(double width, double height);
  void render(Screen &screen);
  void show() const;
};
