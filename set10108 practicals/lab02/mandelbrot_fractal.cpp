#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <future>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../lib/stb_image_write.h"

// max iterations to perform to find pixel value
constexpr size_t max_iterations = 1000;

// dimensions in pixels of image to generate
constexpr size_t dim = 8192;

// mandelbrot dimensions ([-2.1, 1.0], [-1.3, 1.3])
constexpr double xmin = -2.1;
constexpr double xmax = 1.0;
constexpr double ymin = -1.3;
constexpr double ymax = 1.3;

// conversion from mandelbrot coordinate to image coordinate
constexpr double integral_x = (xmax - xmin) / static_cast<double>(dim);
constexpr double integral_y = (ymax - ymin) / static_cast<double>(dim);

std::vector<double> mandelbrot(size_t start_y, size_t end_y) {
    // declare needed variables
    double x, y, x1, y1, xx = 0.0;
    size_t loop_count = 0;
    // vector to store the results
    std::vector<double> results;

    // loop through each line
    y = ymin + (start_y * integral_y);
    for(size_t y_coord = start_y; y_coord < end_y; ++y_coord) {
        x = xmin;
        // loop through each pixel on this line
        for(size_t x_coord = 0; x_coord < dim; ++x_coord) {
            x1 = 0.0, y1 = 0.0;
            loop_count = 0;
            // calculate mandelbrot value
            while(loop_count < max_iterations && std::sqrt((x1 * x1) + (y1 * y1)) < 2.0) {
                ++loop_count;
                xx = (x1 * x1) - (y1 * y1) + x;
                y1 = 2 * x1 * y1 + y;
                x1 = xx;
            }

            // get number of iterations of the loop
            auto val = static_cast<double>(loop_count) / static_cast<double>(max_iterations);
            // push into the vector
            results.push_back(val);
            // increase x based on the integral
            x += integral_x;
        }

        // increase y based on integral
        y += integral_y;
    }

    return results;
}

int main() {
    // get number of supported threads
    auto num_threads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << num_threads << std::endl;

    // determine strip height
    size_t strip_height = dim / num_threads;

    // create futures
    std::vector<std::future<std::vector<double>>> futures;
    for(unsigned int i = 0; i < num_threads; ++i) {
        futures.push_back(std::async(mandelbrot, i * strip_height, (i + 1) * strip_height));
    }

    // vector to store results
    std::vector<std::vector<double>> results;
    //get results
    for(auto &f : futures)
        results.push_back(f.get());

    // create image from the generated data
    for(unsigned int i = 0; i < results.size(); ++i) {
        auto filename = std::to_string(i) + ".png";
        std::cout << "Writing image " << filename << std::endl;
        if(!stbi_write_png(filename.c_str(), dim, static_cast<int>(strip_height), 4, &results[i].front(), 0))
            std::cout << "Error creating image " << filename << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}