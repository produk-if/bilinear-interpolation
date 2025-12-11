/**
 * ═══════════════════════════════════════════════════════════════════════════
 *                    BILINEAR INTERPOLATION - VISUAL DEMO
 *                      Algoritma Komputasi S2 - 2025
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * Program ini menampilkan SETIAP PROSES bilinear interpolation secara visual:
 * 1. Input image (grid pixel)
 * 2. Pemilihan 4 tetangga
 * 3. Perhitungan bobot (weights)
 * 4. Proses interpolasi step-by-step
 * 5. Hasil akhir
 * 
 * Compile: g++ -o bilinear_visual bilinear_visual.cpp -std=c++17
 * Run: ./bilinear_visual (Linux/Mac) atau bilinear_visual.exe (Windows)
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <sstream>

using namespace std;

// ════════════════════════════════════════════════════════════════════════════
// ANSI COLOR CODES
// ════════════════════════════════════════════════════════════════════════════
namespace Color {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string DIM     = "\033[2m";
    const string RED     = "\033[91m";
    const string GREEN   = "\033[92m";
    const string YELLOW  = "\033[93m";
    const string BLUE    = "\033[94m";
    const string MAGENTA = "\033[95m";
    const string CYAN    = "\033[96m";
    const string WHITE   = "\033[97m";
    const string BG_BLUE = "\033[44m";
    const string BG_GREEN= "\033[42m";
    const string BG_YELLOW="\033[43m";
}

// ════════════════════════════════════════════════════════════════════════════
// PIXEL & IMAGE STRUCTURES
// ════════════════════════════════════════════════════════════════════════════
struct Pixel {
    float r, g, b;
    Pixel(float val = 0) : r(val), g(val), b(val) {}
    Pixel(float r, float g, float b) : r(r), g(g), b(b) {}
    float gray() const { return (r + g + b) / 3.0f; }
};

class Image {
public:
    vector<vector<Pixel>> data;
    int width, height;
    
    Image(int h, int w) : height(h), width(w) {
        data.resize(h, vector<Pixel>(w));
    }
    
    Pixel& at(int x, int y) { return data[y][x]; }
    Pixel at(int x, int y) const { return data[y][x]; }
};

// ════════════════════════════════════════════════════════════════════════════
// UI HELPER FUNCTIONS
// ════════════════════════════════════════════════════════════════════════════
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    cout << "\033[2J\033[H";
    #endif
}

void pause(int ms = 500) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void waitEnter() {
    cout << Color::DIM << "\n  [Tekan ENTER untuk lanjut...]" << Color::RESET;
    cin.get();
}

void printHeader(const string& title) {
    cout << Color::CYAN << Color::BOLD;
    cout << "\n  ╔════════════════════════════════════════════════════════════╗\n";
    cout << "  ║" << Color::YELLOW;
    int padding = (60 - title.length()) / 2;
    cout << string(padding, ' ') << title << string(60 - padding - title.length(), ' ');
    cout << Color::CYAN << "║\n";
    cout << "  ╚════════════════════════════════════════════════════════════╝\n";
    cout << Color::RESET << "\n";
}

void printSubHeader(const string& title) {
    cout << Color::GREEN << Color::BOLD;
    cout << "\n  >> " << title << Color::RESET << "\n";
    cout << Color::GREEN << "  " << string(title.length() + 4, '-') << Color::RESET << "\n\n";
}

void printStep(int num, const string& desc) {
    cout << Color::YELLOW << "  [Step " << num << "] " << Color::WHITE << desc << Color::RESET << "\n";
}

// ════════════════════════════════════════════════════════════════════════════
// VISUAL GRID DISPLAY
// ════════════════════════════════════════════════════════════════════════════
void displayImageGrid(const Image& img, const string& title, 
                      int highlightX = -1, int highlightY = -1,
                      int x0 = -1, int y0 = -1, int x1 = -1, int y1 = -1) {
    cout << Color::CYAN << "  " << title << ":\n\n" << Color::RESET;
    
    // Header kolom
    cout << "       ";
    for (int i = 0; i < img.width; i++) {
        cout << Color::DIM << " col" << i << " " << Color::RESET;
    }
    cout << "\n";
    
    // Top border
    cout << "       ┌";
    for (int i = 0; i < img.width; i++) {
        cout << "──────" << (i < img.width-1 ? "┬" : "┐");
    }
    cout << "\n";
    
    // Rows
    for (int j = 0; j < img.height; j++) {
        cout << Color::DIM << "  row" << j << Color::RESET << " │";
        
        for (int i = 0; i < img.width; i++) {
            float val = img.at(i, j).gray();
            
            // Highlight logic
            bool isCorner = (i == x0 && j == y0) || (i == x1 && j == y0) ||
                           (i == x0 && j == y1) || (i == x1 && j == y1);
            bool isTarget = (i == highlightX && j == highlightY);
            
            if (isCorner) {
                cout << Color::BG_GREEN << Color::BOLD;
            } else if (isTarget) {
                cout << Color::BG_YELLOW << Color::BOLD;
            }
            
            cout << " " << setw(4) << fixed << setprecision(0) << val << " ";
            
            if (isCorner || isTarget) {
                cout << Color::RESET;
            }
            cout << "│";
        }
        cout << "\n";
        
        // Row separator
        if (j < img.height - 1) {
            cout << "       ├";
            for (int i = 0; i < img.width; i++) {
                cout << "──────" << (i < img.width-1 ? "┼" : "┤");
            }
            cout << "\n";
        }
    }
    
    // Bottom border
    cout << "       └";
    for (int i = 0; i < img.width; i++) {
        cout << "──────" << (i < img.width-1 ? "┴" : "┘");
    }
    cout << "\n";
}

// ════════════════════════════════════════════════════════════════════════════
// BILINEAR INTERPOLATION WITH VISUALIZATION
// ════════════════════════════════════════════════════════════════════════════
class BilinearVisual {
public:
    
    // Interpolasi dengan visualisasi step-by-step
    Pixel interpolateVisual(const Image& img, float x, float y) {
        clearScreen();
        printHeader("PROSES BILINEAR INTERPOLATION");
        
        // ═══════════════════════════════════════════════════════════════
        // STEP 1: Tampilkan koordinat target
        // ═══════════════════════════════════════════════════════════════
        printStep(1, "Menentukan koordinat target");
        cout << "\n";
        cout << Color::WHITE << "     Koordinat yang ingin di-interpolasi:\n";
        cout << Color::YELLOW << Color::BOLD;
        cout << "     ┌─────────────────────────────┐\n";
        cout << "     │   x = " << fixed << setprecision(2) << setw(5) << x 
             << "   y = " << setw(5) << y << "    │\n";
        cout << "     └─────────────────────────────┘\n";
        cout << Color::RESET;
        
        waitEnter();
        
        // ═══════════════════════════════════════════════════════════════
        // STEP 2: Mencari 4 tetangga terdekat
        // ═══════════════════════════════════════════════════════════════
        clearScreen();
        printHeader("PROSES BILINEAR INTERPOLATION");
        printStep(2, "Mencari 4 tetangga terdekat (neighbors)");
        
        int x0 = (int)floor(x);
        int y0 = (int)floor(y);
        int x1 = min(x0 + 1, img.width - 1);
        int y1 = min(y0 + 1, img.height - 1);
        
        cout << "\n";
        cout << Color::WHITE << "     Koordinat integer (floor):\n";
        cout << "     • x0 = floor(" << x << ") = " << Color::GREEN << x0 << Color::RESET << "\n";
        cout << "     • y0 = floor(" << y << ") = " << Color::GREEN << y0 << Color::RESET << "\n";
        cout << "     • x1 = x0 + 1 = " << Color::GREEN << x1 << Color::RESET << "\n";
        cout << "     • y1 = y0 + 1 = " << Color::GREEN << y1 << Color::RESET << "\n\n";
        
        cout << Color::CYAN << "     4 Tetangga yang digunakan:\n" << Color::RESET;
        cout << "     ┌──────────────────────────────────────────┐\n";
        cout << "     │  " << Color::YELLOW << "f00" << Color::RESET << " = pixel(" << x0 << "," << y0 << ") = " 
             << Color::GREEN << setw(3) << img.at(x0,y0).gray() << Color::RESET << "  (Kiri-Atas)    │\n";
        cout << "     │  " << Color::YELLOW << "f10" << Color::RESET << " = pixel(" << x1 << "," << y0 << ") = " 
             << Color::GREEN << setw(3) << img.at(x1,y0).gray() << Color::RESET << "  (Kanan-Atas)   │\n";
        cout << "     │  " << Color::YELLOW << "f01" << Color::RESET << " = pixel(" << x0 << "," << y1 << ") = " 
             << Color::GREEN << setw(3) << img.at(x0,y1).gray() << Color::RESET << "  (Kiri-Bawah)   │\n";
        cout << "     │  " << Color::YELLOW << "f11" << Color::RESET << " = pixel(" << x1 << "," << y1 << ") = " 
             << Color::GREEN << setw(3) << img.at(x1,y1).gray() << Color::RESET << "  (Kanan-Bawah)  │\n";
        cout << "     └──────────────────────────────────────────┘\n\n";
        
        // Tampilkan grid dengan highlight
        displayImageGrid(img, "Source Image (4 tetangga di-highlight hijau)", -1, -1, x0, y0, x1, y1);
        
        waitEnter();
        
        // ═══════════════════════════════════════════════════════════════
        // STEP 3: Menghitung fraksi (fx, fy)
        // ═══════════════════════════════════════════════════════════════
        clearScreen();
        printHeader("PROSES BILINEAR INTERPOLATION");
        printStep(3, "Menghitung fraksi koordinat (fx, fy)");
        
        float fx = x - floor(x);
        float fy = y - floor(y);
        
        cout << "\n";
        cout << Color::WHITE << "     Fraksi = jarak dari tepi kiri/atas cell\n\n" << Color::RESET;
        
        cout << "     " << Color::YELLOW << "fx" << Color::RESET << " = x - floor(x)\n";
        cout << "        = " << x << " - " << floor(x) << "\n";
        cout << "        = " << Color::GREEN << Color::BOLD << fx << Color::RESET << "\n\n";
        
        cout << "     " << Color::YELLOW << "fy" << Color::RESET << " = y - floor(y)\n";
        cout << "        = " << y << " - " << floor(y) << "\n";
        cout << "        = " << Color::GREEN << Color::BOLD << fy << Color::RESET << "\n\n";
        
        // Visualisasi posisi dalam cell
        cout << Color::CYAN << "     Visualisasi posisi dalam cell:\n\n" << Color::RESET;
        cout << "           x0=" << x0 << "              x1=" << x1 << "\n";
        cout << "       y0=" << y0 << " ●───────────────────●\n";
        cout << "            │                   │\n";
        cout << "            │      fy=" << fixed << setprecision(2) << fy << "       │\n";
        cout << "            │        ↑          │\n";
        cout << "            │        │          │\n";
        cout << "            │        ╳←fx=" << fx << "  │\n";
        cout << "            │     (target)      │\n";
        cout << "            │                   │\n";
        cout << "       y1=" << y1 << " ●───────────────────●\n\n";
        
        waitEnter();
        
        // ═══════════════════════════════════════════════════════════════
        // STEP 4: Menghitung bobot (weights)
        // ═══════════════════════════════════════════════════════════════
        clearScreen();
        printHeader("PROSES BILINEAR INTERPOLATION");
        printStep(4, "Menghitung bobot (weights) untuk setiap tetangga");
        
        float w00 = (1.0f - fx) * (1.0f - fy);
        float w10 = fx * (1.0f - fy);
        float w01 = (1.0f - fx) * fy;
        float w11 = fx * fy;
        
        cout << "\n";
        cout << Color::WHITE << "     Formula bobot:\n" << Color::RESET;
        cout << "     w = (jarak horizontal) × (jarak vertical)\n\n";
        
        cout << "     " << Color::YELLOW << "w00" << Color::RESET << " = (1-fx) × (1-fy)\n";
        cout << "         = (1-" << fx << ") × (1-" << fy << ")\n";
        cout << "         = " << (1-fx) << " × " << (1-fy) << "\n";
        cout << "         = " << Color::GREEN << Color::BOLD << fixed << setprecision(4) << w00 << Color::RESET << "\n\n";
        
        cout << "     " << Color::YELLOW << "w10" << Color::RESET << " = fx × (1-fy)\n";
        cout << "         = " << fx << " × " << (1-fy) << "\n";
        cout << "         = " << Color::GREEN << Color::BOLD << w10 << Color::RESET << "\n\n";
        
        cout << "     " << Color::YELLOW << "w01" << Color::RESET << " = (1-fx) × fy\n";
        cout << "         = " << (1-fx) << " × " << fy << "\n";
        cout << "         = " << Color::GREEN << Color::BOLD << w01 << Color::RESET << "\n\n";
        
        cout << "     " << Color::YELLOW << "w11" << Color::RESET << " = fx × fy\n";
        cout << "         = " << fx << " × " << fy << "\n";
        cout << "         = " << Color::GREEN << Color::BOLD << w11 << Color::RESET << "\n\n";
        
        // Verifikasi sum = 1
        float sumW = w00 + w10 + w01 + w11;
        cout << Color::CYAN << "     Verifikasi: w00 + w10 + w01 + w11 = " 
             << Color::GREEN << sumW << Color::RESET;
        if (abs(sumW - 1.0f) < 0.0001f) {
            cout << Color::GREEN << " ✓ (normalized)" << Color::RESET;
        }
        cout << "\n";
        
        waitEnter();
        
        // ═══════════════════════════════════════════════════════════════
        // STEP 5: Menghitung hasil interpolasi
        // ═══════════════════════════════════════════════════════════════
        clearScreen();
        printHeader("PROSES BILINEAR INTERPOLATION");
        printStep(5, "Menghitung nilai interpolasi (weighted sum)");
        
        float f00 = img.at(x0, y0).gray();
        float f10 = img.at(x1, y0).gray();
        float f01 = img.at(x0, y1).gray();
        float f11 = img.at(x1, y1).gray();
        
        cout << "\n";
        cout << Color::WHITE << "     Formula:\n" << Color::RESET;
        cout << Color::CYAN << "     f(x,y) = f00×w00 + f10×w10 + f01×w01 + f11×w11\n\n" << Color::RESET;
        
        cout << Color::WHITE << "     Perhitungan:\n" << Color::RESET;
        
        float term1 = f00 * w00;
        float term2 = f10 * w10;
        float term3 = f01 * w01;
        float term4 = f11 * w11;
        
        cout << "     • f00 × w00 = " << setw(3) << f00 << " × " << setw(6) << w00 
             << " = " << Color::YELLOW << setw(8) << term1 << Color::RESET << "\n";
        cout << "     • f10 × w10 = " << setw(3) << f10 << " × " << setw(6) << w10 
             << " = " << Color::YELLOW << setw(8) << term2 << Color::RESET << "\n";
        cout << "     • f01 × w01 = " << setw(3) << f01 << " × " << setw(6) << w01 
             << " = " << Color::YELLOW << setw(8) << term3 << Color::RESET << "\n";
        cout << "     • f11 × w11 = " << setw(3) << f11 << " × " << setw(6) << w11 
             << " = " << Color::YELLOW << setw(8) << term4 << Color::RESET << "\n";
        cout << "       " << string(40, '-') << "\n";
        
        float result = term1 + term2 + term3 + term4;
        
        cout << "     • TOTAL    = " << term1 << " + " << term2 << " + " << term3 << " + " << term4 << "\n\n";
        
        cout << Color::GREEN << Color::BOLD;
        cout << "     ╔═════════════════════════════════════════════╗\n";
        cout << "     ║                                             ║\n";
        cout << "     ║   HASIL INTERPOLASI: f(" << setprecision(2) << x << "," << y << ") = " 
             << setprecision(4) << setw(8) << result << "    ║\n";
        cout << "     ║                                             ║\n";
        cout << "     ╚═════════════════════════════════════════════╝\n";
        cout << Color::RESET;
        
        waitEnter();
        
        return Pixel(result);
    }
    
    // Resize dengan visualisasi
    Image resizeVisual(const Image& src, int newH, int newW) {
        clearScreen();
        printHeader("PROSES IMAGE RESIZE");
        
        Image dst(newH, newW);
        
        float scaleX = (float)src.width / newW;
        float scaleY = (float)src.height / newH;
        
        printSubHeader("Parameter Resize");
        cout << "     Source: " << src.width << " × " << src.height << " pixels\n";
        cout << "     Target: " << newW << " × " << newH << " pixels\n";
        cout << "     Scale X: " << fixed << setprecision(4) << scaleX << "\n";
        cout << "     Scale Y: " << scaleY << "\n\n";
        
        displayImageGrid(src, "Source Image");
        
        waitEnter();
        
        // Proses setiap pixel
        for (int j = 0; j < newH; j++) {
            for (int i = 0; i < newW; i++) {
                clearScreen();
                printHeader("PROSES IMAGE RESIZE");
                
                float srcX = i * scaleX;
                float srcY = j * scaleY;
                
                cout << Color::YELLOW << "  Processing pixel (" << i << "," << j << ") dari " 
                     << newW << "×" << newH << Color::RESET << "\n";
                cout << "  Progress: [";
                int progress = (j * newW + i + 1) * 100 / (newW * newH);
                int barLen = progress / 2;
                cout << Color::GREEN << string(barLen, '#') << Color::DIM << string(50-barLen, '.') 
                     << Color::RESET << "] " << progress << "%\n\n";
                
                cout << "  Mapping: dst(" << i << "," << j << ") ← src(" 
                     << fixed << setprecision(2) << srcX << "," << srcY << ")\n\n";
                
                // Hitung 4 tetangga
                int x0 = (int)floor(srcX);
                int y0 = (int)floor(srcY);
                int x1 = min(x0 + 1, src.width - 1);
                int y1 = min(y0 + 1, src.height - 1);
                
                float fx = srcX - floor(srcX);
                float fy = srcY - floor(srcY);
                
                float w00 = (1-fx)*(1-fy);
                float w10 = fx*(1-fy);
                float w01 = (1-fx)*fy;
                float w11 = fx*fy;
                
                float f00 = src.at(x0,y0).gray();
                float f10 = src.at(x1,y0).gray();
                float f01 = src.at(x0,y1).gray();
                float f11 = src.at(x1,y1).gray();
                
                float result = f00*w00 + f10*w10 + f01*w01 + f11*w11;
                
                cout << "  Neighbors: f00=" << f00 << " f10=" << f10 
                     << " f01=" << f01 << " f11=" << f11 << "\n";
                cout << "  Weights:   w00=" << setprecision(3) << w00 << " w10=" << w10 
                     << " w01=" << w01 << " w11=" << w11 << "\n";
                cout << Color::GREEN << "  Result:    " << setprecision(2) << result << Color::RESET << "\n";
                
                dst.at(i, j) = Pixel(result);
                
                // Tampilkan source dengan highlight
                displayImageGrid(src, "Source (neighbors highlighted)", -1, -1, x0, y0, x1, y1);
                
                pause(300);  // Delay untuk visualisasi
            }
        }
        
        // Tampilkan hasil
        clearScreen();
        printHeader("HASIL RESIZE");
        
        cout << Color::CYAN << "  Source Image (" << src.width << "×" << src.height << "):\n" << Color::RESET;
        displayImageGrid(src, "");
        
        cout << "\n" << Color::GREEN << "  Result Image (" << newW << "×" << newH << "):\n" << Color::RESET;
        displayImageGrid(dst, "");
        
        return dst;
    }
};

// ════════════════════════════════════════════════════════════════════════════
// MENU FUNCTIONS
// ════════════════════════════════════════════════════════════════════════════
void printMainMenu() {
    cout << Color::CYAN << Color::BOLD;
    cout << R"(
    ╔═══════════════════════════════════════════════════════════════════╗
    ║                                                                   ║
    ║   ██████╗ ██╗██╗     ██╗███╗   ██╗███████╗ █████╗ ██████╗        ║
    ║   ██╔══██╗██║██║     ██║████╗  ██║██╔════╝██╔══██╗██╔══██╗       ║
    ║   ██████╔╝██║██║     ██║██╔██╗ ██║█████╗  ███████║██████╔╝       ║
    ║   ██╔══██╗██║██║     ██║██║╚██╗██║██╔══╝  ██╔══██║██╔══██╗       ║
    ║   ██████╔╝██║███████╗██║██║ ╚████║███████╗██║  ██║██║  ██║       ║
    ║   ╚═════╝ ╚═╝╚══════╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝       ║
    ║                                                                   ║
    ║            INTERPOLATION - VISUAL STEP-BY-STEP DEMO               ║
    ║                   Algoritma Komputasi S2 2025                     ║
    ╚═══════════════════════════════════════════════════════════════════╝
)" << Color::RESET;

    cout << Color::YELLOW;
    cout << "\n    ┌─────────────────── MENU UTAMA ───────────────────┐\n";
    cout << "    │                                                   │\n";
    cout << Color::WHITE;
    cout << "    │   [1] " << Color::GREEN << "●" << Color::WHITE << " Demo Single Pixel Interpolation        │\n";
    cout << "    │       (Lihat proses step-by-step detail)         │\n";
    cout << "    │                                                   │\n";
    cout << "    │   [2] " << Color::GREEN << "●" << Color::WHITE << " Demo Image Resize                       │\n";
    cout << "    │       (Resize 4x4 → 6x6 dengan visualisasi)      │\n";
    cout << "    │                                                   │\n";
    cout << "    │   [3] " << Color::GREEN << "●" << Color::WHITE << " Custom Interpolation                    │\n";
    cout << "    │       (Input koordinat sendiri)                  │\n";
    cout << "    │                                                   │\n";
    cout << "    │   [4] " << Color::GREEN << "●" << Color::WHITE << " Penjelasan Teori                        │\n";
    cout << "    │       (Rumus dan konsep bilinear)                │\n";
    cout << "    │                                                   │\n";
    cout << Color::RED;
    cout << "    │   [0] " << Color::RED << "●" << Color::WHITE << " Keluar                                   │\n";
    cout << Color::YELLOW;
    cout << "    │                                                   │\n";
    cout << "    └───────────────────────────────────────────────────┘\n\n";
    cout << Color::RESET;
    
    cout << "    Pilih menu [0-4]: ";
}

// ════════════════════════════════════════════════════════════════════════════
// DEMO FUNCTIONS
// ════════════════════════════════════════════════════════════════════════════
Image createTestImage() {
    Image img(4, 4);
    // Gradient values
    float values[4][4] = {
        { 10,  30,  50,  70},
        { 40,  60,  80, 100},
        { 70,  90, 110, 130},
        {100, 120, 140, 160}
    };
    
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            img.at(i, j) = Pixel(values[j][i]);
        }
    }
    return img;
}

void demo1_SinglePixel() {
    Image img = createTestImage();
    BilinearVisual bv;
    
    // Interpolasi pada posisi (1.5, 1.5)
    bv.interpolateVisual(img, 1.5f, 1.5f);
}

void demo2_Resize() {
    Image img = createTestImage();
    BilinearVisual bv;
    
    // Resize dari 4x4 ke 6x6
    bv.resizeVisual(img, 6, 6);
    
    waitEnter();
}

void demo3_Custom() {
    clearScreen();
    printHeader("CUSTOM INTERPOLATION");
    
    Image img = createTestImage();
    
    cout << "\n";
    displayImageGrid(img, "Source Image 4x4");
    
    float x, y;
    cout << "\n  Masukkan koordinat untuk interpolasi:\n";
    cout << "  X (0.0 - 3.0): ";
    cin >> x;
    cout << "  Y (0.0 - 3.0): ";
    cin >> y;
    cin.ignore();
    
    // Clamp values
    x = max(0.0f, min(x, 2.99f));
    y = max(0.0f, min(y, 2.99f));
    
    BilinearVisual bv;
    bv.interpolateVisual(img, x, y);
}

void demo4_Theory() {
    clearScreen();
    printHeader("TEORI BILINEAR INTERPOLATION");
    
    cout << Color::CYAN << R"(
  ╔═══════════════════════════════════════════════════════════════════════╗
  ║                        KONSEP DASAR                                   ║
  ╚═══════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;

    cout << R"(
  Bilinear interpolation menghitung nilai pada posisi (x,y) berdasarkan
  4 tetangga terdekat menggunakan weighted average (rata-rata berbobot).

)" << Color::YELLOW << R"(
  ╔═══════════════════════════════════════════════════════════════════════╗
  ║                        FORMULA UTAMA                                  ║
  ╠═══════════════════════════════════════════════════════════════════════╣
  ║                                                                       ║
  ║    f(x,y) = f₀₀·w₀₀ + f₁₀·w₁₀ + f₀₁·w₀₁ + f₁₁·w₁₁                  ║
  ║                                                                       ║
  ╚═══════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;

    cout << Color::GREEN << R"(
  ╔═══════════════════════════════════════════════════════════════════════╗
  ║                        BOBOT (WEIGHTS)                                ║
  ╠═══════════════════════════════════════════════════════════════════════╣
  ║                                                                       ║
  ║    w₀₀ = (1 - fx) × (1 - fy)    ← Kiri-Atas                          ║
  ║    w₁₀ =    fx    × (1 - fy)    ← Kanan-Atas                         ║
  ║    w₀₁ = (1 - fx) ×    fy       ← Kiri-Bawah                         ║
  ║    w₁₁ =    fx    ×    fy       ← Kanan-Bawah                        ║
  ║                                                                       ║
  ║    Dimana: fx = x - floor(x),  fy = y - floor(y)                     ║
  ║                                                                       ║
  ╚═══════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;

    cout << Color::CYAN << R"(
  ╔═══════════════════════════════════════════════════════════════════════╗
  ║                     VISUALISASI GEOMETRI                              ║
  ╠═══════════════════════════════════════════════════════════════════════╣
  ║                                                                       ║
  ║         (x₀,y₀)═══════════════════════(x₁,y₀)                        ║
  ║            ║                              ║                           ║
  ║            ║        fy                    ║                           ║
  ║            ║         ↑                    ║                           ║
  ║            ║         │                    ║                           ║
  ║            ║         ●────→ fx            ║                           ║
  ║            ║      (x,y)                   ║                           ║
  ║            ║                              ║                           ║
  ║         (x₀,y₁)═══════════════════════(x₁,y₁)                        ║
  ║                                                                       ║
  ╚═══════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;

    cout << Color::MAGENTA << R"(
  ╔═══════════════════════════════════════════════════════════════════════╗
  ║                     PROPERTI PENTING                                  ║
  ╠═══════════════════════════════════════════════════════════════════════╣
  ║                                                                       ║
  ║   ✓ w₀₀ + w₁₀ + w₀₁ + w₁₁ = 1  (normalized weights)                 ║
  ║   ✓ Semakin dekat ke pixel, semakin besar bobotnya                   ║
  ║   ✓ Hasil selalu dalam range nilai 4 tetangga                        ║
  ║   ✓ Menghasilkan transisi yang smooth                                ║
  ║   ✓ Kompleksitas: O(1) per pixel                                     ║
  ║                                                                       ║
  ╚═══════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;

    waitEnter();
}

// ════════════════════════════════════════════════════════════════════════════
// MAIN
// ════════════════════════════════════════════════════════════════════════════
int main() {
    // Enable ANSI colors on Windows
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif
    
    int choice;
    
    do {
        clearScreen();
        printMainMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: demo1_SinglePixel(); break;
            case 2: demo2_Resize(); break;
            case 3: demo3_Custom(); break;
            case 4: demo4_Theory(); break;
            case 0:
                clearScreen();
                cout << Color::CYAN << R"(
    ╔═══════════════════════════════════════════════════════════════════╗
    ║                                                                   ║
    ║                  Terima kasih telah menggunakan                   ║
    ║              BILINEAR INTERPOLATION VISUAL DEMO                   ║
    ║                                                                   ║
    ║                       Sampai jumpa! 👋                            ║
    ║                                                                   ║
    ║                  Algoritma Komputasi S2 - 2025                    ║
    ║                                                                   ║
    ╚═══════════════════════════════════════════════════════════════════╝
)" << Color::RESET << "\n";
                break;
            default:
                cout << Color::RED << "\n    ⚠ Pilihan tidak valid!" << Color::RESET;
                pause(1000);
        }
    } while (choice != 0);
    
    return 0;
}
