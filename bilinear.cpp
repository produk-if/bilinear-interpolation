#include "interpolation.h"
#include <cmath>
#include <omp.h>
#include <thread>
#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;

namespace interpolation {

// ============================================================================
// BILINEAR INTERPOLATION - HELPER FUNCTIONS
// ============================================================================

inline float BilinearInterpolation::getValueSafe(const Image& img, 
                                                   int x, int y) const {
    if (x < 0 || x >= img.getWidth() || y < 0 || y >= img.getHeight()) {
        return 0.0f;
    }
    // Assuming Pixel has grayscale conversion
    Pixel p = img.getPixel(x, y);
    return (p.r + p.g + p.b) / 3.0f;
}

// ============================================================================
// BILINEAR INTERPOLATION - MAIN INTERPOLATE FUNCTION
// ============================================================================

Pixel BilinearInterpolation::interpolate(const Image& image, 
                                         float x, float y) const {
    // Clamp coordinates to valid range
    x = max(0.0f, min(x, (float)image.getWidth() - 1.001f));
    y = max(0.0f, min(y, (float)image.getHeight() - 1.001f));
    
    // Get integer coordinates
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int x1 = min(x0 + 1, image.getWidth() - 1);
    int y1 = min(y0 + 1, image.getHeight() - 1);
    
    // Get fractional parts
    float fx = x - floor(x);
    float fy = y - floor(y);
    
    // Get 4 pixel values
    Pixel f00 = image.getPixel(x0, y0);
    Pixel f10 = image.getPixel(x1, y0);
    Pixel f01 = image.getPixel(x0, y1);
    Pixel f11 = image.getPixel(x1, y1);
    
    // Compute weights
    float w00 = (1.0f - fx) * (1.0f - fy);
    float w10 = fx * (1.0f - fy);
    float w01 = (1.0f - fx) * fy;
    float w11 = fx * fy;
    
    // Weighted sum
    Pixel result(
        f00.r * w00 + f10.r * w10 + f01.r * w01 + f11.r * w11,
        f00.g * w00 + f10.g * w10 + f01.g * w01 + f11.g * w11,
        f00.b * w00 + f10.b * w10 + f01.b * w01 + f11.b * w11,
        f00.a * w00 + f10.a * w10 + f01.a * w01 + f11.a * w11
    );
    
    return result;
}

// ============================================================================
// SERIAL IMPLEMENTATION
// ============================================================================

Image BilinearInterpolation::resize_serial(const Image& src, 
                                           int new_height, 
                                           int new_width) const {
    Image dst(new_height, new_width);
    
    float scale_y = (float)src.getHeight() / new_height;
    float scale_x = (float)src.getWidth() / new_width;
    
    for (int j = 0; j < new_height; j++) {
        for (int i = 0; i < new_width; i++) {
            float src_x = i * scale_x;
            float src_y = j * scale_y;
            
            Pixel pixel = interpolate(src, src_x, src_y);
            dst.setPixel(i, j, pixel);
        }
    }
    
    return dst;
}

// ============================================================================
// OPENMP IMPLEMENTATION
// ============================================================================

Image BilinearInterpolation::resize_openmp(const Image& src, 
                                           int new_height, 
                                           int new_width) const {
    Image dst(new_height, new_width);
    
    float scale_y = (float)src.getHeight() / new_height;
    float scale_x = (float)src.getWidth() / new_width;
    
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int j = 0; j < new_height; j++) {
        for (int i = 0; i < new_width; i++) {
            float src_x = i * scale_x;
            float src_y = j * scale_y;
            
            // Inline bilinear interpolation untuk minimize function call overhead
            int x0 = (int)floor(src_x);
            int y0 = (int)floor(src_y);
            int x1 = min(x0 + 1, src.getWidth() - 1);
            int y1 = min(y0 + 1, src.getHeight() - 1);
            
            x0 = max(0, x0);
            y0 = max(0, y0);
            
            float fx = src_x - floor(src_x);
            float fy = src_y - floor(src_y);
            
            Pixel f00 = src.getPixel(x0, y0);
            Pixel f10 = src.getPixel(x1, y0);
            Pixel f01 = src.getPixel(x0, y1);
            Pixel f11 = src.getPixel(x1, y1);
            
            float w00 = (1.0f - fx) * (1.0f - fy);
            float w10 = fx * (1.0f - fy);
            float w01 = (1.0f - fx) * fy;
            float w11 = fx * fy;
            
            Pixel result(
                f00.r * w00 + f10.r * w10 + f01.r * w01 + f11.r * w11,
                f00.g * w00 + f10.g * w10 + f01.g * w01 + f11.g * w11,
                f00.b * w00 + f10.b * w10 + f01.b * w01 + f11.b * w11,
                f00.a * w00 + f10.a * w10 + f01.a * w01 + f11.a * w11
            );
            
            dst.setPixel(i, j, result);
        }
    }
    
    return dst;
}

// ============================================================================
// THREADING IMPLEMENTATION
// ============================================================================

