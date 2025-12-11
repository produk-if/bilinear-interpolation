# Image Processing Library - C++ Project

Sebuah library comprehensive untuk digital image processing dengan fokus pada teknik interpolasi dan transformasi citra.

## 📁 Struktur Project

```
image-processing-project/
├── src/                      # Source code implementation
│   ├── core/
│   │   ├── image.cpp         # Image class implementation
│   │   └── pixel.cpp         # Pixel operations
│   ├── interpolation/
│   │   ├── nearest.cpp       # Nearest neighbor interpolation
│   │   ├── bilinear.cpp      # Bilinear interpolation (MODUL UTAMA)
│   │   ├── bicubic.cpp       # Bicubic interpolation
│   │   └── lanczos.cpp       # Lanczos interpolation
│   ├── transforms/
│   │   ├── resize.cpp        # Image resizing
│   │   ├── rotate.cpp        # Image rotation
│   │   └── warp.cpp          # Geometric warping
│   └── filters/
│       ├── gaussian.cpp      # Gaussian blur
│       ├── sobel.cpp         # Edge detection
│       └── median.cpp        # Median filter
│
├── include/                  # Header files
│   ├── image.h
│   ├── interpolation.h       # Include semua interpolation methods
│   ├── transforms.h
│   └── filters.h
│
├── docs/                     # Documentation
│   ├── ARCHITECTURE.md       # Design dan arsitektur
│   ├── interpolation/        # Dokumentasi spesifik interpolasi
│   │   ├── overview.md       # Overview semua metode
│   │   ├── bilinear.md       # Dokumentasi lengkap bilinear
│   │   ├── bilinear.html     # Dokumentasi interaktif
│   │   └── performance.md    # Analisis performa
│   └── API.md
│
├── tests/                    # Unit tests
│   ├── test_interpolation.cpp
│   ├── test_transforms.cpp
│   └── test_filters.cpp
│
├── examples/                 # Contoh penggunaan
│   ├── example_resize.cpp
│   ├── example_interpolation_comparison.cpp
│   └── example_bilinear_detailed.cpp
│
├── build/                    # Build output
│   └── CMakeLists.txt
│
└── CMakeLists.txt           # Build configuration
```

## 📚 Modul Utama: Interpolasi Bilinear

Modul ini adalah bagian dari sistem interpolasi yang lebih besar. Berikut adalah overview:

### Interpolasi Methods dalam Project

1. **Nearest Neighbor** - Tercepat, kualitas terendah
2. **Bilinear** ⭐ - Balance antara kecepatan dan kualitas
3. **Bicubic** - Kualitas lebih baik, lebih lambat
4. **Lanczos** - Kualitas terbaik, paling lambat

### Bilinear Interpolation

**Lokasi Source Code:**
- `src/interpolation/bilinear.cpp` - Implementation
- `include/interpolation.h` - Declaration
- `docs/interpolation/bilinear.md` - Teori dan rumus
- `docs/interpolation/bilinear.html` - Dokumentasi interaktif
- `examples/example_bilinear_detailed.cpp` - Contoh detail

**Fitur:**
- ✅ Serial implementation
- ✅ OpenMP parallelization
- ✅ C++ Threading implementation
- ✅ Performance benchmarking
- ✅ Unit tests
- ✅ Comprehensive documentation

## 🛠️ Build dan Kompilasi

```bash
cd image-processing-project
mkdir -p build
cd build
cmake ..
make
```

## 📖 Dokumentasi

### Overview Interpolasi
Baca: `docs/interpolation/overview.md`

### Bilinear Interpolation - Detail
Baca: `docs/interpolation/bilinear.md`
Buka: `docs/interpolation/bilinear.html` (di browser)

### Performance Analysis
Baca: `docs/interpolation/performance.md`

## 🧪 Testing

```bash
cd build
./test_interpolation
./test_transforms
```

## 💡 Contoh Penggunaan

### Basic Usage
```cpp
#include "image.h"
#include "interpolation.h"

int main() {
    Image img = Image::load("input.png");
    
    // Resize menggunakan bilinear interpolation
    Image resized = img.resize(800, 600, InterpolationMethod::BILINEAR);
    
    // Rotate menggunakan bilinear interpolation
    Image rotated = img.rotate(45.0, InterpolationMethod::BILINEAR);
    
    resized.save("output.png");
    return 0;
}
```

### Perbandingan Interpolation Methods
```cpp
#include "image.h"
#include "interpolation.h"

int main() {
    Image original("input.png");
    
    // Compare different interpolation methods
    Image nearest = original.resize(2048, 2048, InterpolationMethod::NEAREST);
    Image bilinear = original.resize(2048, 2048, InterpolationMethod::BILINEAR);
    Image bicubic = original.resize(2048, 2048, InterpolationMethod::BICUBIC);
    Image lanczos = original.resize(2048, 2048, InterpolationMethod::LANCZOS);
    
    // Save results
    nearest.save("nearest.png");
    bilinear.save("bilinear.png");
    bicubic.save("bicubic.png");
    lanczos.save("lanczos.png");
    
    return 0;
}
```

### Analisis Performa Bilinear
```cpp
#include "image.h"
#include "interpolation.h"
#include <iostream>

int main() {
    Image img = Image::load("input.png");
    BilinearInterpolator bi;
    
    // Performance comparison
    bi.benchmark_serial(img);
    bi.benchmark_openmp(img);
    bi.benchmark_threading(img);
    
    return 0;
}
```

## 📊 Hasil Performa (Bilinear Interpolation)

**Test Case:** 512×512 → 1024×1024

| Method | Time | Speedup | Status |
|--------|------|---------|--------|
| Serial | 15 ms | 1.0x | Baseline |
| OpenMP | 88 ms | 0.17x | ❌ Overhead |
| Threading | 8 ms | 1.875x | ✅ Best |

## 🎯 Fitur Roadmap

- [x] Nearest Neighbor interpolation
- [x] Bilinear interpolation
- [ ] Bicubic interpolation
- [ ] Lanczos interpolation
- [ ] SIMD optimization
- [ ] GPU acceleration (CUDA)
- [ ] Image filters (Gaussian, Sobel, Median)
- [ ] Advanced transforms (perspective, affine)

## 👨‍💻 Developer Notes

Modul interpolasi dirancang dengan:
- **Modularity**: Setiap metode interpolasi terpisah
- **Extensibility**: Mudah menambah metode baru
- **Performance**: Multiple implementation (serial, OpenMP, threading)
- **Testing**: Comprehensive unit tests
- **Documentation**: Teori, implementasi, dan performance analysis

Lihat `docs/ARCHITECTURE.md` untuk desain detail.

## 📝 License

Educational purposes

---

**Last Updated:** 2024
**Maintained by:** Image Processing Lab
