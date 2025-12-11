# Image Processing - Interpolation Methods Overview

## 📚 Pengantar

Modul **Interpolation** adalah bagian integral dari Image Processing Library yang menyediakan berbagai teknik untuk menginterpolasi nilai pixel ketika melakukan transformasi geometri seperti resize, rotate, atau warp.

## 🎯 Interpolation Methods dalam Project

Project ini menyediakan 4 metode interpolasi dengan berbagai trade-off antara kecepatan dan kualitas:

### 1. Nearest Neighbor (Fastest, Lowest Quality)
- **Waktu:** Sangat cepat
- **Kualitas:** Rendah - terlihat pixelated
- **Use case:** Real-time preview, low-resolution operations
- **Lokasi:** `src/interpolation/nearest.cpp`

```
Concept:
┌─────────┬─────────┐
│  10     │  12     │ Select nilai pixel terdekat
└─────────┴─────────┘  tanpa interpolasi
│  15     │  20     │
└─────────┴─────────┘
```

### 2. **BILINEAR** ⭐ (Recommended Balance)
- **Waktu:** Medium
- **Kualitas:** Good - balance antara speed dan quality
- **Use case:** Most general-purpose image resizing
- **Lokasi:** `src/interpolation/bilinear.cpp`
- **Status:** MODUL UTAMA PROJECT INI

```
Concept:
┌─────────┬─────────┐
│ f00 (10)│ f10 (12)│ Weighted average dari
├─────────┼─────────┤ 4 tetangga berdasarkan
│ f01 (15)│ f11 (20)│ jarak fraksi
└─────────┴─────────┘

f(x,y) = f00*w00 + f10*w10 + f01*w01 + f11*w11
```

### 3. Bicubic (Higher Quality, Slower)
- **Waktu:** Lebih lambat
- **Kualitas:** Lebih baik - smoother result
- **Use case:** Professional image resizing, photography
- **Lokasi:** `src/interpolation/bicubic.cpp`

```
Concept:
 16 neighbors (4x4) dengan cubic basis function
```

### 4. Lanczos (Best Quality, Slowest)
- **Waktu:** Paling lambat
- **Kualitas:** Terbaik - windowed sinc function
- **Use case:** High-quality resizing, archival
- **Lokasi:** `src/interpolation/lanczos.cpp`

```
Concept:
Menggunakan windowed sinc function dengan configurable window size
```

## 📊 Perbandingan Tabel

| Method | Speed | Quality | Recommended For |
|--------|-------|---------|-----------------|
| Nearest | ⚡⚡⚡ | ⭐ | Quick preview |
| **Bilinear** | ⚡⚡ | ⭐⭐⭐ | General purpose |
| Bicubic | ⚡ | ⭐⭐⭐⭐ | Professional |
| Lanczos | 🐢 | ⭐⭐⭐⭐⭐ | Best quality |

## 🏗️ Arsitektur

Semua interpolation methods mengikuti pattern yang sama:

```
┌─────────────────────────────────────┐
│   InterpolationBase (Abstract)       │
│  - interpolate()                    │
│  - resize()                         │
└──────────────┬──────────────────────┘
               │
      ┌────────┼────────┬─────────┐
      │        │        │         │
      ▼        ▼        ▼         ▼
    Nearest  Bilinear Bicubic  Lanczos
```

## 📄 File Structure

```
src/interpolation/
├── nearest.cpp          # Nearest Neighbor implementation
├── bilinear.cpp         # BILINEAR INTERPOLATION (MODUL UTAMA)
├── bicubic.cpp         # Bicubic implementation
└── lanczos.cpp         # Lanczos implementation

include/
└── interpolation.h      # All declarations

docs/interpolation/
├── overview.md         # File ini
├── bilinear.md         # Detailed theory & formulas
├── bilinear.html       # Interactive documentation
└── performance.md      # Performance analysis
```

## 🔗 Relationship dalam Project

```
Image Processing Library
│
├── Core Module (Image class, Pixel operations)
│
├── ⭐ INTERPOLATION MODULE ⭐
│   ├── NearestNeighbor
│   ├── Bilinear          <-- FOKUS UTAMA
│   ├── Bicubic
│   └── Lanczos
│
├── Transform Module (menggunakan Interpolation)
│   ├── Resize (image resizing)
│   ├── Rotate (image rotation)
│   └── Warp (geometric warping)
│
└── Filter Module
    ├── Gaussian blur
    ├── Edge detection
    └── Median filter
```

## 🚀 Cara Menggunakan

### Basic Usage - Gunakan Default (Bilinear)

```cpp
#include "image.h"
#include "interpolation.h"

int main() {
    Image img = Image::load("input.png");
    
    // Default menggunakan BILINEAR
    Image resized = img.resize(800, 600);
    
    resized.save("output.png");
    return 0;
}
```

### Advanced - Explicit Method Selection

```cpp
#include "image.h"
#include "interpolation.h"

int main() {
    Image img = Image::load("input.png");
    
    // Pilih metode interpolasi spesifik
    Image nearest = img.resize(800, 600, 
                               InterpolationMethod::NEAREST);
    Image bilinear = img.resize(800, 600, 
                                InterpolationMethod::BILINEAR);
    Image bicubic = img.resize(800, 600, 
                               InterpolationMethod::BICUBIC);
    Image lanczos = img.resize(800, 600, 
                               InterpolationMethod::LANCZOS);
    
    return 0;
}
```

### Compare All Methods - Bilinear Benchmark

```cpp
#include "image.h"
#include "interpolation.h"

int main() {
    Image img = Image::load("input.png");
    
    interpolation::BilinearInterpolation bi;
    auto result = bi.benchmark(img, 2048, 2048, 3);
    
    // result.best_method akan berisi "Threading" 
    // (berdasarkan performance analysis)
    
    return 0;
}
```

## 📖 Dokumentasi Detail

### Untuk Bilinear Interpolation:

1. **Teori dan Formula** → Baca: `docs/interpolation/bilinear.md`
2. **Implementasi Detail** → Lihat: `src/interpolation/bilinear.cpp`
3. **Interactive Docs** → Buka: `docs/interpolation/bilinear.html` di browser
4. **Performance Analysis** → Baca: `docs/interpolation/performance.md`

### Untuk Metode Lain:

- Nearest Neighbor → `docs/interpolation/nearest.md`
- Bicubic → `docs/interpolation/bicubic.md`
- Lanczos → `docs/interpolation/lanczos.md`

## 🧪 Testing Interpolation

```bash
cd build
cmake ..
make

# Run interpolation tests
./test_interpolation

# Run bilinear benchmark
./example_bilinear_detailed
```

## 🎯 Next Steps

1. **Pahami Bilinear** → Baca teori di `docs/interpolation/bilinear.md`
2. **Lihat Implementation** → Examine `src/interpolation/bilinear.cpp`
3. **Run Examples** → Jalankan `examples/example_bilinear_detailed.cpp`
4. **Benchmark** → Lihat performa di `docs/interpolation/performance.md`
5. **Explore Others** → Pelajari metode lain setelah bilinear

## 📚 References

- "Digital Image Processing" - Gonzalez & Woods
- "Computer Vision: Algorithms and Applications" - Szeliski
- "The Art of Computer Graphics" - Watt & Watt

---

**Bilinear Interpolation adalah modul UTAMA yang menunjukkan konsep fundamental dari image interpolation. Metode ini menjadi baseline untuk memahami teknik interpolasi yang lebih advanced.**
