#include "ImageDisplay.h"
#include <memory>

ImageDisplay::ImageDisplay(int width, int height) :
        imageBuffer_(width * height * 3), width_(width), height_(height) {

    image_ = cimg_library::CImg<uint8_t>(imageBuffer_.data(), width, height, 1, 3, true);
}

ImageDisplay::~ImageDisplay() {
    if (display_ && !display_->is_closed()) {
        display_->close();
    }
}

void ImageDisplay::set(int x, int y, const Colour &colour) {
    std::shared_lock lock{imageMutex_};
    auto srgbValues = colour.srgb_gamma_correction();
    imageBuffer_[y * width_ + x] = srgbValues[0];
    imageBuffer_[y * width_ + x + width_ * height_] = srgbValues[1];
    imageBuffer_[y * width_ + x + 2 * width_ * height_] = srgbValues[2];
}

void ImageDisplay::refresh() {
    std::unique_lock lock{imageMutex_};
    if (!display_) {
        display_ = std::make_unique<cimg_library::CImgDisplay>(width_, height_, "Render");
    }
    display_->display(image_);
    display_->wait(1);
}

void ImageDisplay::save(const std::string &filename) const {
    image_.save_jxl(filename.c_str(), 0.0);
}

void ImageDisplay::pause(double seconds) {
    if (display_) {
        display_->wait(int(seconds * 1000));
    }
}
