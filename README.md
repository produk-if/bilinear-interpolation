# 🖼️ Bilinear Interpolation

**Advanced Image Processing Algorithm Implementation**

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![GitHub](https://img.shields.io/badge/GitHub-devnolife-black.svg)](https://github.com/devnolife)

---

## 👤 Author Information

**Andi Agung Dwi Arya**  
- 🎓 **Program**: Algoritma Komputasi S2 (Master's Degree)
- 💻 **GitHub**: [@devnolife](https://github.com/devnolife)
- 📅 **Year**: 2025
- 🏫 **Project**: Implementasi Bilinear Interpolation untuk Image Processing

---

## 📋 Table of Contents

- [Tentang Project](#-tentang-project)
- [Apa itu Bilinear Interpolation?](#-apa-itu-bilinear-interpolation)
- [Fitur](#-fitur)
- [Struktur Project](#-struktur-project)
- [Cara Instalasi](#-cara-instalasi)
- [Cara Penggunaan](#-cara-penggunaan)
- [Penjelasan Algoritma](#-penjelasan-algoritma)
- [Contoh Output](#-contoh-output)
- [Dokumentasi](#-dokumentasi)
- [Teknologi yang Digunakan](#-teknologi-yang-digunakan)
- [Kontribusi](#-kontribusi)
- [Lisensi](#-lisensi)

---

## 🎯 Tentang Project

Project ini merupakan implementasi lengkap **Bilinear Interpolation** dalam C++ untuk keperluan image processing. Bilinear interpolation adalah teknik fundamental dalam computer vision yang digunakan untuk:

- ✅ **Image Resizing** - Memperbesar atau memperkecil gambar
- ✅ **Image Rotation** - Rotasi gambar dengan hasil smooth
- ✅ **Image Warping** - Transformasi geometri gambar
- ✅ **Texture Mapping** - Mapping texture pada 3D graphics

Project ini dibuat untuk:
1. **Pendidikan** - Memahami algoritma interpolasi secara mendalam
2. **Implementasi** - Kode C++ yang clean, terstruktur, dan terdokumentasi
3. **Performance** - Optimasi dengan parallelization (OpenMP)

---

## 🔍 Apa itu Bilinear Interpolation?

**Bilinear Interpolation** adalah metode untuk menentukan nilai pada koordinat non-integer dengan menggunakan **4 pixel tetangga terdekat**.

### Konsep Dasar

```
    x0              x1
    │               │
y0 ─┼───────────────┼─
    │ f00       f10 │
    │       ●       │  ← Target point P(x,y)
    │ f01       f11 │
y1 ─┼───────────────┼─
```

**Formula:**
```
f(x,y) = f00·(1-fx)·(1-fy) + f10·fx·(1-fy) + 
         f01·(1-fx)·fy     + f11·fx·fy
```

Dimana:
- `fx = x - floor(x)` → jarak horizontal dari tepi kiri
- `fy = y - floor(y)` → jarak vertical dari tepi atas
- `f00, f10, f01, f11` → nilai 4 pixel tetangga

### Kenapa "Bilinear"?

- **Bi** = 2 dimensi (X dan Y)
- **Linear** = Interpolasi linear pada kedua arah

Algoritma ini:
1. Interpolasi linear pada arah X (horizontal)
2. Interpolasi linear pada arah Y (vertical)

---

## ✨ Fitur

### 🎨 Implementasi Lengkap
- ✅ **Single File C++** - Tidak perlu file header terpisah
- ✅ **Komentar Detail** - Setiap baris dijelaskan dengan jelas
- ✅ **Step-by-Step Demo** - Visualisasi proses perhitungan
- ✅ **Interactive Menu** - User-friendly console interface

### ⚡ Performance
- ✅ **Serial Version** - Single-threaded untuk pembelajaran
- ✅ **OpenMP Version** - Multi-threaded untuk gambar besar
- ✅ **Optimized** - Efficient memory access pattern

### 📊 Demo & Visualization
- ✅ **ASCII Grid Display** - Visualisasi gambar dalam terminal
- ✅ **Mathematical Breakdown** - Penjelasan perhitungan step-by-step
- ✅ **Custom Coordinates** - Test dengan koordinat bebas
- ✅ **Image Resize Demo** - Contoh resize 4×4 → 6×6

---

## 📁 Struktur Project

```
interpolasi/
│
├── 📄 README.md                    # Dokumentasi utama (file ini)
├── 📄 LICENSE                      # MIT License
│
├── 📂 src/                         # Source code
│   ├── bilinear_interpolation.cpp # ⭐ Main program (ALL-IN-ONE)
│   └── bilinear.cpp                # Original implementation
│
├── 📂 demo/                        # Demo programs
│   ├── console/                    # Console-based demos
│   │   ├── main.cpp
│   │   └── bilinear_demo.exe
│   └── web/                        # Web-based visualization
│       └── index.html
│
├── 📂 examples/                    # Code examples
│   └── bilinear_example.cpp
│
├── 📂 docs/                        # Documentation
│   ├── overview.md                 # Algorithm overview
│   ├── bilinear.md                 # Bilinear details
│   └── ARCHITECTURE.md             # Code architecture
│
└── 📂 assets/                      # Images, diagrams, etc.
```

---

## 🚀 Cara Instalasi

### Prerequisites

Pastikan kamu sudah install:
- **C++ Compiler** (GCC/MinGW/MSVC)
- **Git** (optional, untuk clone repository)

### Step 1: Clone Repository

```bash
git clone https://github.com/devnolife/interpolasi.git
cd interpolasi
```

### Step 2: Compile Program

**Windows (MinGW):**
```bash
cd src
g++ -o bilinear_demo.exe bilinear_interpolation.cpp -std=c++17
```

**Linux/Mac:**
```bash
cd src
g++ -o bilinear_demo bilinear_interpolation.cpp -std=c++17
```

**Dengan OpenMP (untuk parallel processing):**
```bash
g++ -fopenmp -o bilinear_demo bilinear_interpolation.cpp -std=c++17
```

### Step 3: Jalankan Program

**Windows:**
```bash
bilinear_demo.exe
```

**Linux/Mac:**
```bash
./bilinear_demo
```

---

## 💻 Cara Penggunaan

### Menu Utama

Setelah menjalankan program, kamu akan melihat menu:

```
┌─────────────────────────────────────┐
│            MENU UTAMA               │
├─────────────────────────────────────┤
│  [1] Demo Interpolasi Single Pixel  │
│  [2] Demo Resize Gambar             │
│  [3] Input Koordinat Custom         │
│  [0] Keluar                         │
└─────────────────────────────────────┘
```

### Option 1: Demo Interpolasi Single Pixel

Menampilkan perhitungan step-by-step untuk koordinat (1.5, 1.5):

```
LANGKAH 1: Tentukan 4 pixel tetangga
─────────────────────────────────────
  x0 = floor(1.50) = 1
  y0 = floor(1.50) = 1
  x1 = x0 + 1 = 2
  y1 = y0 + 1 = 2

LANGKAH 2: Ambil nilai 4 tetangga
─────────────────────────────────────
  f00 = image[1][1] = 60 (Kiri-Atas)
  f10 = image[2][1] = 80 (Kanan-Atas)
  f01 = image[1][2] = 90 (Kiri-Bawah)
  f11 = image[2][2] = 110 (Kanan-Bawah)

...
```

### Option 2: Demo Resize Gambar

Resize gambar dari 4×4 menjadi 6×6:

```
SEBELUM (4×4):
┌─────────────────────────┐
│  10   30   50   70      │
│  40   60   80  100      │
│  70   90  110  130      │
│ 100  120  140  160      │
└─────────────────────────┘

SESUDAH (6×6):
┌─────────────────────────────────┐
│  10   17   30   43   50   60    │
│  22   30   43   57   63   73    │
│  40   48   60   73   80   90    │
│  58   67   78   90   97  107    │
│  70   78   90  103  110  120    │
│  85   93  105  118  125  135    │
└─────────────────────────────────┘
```

### Option 3: Input Koordinat Custom

Test dengan koordinat kustom:

```
Masukkan koordinat X (0.0 - 2.99): 2.3
Masukkan koordinat Y (0.0 - 2.99): 1.7

[Akan menampilkan perhitungan lengkap untuk (2.3, 1.7)]
```

---

## 📐 Penjelasan Algoritma

### Konsep Matematis

Bilinear interpolation menghitung nilai pada titik P(x,y) menggunakan **weighted average** dari 4 tetangga:

#### 1️⃣ **Tentukan 4 Tetangga**

```cpp
int x0 = floor(x);    // Koordinat kiri
int y0 = floor(y);    // Koordinat atas
int x1 = x0 + 1;      // Koordinat kanan
int y1 = y0 + 1;      // Koordinat bawah
```

#### 2️⃣ **Hitung Fraksi**

```cpp
float fx = x - floor(x);  // Jarak relatif horizontal (0-1)
float fy = y - floor(y);  // Jarak relatif vertical (0-1)
```

#### 3️⃣ **Hitung Bobot**

Bobot dihitung berdasarkan **luas area diagonal**:

```cpp
float w00 = (1-fx) * (1-fy);  // Area diagonal ke (x1,y1)
float w10 = fx * (1-fy);      // Area diagonal ke (x0,y1)
float w01 = (1-fx) * fy;      // Area diagonal ke (x1,y0)
float w11 = fx * fy;          // Area diagonal ke (x0,y0)
```

**Properti penting:** `w00 + w10 + w01 + w11 = 1.0`

#### 4️⃣ **Weighted Sum**

```cpp
result = f00*w00 + f10*w10 + f01*w01 + f11*w11
```

### Visualisasi Geometri

```
     (x0,y0)────────────(x1,y0)
        │                  │
        │    w11 │  w01    │
        │   ─────P─────    │
        │    w10 │  w00    │
        │                  │
     (x0,y1)────────────(x1,y1)

Bobot berdasarkan luas:
- w00 = luas persegi hijau (diagonal dari P ke kanan-bawah)
- w10 = luas persegi biru (diagonal dari P ke kiri-bawah)
- w01 = luas persegi merah (diagonal dari P ke kanan-atas)
- w11 = luas persegi kuning (diagonal dari P ke kiri-atas)
```

### Contoh Numerik

**Input:**
- Target: P(1.5, 1.5)
- f00 = 60, f10 = 80, f01 = 90, f11 = 110

**Perhitungan:**
```
fx = 1.5 - 1 = 0.5
fy = 1.5 - 1 = 0.5

w00 = (1-0.5)(1-0.5) = 0.25
w10 = 0.5(1-0.5)     = 0.25
w01 = (1-0.5)0.5     = 0.25
w11 = 0.5×0.5        = 0.25

result = 60×0.25 + 80×0.25 + 90×0.25 + 110×0.25
       = 15 + 20 + 22.5 + 27.5
       = 85
```

**Output:** `f(1.5, 1.5) = 85` ✅

---

## 📸 Contoh Output

### Terminal Output

```
╔═══════════════════════════════════════════════════════════════════════╗
║                                                                       ║
║               BILINEAR INTERPOLATION DEMO                             ║
║                   Image Processing Algorithm                          ║
║                                                                       ║
╠═══════════════════════════════════════════════════════════════════════╣
║  Author  : Andi Agung Dwi Arya                                        ║
║  GitHub  : devnolife                                                  ║
║  Course  : Algoritma Komputasi S2 - 2025                              ║
╚═══════════════════════════════════════════════════════════════════════╝

GAMBAR ASLI (4x4):
┌───────────────────────────────┐
│   10    30    50    70        │
│   40    60    80   100        │
│   70    90   110   130        │
│  100   120   140   160        │
└───────────────────────────────┘
```

---

## 📚 Dokumentasi

Dokumentasi lengkap tersedia di folder `docs/`:

- **[overview.md](docs/overview.md)** - Overview algoritma interpolasi
- **[bilinear.md](docs/bilinear.md)** - Detail bilinear interpolation
- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Arsitektur kode

---

## 🛠️ Teknologi yang Digunakan

- **Language:** C++17
- **Compiler:** GCC 15.2.0 (MinGW-w64)
- **Parallelization:** OpenMP (optional)
- **Build System:** Manual compilation
- **Version Control:** Git

---

## 🤝 Kontribusi

Kontribusi sangat diterima! Silakan:

1. Fork repository ini
2. Buat branch feature (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push ke branch (`git push origin feature/amazing-feature`)
5. Buka Pull Request

---

## 📝 Lisensi

Project ini dilisensikan di bawah **MIT License** - lihat file [LICENSE](LICENSE) untuk detail.

---

## 📧 Kontak

**Andi Agung Dwi Arya**

- GitHub: [@devnolife](https://github.com/devnolife)
- Email: [Your Email]
- LinkedIn: [Your LinkedIn]

---

## 🌟 Acknowledgments

- Algoritma Komputasi S2 - 2025
- Referensi: Digital Image Processing (Gonzalez & Woods)
- Inspiration: Computer Vision algorithms

---

<div align="center">

**⭐ Jangan lupa Star repository ini jika bermanfaat! ⭐**

Made with ❤️ by [Andi Agung Dwi Arya](https://github.com/devnolife)

</div>