void resizeThreadTile(const Image& src, Image& dst,
                      int start_j, int end_j, int new_width,
                      float scale_x, float scale_y) {
    for (int j = start_j; j < end_j; j++) {
        for (int i = 0; i < new_width; i++) {
            float src_x = i * scale_x;
            float src_y = j * scale_y;
            
            // Inline bilinear interpolation
            int x0 = (int)floor(src_x);
            int y0 = (int)floor(src_y);
            int x1 = min(x0 + 1, src.getWidth() - 1);
            int y1 = min(y0 + 1, src.getHeight() - 1);
            
            x0 = max(0, x0);
            y0 = max(0, y0);
            
            float fx = src_x - floor(src_x);
            float fy = src_y - floor(src_y);
            
            Pixel f00 = src.getPixel(x0, y0);
            Pixel f10 = src.getPixel(x1, y0);
            Pixel f01 = src.getPixel(x0, y1);
            Pixel f11 = src.getPixel(x1, y1);
            
            float w00 = (1.0f - fx) * (1.0f - fy);
            float w10 = fx * (1.0f - fy);
            float w01 = (1.0f - fx) * fy;
            float w11 = fx * fy;
            
            Pixel result(
                f00.r * w00 + f10.r * w10 + f01.r * w01 + f11.r * w11,
                f00.g * w00 + f10.g * w10 + f01.g * w01 + f11.g * w11,
                f00.b * w00 + f10.b * w10 + f01.b * w01 + f11.b * w11,
                f00.a * w00 + f10.a * w10 + f01.a * w01 + f11.a * w11
            );
            
            dst.setPixel(i, j, result);
        }
    }
}

Image BilinearInterpolation::resize_threading(const Image& src, 
                                              int new_height, 
                                              int new_width) const {
    Image dst(new_height, new_width);
    
    float scale_y = (float)src.getHeight() / new_height;
    float scale_x = (float)src.getWidth() / new_width;
    
    // Determine number of threads
    int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    
    vector<thread> threads;
    int tile_height = new_height / num_threads;
    
    // Create threads for each tile
    for (int t = 0; t < num_threads; t++) {
        int start_j = t * tile_height;
        int end_j = (t == num_threads - 1) ? new_height : (t + 1) * tile_height;
        
        threads.emplace_back(resizeThreadTile, ref(src), ref(dst),
                           start_j, end_j, new_width, scale_x, scale_y);
    }
    
    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }
    
    return dst;
}

// ============================================================================
// BENCHMARK IMPLEMENTATION
// ============================================================================

BilinearInterpolation::BenchmarkResult BilinearInterpolation::benchmark(
    const Image& src, int new_height, int new_width, int iterations) const {
    
    BenchmarkResult result{};
    
    cout << "\n========================================\n";
    cout << "BILINEAR INTERPOLATION BENCHMARK\n";
    cout << "========================================\n";
    cout << "Source: " << src.getWidth() << "x" << src.getHeight() << endl;
    cout << "Target: " << new_width << "x" << new_height << endl;
    cout << "Iterations: " << iterations << endl;
    cout << "========================================\n\n";
    
    // Serial benchmark
    {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; i++) {
            Image dummy = resize_serial(src, new_height, new_width);
        }
        auto end = chrono::high_resolution_clock::now();
        result.time_serial = chrono::duration_cast<chrono::milliseconds>(
            end - start).count();
        cout << "Serial:     " << result.time_serial << " ms\n";
    }
    
    // OpenMP benchmark
    {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; i++) {
            Image dummy = resize_openmp(src, new_height, new_width);
        }
        auto end = chrono::high_resolution_clock::now();
        result.time_openmp = chrono::duration_cast<chrono::milliseconds>(
            end - start).count();
        result.speedup_openmp = (float)result.time_serial / result.time_openmp;
        cout << "OpenMP:     " << result.time_openmp << " ms (Speedup: " 
             << result.speedup_openmp << "x)\n";
    }
    
    // Threading benchmark
    {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; i++) {
            Image dummy = resize_threading(src, new_height, new_width);
        }
        auto end = chrono::high_resolution_clock::now();
        result.time_threading = chrono::duration_cast<chrono::milliseconds>(
            end - start).count();
        result.speedup_threading = (float)result.time_serial / result.time_threading;
        cout << "Threading:  " << result.time_threading << " ms (Speedup: " 
             << result.speedup_threading << "x)\n";
    }
    
    // Determine best method
    long long best_time = min({result.time_serial, 
                              result.time_openmp, 
                              result.time_threading});
    
    if (best_time == result.time_serial) {
        result.best_method = "Serial";
    } else if (best_time == result.time_openmp) {
        result.best_method = "OpenMP";
    } else {
        result.best_method = "Threading";
    }
    
    cout << "\n========================================\n";
    cout << "BEST METHOD: " << result.best_method << endl;
    cout << "========================================\n";
    
    return result;
}

void BilinearInterpolation::printBenchmarkResults(const BenchmarkResult& result) {
    cout << "\nBENCHMARK RESULTS:\n";
    cout << "Serial:     " << result.time_serial << " ms (baseline)\n";
    cout << "OpenMP:     " << result.time_openmp << " ms (speedup: " 
         << result.speedup_openmp << "x)\n";
    cout << "Threading:  " << result.time_threading << " ms (speedup: " 
         << result.speedup_threading << "x)\n";
    cout << "Best:       " << result.best_method << "\n";
}

} // namespace interpolation
