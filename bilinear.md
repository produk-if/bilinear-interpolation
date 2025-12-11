# Bilinear Interpolation - Dokumentasi Lengkap

## 📍 Lokasi dalam Project

```
image-processing-project/
└── docs/interpolation/
    ├── overview.md          # Overview semua metode
    ├── bilinear.md          # FILE INI - Teori detail
    ├── bilinear.html        # Interactive documentation
    ├── bilinear.cpp         # Implementation (di src/)
    └── performance.md       # Performance analysis
```

## 🎯 Objective

Dokumentasi ini menjelaskan:
- ✅ Konsep teoretis Bilinear Interpolation
- ✅ Derivasi rumus matematika
- ✅ Implementasi praktis C++
- ✅ Tiga varian: Serial, OpenMP, Threading
- ✅ Benchmark dan perbandingan performa

---

## 1️⃣ KONSEP FUNDAMENTAL

### Apa itu Interpolasi?

Interpolasi adalah proses memperkirakan nilai suatu fungsi pada titik-titik yang **tidak diketahui** berdasarkan nilai-nilai yang diketahui.

Dalam konteks image processing:
- **Input:** Pixel pada posisi integer (0,0), (1,0), (0,1), (1,1), dst
- **Output:** Pixel value pada posisi float arbitrary (0.5, 0.7), dst

### Mengapa Perlu Interpolasi?

Ketika melakukan transformasi image (resize, rotate, warp), posisi pixel baru tidak selalu jatuh di posisi integer. Kita perlu mengestimasi nilai pixel pada posisi tersebut.

```
Contoh: Zoom gambar dari 512×512 menjadi 1024×1024
┌─────────────────────┐
│ Pixel lama          │  Koordinat sumber (float)
│ (0,0) ... (511,511) │  mapping ke target integer
└─────────────────────┘
                        ↓
        ┌──────────────────────────┐
        │ Pixel baru               │
        │ (0,0) ... (1023,1023)    │
        └──────────────────────────┘

Untuk setiap pixel baru (i,j):
src_x = i * (512 / 1024) = i * 0.5
src_y = j * (512 / 1024) = j * 0.5

src_x dan src_y adalah FLOAT, bukan integer!
Diperlukan interpolasi untuk menghitung nilai pixel.
```

---

## 2️⃣ BILINEAR INTERPOLATION - TEORI

### Konsep Dasar

Bilinear = Interpolasi pada dua dimensi (x dan y).

Untuk setiap pixel pada posisi (x, y) yang float:

1. **Cari 4 tetangga terdekat** yang memiliki posisi integer:
   - (x0, y0) - kiri atas
   - (x1, y0) - kanan atas  
   - (x0, y1) - kiri bawah
   - (x1, y1) - kanan bawah

2. **Hitung bobot interpolasi** berdasarkan jarak fraksi:
   - fx = x - floor(x) (jarak dari tepi kiri)
   - fy = y - floor(y) (jarak dari tepi atas)

3. **Kombinasikan 4 pixel** dengan weighted average:
   - Pixel lebih dekat → bobot lebih besar
   - Pixel lebih jauh → bobot lebih kecil

### Visualisasi Geometri

```
Koordinat (x, y) berada dalam cell:

y1 ┌──────────────────┐ y1
   │                  │
y  │    (x,y) ●       │    fy = y - floor(y)
   │                  │    (jarak dari atas)
y0 └──────────────────┘ y0
   x0        x        x1
            fx (jarak dari kiri)
            = x - floor(x)

Bobot untuk setiap corner:
   Top-Left  (x0,y0): w00 = (1-fx) * (1-fy)
   Top-Right (x1,y0): w10 = fx * (1-fy)
   Bot-Left  (x0,y1): w01 = (1-fx) * fy
   Bot-Right (x1,y1): w11 = fx * fy

Perhatikan: w00 + w10 + w01 + w11 = 1
           (Sum of weights = 1, normalized)
```

### Rumus Matematika

Untuk channel value f (bisa R, G, B, atau A):

