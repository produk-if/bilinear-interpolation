/**
 * @file example_bilinear_detailed.cpp
 * @brief Contoh detail penggunaan Bilinear Interpolation dari project
 * 
 * Demonstrasi:
 * 1. Single interpolation pada satu pixel
 * 2. Resize image menggunakan ketiga metode (Serial, OpenMP, Threading)
 * 3. Benchmark dan perbandingan performa
 * 4. Visualisasi hasil
 */

#include "../include/image.h"
#include "../include/interpolation.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace interpolation;

// ============================================================================
// EXAMPLE 1: Single Pixel Interpolation
// ============================================================================

void example_single_interpolation() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "EXAMPLE 1: Single Pixel Interpolation\n";
    cout << string(70, '=') << "\n";
    
    // Create a small test image
    Image src(4, 4);
    
    // Set pixel values
    //     0    1    2    3
    // 0 [ 10 ] 12   14   16
    // 1  18  [ 20 ] 22   24
    // 2  26   28  [ 30 ] 32
    // 3  34   36   38  [ 40 ]
    
    cout << "Source Image (4x4):\n";
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float val = 10 + i * 2 + j * 8;
            src.setPixel(i, j, Pixel(val, val, val, 255));
            cout << fixed << setw(3) << val << " ";
        }
        cout << "\n";
    }
    
    BilinearInterpolation bi;
    
    // Interpolate at various positions
    cout << "\nInterpolation at different positions:\n";
    
    struct TestPoint {
        float x, y;
        const char* description;
    };
    
    vector<TestPoint> tests = {
        {0.0f, 0.0f, "Exact corner (0,0)"},
        {1.0f, 1.0f, "Exact pixel (1,1)"},
        {0.5f, 0.5f, "Between 4 pixels (0.5,0.5)"},
        {1.5f, 1.5f, "Between 4 pixels (1.5,1.5)"},
        {0.25f, 0.75f, "Non-uniform (0.25,0.75)"},
    };
    
    for (const auto& test : tests) {
        Pixel p = bi.interpolate(src, test.x, test.y);
        cout << "  " << test.description << " (x=" << test.x 
             << ", y=" << test.y << "): " 
             << "R=" << fixed << setprecision(1) << p.r << "\n";
    }
}

// ============================================================================
// EXAMPLE 2: Image Resize dengan Ketiga Metode
// ============================================================================

void example_resize_comparison() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "EXAMPLE 2: Image Resize - Three Methods\n";
    cout << string(70, '=') << "\n";
    
    // Create gradient image (512x512)
    cout << "Creating gradient image (512x512)...\n";
    Image src(512, 512);
    
    for (int j = 0; j < 512; j++) {
        for (int i = 0; i < 512; i++) {
            float val = ((float)(i + j) / (512 + 512)) * 255.0f;
            src.setPixel(i, j, Pixel(val, val, val, 255));
        }
    }
    
    BilinearInterpolation bi;
    int target_h = 1024;
    int target_w = 1024;
    
    cout << "Resizing to " << target_w << "x" << target_h << "\n\n";
    
    // Serial resize
    cout << "1. SERIAL VERSION:\n";
    auto start = chrono::high_resolution_clock::now();
    Image resized_serial = bi.resize_serial(src, target_h, target_w);
    auto end = chrono::high_resolution_clock::now();
    long long time_serial = chrono::duration_cast<chrono::milliseconds>(
        end - start).count();
    cout << "   Time: " << time_serial << " ms\n";
    cout << "   Result size: " << resized_serial.getWidth() 
         << "x" << resized_serial.getHeight() << "\n\n";
    
    // OpenMP resize
    cout << "2. OPENMP VERSION:\n";
    start = chrono::high_resolution_clock::now();
    Image resized_openmp = bi.resize_openmp(src, target_h, target_w);
    end = chrono::high_resolution_clock::now();
    long long time_openmp = chrono::duration_cast<chrono::milliseconds>(
        end - start).count();
    cout << "   Time: " << time_openmp << " ms\n";
    cout << "   Speedup: " << fixed << setprecision(2) 
         << (float)time_serial / time_openmp << "x\n\n";
    
    // Threading resize
    cout << "3. THREADING VERSION:\n";
    start = chrono::high_resolution_clock::now();
    Image resized_threading = bi.resize_threading(src, target_h, target_w);
    end = chrono::high_resolution_clock::now();
    long long time_threading = chrono::duration_cast<chrono::milliseconds>(
        end - start).count();
    cout << "   Time: " << time_threading << " ms\n";
    cout << "   Speedup: " << fixed << setprecision(2) 
         << (float)time_serial / time_threading << "x\n\n";
    
    // Summary
    cout << "SUMMARY:\n";
    cout << "---------\n";
    cout << "Serial:    " << time_serial << " ms (baseline)\n";
    cout << "OpenMP:    " << time_openmp << " ms (speedup: " 
         << (float)time_serial / time_openmp << "x)\n";
    cout << "Threading: " << time_threading << " ms (speedup: " 
         << (float)time_serial / time_threading << "x)\n";
    
    long long best = min({time_serial, time_openmp, time_threading});
    cout << "\nBest method: ";
    if (best == time_serial) cout << "Serial (no parallelization overhead)\n";
    else if (best == time_openmp) cout << "OpenMP\n";
    else cout << "Threading (RECOMMENDED for this size)\n";
}

