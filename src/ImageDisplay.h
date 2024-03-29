#pragma once

#ifndef IMAGE_DISPLAY_H_INCLUDED
#define IMAGE_DISPLAY_H_INCLUDED

#include "Colour.h"
#include "NonCopyable.h"

#include <CImg.h>

#include <string>
#include <memory>

/**
 * \brief Class to display and save images.
 *
 * The ImageDisplay class provides a simple method to make images, show them in
 * a window, and save them to a file. It is a wrapper around some <a href="http://cimg.eu/">CImg library</a>
 * functions that provides an abstraction that gives just enough functionality
 * for the Ray Tracer.
 *
 * Note that ImageDisplay is NonCopyable, so does not have a copy constructor or
 * assignment operator available. You can, however, create multiple displays.
 */
class ImageDisplay : private NonCopyable {
public:

    /**
     * \brief ImageDisplay constructor.
     *
     * A ImageDisplay is an image associated with a window where it can be shown.
     * Multiple displays can be created, and should normally have different window
     * names, but this is not enforced. Two ImageDisplay objects with the same window
     * name will function correctly, and will store distinct images. It just won't 
     * be possible to view them both at the same time.
     *
     * \param width The width, in pixels, of the image associated with this ImageDisplay.
     * \param height The height, in pixels, of the image associated with this ImageDisplay.
     */
    ImageDisplay(int width, int height);

    ~ImageDisplay();

    /**
     * \brief Set a pixel value.
     *
     * Set a pixel value to the specified Colour. Pixels are indexed from (0,0) 
     * in the top-left corner, with the x-axis running left to right and the 
     * y-axis running top to bottom.
     *
     * Note that this updates the <em>internal</em> image, but not the display
     * shown in the window. To update the window call refresh().
     *
     * \param x The x co-ordinate of the pixel to set.
     * \param y The y co-ordinate of the pixel to set.
     * \param colour The Colour to set at (x,y).
     */
    void set(int x, int y, const Colour &colour);

    /**
     * \brief Update the window displaying the image.
     * 
     * The window displaying the image is not automatically updated when the 
     * image changes for efficiency reasons. Calling refresh() causes the 
     * current image to be displayed.
     */
    void refresh();

    /**
     * \brief Save an image to file.
     *
     * Images can be saved to files, and the image format is determined by
     * the file extension. For example, saving to \c render.png would write a 
     * PNG image, while saving to \c RENDER.JPG would write a JPEG.
     *
     * \param filename The file to save to, with an appropriate extension.
     */
    void save(const std::string &filename) const;

    /**
     * \brief Wait for a specified duration.
     *
     * This function pauses the program for a specified duration
     *
     * \param seconds The number of seconds to wait (fractions allowed).
     */
    void pause(double seconds);

private:

    cimg_library::CImg<unsigned char> image_; //!< Internal storage of the image to render to.
    std::unique_ptr<cimg_library::CImgDisplay> display_ = nullptr; //!< (Optional) window to display the image in
    int width_; //!< The width of the image
    int height_; //!< The height of the image

};

#endif