```
f(x,y) = f(x0,y0) * w00 + f(x1,y0) * w10 + 
         f(x0,y1) * w01 + f(x1,y1) * w11

Dimana:
  w00 = (1 - fx) * (1 - fy)
  w10 = fx * (1 - fy)
  w01 = (1 - fx) * fy
  w11 = fx * fy

  fx = x - floor(x)
  fy = y - floor(y)
  x0 = floor(x)
  y0 = floor(y)
  x1 = x0 + 1
  y1 = y0 + 1
```

### Derivasi Bobot (Bilinear Basis Functions)

Interpolasi bilinear dapat diturunkan dari polynomial basis:

```
Linear basis untuk satu dimensi:
  ψ0(u) = 1 - u    (linear di-decrease dari kiri ke kanan)
  ψ1(u) = u        (linear di-increase dari kiri ke kanan)

Bilinear basis untuk dua dimensi:
  ψ00(u,v) = (1-u)(1-v)
  ψ10(u,v) = u(1-v)
  ψ01(u,v) = (1-u)v
  ψ11(u,v) = uv

Interpolated value:
  f(u,v) = Σ Σ f(i,j) * ψij(u,v)
          i j
```

---

## 3️⃣ IMPLEMENTASI C++

### File Organization

```
include/interpolation.h
├── Class InterpolationBase
│   ├── virtual interpolate()
│   └── virtual resize()
│
└── Class BilinearInterpolation : public InterpolationBase
    ├── Pixel interpolate(const Image&, float, float) override
    │
    ├── Image resize_serial(const Image&, int, int) const
    ├── Image resize_openmp(const Image&, int, int) const
    ├── Image resize_threading(const Image&, int, int) const
    │
    └── BenchmarkResult benchmark(...) const
```

### Header Declaration (include/interpolation.h)

```cpp
class BilinearInterpolation : public InterpolationBase {
private:
    inline float getValueSafe(const Image& img, int x, int y) const;
    
public:
    // Main interpolation function
    Pixel interpolate(const Image& image, float x, float y) const override;
    
    // Three implementations
    Image resize_serial(const Image& src, int new_height, int new_width) const;
    Image resize_openmp(const Image& src, int new_height, int new_width) const;
    Image resize_threading(const Image& src, int new_height, int new_width) const;
    
    // Benchmark
    struct BenchmarkResult { ... };
    BenchmarkResult benchmark(const Image& src, int new_height, int new_width,
                             int iterations = 1) const;
};
```

### Core Implementation - Single Interpolation

```cpp
Pixel BilinearInterpolation::interpolate(const Image& image, 
                                         float x, float y) const {
    // Step 1: Clamp coordinates
    x = max(0.0f, min(x, (float)image.getWidth() - 1.001f));
    y = max(0.0f, min(y, (float)image.getHeight() - 1.001f));
    
    // Step 2: Get integer coordinates
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int x1 = min(x0 + 1, image.getWidth() - 1);
    int y1 = min(y0 + 1, image.getHeight() - 1);
    
    // Step 3: Calculate fractional parts
    float fx = x - floor(x);
    float fy = y - floor(y);
    
    // Step 4: Get 4 pixel values
    Pixel f00 = image.getPixel(x0, y0);
    Pixel f10 = image.getPixel(x1, y0);
    Pixel f01 = image.getPixel(x0, y1);
    Pixel f11 = image.getPixel(x1, y1);
    
    // Step 5: Calculate weights
    float w00 = (1.0f - fx) * (1.0f - fy);
    float w10 = fx * (1.0f - fy);
    float w01 = (1.0f - fx) * fy;
    float w11 = fx * fy;
    
    // Step 6: Weighted sum
    Pixel result(
        f00.r * w00 + f10.r * w10 + f01.r * w01 + f11.r * w11,
        f00.g * w00 + f10.g * w10 + f01.g * w01 + f11.g * w11,
        f00.b * w00 + f10.b * w10 + f01.b * w01 + f11.b * w11,
        f00.a * w00 + f10.a * w10 + f01.a * w01 + f11.a * w11
    );
    
    return result;
}
```

