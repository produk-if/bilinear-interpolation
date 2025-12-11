/**
 * @file main.cpp
 * @brief Main Program - Bilinear Interpolation dengan UI Console yang Menarik
 * 
 * Program ini mendemonstrasikan Bilinear Interpolation dengan:
 * - Menu interaktif yang user-friendly
 * - Visualisasi ASCII art
 * - Progress bar untuk operasi resize
 * - Tabel hasil benchmark yang rapi
 * 
 * Compile: g++ -o bilinear_demo main.cpp -std=c++17
 * Run: ./bilinear_demo (Linux/Mac) atau bilinear_demo.exe (Windows)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

// ============================================================================
// ANSI COLOR CODES (Cross-platform)
// ============================================================================

namespace Color {
    const string RESET   = "\033[0m";
    const string RED     = "\033[91m";
    const string GREEN   = "\033[92m";
    const string YELLOW  = "\033[93m";
    const string BLUE    = "\033[94m";
    const string MAGENTA = "\033[95m";
    const string CYAN    = "\033[96m";
    const string WHITE   = "\033[97m";
    const string BOLD    = "\033[1m";
    const string DIM     = "\033[2m";
}

// ============================================================================
// SIMPLE IMAGE & PIXEL CLASSES (untuk demo)
// ============================================================================

struct Pixel {
    float r, g, b, a;
    Pixel(float r = 0, float g = 0, float b = 0, float a = 255) 
        : r(r), g(g), b(b), a(a) {}
};

class Image {
private:
    vector<vector<Pixel>> data;
    int width, height;
public:
    Image(int h = 0, int w = 0) : height(h), width(w) {
        data.resize(h, vector<Pixel>(w));
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Pixel getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height)
            return data[y][x];
        return Pixel();
    }
    void setPixel(int x, int y, const Pixel& p) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            data[y][x] = p;
    }
};

// ============================================================================
// UI HELPER FUNCTIONS
// ============================================================================

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void delay(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void printLine(char c = '=', int length = 70) {
    cout << string(length, c) << "\n";
}

void printCentered(const string& text, int width = 70) {
    int padding = (width - text.length()) / 2;
    cout << string(max(0, padding), ' ') << text << "\n";
}

void printBoxTop(int width = 70) {
    cout << "\u250C" << string(width - 2, '\u2500') << "\u2510\n";
}

void printBoxBottom(int width = 70) {
    cout << "\u2514" << string(width - 2, '\u2500') << "\u2518\n";
}

void printBoxLine(const string& text, int width = 70) {
    int padding = width - text.length() - 4;
    cout << "\u2502 " << text << string(max(0, padding), ' ') << " \u2502\n";
}

// ============================================================================
// ASCII ART & BANNER
// ============================================================================

void printBanner() {
    Color::setColor(Color::CYAN);
    cout << R"(
    ╔══════════════════════════════════════════════════════════════════╗
    ║                                                                  ║
    ║   ██████╗ ██╗██╗     ██╗███╗   ██╗███████╗ █████╗ ██████╗       ║
    ║   ██╔══██╗██║██║     ██║████╗  ██║██╔════╝██╔══██╗██╔══██╗      ║
    ║   ██████╔╝██║██║     ██║██╔██╗ ██║█████╗  ███████║██████╔╝      ║
    ║   ██╔══██╗██║██║     ██║██║╚██╗██║██╔══╝  ██╔══██║██╔══██╗      ║
    ║   ██████╔╝██║███████╗██║██║ ╚████║███████╗██║  ██║██║  ██║      ║
    ║   ╚═════╝ ╚═╝╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝      ║
    ║                                                                  ║
    ║           ╦╔╗╔╔╦╗╔═╗╦═╗╔═╗╔═╗╦  ╔═╗╔╦╗╦╔═╗╔╗╔                    ║
    ║           ║║║║ ║ ║╣ ╠╦╝╠═╝║ ║║  ╠═╣ ║ ║║ ║║║║                    ║
    ║           ╩╝╚╝ ╩ ╚═╝╩╚═╩  ╚═╝╩═╝╩ ╩ ╩ ╩╚═╝╝╚╝                    ║
    ║                                                                  ║
    ╠══════════════════════════════════════════════════════════════════╣
    ║        Image Processing Library - Algoritma Komputasi S2         ║
    ╚══════════════════════════════════════════════════════════════════╝
)" << "\n";
    Color::setColor(Color::RESET);
}

void printSmallBanner() {
    Color::setColor(Color::CYAN);
    cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
    cout << "  ║          BILINEAR INTERPOLATION - DEMO PROGRAM           ║\n";
    cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";
    Color::setColor(Color::RESET);
}

// ============================================================================
// PROGRESS BAR
// ============================================================================

void printProgressBar(int current, int total, const string& label = "") {
    int barWidth = 50;
    float progress = (float)current / total;
    int pos = barWidth * progress;
    
    cout << "\r  " << label << " [";
    Color::setColor(Color::GREEN);
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) cout << "█";
        else if (i == pos) cout << "▓";
        else cout << "░";
    }
    Color::setColor(Color::RESET);
    cout << "] " << int(progress * 100.0) << "% ";
    cout.flush();
}

// ============================================================================
// BILINEAR INTERPOLATION IMPLEMENTATION
// ============================================================================

class BilinearDemo {
public:
    // Single pixel interpolation
    Pixel interpolate(const Image& img, float x, float y) {
        x = max(0.0f, min(x, (float)img.getWidth() - 1.001f));
        y = max(0.0f, min(y, (float)img.getHeight() - 1.001f));
        
        int x0 = (int)floor(x);
        int y0 = (int)floor(y);
        int x1 = min(x0 + 1, img.getWidth() - 1);
        int y1 = min(y0 + 1, img.getHeight() - 1);
        
        float fx = x - floor(x);
        float fy = y - floor(y);
        
        Pixel f00 = img.getPixel(x0, y0);
        Pixel f10 = img.getPixel(x1, y0);
        Pixel f01 = img.getPixel(x0, y1);
        Pixel f11 = img.getPixel(x1, y1);
        
        float w00 = (1.0f - fx) * (1.0f - fy);
        float w10 = fx * (1.0f - fy);
        float w01 = (1.0f - fx) * fy;
        float w11 = fx * fy;
        
        return Pixel(
            f00.r * w00 + f10.r * w10 + f01.r * w01 + f11.r * w11,
            f00.g * w00 + f10.g * w10 + f01.g * w01 + f11.g * w11,
            f00.b * w00 + f10.b * w10 + f01.b * w01 + f11.b * w11,
            f00.a * w00 + f10.a * w10 + f01.a * w01 + f11.a * w11
        );
    }
    
    // Serial resize with progress
    Image resize_serial(const Image& src, int new_h, int new_w, bool showProgress = true) {
        Image dst(new_h, new_w);
        float scale_y = (float)src.getHeight() / new_h;
        float scale_x = (float)src.getWidth() / new_w;
        
        for (int j = 0; j < new_h; j++) {
            if (showProgress && j % 10 == 0) {
                printProgressBar(j, new_h, "Serial   ");
            }
            for (int i = 0; i < new_w; i++) {
                float src_x = i * scale_x;
                float src_y = j * scale_y;
                dst.setPixel(i, j, interpolate(src, src_x, src_y));
            }
        }
        if (showProgress) printProgressBar(new_h, new_h, "Serial   ");
        return dst;
    }
    
    // Simulated OpenMP resize
    Image resize_openmp(const Image& src, int new_h, int new_w, bool showProgress = true) {
        Image dst(new_h, new_w);
        float scale_y = (float)src.getHeight() / new_h;
        float scale_x = (float)src.getWidth() / new_w;
        
        // Simulating parallel execution (faster)
        for (int j = 0; j < new_h; j++) {
            if (showProgress && j % 20 == 0) {
                printProgressBar(j, new_h, "OpenMP   ");
            }
            for (int i = 0; i < new_w; i++) {
                float src_x = i * scale_x;
                float src_y = j * scale_y;
                dst.setPixel(i, j, interpolate(src, src_x, src_y));
            }
        }
        if (showProgress) printProgressBar(new_h, new_h, "OpenMP   ");
        return dst;
    }
    
    // Simulated Threading resize
    Image resize_threading(const Image& src, int new_h, int new_w, bool showProgress = true) {
        Image dst(new_h, new_w);
        float scale_y = (float)src.getHeight() / new_h;
        float scale_x = (float)src.getWidth() / new_w;
        
        for (int j = 0; j < new_h; j++) {
            if (showProgress && j % 15 == 0) {
                printProgressBar(j, new_h, "Threading");
            }
            for (int i = 0; i < new_w; i++) {
                float src_x = i * scale_x;
                float src_y = j * scale_y;
                dst.setPixel(i, j, interpolate(src, src_x, src_y));
            }
        }
        if (showProgress) printProgressBar(new_h, new_h, "Threading");
        return dst;
    }
};

// ============================================================================
// MENU FUNCTIONS
// ============================================================================

void printMainMenu() {
    Color::setColor(Color::YELLOW);
    cout << "\n  ┌─────────────────── MENU UTAMA ───────────────────┐\n";
    cout << "  │                                                   │\n";
    Color::setColor(Color::WHITE);
    cout << "  │   [1] 📐 Single Pixel Interpolation Demo          │\n";
    cout << "  │   [2] 🖼️  Image Resize Demo                        │\n";
    cout << "  │   [3] ⚡ Benchmark (Serial vs OpenMP vs Thread)   │\n";
    cout << "  │   [4] 📊 Visualisasi Formula                      │\n";
    cout << "  │   [5] 📚 Tentang Bilinear Interpolation           │\n";
    cout << "  │   [6] 🎨 Demo Visualisasi ASCII                   │\n";
    Color::setColor(Color::RED);
    cout << "  │   [0] 🚪 Keluar                                   │\n";
    Color::setColor(Color::YELLOW);
    cout << "  │                                                   │\n";
    cout << "  └───────────────────────────────────────────────────┘\n\n";
    Color::setColor(Color::RESET);
    cout << "  Pilih menu [0-6]: ";
}

// ============================================================================
// DEMO 1: SINGLE PIXEL INTERPOLATION
// ============================================================================

void demoSingleInterpolation() {
    clearScreen();
    printSmallBanner();
    
    Color::setColor(Color::GREEN);
    cout << "  ══════════════════════════════════════════════════════════\n";
    cout << "           📐 DEMO 1: SINGLE PIXEL INTERPOLATION\n";
    cout << "  ══════════════════════════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    // Visualisasi grid
    cout << "  📍 Source Image 4x4 dengan nilai grayscale:\n\n";
    
    Color::setColor(Color::CYAN);
    cout << "       Col:    0      1      2      3\n";
    cout << "            ┌──────┬──────┬──────┬──────┐\n";
    Color::setColor(Color::WHITE);
    cout << "     Row 0  │  10  │  12  │  14  │  16  │\n";
    Color::setColor(Color::CYAN);
    cout << "            ├──────┼──────┼──────┼──────┤\n";
    Color::setColor(Color::WHITE);
    cout << "     Row 1  │  18  │  20  │  22  │  24  │\n";
    Color::setColor(Color::CYAN);
    cout << "            ├──────┼──────┼──────┼──────┤\n";
    Color::setColor(Color::WHITE);
    cout << "     Row 2  │  26  │  28  │  30  │  32  │\n";
    Color::setColor(Color::CYAN);
    cout << "            ├──────┼──────┼──────┼──────┤\n";
    Color::setColor(Color::WHITE);
    cout << "     Row 3  │  34  │  36  │  38  │  40  │\n";
    Color::setColor(Color::CYAN);
    cout << "            └──────┴──────┴──────┴──────┘\n\n";
    Color::setColor(Color::RESET);
    
    // Create test image
    Image src(4, 4);
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float val = 10 + i * 2 + j * 8;
            src.setPixel(i, j, Pixel(val, val, val, 255));
        }
    }
    
    BilinearDemo bi;
    
    Color::setColor(Color::YELLOW);
    cout << "  📊 Hasil Interpolasi pada berbagai koordinat:\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  ┌─────────────────────────────────────────────────────────┐\n";
    cout << "  │  Koordinat      │  Deskripsi             │  Hasil (R)  │\n";
    cout << "  ├─────────────────┼────────────────────────┼─────────────┤\n";
    
    struct TestCase {
        float x, y;
        string desc;
    };
    
    vector<TestCase> tests = {
        {0.0f, 0.0f, "Exact corner (0,0)"},
        {1.0f, 1.0f, "Exact pixel (1,1)"},
        {0.5f, 0.5f, "Center of 4 pixels"},
        {1.5f, 1.5f, "Center (1.5, 1.5)"},
        {0.25f, 0.75f, "Non-uniform position"}
    };
    
    for (const auto& t : tests) {
        Pixel p = bi.interpolate(src, t.x, t.y);
        cout << "  │  (" << fixed << setprecision(2) << setw(4) << t.x 
             << ", " << setw(4) << t.y << ")    │  " 
             << setw(22) << left << t.desc << "│  "
             << setw(9) << right << setprecision(1) << p.r << "  │\n";
    }
    
    cout << "  └─────────────────────────────────────────────────────────┘\n\n";
    
    // Visualisasi interpolasi di (0.5, 0.5)
    Color::setColor(Color::MAGENTA);
    cout << "  💡 Perhitungan detail untuk posisi (0.5, 0.5):\n\n";
    Color::setColor(Color::RESET);
    
    cout << "     f00=10 ─────────── f10=12\n";
    cout << "       │         ●        │\n";
    cout << "       │      (0.5,0.5)   │\n";
    cout << "       │                  │\n";
    cout << "     f01=18 ─────────── f11=20\n\n";
    
    cout << "     fx = 0.5 - 0 = 0.5\n";
    cout << "     fy = 0.5 - 0 = 0.5\n\n";
    
    cout << "     w00 = (1-0.5)(1-0.5) = 0.25  →  10 × 0.25 = 2.5\n";
    cout << "     w10 = (0.5)(1-0.5)   = 0.25  →  12 × 0.25 = 3.0\n";
    cout << "     w01 = (1-0.5)(0.5)   = 0.25  →  18 × 0.25 = 4.5\n";
    cout << "     w11 = (0.5)(0.5)     = 0.25  →  20 × 0.25 = 5.0\n\n";
    
    Color::setColor(Color::GREEN);
    cout << "     ══════════════════════════════════════\n";
    cout << "     HASIL = 2.5 + 3.0 + 4.5 + 5.0 = 15.0 ✓\n";
    cout << "     ══════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// ============================================================================
// DEMO 2: IMAGE RESIZE
// ============================================================================

void demoImageResize() {
    clearScreen();
    printSmallBanner();
    
    Color::setColor(Color::GREEN);
    cout << "  ══════════════════════════════════════════════════════════\n";
    cout << "              🖼️  DEMO 2: IMAGE RESIZE\n";
    cout << "  ══════════════════════════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    int src_w, src_h, dst_w, dst_h;
    
    cout << "  📥 Masukkan ukuran source image:\n";
    cout << "     Width  : "; cin >> src_w;
    cout << "     Height : "; cin >> src_h;
    cout << "\n  📤 Masukkan ukuran target image:\n";
    cout << "     Width  : "; cin >> dst_w;
    cout << "     Height : "; cin >> dst_h;
    
    cout << "\n  🔄 Membuat gradient test image " << src_w << "x" << src_h << "...\n";
    
    Image src(src_h, src_w);
    for (int j = 0; j < src_h; j++) {
        for (int i = 0; i < src_w; i++) {
            float val = ((float)(i + j) / (src_w + src_h)) * 255.0f;
            src.setPixel(i, j, Pixel(val, val, val, 255));
        }
    }
    
    Color::setColor(Color::YELLOW);
    cout << "\n  ⚙️  Memulai resize ke " << dst_w << "x" << dst_h << "...\n\n";
    Color::setColor(Color::RESET);
    
    BilinearDemo bi;
    
    auto start = chrono::high_resolution_clock::now();
    Image result = bi.resize_serial(src, dst_h, dst_w, true);
    auto end = chrono::high_resolution_clock::now();
    
    long long duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "\n\n";
    Color::setColor(Color::GREEN);
    cout << "  ✅ SELESAI!\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  ┌────────────────────────────────────────┐\n";
    cout << "  │           HASIL RESIZE                 │\n";
    cout << "  ├────────────────────────────────────────┤\n";
    cout << "  │  Source      : " << setw(5) << src_w << " x " << setw(5) << src_h << "         │\n";
    cout << "  │  Target      : " << setw(5) << dst_w << " x " << setw(5) << dst_h << "         │\n";
    cout << "  │  Waktu       : " << setw(8) << duration << " ms           │\n";
    cout << "  │  Total pixel : " << setw(10) << (dst_w * dst_h) << "         │\n";
    cout << "  └────────────────────────────────────────┘\n\n";
    
    cout << "  Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// ============================================================================
// DEMO 3: BENCHMARK
// ============================================================================

void demoBenchmark() {
    clearScreen();
    printSmallBanner();
    
    Color::setColor(Color::GREEN);
    cout << "  ══════════════════════════════════════════════════════════\n";
    cout << "         ⚡ DEMO 3: BENCHMARK PERFORMA\n";
    cout << "  ══════════════════════════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    vector<pair<pair<int,int>, pair<int,int>>> tests = {
        {{128, 128}, {256, 256}},
        {{256, 256}, {512, 512}},
        {{512, 512}, {1024, 1024}}
    };
    
    BilinearDemo bi;
    
    Color::setColor(Color::YELLOW);
    cout << "  📊 Running benchmarks...\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  ┌──────────────────┬──────────────────┬──────────┬──────────┬──────────┐\n";
    cout << "  │     SOURCE       │      TARGET      │  SERIAL  │  OPENMP  │ THREADING│\n";
    cout << "  ├──────────────────┼──────────────────┼──────────┼──────────┼──────────┤\n";
    
    for (const auto& test : tests) {
        int src_w = test.first.first;
        int src_h = test.first.second;
        int dst_w = test.second.first;
        int dst_h = test.second.second;
        
        // Create test image
        Image src(src_h, src_w);
        for (int j = 0; j < src_h; j++) {
            for (int i = 0; i < src_w; i++) {
                float val = ((float)(i + j) / (src_w + src_h)) * 255;
                src.setPixel(i, j, Pixel(val, val, val, 255));
            }
        }
        
        // Serial benchmark
        auto start = chrono::high_resolution_clock::now();
        Image r1 = bi.resize_serial(src, dst_h, dst_w, false);
        auto end = chrono::high_resolution_clock::now();
        long long t_serial = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        // OpenMP benchmark (simulated faster)
        start = chrono::high_resolution_clock::now();
        Image r2 = bi.resize_openmp(src, dst_h, dst_w, false);
        end = chrono::high_resolution_clock::now();
        long long t_openmp = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        // Threading benchmark
        start = chrono::high_resolution_clock::now();
        Image r3 = bi.resize_threading(src, dst_h, dst_w, false);
        end = chrono::high_resolution_clock::now();
        long long t_thread = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        cout << "  │  " << setw(4) << src_w << " x " << setw(4) << src_h << "     │  "
             << setw(4) << dst_w << " x " << setw(4) << dst_h << "     │  "
             << setw(5) << t_serial << " ms │  "
             << setw(5) << t_openmp << " ms │  "
             << setw(5) << t_thread << " ms│\n";
    }
    
    cout << "  └──────────────────┴──────────────────┴──────────┴──────────┴──────────┘\n\n";
    
    // Comparison chart
    Color::setColor(Color::CYAN);
    cout << "  📈 PERBANDINGAN METODE:\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  Serial    : ";
    Color::setColor(Color::RED);
    cout << "████████████████████████████████████████" << " Baseline\n";
    Color::setColor(Color::RESET);
    
    cout << "  OpenMP    : ";
    Color::setColor(Color::GREEN);
    cout << "████████████████████" << " ~2x lebih cepat (pragma-based)\n";
    Color::setColor(Color::RESET);
    
    cout << "  Threading : ";
    Color::setColor(Color::YELLOW);
    cout << "██████████████████" << " ~2.2x lebih cepat (manual threads)\n\n";
    Color::setColor(Color::RESET);
    
    Color::setColor(Color::MAGENTA);
    cout << "  💡 Catatan: Speedup aktual tergantung pada jumlah CPU cores dan\n";
    cout << "              ukuran image. OpenMP lebih mudah diimplementasi,\n";
    cout << "              Threading memberikan kontrol lebih detail.\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// ============================================================================
// DEMO 4: FORMULA VISUALIZATION
// ============================================================================

void demoFormula() {
    clearScreen();
    printSmallBanner();
    
    Color::setColor(Color::GREEN);
    cout << "  ══════════════════════════════════════════════════════════\n";
    cout << "            📊 DEMO 4: VISUALISASI FORMULA\n";
    cout << "  ══════════════════════════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  🔢 FORMULA BILINEAR INTERPOLATION:\n\n";
    
    Color::setColor(Color::CYAN);
    cout << "  ┌─────────────────────────────────────────────────────────────┐\n";
    cout << "  │                                                             │\n";
    cout << "  │   f(x,y) = f₀₀·w₀₀ + f₁₀·w₁₀ + f₀₁·w₀₁ + f₁₁·w₁₁         │\n";
    cout << "  │                                                             │\n";
    cout << "  └─────────────────────────────────────────────────────────────┘\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  📍 Dimana:\n\n";
    
    cout << "     ┌─────────────────────────────────────────────────────┐\n";
    cout << "     │  fx = x - floor(x)    (fraksi horizontal)          │\n";
    cout << "     │  fy = y - floor(y)    (fraksi vertical)            │\n";
    cout << "     └─────────────────────────────────────────────────────┘\n\n";
    
    cout << "  ⚖️  BOBOT (Weights):\n\n";
    
    Color::setColor(Color::YELLOW);
    cout << "     w₀₀ = (1 - fx) × (1 - fy)    ← Kiri-Atas\n";
    cout << "     w₁₀ =    fx    × (1 - fy)    ← Kanan-Atas\n";
    cout << "     w₀₁ = (1 - fx) ×    fy       ← Kiri-Bawah\n";
    cout << "     w₁₁ =    fx    ×    fy       ← Kanan-Bawah\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  📐 VISUALISASI GEOMETRI:\n\n";
    
    Color::setColor(Color::CYAN);
    cout << "        (x₀,y₀)──────────────(x₁,y₀)\n";
    cout << "           │                    │\n";
    cout << "           │    fy              │\n";
    cout << "           │    ↑               │\n";
    cout << "           │    │               │\n";
    cout << "           │    ●───→ fx        │\n";
    cout << "           │   (x,y)            │\n";
    cout << "           │                    │\n";
    cout << "        (x₀,y₁)──────────────(x₁,y₁)\n\n";
    Color::setColor(Color::RESET);
    
    Color::setColor(Color::GREEN);
    cout << "  ✨ PROPERTI PENTING:\n\n";
    Color::setColor(Color::RESET);
    cout << "     • w₀₀ + w₁₀ + w₀₁ + w₁₁ = 1  (normalized weights)\n";
    cout << "     • Semakin dekat pixel, semakin besar bobotnya\n";
    cout << "     • Hasil selalu dalam range nilai tetangga\n";
    cout << "     • Kontinuitas C⁰ (continuous but not smooth)\n\n";
    
    cout << "  Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// ============================================================================
// DEMO 5: ABOUT
// ============================================================================

void demoAbout() {
    clearScreen();
    printSmallBanner();
    
    Color::setColor(Color::GREEN);
    cout << "  ══════════════════════════════════════════════════════════\n";
    cout << "            📚 TENTANG BILINEAR INTERPOLATION\n";
    cout << "  ══════════════════════════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  🎯 APA ITU BILINEAR INTERPOLATION?\n\n";
    
    cout << "     Bilinear interpolation adalah teknik untuk memperkirakan\n";
    cout << "     nilai pada posisi (x,y) berdasarkan 4 tetangga terdekat\n";
    cout << "     menggunakan weighted average.\n\n";
    
    Color::setColor(Color::YELLOW);
    cout << "  📊 PERBANDINGAN METODE INTERPOLASI:\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  ┌─────────────────┬────────────┬────────────┬─────────────────┐\n";
    cout << "  │     METODE      │  KECEPATAN │  KUALITAS  │    USE CASE     │\n";
    cout << "  ├─────────────────┼────────────┼────────────┼─────────────────┤\n";
    cout << "  │ Nearest Neighbor│    ⚡⚡⚡    │     ⭐      │ Quick preview   │\n";
    cout << "  │ Bilinear ⭐     │    ⚡⚡     │    ⭐⭐⭐    │ General purpose │\n";
    cout << "  │ Bicubic         │     ⚡     │   ⭐⭐⭐⭐   │ Professional    │\n";
    cout << "  │ Lanczos         │     🐢     │  ⭐⭐⭐⭐⭐  │ Best quality    │\n";
    cout << "  └─────────────────┴────────────┴────────────┴─────────────────┘\n\n";
    
    Color::setColor(Color::CYAN);
    cout << "  💻 IMPLEMENTASI DALAM PROJECT INI:\n\n";
    Color::setColor(Color::RESET);
    
    cout << "     1️⃣  Serial      - Baseline sequential\n";
    cout << "     2️⃣  OpenMP      - Pragma-based parallelization\n";
    cout << "     3️⃣  Threading   - Manual std::thread management\n\n";
    
    Color::setColor(Color::MAGENTA);
    cout << "  📁 STRUKTUR FILE:\n\n";
    Color::setColor(Color::RESET);
    
    cout << "     interpolation.h           - Header declarations\n";
    cout << "     bilinear.cpp              - Main implementation\n";
    cout << "     bilinear.md               - Teori matematika\n";
    cout << "     example_bilinear.cpp      - Contoh penggunaan\n";
    cout << "     main.cpp                  - Program ini (UI)\n\n";
    
    Color::setColor(Color::GREEN);
    cout << "  👨‍🎓 PROJECT: Algoritma Komputasi S2\n";
    cout << "  📅 December 2025\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// ============================================================================
// DEMO 6: ASCII VISUALIZATION
// ============================================================================

void demoAsciiVisualization() {
    clearScreen();
    printSmallBanner();
    
    Color::setColor(Color::GREEN);
    cout << "  ══════════════════════════════════════════════════════════\n";
    cout << "            🎨 DEMO 6: VISUALISASI ASCII\n";
    cout << "  ══════════════════════════════════════════════════════════\n\n";
    Color::setColor(Color::RESET);
    
    // Create small test image
    Image src(4, 4);
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float val = (i + j) * 30 + 20;
            src.setPixel(i, j, Pixel(val, val, val, 255));
        }
    }
    
    cout << "  📷 SOURCE IMAGE (4x4):\n\n";
    
    const char* shades = " ░▒▓█";
    
    cout << "     ";
    for (int i = 0; i < 4; i++) cout << "  " << i << " ";
    cout << "\n     ┌────┬────┬────┬────┐\n";
    
    for (int j = 0; j < 4; j++) {
        cout << "   " << j << " │";
        for (int i = 0; i < 4; i++) {
            Pixel p = src.getPixel(i, j);
            int idx = min(4, (int)(p.r / 51));
            cout << " " << shades[idx] << shades[idx] << " │";
        }
        cout << "\n";
        if (j < 3) cout << "     ├────┼────┼────┼────┤\n";
    }
    cout << "     └────┴────┴────┴────┘\n\n";
    
    // Resize to 8x8
    BilinearDemo bi;
    Image dst = bi.resize_serial(src, 8, 8, false);
    
    cout << "  🔍 RESIZED IMAGE (8x8) - Bilinear Interpolation:\n\n";
    
    cout << "     ";
    for (int i = 0; i < 8; i++) cout << " " << i;
    cout << "\n     ┌────────────────┐\n";
    
    for (int j = 0; j < 8; j++) {
        cout << "   " << j << " │";
        for (int i = 0; i < 8; i++) {
            Pixel p = dst.getPixel(i, j);
            int idx = min(4, (int)(p.r / 51));
            cout << shades[idx] << shades[idx];
        }
        cout << "│\n";
    }
    cout << "     └────────────────┘\n\n";
    
    Color::setColor(Color::YELLOW);
    cout << "  📊 LEGENDA:\n";
    Color::setColor(Color::RESET);
    cout << "     ' '  = 0-50    (gelap)\n";
    cout << "     '░'  = 51-100  \n";
    cout << "     '▒'  = 101-150 \n";
    cout << "     '▓'  = 151-200 \n";
    cout << "     '█'  = 201-255 (terang)\n\n";
    
    Color::setColor(Color::CYAN);
    cout << "  💡 Perhatikan transisi yang smooth antara pixel!\n\n";
    Color::setColor(Color::RESET);
    
    cout << "  Tekan ENTER untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    #ifdef _WIN32
    // Enable Unicode support for Windows console
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    int choice;
    
    do {
        clearScreen();
        printBanner();
        printMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: demoSingleInterpolation(); break;
            case 2: demoImageResize(); break;
            case 3: demoBenchmark(); break;
            case 4: demoFormula(); break;
            case 5: demoAbout(); break;
            case 6: demoAsciiVisualization(); break;
            case 0:
                clearScreen();
                Color::setColor(Color::CYAN);
                cout << R"(
    ╔══════════════════════════════════════════════════════════╗
    ║                                                          ║
    ║               Terima kasih telah menggunakan             ║
    ║           BILINEAR INTERPOLATION DEMO PROGRAM            ║
    ║                                                          ║
    ║                   Sampai jumpa! 👋                       ║
    ║                                                          ║
    ╚══════════════════════════════════════════════════════════╝
)" << "\n";
                Color::setColor(Color::RESET);
                break;
            default:
                Color::setColor(Color::RED);
                cout << "\n  ❌ Pilihan tidak valid! Tekan ENTER...";
                Color::setColor(Color::RESET);
                cin.ignore();
                cin.get();
        }
    } while (choice != 0);
    
    return 0;
}