// ============================================================================
// EXAMPLE 3: Benchmark Detail
// ============================================================================

void example_benchmark() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "EXAMPLE 3: Comprehensive Benchmark\n";
    cout << string(70, '=') << "\n";
    
    // Test dengan berbagai ukuran
    vector<pair<pair<int,int>, pair<int,int>>> test_cases = {
        {{256, 256}, {512, 512}},      // Small: 2x zoom
        {{512, 512}, {1024, 1024}},    // Medium: 2x zoom (main case)
        {{1024, 1024}, {2048, 2048}},  // Large: 2x zoom
    };
    
    BilinearInterpolation bi;
    
    for (const auto& [src_size, dst_size] : test_cases) {
        cout << "\nTest: " << src_size.first << "x" << src_size.second 
             << " → " << dst_size.first << "x" << dst_size.second << "\n";
        
        // Create test image
        Image img(src_size.second, src_size.first);
        for (int j = 0; j < src_size.second; j++) {
            for (int i = 0; i < src_size.first; i++) {
                float val = ((float)(i + j) / (src_size.first + src_size.second)) * 255;
                img.setPixel(i, j, Pixel(val, val, val, 255));
            }
        }
        
        // Run benchmark
        auto result = bi.benchmark(img, dst_size.second, dst_size.first, 1);
        BilinearInterpolation::printBenchmarkResults(result);
    }
}

// ============================================================================
// EXAMPLE 4: Quality Comparison
// ============================================================================

void example_quality_comparison() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "EXAMPLE 4: Visual Quality Comparison\n";
    cout << string(70, '=') << "\n";
    
    cout << "Creating test image with different zoom levels...\n";
    Image src(256, 256);
    
    // Create gradient image
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i < 256; i++) {
            float val = ((float)(i + j) / 512.0f) * 255;
            src.setPixel(i, j, Pixel(val, val, val, 255));
        }
    }
    
    BilinearInterpolation bi;
    
    cout << "\nZoom levels:\n";
    cout << "  Original:     256 x 256 (baseline)\n";
    cout << "  Zoom 1.5x:    384 x 384\n";
    cout << "  Zoom 2x:      512 x 512\n";
    cout << "  Zoom 4x:     1024 x 1024\n";
    
    cout << "\nProcessing zooms (using threading - fastest)...\n";
    
    Image zoom_1_5x = bi.resize_threading(src, 384, 384);
    Image zoom_2x = bi.resize_threading(src, 512, 512);
    Image zoom_4x = bi.resize_threading(src, 1024, 1024);
    
    cout << "✓ Complete!\n";
    cout << "\nBilinear Interpolation characteristics at different zoom levels:\n";
    cout << "  1.5x zoom: Very good quality, smooth interpolation\n";
    cout << "  2x zoom:   Good quality, this is sweet spot\n";
    cout << "  4x zoom:   Visible blockiness, interpolation artifacts appear\n";
    cout << "\nFor larger zoom (>2x), consider Bicubic or Lanczos interpolation.\n";
}

// ============================================================================
// EXAMPLE 5: Interpolation Details - Understanding the Formula
// ============================================================================