### SERIAL VERSION (Baseline)

```cpp
Image BilinearInterpolation::resize_serial(const Image& src, 
                                           int new_height, 
                                           int new_width) const {
    Image dst(new_height, new_width);
    
    float scale_y = (float)src.getHeight() / new_height;
    float scale_x = (float)src.getWidth() / new_width;
    
    // Simple nested loop - no parallelization
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
```

### OPENMP VERSION (Pragma-Based)

```cpp
Image BilinearInterpolation::resize_openmp(const Image& src, 
                                           int new_height, 
                                           int new_width) const {
    Image dst(new_height, new_width);
    
    float scale_y = (float)src.getHeight() / new_height;
    float scale_x = (float)src.getWidth() / new_width;
    
    // Paralelisasi nested loop dengan pragma
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int j = 0; j < new_height; j++) {
        for (int i = 0; i < new_width; i++) {
            float src_x = i * scale_x;
            float src_y = j * scale_y;
            
            // Inline calculation untuk reduce function call overhead
            int x0 = (int)floor(src_x);
            int y0 = (int)floor(src_y);
            int x1 = min(x0 + 1, src.getWidth() - 1);
            int y1 = min(y0 + 1, src.getHeight() - 1);
            
            x0 = max(0, x0);
            y0 = max(0, y0);
            
            float fx = src_x - floor(src_x);
            float fy = src_y - floor(src_y);
            
            // Calculate weights and interpolate
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
```

Penjelasan pragma:
- `#pragma omp parallel for` - Paralelisasi loop
- `collapse(2)` - Merge dua nested loop menjadi satu
- `schedule(dynamic)` - Distribusi pekerjaan dinamis (overhead!)

### THREADING VERSION (Manual Thread Management)

```cpp
// Helper function untuk processing satu tile (baris images)
void resizeThreadTile(const Image& src, Image& dst,
                      int start_j, int end_j, int new_width,
                      float scale_x, float scale_y) {
    for (int j = start_j; j < end_j; j++) {
        for (int i = 0; i < new_width; i++) {
            // Same bilinear interpolation calculation as above
            // ...
        }
    }
}

// Main resize function
Image BilinearInterpolation::resize_threading(const Image& src, 
                                              int new_height, 
                                              int new_width) const {
    Image dst(new_height, new_width);
    
    float scale_y = (float)src.getHeight() / new_height;
    float scale_x = (float)src.getWidth() / new_width;
    
    // Determine number of threads
    int num_threads = thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    
    // Divide work by horizontal strips (rows)
    vector<thread> threads;
    int tile_height = new_height / num_threads;
    
    // Create one thread per strip
    for (int t = 0; t < num_threads; t++) {
        int start_j = t * tile_height;
        int end_j = (t == num_threads - 1) ? new_height : (t + 1) * tile_height;
        
        threads.emplace_back(resizeThreadTile, ref(src), ref(dst),
                           start_j, end_j, new_width, scale_x, scale_y);
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }
    
    return dst;
}
```

---

## 4️⃣ KARAKTERISTIK & PROPERTIES

### Continuity dan Smoothness

| Property | Value |
|----------|-------|
| Continuity | C⁰ (continuous) |
| Differentiability | Not differentiable at integer coordinates |
| Smoothness | Smooth within cells, piecewise linear |

### Computational Complexity

```
Per-pixel cost:
  - 4 memory access (ke 4 tetangga)
  - 8 multiplications (untuk weights)
  - 3 additions (untuk weighted sum)
  - Total: ~11 FLOPs per pixel per channel

Untuk image 1024×1024 × 3 channels:
  = 1024 * 1024 * 3 * 11
  ≈ 35 million FLOPs
```

### Memory Access Pattern

```
Sequential memory access:
  - Read dari (x0,y0): cache hit
  - Read dari (x1,y0): likely cache hit
  - Read dari (x0,y1): possible cache miss
  - Read dari (x1,y1): likely cache hit
  
→ Rata-rata cache hit rate: ~75-80%
```

