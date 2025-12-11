# Image Processing Library - Architecture & Design

## 🏗️ High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                  Image Processing Library (C++)                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  ┌────────────────────┐    ┌─────────────────────────────┐           │
│  │  CORE MODULE       │    │ ⭐ INTERPOLATION MODULE ⭐ │           │
│  │                    │    │                             │           │
│  │  • Image class     │    │  • NearestNeighbor         │           │
│  │  • Pixel class     │    │  • Bilinear (MODUL UTAMA) │           │
│  │  • I/O operations  │    │  • Bicubic                 │           │
│  │  • Basic filters   │    │  • Lanczos                 │           │
│  │                    │    │                             │           │
│  └────────┬───────────┘    └─────────────┬───────────────┘           │
│           │                              │                           │
│           └──────────────────┬───────────┘                           │
│                              │                                       │
│              ┌───────────────▼──────────────┐                        │
│              │  TRANSFORM MODULE            │                        │
│              │                              │                        │
│              │  • Image::resize()      ────┼──> Uses                 │
│              │  • Image::rotate()      ────┼──> Interpolation       │
│              │  • Image::warp()        ────┼──> Methods              │
│              │                              │                        │
│              └───────────────┬──────────────┘                        │
│                              │                                       │
│              ┌───────────────▼──────────────┐                        │
│              │  FILTER MODULE               │                        │
│              │                              │                        │
│              │  • Gaussian blur             │                        │
│              │  • Edge detection            │                        │
│              │  • Morphological ops         │                        │
│              │  • Custom kernels            │                        │
│              │                              │                        │
│              └──────────────────────────────┘                        │
│                                                                       │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                         USER APPLICATION                             │
│  (menggunakan library untuk berbagai image processing tasks)         │
└─────────────────────────────────────────────────────────────────────┘
```

## 🎯 Focus: Bilinear Interpolation Module

Bilinear Interpolation adalah modul utama yang mendemonstrasikan:

### 1. **Concept & Theory**
   - Interpolasi sebagai teknik fundamental dalam image processing
   - Trade-off antara kecepatan dan kualitas
   - Mathematical foundation (weighted average, basis functions)

### 2. **Implementation Variants**
   - **Serial:** Baseline implementation, non-parallelized
   - **OpenMP:** Pragma-based parallelization (#pragma omp)
   - **Threading:** Manual std::thread management

### 3. **Performance Analysis**
   - Benchmark setiap implementasi
   - Analyze overhead dari setiap approach
   - Identify best method untuk problem size yang diberikan

### 4. **Documentation**
   - Teori matematika lengkap
   - Source code dengan dokumentasi inline
   - Interactive HTML documentation
   - Working examples

## 📁 Complete Directory Structure

```
image-processing-project/
│
├── README.md                    # Project overview
├── CMakeLists.txt              # Build configuration
│
├── include/                    # Header files
│   ├── image.h                # Image & Pixel class declarations
│   ├── interpolation.h         # All interpolation methods
│   ├── transforms.h           # Transform operations
│   └── filters.h              # Filter operations
│
├── src/                        # Implementation files
│   ├── core/
│   │   ├── image.cpp          # Image class implementation
│   │   └── pixel.cpp          # Pixel operations
│   │
│   ├── interpolation/          # ⭐ BILINEAR MODULE
│   │   ├── nearest.cpp        # Nearest Neighbor
│   │   ├── bilinear.cpp       # BILINEAR - MAIN IMPLEMENTATION
│   │   ├── bicubic.cpp        # Bicubic
│   │   └── lanczos.cpp        # Lanczos
│   │
│   ├── transforms/
│   │   ├── resize.cpp
│   │   ├── rotate.cpp
│   │   └── warp.cpp
│   │
│   └── filters/
│       ├── gaussian.cpp
│       ├── sobel.cpp
│       └── median.cpp
│
├── docs/                       # Documentation
│   ├── ARCHITECTURE.md        # FILE INI
│   ├── API.md                 # API Reference
│   │
│   └── interpolation/         # ⭐ BILINEAR DOCUMENTATION
│       ├── overview.md        # Overview all methods
│       ├── bilinear.md        # DETAILED THEORY & FORMULAS
│       ├── bilinear.html      # INTERACTIVE DOCS
│       └── performance.md     # Performance analysis
│
├── examples/                  # Usage examples
│   ├── example_resize.cpp
│   ├── example_interpolation_comparison.cpp
│   └── example_bilinear_detailed.cpp    # BILINEAR EXAMPLE
│
├── tests/                     # Unit tests
│   ├── test_interpolation.cpp
│   ├── test_transforms.cpp
│   └── test_filters.cpp
│
└── build/                     # Build directory
    └── (generated files)
```

## 🔄 Data Flow: Image Resizing Example

```
User Code:
    Image img = Image::load("photo.png");
    Image resized = img.resize(800, 600);
    resized.save("resized.png");
           │
           ▼
┌──────────────────────────────────────────┐
│ Image::resize(h, w, method)              │
│                                          │
│ Determina method (default: BILINEAR)    │
│ Create interpolator object               │
└─────────────┬────────────────────────────┘
              │
              ▼
