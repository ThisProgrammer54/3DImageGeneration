#include "render.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"
#include "screen.hpp"

using namespace cv;

Render::Render(double width, double height) : width(width), height(height) {

  img = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 100));
  if (img.empty()) {
    std::cout << "\n Image not created. You"
                 " have done something wrong. \n";
  }
};
void Render::render(Screen &screen) {
  // check sizes
  int realwidth = screen.width * screen.dpi;
  int realheight = screen.height * screen.dpi;

  if (realwidth != this->width || realheight != this->height) {
    std::cerr << "sizes of screen and rendered screen dont match" << std::endl;
    return;
  }

  for (int y = 0; y < this->height; y++) {
    for (int x = 0; x < this->width; x++) {
      Vec3b &color = img.at<Vec3b>(y, x);
      RGB &newColor = screen.values[height - y - 1][x].value;

      color[0] = newColor.blue * 255;
      color[1] = newColor.green * 255;
      color[2] = newColor.red * 255;
    }
  }
}
void Render::show() const {
  namedWindow("A_good_name");

  imshow("A_good_name", this->img);

  waitKey(0);

  destroyWindow("A_good_name");
}
