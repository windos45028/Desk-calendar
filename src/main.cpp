#include "raylib.h"
#include <iostream>

// macOS 特有：用於取得執行檔絕對路徑與切換工作目錄
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <unistd.h>
#include <libgen.h>
#endif

void ChangeToResourcesDir() {
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        // 取得執行檔所在的目錄 (即 .../calendar.app/Contents/MacOS)
        char* dir = dirname(path);
        // 將程式的工作目錄強制切換到執行檔身邊
        chdir(dir);
        std::cout << "Current Working Directory set to: " << dir << std::endl;
    }
#endif
}

int main() {
    // 1. 【關鍵修復】在載入任何資源之前，強制把 Working Directory 切換到 .app 裡面！
    ChangeToResourcesDir();

    InitWindow(800, 600, "Raylib Calendar");

    // 1. 載入 PNG 圖片資源
    Image icon = LoadImage("assets/img/12.png"); 

    ImageResize(&icon, 256, 256);

    // 2. 設定為視窗圖示
    SetWindowIcon(icon);

    // 3. 設定完後，原圖指標就可以釋放掉了
    UnloadImage(icon);

    // 1. 定義你要顯示的中文字串
    const char* utf8Text = "2026 年 9 月 - 月曆";

    // 2. 告訴 Raylib 要載入哪些中文字碼 (Codepoints)
    int codepointsCount = 0;
    // 將 UTF-8 字串轉為 Raylib 看得懂的 Unicode 碼點陣列
    int* codepoints = LoadCodepoints(utf8Text, &codepointsCount);

    // 3. 載入支援中文的字型檔 (請準備一個 ttf 檔放在專案目錄，例如 SourceHanSans.ttf)
    // 參數說明：字型路徑, 字型大小, 碼點陣列, 碼點數量
    Font fontChinese = LoadFontEx("assets/SourceHanSans.ttf", 32, codepoints, codepointsCount);

    // 釋放剛才轉碼用的暫存記憶體
    UnloadCodepoints(codepoints);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // 4. 使用專用繪製函數 DrawTextEx 畫出中文字
            Vector2 position = { 100.0f, 100.0f };
            DrawTextEx(fontChinese, utf8Text, position, 32.0f, 2.0f, BLACK);

        EndDrawing();
    }

    // 5. 離開前記得釋放字型資源
    UnloadFont(fontChinese);
    CloseWindow();

    return 0;
}