┌──────────────────────────────────────────┐
│ BilinearInterpolation::resize_*()        │
│ (variant: serial, openmp, threading)     │
│                                          │
│ For each pixel (i,j) in destination:    │
│   • Calculate source coordinates (x, y) │
│   • Call interpolate(src, x, y)         │
│   • Write result to destination          │
└─────────────┬────────────────────────────┘
              │
              ▼
┌──────────────────────────────────────────┐
│ BilinearInterpolation::interpolate()    │
│                                          │
│ 1. Clamp coordinates                     │
│ 2. Get 4 neighbors (x0,y0), (x1,y0),    │
│                     (x0,y1), (x1,y1)    │
│ 3. Calculate fractional part (fx, fy)   │
│ 4. Calculate weights (w00, w10, etc)    │
│ 5. Weighted sum: result = Σ wi*fi       │
└─────────────┬────────────────────────────┘
              │
              ▼
   Return interpolated pixel value
        (untuk setiap channel RGBA)
```

## 🔗 How Bilinear Fits in the Project

```
1. CORE ABSTRACTION
   ┌──────────────────────────┐
   │ InterpolationBase        │
   │ (Abstract base class)    │
   └─────────────┬────────────┘
                 │
        ┌────────┴────────┬─────────────┬───────────┐
        │                 │             │           │
        ▼                 ▼             ▼           ▼
    Nearest         Bilinear (⭐)    Bicubic      Lanczos
                    MODUL UTAMA

2. USAGE IN TRANSFORMS
   ┌────────────────────────────┐
   │ Image::resize()           │
   │   ├─> Uses interpolator    │
   │   └─> Default: Bilinear    │
   └────────────────────────────┘

3. EXAMPLE & TEST
   ┌────────────────────────────────────────┐
   │ example_bilinear_detailed.cpp           │
   │  ├─> Single pixel interpolation         │
   │  ├─> Three implementations comparison   │
   │  ├─> Benchmark                          │
   │  └─> Use case examples                  │
   └────────────────────────────────────────┘

4. DOCUMENTATION
   ┌─────────────────────────────────────┐
   │ docs/interpolation/bilinear.md      │
   │  • Konsep & teori                   │
   │  • Formula derivation               │
   │  • Implementation detail            │
   │  • Pros & cons vs other methods     │
   └─────────────────────────────────────┘
```

## 💡 Design Principles

### 1. **Modularity**
- Setiap interpolation method terpisah
- Mudah menambah metode baru
- Interface yang konsisten (InterpolationBase)

### 2. **Extensibility**
```cpp
// Untuk menambah metode baru, tinggal:
class MyCustomInterpolation : public InterpolationBase {
    Pixel interpolate(...) override { /* implementation */ }
};
```

### 3. **Performance-Focused**
- Three implementations dengan trade-off berbeda
- Benchmark built-in
- Inline calculations untuk reduce overhead

### 4. **Well-Documented**
- Theory dan formula
- Code comments
- Practical examples
- Performance analysis

## 🚀 Build System

```
CMakeLists.txt (root)
├─ Includes
├─ Source files
├─ Compiler flags (-fopenmp, -O3)
├─ Linking libraries
│
├─ Executable: bilinear_example
├─ Executable: interpolation_benchmark
└─ Executable: test_suite
```

## 📊 Performance Characteristics

### Bilinear Interpolation (512×512 → 1024×1024)

```
Serial:     15 ms  (baseline)
OpenMP:     88 ms  (overhead problem)
Threading:   8 ms  ✓ BEST (1.875x faster)

Efficiency:
- On 4 cores: 46.9% (memory-bound)
- Bottleneck: Memory bandwidth (~50 GB/s needed)
- Improvement opportunities: SIMD, GPU
```

## 🔍 Key Learning Points

### Untuk memahami project ini:

1. **Pahami Image Representation**
   - Pixel sebagai unit dasar
   - 2D image sebagai vector<vector<Pixel>>
   - Channel (R, G, B, A) operations

2. **Interpolasi Konsep**
   - Mengapa perlu interpolasi
   - Weighted average principle
   - Trade-off: speed vs quality

3. **Bilinear Formula**
   - 4 neighbors
   - Fractional part calculation
   - Weight computation

4. **Parallelisasi**
   - Serial vs Parallel overhead
   - OpenMP pragma approach
   - Manual threading approach

5. **Performance Analysis**
   - Benchmarking methodology
   - Identifying bottlenecks
   - When to use each method

## 📈 Roadmap

```
✅ IMPLEMENTED:
  • Bilinear Interpolation (serial, OpenMP, threading)
  • Performance benchmarking
  • Comprehensive documentation

🔄 IN PROGRESS:
  • Bicubic interpolation
  • Bilinear tests

⏳ PLANNED:
  • Lanczos interpolation
  • SIMD vectorization
  • GPU acceleration (CUDA)
  • Advanced transforms (perspective, affine)
  • More filter operations
```

---

**Bilinear Interpolation adalah modul inti yang mendemonstrasikan best practices dalam:**
- Computer science fundamentals
- Parallel programming
- Performance optimization
- Software engineering practices

**Gunakan project ini sebagai learning resource dan foundation untuk studying image processing techniques.**
