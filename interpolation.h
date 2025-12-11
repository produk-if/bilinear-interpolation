#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "image.h"
#include <chrono>

/**
 * @namespace interpolation
 * @brief Namespace untuk semua interpolation methods
 */
namespace interpolation {

/**
 * @class InterpolationBase
 * @brief Base class untuk semua interpolation methods
 */
class InterpolationBase {
public:
    virtual ~InterpolationBase() = default;
    
    /**
     * @brief Interpolate value pada posisi (x, y)
     * @param image Source image
     * @param x X coordinate (dapat berupa float)
     * @param y Y coordinate (dapat berupa float)
     * @return Interpolated pixel value
     */
    virtual Pixel interpolate(const Image& image, float x, float y) const = 0;
    
    /**
     * @brief Resize image menggunakan metode ini
     */
    virtual Image resize(const Image& src, int new_height, int new_width) const;
};

/**
 * @class NearestNeighbor
 * @brief Nearest neighbor interpolation (tercepat, kualitas terendah)
 */
class NearestNeighbor : public InterpolationBase {
public:
    Pixel interpolate(const Image& image, float x, float y) const override;
};

/**
 * @class BilinearInterpolation
 * @brief Bilinear interpolation (balance antara kecepatan dan kualitas)
 * 
 * Implementasi lengkap dengan 3 varian:
 * - Serial: Baseline implementation
 * - OpenMP: Pragma-based parallelization
 * - Threading: Manual thread management
 */
class BilinearInterpolation : public InterpolationBase {
private:
    // Helper function untuk inline computation
    inline float getValueSafe(const Image& img, int x, int y) const;
    
public:
    /**
     * @brief Interpolate menggunakan bilinear method
     * 
     * Formula:
     * f(x,y) = f00*(1-fx)*(1-fy) + f10*fx*(1-fy) + 
     *          f01*(1-fx)*fy + f11*fx*fy
     * 
     * Dimana:
     * - (x0,y0), (x1,y0), (x0,y1), (x1,y1) adalah 4 tetangga terdekat
     * - fx, fy adalah fractional part dari koordinat
     */
    Pixel interpolate(const Image& image, float x, float y) const override;
    
    // === SERIAL IMPLEMENTATION ===
    
    /**
     * @brief Resize image - Serial version (baseline)
     * @param src Source image
     * @param new_height Target height
     * @param new_width Target width
     * @return Resized image
     */
    Image resize_serial(const Image& src, int new_height, int new_width) const;
    
    // === OPENMP IMPLEMENTATION ===
    
    /**
     * @brief Resize image - OpenMP version
     * Menggunakan #pragma omp parallel for collapse(2)
     */
    Image resize_openmp(const Image& src, int new_height, int new_width) const;
    
    // === THREADING IMPLEMENTATION ===
    
    /**
     * @brief Resize image - C++ Threading version
     * Tile-based parallelization dengan std::thread
     */
    Image resize_threading(const Image& src, int new_height, int new_width) const;
    
    // === BENCHMARK & PERFORMANCE ===
    
    /**
     * @struct BenchmarkResult
     * @brief Hasil benchmark untuk comparison
     */
    struct BenchmarkResult {
        long long time_serial;      // ms
        long long time_openmp;      // ms
        long long time_threading;   // ms
        float speedup_openmp;
        float speedup_threading;
        std::string best_method;
    };
    
    /**
     * @brief Run comprehensive benchmark
     */
    BenchmarkResult benchmark(const Image& src, int new_height, int new_width,
                             int iterations = 1) const;
    
    /**
     * @brief Print benchmark results
     */
    static void printBenchmarkResults(const BenchmarkResult& result);
};

/**
 * @class BicubicInterpolation
 * @brief Bicubic interpolation (higher quality, slower)
 */
class BicubicInterpolation : public InterpolationBase {
public:
    /**
     * @brief Bicubic interpolation using 16 neighbors (4x4)
     * 
     * Formula:
     * f(x,y) = Σ Σ wij * f(xi, yj)
     * 
     * Dimana wij adalah bicubic basis function weights
     */
    Pixel interpolate(const Image& image, float x, float y) const override;
};

/**
 * @class LanczosInterpolation
 * @brief Lanczos interpolation (best quality, slowest)
 */
class LanczosInterpolation : public InterpolationBase {
private:
    int windowSize;  // Typically 2, 3, or 4
    
    float lanczosKernel(float x, int a) const;
    
public:
    explicit LanczosInterpolation(int window = 3) : windowSize(window) {}
    
    /**
     * @brief Lanczos interpolation with configurable window
     * 
     * Best quality but slowest. Uses windowed sinc function.
     */
    Pixel interpolate(const Image& image, float x, float y) const override;
};

/**
 * @brief Factory function untuk mendapatkan interpolator
 */
std::unique_ptr<InterpolationBase> createInterpolator(InterpolationMethod method);

/**
 * @brief Utility untuk benchmark semua metode
 */
struct InterpolationComparison {
    static void compareAllMethods(const Image& src, int new_height, int new_width);
    static void compareQuality(const Image& original, const Image& reference);
};

} // namespace interpolation

#endif // INTERPOLATION_H
