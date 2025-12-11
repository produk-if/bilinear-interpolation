/**
 * BILINEAR INTERPOLATION - VISUAL STEP-BY-STEP DEMO
 * Algoritma Komputasi S2 - 2025
 * 
 * Compile: g++ -o demo.exe demo_bilinear.cpp -std=c++17
 * Run: demo.exe
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

// ============================================================================
// STRUCTURES
// ============================================================================
struct Pixel {
    float value;
    Pixel(float v = 0) : value(v) {}
};

class Image {
public:
    vector<vector<float>> data;
    int width, height;
    
    Image(int h, int w) : height(h), width(w) {
        data.resize(h, vector<float>(w, 0));
    }
    
    float at(int x, int y) const { 
        if (x >= 0 && x < width && y >= 0 && y < height)
            return data[y][x]; 
        return 0;
    }
    
    void set(int x, int y, float v) { 
        if (x >= 0 && x < width && y >= 0 && y < height)
            data[y][x] = v; 
    }
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================
void clearScreen() {
    system("cls");
}

void printLine(int len = 60) {
    cout << "  " << string(len, '=') << "\n";
}

void printTitle(const string& title) {
    cout << "\n";
    printLine();
    cout << "  " << title << "\n";
    printLine();
    cout << "\n";
}

void waitEnter() {
    cout << "\n  >>> Tekan ENTER untuk lanjut...";
    cin.get();
}

void displayGrid(const Image& img, const string& title, int hx0=-1, int hy0=-1, int hx1=-1, int hy1=-1) {
    cout << "  " << title << ":\n\n";
    
    // Header
    cout << "        ";
    for (int i = 0; i < img.width; i++) {
        cout << "  [" << i << "]  ";
    }
    cout << "\n";
    
    // Top border
    cout << "       +";
    for (int i = 0; i < img.width; i++) {
        cout << "------+";
    }
    cout << "\n";
    
    // Rows
    for (int j = 0; j < img.height; j++) {
        cout << "   [" << j << "] |";
        for (int i = 0; i < img.width; i++) {
            bool highlight = (i == hx0 && j == hy0) || (i == hx1 && j == hy0) ||
                           (i == hx0 && j == hy1) || (i == hx1 && j == hy1);
            
            if (highlight) {
                cout << "*" << setw(4) << fixed << setprecision(0) << img.at(i,j) << "*|";
            } else {
                cout << " " << setw(4) << fixed << setprecision(0) << img.at(i,j) << " |";
            }
        }
        cout << "\n";
        
        cout << "       +";
        for (int i = 0; i < img.width; i++) {
            cout << "------+";
        }
        cout << "\n";
    }
    cout << "\n";
}

// ============================================================================
// BILINEAR INTERPOLATION WITH STEP-BY-STEP VISUALIZATION
// ============================================================================
float bilinearInterpolate(const Image& img, float x, float y, bool showSteps) {
    
    if (showSteps) {
        clearScreen();
        printTitle("STEP 1: KOORDINAT TARGET");
        
        cout << "  Kita ingin mencari nilai pada koordinat:\n\n";
        cout << "       +---------------------------+\n";
        cout << "       |   x = " << fixed << setprecision(2) << setw(5) << x << "               |\n";
        cout << "       |   y = " << setw(5) << y << "               |\n";
        cout << "       +---------------------------+\n\n";
        
        cout << "  Koordinat ini BUKAN integer, jadi kita perlu\n";
        cout << "  interpolasi dari pixel-pixel terdekat.\n";
        
        waitEnter();
    }
    
    // Step 2: Find 4 neighbors
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int x1 = min(x0 + 1, img.width - 1);
    int y1 = min(y0 + 1, img.height - 1);
    
    if (showSteps) {
        clearScreen();
        printTitle("STEP 2: CARI 4 TETANGGA TERDEKAT");
        
        cout << "  Koordinat integer (floor):\n\n";
        cout << "    x0 = floor(" << x << ") = " << x0 << "\n";
        cout << "    y0 = floor(" << y << ") = " << y0 << "\n";
        cout << "    x1 = x0 + 1 = " << x1 << "\n";
        cout << "    y1 = y0 + 1 = " << y1 << "\n\n";
        
        cout << "  4 Tetangga yang digunakan:\n\n";
        cout << "    f00 = pixel[" << x0 << "][" << y0 << "] = " << img.at(x0,y0) << "  (Kiri-Atas)\n";
        cout << "    f10 = pixel[" << x1 << "][" << y0 << "] = " << img.at(x1,y0) << "  (Kanan-Atas)\n";
        cout << "    f01 = pixel[" << x0 << "][" << y1 << "] = " << img.at(x0,y1) << "  (Kiri-Bawah)\n";
        cout << "    f11 = pixel[" << x1 << "][" << y1 << "] = " << img.at(x1,y1) << "  (Kanan-Bawah)\n\n";
        
        displayGrid(img, "Grid (* = tetangga yang digunakan)", x0, y0, x1, y1);
        
        waitEnter();
    }
    
    // Step 3: Calculate fractions
    float fx = x - floor(x);
    float fy = y - floor(y);
    
    if (showSteps) {
        clearScreen();
        printTitle("STEP 3: HITUNG FRAKSI (fx, fy)");
        
        cout << "  Fraksi = jarak dari tepi kiri/atas cell\n\n";
        
        cout << "    fx = x - floor(x)\n";
        cout << "       = " << x << " - " << floor(x) << "\n";
        cout << "       = " << fx << "\n\n";
        
        cout << "    fy = y - floor(y)\n";
        cout << "       = " << y << " - " << floor(y) << "\n";
        cout << "       = " << fy << "\n\n";
        
        cout << "  Visualisasi posisi dalam cell:\n\n";
        cout << "    (x0,y0)=" << img.at(x0,y0) << " ----------- (x1,y0)=" << img.at(x1,y0) << "\n";
        cout << "        |                       |\n";
        cout << "        |     fy=" << setprecision(2) << fy << "           |\n";
        cout << "        |       |               |\n";
        cout << "        |       v               |\n";
        cout << "        |       X <-- fx=" << fx << "  |\n";
        cout << "        |    (target)           |\n";
        cout << "        |                       |\n";
        cout << "    (x0,y1)=" << img.at(x0,y1) << " ----------- (x1,y1)=" << img.at(x1,y1) << "\n";
        
        waitEnter();
    }
    
    // Step 4: Calculate weights
    float w00 = (1.0f - fx) * (1.0f - fy);
    float w10 = fx * (1.0f - fy);
    float w01 = (1.0f - fx) * fy;
    float w11 = fx * fy;
    
    if (showSteps) {
        clearScreen();
        printTitle("STEP 4: HITUNG BOBOT (WEIGHTS)");
        
        cout << "  Formula bobot:\n";
        cout << "  w = (jarak horizontal) x (jarak vertical)\n\n";
        
        cout << "  w00 = (1-fx) x (1-fy)\n";
        cout << "      = (1-" << fx << ") x (1-" << fy << ")\n";
        cout << "      = " << (1-fx) << " x " << (1-fy) << "\n";
        cout << "      = " << setprecision(4) << w00 << "\n\n";
        
        cout << "  w10 = fx x (1-fy)\n";
        cout << "      = " << fx << " x " << (1-fy) << "\n";
        cout << "      = " << w10 << "\n\n";
        
        cout << "  w01 = (1-fx) x fy\n";
        cout << "      = " << (1-fx) << " x " << fy << "\n";
        cout << "      = " << w01 << "\n\n";
        
        cout << "  w11 = fx x fy\n";
        cout << "      = " << fx << " x " << fy << "\n";
        cout << "      = " << w11 << "\n\n";
        
        float sumW = w00 + w10 + w01 + w11;
        cout << "  VERIFIKASI: w00 + w10 + w01 + w11 = " << sumW;
        if (abs(sumW - 1.0f) < 0.001f) {
            cout << " [OK - normalized!]";
        }
        cout << "\n";
        
        waitEnter();
    }
    
    // Step 5: Calculate result
    float f00 = img.at(x0, y0);
    float f10 = img.at(x1, y0);
    float f01 = img.at(x0, y1);
    float f11 = img.at(x1, y1);
    
    float term1 = f00 * w00;
    float term2 = f10 * w10;
    float term3 = f01 * w01;
    float term4 = f11 * w11;
    float result = term1 + term2 + term3 + term4;
    
    if (showSteps) {
        clearScreen();
        printTitle("STEP 5: HITUNG HASIL (WEIGHTED SUM)");
        
        cout << "  Formula:\n";
        cout << "  f(x,y) = f00*w00 + f10*w10 + f01*w01 + f11*w11\n\n";
        
        cout << "  Perhitungan:\n\n";
        cout << "    f00 x w00 = " << setw(3) << f00 << " x " << setw(6) << w00 << " = " << setw(8) << term1 << "\n";
        cout << "    f10 x w10 = " << setw(3) << f10 << " x " << setw(6) << w10 << " = " << setw(8) << term2 << "\n";
        cout << "    f01 x w01 = " << setw(3) << f01 << " x " << setw(6) << w01 << " = " << setw(8) << term3 << "\n";
        cout << "    f11 x w11 = " << setw(3) << f11 << " x " << setw(6) << w11 << " = " << setw(8) << term4 << "\n";
        cout << "    " << string(44, '-') << "\n";
        cout << "    TOTAL     = " << term1 << " + " << term2 << " + " << term3 << " + " << term4 << "\n\n";
        
        cout << "  +=============================================+\n";
        cout << "  |                                             |\n";
        cout << "  |   HASIL: f(" << setprecision(2) << x << ", " << y << ") = " << setprecision(4) << setw(10) << result << "       |\n";
        cout << "  |                                             |\n";
        cout << "  +=============================================+\n";
        
        waitEnter();
    }
    
    return result;
}

// ============================================================================
// DEMO: IMAGE RESIZE
// ============================================================================
void demoResize() {
    clearScreen();
    printTitle("DEMO: IMAGE RESIZE DENGAN BILINEAR INTERPOLATION");
    
    // Create source image 4x4
    Image src(4, 4);
    float vals[4][4] = {
        { 10,  30,  50,  70},
        { 40,  60,  80, 100},
        { 70,  90, 110, 130},
        {100, 120, 140, 160}
    };
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            src.set(i, j, vals[j][i]);
    
    cout << "  Source image: 4 x 4 pixels\n";
    cout << "  Target image: 6 x 6 pixels\n\n";
    
    displayGrid(src, "SOURCE IMAGE (4x4)");
    
    waitEnter();
    
    // Resize to 6x6
    int newW = 6, newH = 6;
    Image dst(newH, newW);
    
    float scaleX = (float)src.width / newW;
    float scaleY = (float)src.height / newH;
    
    clearScreen();
    printTitle("PROSES RESIZE: MAPPING KOORDINAT");
    
    cout << "  Scale X = " << src.width << " / " << newW << " = " << fixed << setprecision(4) << scaleX << "\n";
    cout << "  Scale Y = " << src.height << " / " << newH << " = " << scaleY << "\n\n";
    
    cout << "  Untuk setiap pixel di target (i,j):\n";
    cout << "    srcX = i * scaleX\n";
    cout << "    srcY = j * scaleY\n\n";
    
    cout << "  Mapping koordinat:\n\n";
    cout << "    dst(i,j)  ->  src(x, y)\n";
    cout << "    " << string(30, '-') << "\n";
    
    for (int j = 0; j < newH; j++) {
        for (int i = 0; i < newW; i++) {
            float srcX = i * scaleX;
            float srcY = j * scaleY;
            cout << "    (" << i << "," << j << ")      ->  (" << setprecision(2) << srcX << ", " << srcY << ")\n";
        }
    }
    
    waitEnter();
    
    // Process each pixel
    for (int j = 0; j < newH; j++) {
        for (int i = 0; i < newW; i++) {
            float srcX = i * scaleX;
            float srcY = j * scaleY;
            
            // Clamp
            srcX = min(srcX, (float)src.width - 1.001f);
            srcY = min(srcY, (float)src.height - 1.001f);
            
            float val = bilinearInterpolate(src, srcX, srcY, false);
            dst.set(i, j, val);
        }
    }
    
    // Show result
    clearScreen();
    printTitle("HASIL RESIZE");
    
    displayGrid(src, "SOURCE (4x4)");
    displayGrid(dst, "RESULT (6x6) - setelah bilinear interpolation");
    
    cout << "  Perhatikan transisi nilai yang SMOOTH!\n";
    cout << "  Tidak ada perubahan drastis antar pixel.\n";
    
    waitEnter();
}

// ============================================================================
// THEORY
// ============================================================================
void showTheory() {
    clearScreen();
    printTitle("TEORI BILINEAR INTERPOLATION");
    
    cout << "  KONSEP DASAR\n";
    cout << "  " << string(50, '-') << "\n\n";
    
    cout << "  Bilinear interpolation menghitung nilai pada posisi\n";
    cout << "  (x,y) berdasarkan 4 tetangga terdekat menggunakan\n";
    cout << "  weighted average (rata-rata berbobot).\n\n";
    
    cout << "  FORMULA UTAMA\n";
    cout << "  " << string(50, '-') << "\n\n";
    
    cout << "    f(x,y) = f00*w00 + f10*w10 + f01*w01 + f11*w11\n\n";
    
    cout << "  BOBOT (WEIGHTS)\n";
    cout << "  " << string(50, '-') << "\n\n";
    
    cout << "    w00 = (1 - fx) * (1 - fy)   <- Kiri-Atas\n";
    cout << "    w10 =    fx    * (1 - fy)   <- Kanan-Atas\n";
    cout << "    w01 = (1 - fx) *    fy      <- Kiri-Bawah\n";
    cout << "    w11 =    fx    *    fy      <- Kanan-Bawah\n\n";
    
    cout << "    Dimana: fx = x - floor(x)\n";
    cout << "            fy = y - floor(y)\n\n";
    
    cout << "  VISUALISASI\n";
    cout << "  " << string(50, '-') << "\n\n";
    
    cout << "    (x0,y0)--------------(x1,y0)\n";
    cout << "        |                    |\n";
    cout << "        |       fy           |\n";
    cout << "        |        |           |\n";
    cout << "        |        v           |\n";
    cout << "        |        X <-- fx    |\n";
    cout << "        |     (target)       |\n";
    cout << "        |                    |\n";
    cout << "    (x0,y1)--------------(x1,y1)\n\n";
    
    cout << "  PROPERTI PENTING\n";
    cout << "  " << string(50, '-') << "\n\n";
    
    cout << "    * w00 + w10 + w01 + w11 = 1 (normalized)\n";
    cout << "    * Semakin dekat pixel, semakin besar bobotnya\n";
    cout << "    * Hasil selalu dalam range nilai tetangga\n";
    cout << "    * Menghasilkan transisi yang smooth\n";
    
    waitEnter();
}

// ============================================================================
// MAIN MENU
// ============================================================================
void printMenu() {
    cout << "\n";
    cout << "  +=========================================================+\n";
    cout << "  |                                                         |\n";
    cout << "  |        BILINEAR INTERPOLATION - VISUAL DEMO             |\n";
    cout << "  |           Algoritma Komputasi S2 - 2025                 |\n";
    cout << "  |                                                         |\n";
    cout << "  +=========================================================+\n";
    cout << "\n";
    cout << "  +-------------------  MENU  -------------------+\n";
    cout << "  |                                              |\n";
    cout << "  |  [1] Demo Single Pixel (Step-by-Step)        |\n";
    cout << "  |  [2] Demo Image Resize (4x4 -> 6x6)          |\n";
    cout << "  |  [3] Custom Koordinat                        |\n";
    cout << "  |  [4] Lihat Teori                             |\n";
    cout << "  |  [0] Keluar                                  |\n";
    cout << "  |                                              |\n";
    cout << "  +----------------------------------------------+\n";
    cout << "\n";
    cout << "  Pilih [0-4]: ";
}

int main() {
    // Set console untuk Windows
    system("chcp 65001 > nul 2>&1");
    
    // Create test image
    Image testImg(4, 4);
    float testVals[4][4] = {
        { 10,  30,  50,  70},
        { 40,  60,  80, 100},
        { 70,  90, 110, 130},
        {100, 120, 140, 160}
    };
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            testImg.set(i, j, testVals[j][i]);
    
    int choice;
    do {
        clearScreen();
        printMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                // Demo single pixel at (1.5, 1.5)
                bilinearInterpolate(testImg, 1.5f, 1.5f, true);
                break;
            }
            case 2: {
                demoResize();
                break;
            }
            case 3: {
                clearScreen();
                printTitle("CUSTOM INTERPOLATION");
                
                displayGrid(testImg, "Source Image 4x4");
                
                float x, y;
                cout << "  Masukkan koordinat (range 0.0 - 2.99):\n";
                cout << "  X = ";
                cin >> x;
                cout << "  Y = ";
                cin >> y;
                cin.ignore();
                
                x = max(0.0f, min(x, 2.99f));
                y = max(0.0f, min(y, 2.99f));
                
                bilinearInterpolate(testImg, x, y, true);
                break;
            }
            case 4: {
                showTheory();
                break;
            }
            case 0: {
                clearScreen();
                cout << "\n";
                cout << "  +=========================================================+\n";
                cout << "  |                                                         |\n";
                cout << "  |            Terima kasih telah menggunakan               |\n";
                cout << "  |        BILINEAR INTERPOLATION VISUAL DEMO               |\n";
                cout << "  |                                                         |\n";
                cout << "  |                  Sampai jumpa!                          |\n";
                cout << "  |                                                         |\n";
                cout << "  +=========================================================+\n";
                cout << "\n";
                break;
            }
            default:
                cout << "\n  Pilihan tidak valid!\n";
                cin.get();
        }
    } while (choice != 0);
    
    return 0;
}
