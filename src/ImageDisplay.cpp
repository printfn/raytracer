#include "ImageDisplay.h"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <jxl/encode_cxx.h>
#include <memory>
#if USE_GUI
#include <mutex>
#endif

ImageDisplay::ImageDisplay(int width, int height) :
        imageBuffer_(width * height * 3), width_(width), height_(height) {
#if USE_GUI
    image_ = cimg_library::CImg<uint8_t>(imageBuffer_.data(), width, height, 1, 3, true);
#endif
}

ImageDisplay::~ImageDisplay() {
#if USE_GUI
    if (display_ && !display_->is_closed()) {
        display_->close();
    }
#endif
}

void ImageDisplay::set(int x, int y, const Colour &colour) {
    std::shared_lock lock{imageMutex_};
    auto srgbValues = colour.srgb_gamma_correction();
    imageBuffer_[y * width_ + x] = srgbValues[0];
    imageBuffer_[y * width_ + x + width_ * height_] = srgbValues[1];
    imageBuffer_[y * width_ + x + 2 * width_ * height_] = srgbValues[2];
}

void ImageDisplay::refresh() {
#if USE_GUI
    std::unique_lock lock{imageMutex_};
    if (!display_) {
        display_ = std::make_unique<cimg_library::CImgDisplay>(width_, height_, "Render");
    }
    display_->display(image_);
    display_->wait(1);
#endif
}

void ImageDisplay::save(const std::string &filename) const {
    std::vector<uint8_t> interleavedChannelBuffer(imageBuffer_.size());
    for (int x = 0; x < width_; ++x) {
        for (int y = 0; y < height_; ++y) {
            interleavedChannelBuffer[(x + y * width_) * 3] = imageBuffer_[y * width_ + x];
            interleavedChannelBuffer[(x + y * width_) * 3 + 1] = imageBuffer_[y * width_ + x + width_ * height_];
            interleavedChannelBuffer[(x + y * width_) * 3 + 2] = imageBuffer_[y * width_ + x + 2 * width_ * height_];
        }
    }
    auto encoder = JxlEncoderMake(nullptr);
    JxlBasicInfo basicInfo;
    JxlEncoderInitBasicInfo(&basicInfo);
    JxlPixelFormat pixelFormat = {
        3,
        JXL_TYPE_UINT8,
        JXL_LITTLE_ENDIAN,
        0,
    };
    basicInfo.xsize = width_;
    basicInfo.ysize = height_;
    basicInfo.num_color_channels = 3;
    basicInfo.bits_per_sample = 8;
    basicInfo.uses_original_profile = JXL_TRUE;

    if (JxlEncoderSetBasicInfo(encoder.get(), &basicInfo) != JXL_ENC_SUCCESS) {
        std::cerr << "failed to set basic info when saving JXL file" << std::endl;
        return;
    }
    JxlColorEncoding colorEncoding = {};
    JxlColorEncodingSetToSRGB(&colorEncoding, JXL_FALSE);
    if (JxlEncoderSetColorEncoding(encoder.get(), &colorEncoding) != JXL_ENC_SUCCESS) {
        std::cerr << "failed to set colour encoding when saving JXL file" << std::endl;
        return;
    }
    JxlEncoderFrameSettings* frameSettings = JxlEncoderFrameSettingsCreate(encoder.get(), nullptr);
    if (JxlEncoderSetFrameLossless(frameSettings, JXL_TRUE) != JXL_ENC_SUCCESS) {
        std::cerr << "failed to set frame to lossless when saving JXL file" << std::endl;
        return;
    }
    if (JxlEncoderAddImageFrame(frameSettings, &pixelFormat, interleavedChannelBuffer.data(), interleavedChannelBuffer.size()) != JXL_ENC_SUCCESS) {
        std::cerr << "failed to add image frame to JXL file" << std::endl;
        return;
    }
    JxlEncoderCloseInput(encoder.get());

    JxlEncoderStatus processResult = JXL_ENC_NEED_MORE_OUTPUT;
    std::vector<uint8_t> outputData(256);
    uint8_t *nextOut = outputData.data();
    while (processResult == JXL_ENC_NEED_MORE_OUTPUT) {
        size_t availOut = outputData.size() - (nextOut - outputData.data());
        processResult = JxlEncoderProcessOutput(encoder.get(), &nextOut, &availOut);
        if (processResult == JXL_ENC_NEED_MORE_OUTPUT) {
            size_t offset = nextOut - outputData.data();
            outputData.resize(outputData.size() * 2, 0);
            nextOut = outputData.data() + offset;
        }
    }
    if (processResult != JXL_ENC_SUCCESS) {
        std::cerr << "failed to encode image when saving JXL file" << std::endl;
        return;
    }
    std::ofstream file{filename, std::ios::binary};
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << ": " << strerror(errno) << std::endl;
        exit(-1);
    }
    file.write(reinterpret_cast<char*>(outputData.data()), nextOut - outputData.data());
    file.close();
}

void ImageDisplay::pause(double seconds) {
#if USE_GUI
    if (display_) {
        display_->wait(int(seconds * 1000));
    }
#else
    (void)seconds;
#endif
}
