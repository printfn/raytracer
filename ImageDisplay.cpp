#include "ImageDisplay.h"
#include <memory>

ImageDisplay::ImageDisplay(int width, int height) :
        image_(width, height, 1, 3, 0), width_(width), height_(height) {

}

ImageDisplay::~ImageDisplay() {
    if (display_ && !display_->is_closed()) {
        display_->close();
    }
}

void ImageDisplay::set(int x, int y, const Colour &colour) {
    auto srgbValues = colour.srgb_gamma_correction();
    image_(x, y, 0, 0) = int(srgbValues[0]);
    image_(x, y, 0, 1) = int(srgbValues[1]);
    image_(x, y, 0, 2) = int(srgbValues[2]);
}

void ImageDisplay::refresh() {
    if (!display_) {
        display_ = std::make_unique<cimg_library::CImgDisplay>(width_, height_, "Render");
    }
    display_->display(image_);
    display_->wait(1);
}

void ImageDisplay::save(const std::string &filename) const {
    image_.save_png(filename.c_str());
}

void ImageDisplay::pause(double seconds) {
    if (display_) {
        display_->wait(int(seconds * 1000));
    }
}
