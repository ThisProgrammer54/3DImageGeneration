#include <screen.hpp>


Pixel::Pixel(int x, int y, RGB value, Vector4D visPoint) : x(x), y(y) , value(value), visPoint(visPoint) {};

Screen::Screen(int heigth, int width, int dpi, Vector4D observer, Vector4D position) : heigth(heigth), width(width), dpi(dpi), observer(observer), position(position), values(heigth, vector<Pixel>(width)) {};