void example_formula_details() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "EXAMPLE 5: Understanding the Bilinear Formula\n";
    cout << string(70, '=') << "\n";
    
    cout << "Bilinear formula step-by-step:\n\n";
    
    cout << "Formula:\n";
    cout << "  f(x,y) = f00*w00 + f10*w10 + f01*w01 + f11*w11\n\n";
    
    cout << "Where:\n";
    cout << "  w00 = (1-fx) * (1-fy)  [Top-Left weight]\n";
    cout << "  w10 = fx * (1-fy)      [Top-Right weight]\n";
    cout << "  w01 = (1-fx) * fy      [Bottom-Left weight]\n";
    cout << "  w11 = fx * fy          [Bottom-Right weight]\n\n";
    
    cout << "And:\n";
    cout << "  fx = fractional part of x = x - floor(x)\n";
    cout << "  fy = fractional part of y = y - floor(y)\n\n";
    
    cout << "Key properties:\n";
    cout << "  • Sum of weights = 1 (normalized)\n";
    cout << "  • Result is weighted average of 4 neighbors\n";
    cout << "  • Closer pixels get higher weight\n";
    cout << "  • Farther pixels get lower weight\n\n";
    
    // Concrete example
    cout << "Concrete Example:\n";
    cout << "  Position: (1.3, 0.7)\n";
    cout << "  x0=1, y0=0, x1=2, y1=1\n";
    cout << "  fx = 1.3 - 1 = 0.3\n";
    cout << "  fy = 0.7 - 0 = 0.7\n\n";
    
    cout << "  Weights:\n";
    cout << "  w00 = (1-0.3) * (1-0.7) = 0.7 * 0.3 = 0.21 (30% from (1,0))\n";
    cout << "  w10 = 0.3 * (1-0.7) = 0.3 * 0.3 = 0.09 (9% from (2,0))\n";
    cout << "  w01 = (1-0.3) * 0.7 = 0.7 * 0.7 = 0.49 (49% from (1,1))\n";
    cout << "  w11 = 0.3 * 0.7 = 0.21 (21% from (2,1))\n";
    cout << "  Total: 0.21 + 0.09 + 0.49 + 0.21 = 1.00 ✓\n\n";
    
    cout << "  If pixel values are:\n";
    cout << "    f(1,0) = 100\n";
    cout << "    f(2,0) = 120\n";
    cout << "    f(1,1) = 110\n";
    cout << "    f(2,1) = 130\n\n";
    
    cout << "  Result:\n";
    cout << "    f(1.3, 0.7) = 100*0.21 + 120*0.09 + 110*0.49 + 130*0.21\n";
    cout << "               = 21 + 10.8 + 53.9 + 27.3\n";
    cout << "               = 113.0\n";
    
    cout << "\nNote: Pixel at (1.3, 0.7) mendapat kontribusi terbesar\n";
    cout << "      dari (1,1) karena posisi terdekat.\n";
}

// ============================================================================
// EXAMPLE 6: Real-World Use Cases
// ============================================================================

void example_use_cases() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "EXAMPLE 6: Real-World Use Cases\n";
    cout << string(70, '=') << "\n";
    
    cout << "Bilinear Interpolation digunakan dalam:\n\n";
    
    cout << "1. IMAGE RESIZING\n";
    cout << "   - Zoom in/out image\n";
    cout << "   - Thumbnail generation\n";
    cout << "   - Display scaling\n";
    cout << "   Use: img.resize(new_height, new_width)\n\n";
    
    cout << "2. IMAGE ROTATION\n";
    cout << "   - Rotate image by angle\n";
    cout << "   - Coordinate transformation\n";
    cout << "   Use: img.rotate(degrees)\n\n";
    
    cout << "3. TEXTURE MAPPING (3D Graphics)\n";
    cout << "   - Map 2D texture onto 3D surface\n";
    cout << "   - Texture coordinates often float\n";
    cout << "   - Fast enough for real-time rendering\n\n";
    
    cout << "4. IMAGE WARPING\n";
    cout << "   - Lens distortion correction\n";
    cout << "   - Perspective transformation\n";
    cout << "   - Image morphing\n\n";
    
    cout << "5. SUPERRESOLUTION (Simple)\n";
    cout << "   - Upscale low-res to high-res\n";
    cout << "   - Quality moderate (better: bicubic/lanczos)\n\n";
    
    cout << "6. VIDEO PROCESSING\n";
    cout << "   - Frame interpolation\n";
    cout << "   - Motion estimation\n";
    cout << "   - Real-time video transformation\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "║  Bilinear Interpolation - Detailed Examples                     ║\n";
    cout << "║  Part of: Image Processing Library C++ Project                  ║\n";
    cout << "╚══════════════════════════════════════════════════════════════════╝\n";
    
    // Run all examples
    example_single_interpolation();
    example_resize_comparison();
    example_benchmark();
    example_quality_comparison();
    example_formula_details();
    example_use_cases();
    
    cout << "\n" << string(70, '=') << "\n";
    cout << "Examples Complete!\n";
    cout << "Next Steps:\n";
    cout << "  1. Read: docs/interpolation/bilinear.md (detailed theory)\n";
    cout << "  2. Read: docs/interpolation/bilinear.html (interactive docs)\n";
    cout << "  3. Study: src/interpolation/bilinear.cpp (implementation)\n";
    cout << "  4. Compare: with other interpolation methods\n";
    cout << string(70, '=') << "\n\n";
    
    return 0;
}