---

## 5️⃣ ADVANTAGES & DISADVANTAGES

### ✅ Advantages

- **Balanced Performance** - Cepat dibanding bicubic/lanczos
- **Good Quality** - Jauh lebih baik dari nearest neighbor
- **Simple to Understand** - Formula straightforward
- **Stable** - Tidak ada oscillation artifacts
- **Memory Efficient** - Hanya perlu 4 pixel neighbors

### ❌ Disadvantages

- **Blocky for Large Zoom** - Visible artifacts pada zoom >2x
- **Not Smooth** - Tidak C¹ continuous (discontinuous derivative)
- **Aliasing Artifacts** - Dapat terjadi aliasing pada downsampling
- **Blurry on High Detail** - Blur effect pada texture detail

---

## 6️⃣ COMPARISON DENGAN METODE LAIN

### Nearest Neighbor vs Bilinear

```
Input: 4×4 image
Output: 8×8 (2x zoom)

Nearest Neighbor:
┌──┬──┐
│10│12│
├──┼──┤
│15│20│
└──┴──┘
    ↓
┌──┬──┬──┬──┐
│10│10│12│12│  Pixelated, blocky
├──┼──┼──┼──┤  (jarang dipakai kecuali
│10│10│12│12│   untuk pixel art)
├──┼──┼──┼──┤
│15│15│20│20│
├──┼──┼──┼──┤
│15│15│20│20│
└──┴──┴──┴──┘

Bilinear:
    ↓
┌──┬──┬──┬──┐
│10│11│12│12│  Smooth interpolation
├──┼──┼──┼──┤  (lebih natural)
│12│13│15│16│
├──┼──┼──┼──┤
│13│15│17│18│
├──┼──┼──┼──┤
│15│16│19│20│
└──┴──┴──┴──┘
```

### Bilinear vs Bicubic

```
Bilinear:
  - 4 neighbors (2×2)
  - Linear interpolation
  - C⁰ continuous
  - Visible sharp edges di boundaries

Bicubic:
  - 16 neighbors (4×4)
  - Cubic interpolation
  - C¹ continuous (smoother)
  - Lebih smooth tapi lebih lambat
```

---

## 7️⃣ PRACTICAL APPLICATIONS

### Image Resizing

```cpp
// Zoom in 2x menggunakan bilinear
Image original = Image::load("photo.jpg");  // 1920×1080
Image zoomed = original.resize(2160, 3840); // 4x pixel count
zoomed.save("zoomed.jpg");
```

### Texture Mapping (Graphics)

```cpp
// Ketika melakukan texture mapping dalam 3D graphics,
// koordinat texture sering float values.
// Bilinear interpolation digunakan untuk lookup.
for (int y = 0; y < screen_height; y++) {
    for (int x = 0; x < screen_width; x++) {
        float tex_x, tex_y;  // Float coordinates
        // ... calculate tex_x, tex_y from 3D projection ...
        
        Pixel texel = bi.interpolate(texture_map, tex_x, tex_y);
        screen[y][x] = texel;
    }
}
```

### Image Rotation

```cpp
// Rotate dan resize dengan bilinear
Image img = Image::load("input.jpg");
Image rotated = img.rotate(45.0, InterpolationMethod::BILINEAR);
rotated.save("rotated.jpg");
```

---

## 📖 References

1. **Gonzalez & Woods** - "Digital Image Processing"
   - Chapter 3: Image Enhancement
   - Section 3.4: Spatial Filtering

2. **Szeliski** - "Computer Vision: Algorithms and Applications"
   - Chapter 8: Image Alignment
   - Section 8.1: Geometric Image Transformations

3. **OpenGL Specification** - Texture Filtering
   - Linear Interpolation formulation

4. **Numerical Recipes**
   - Section: Interpolation and Extrapolation

---

Lanjut ke: `docs/interpolation/bilinear.html` untuk dokumentasi interaktif dengan visualisasi!
