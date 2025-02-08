#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib> 

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma comment(lib, "glew32.lib") 

using namespace std;




const int numVertices = 50;
static int width, height;
static float pointSize = 5.5;
static int shape = 3;
#define PI 3.14159265358979324
static float R, G, B = 0;
static float brushR = 0.6, brushG = 0.5, brushB = 0.5;
static float lineThickness = 1.0f;  
static bool brushMode = false;
static bool randomColorMode = false;
float bgR = 0.8f, bgG = 0.8f, bgB = 0.8f;  


void eraseButton(void)
{

    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.42 * height;

    glColor3f(0.9333, 0.4980, 0.3725);
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    float textX = buttonX1 + (buttonWidth / 2) - 36;
    float textY = buttonY1 + (buttonHeight / 2) - 7;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"E=eraser");
}

void randomColorButton(void)
{
    glColor3f(bgR, bgG, bgB);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 130);
    glVertex2f(width - 10, 130);
    glVertex2f(width - 10, 150);
    glVertex2f(width - 70, 150);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 130);
    glVertex2f(width - 10, 130);
    glVertex2f(width - 10, 150);
    glVertex2f(width - 70, 150);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    float textX = width - 48;
    float textY = 133;

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Q");
}

void setNonConflictingRandomColor() {
    do {
        R = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        G = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        B = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    } while ((R > 0.8f && G < 0.2f && B < 0.2f) ||
             (R < 0.2f && G > 0.8f && B < 0.2f) ||
             (R < 0.2f && G < 0.2f && B > 0.8f));
}



void drawGrid(int gridSize) {
    glColor3f(0.8f, 0.9f, 0.8f); 
    glLineWidth(1.0f);          

    glBegin(GL_LINES);

   
    for (int x = 0; x <= width; x += gridSize) {
        glVertex2f(x, 0);
        glVertex2f(x, height);
    }

    
    for (int y = 0; y <= height; y += gridSize) {
        glVertex2f(0, y);
        glVertex2f(width, y);
    }

    glEnd();
}



void saveScreenshot(const char* filename)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = viewport[2];
    int height = viewport[3];

    unsigned char* pixels = new unsigned char[3 * width * height]; 

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    unsigned char* flippedPixels = new unsigned char[3 * width * height];
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            flippedPixels[(height - y - 1) * width * 3 + x * 3 + 0] = pixels[y * width * 3 + x * 3 + 0];
            flippedPixels[(height - y - 1) * width * 3 + x * 3 + 1] = pixels[y * width * 3 + x * 3 + 1];
            flippedPixels[(height - y - 1) * width * 3 + x * 3 + 2] = pixels[y * width * 3 + x * 3 + 2];
        }
    }

    std::string path = std::string(getenv("USERPROFILE")) + "\\Desktop\\" + filename;
    stbi_write_png(path.c_str(), width, height, 3, flippedPixels, width * 3);

    
    delete[] pixels;
    delete[] flippedPixels;

    std::cout << "Screenshot saved to desktop as " << filename << std::endl;
}




void setLineThickness(float thickness) {
    if (thickness > 0.0f) {  
        lineThickness = thickness;
    }
}






void lineButton(void)
{
    glColor3f(0.8, 0.8, 0.8);

    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.75 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.5, 0.5); 

    float lineX1 = buttonX1 + 0.01 * width; 
    float lineY1 = buttonY1 + buttonHeight / 2; 

    float lineX2 = buttonX1 + buttonWidth - 0.01 * width; 
    float lineY2 = lineY1; 

    glBegin(GL_LINES);
    glVertex2f(lineX1, lineY1);
    glVertex2f(lineX2, lineY2);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);  
    glLineWidth(2);  

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);  
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  
    glEnd();
}

void circleButton(void)
{
    glColor3f(0.8, 0.8, 0.8);  

    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.85 * height;

    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    glColor3f(0.0, 0.5, 0.5);  

    float circleRadius = 0.01 * width;  

    float circleCenterX = buttonX1 + buttonWidth / 2;
    float circleCenterY = buttonY1 + buttonHeight / 2;

    
    glBegin(GL_TRIANGLE_FAN);  
    glVertex2f(circleCenterX, circleCenterY);  
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;  
        float x = circleCenterX + cos(angle) * circleRadius;
        float y = circleCenterY + sin(angle) * circleRadius;
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);  
    glLineWidth(2); 

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight); 
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  
    glEnd();
}

void squareButton(void)
{
    glColor3f(0.8, 0.8, 0.8);  // رنگ دکمه (خاکی روشن‌تر)

    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.95 * height;

    // رسم دکمه به صورت چهارگوش
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    // رنگ مربع داخل دکمه
    glColor3f(0.0, 0.5, 0.5);  // رنگ مربع آبی ملایم

    // ابعاد مربع داخل دکمه
    float squareSize = 0.02 * width;  // اندازه مربع کوچکتر از اندازه دکمه

    // موقعیت مربع (مرکز مربع در مرکز دکمه)
    float squareX = buttonX1 + (buttonWidth - squareSize) / 2;
    float squareY = buttonY1 + (buttonHeight - squareSize) / 2;

    // رسم مربع داخل دکمه
    glBegin(GL_QUADS);
    glVertex2f(squareX, squareY);
    glVertex2f(squareX + squareSize, squareY);
    glVertex2f(squareX + squareSize, squareY + squareSize);
    glVertex2f(squareX, squareY + squareSize);
    glEnd();

    // رسم مرز اطراف دکمه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز سیاه
    glLineWidth(2);  // ضخامت خط مرز

    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  // پایین چپ
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  // پایین راست
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);  // بالا راست
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  // بالا چپ
    glEnd();
}








void clearButton(void)
{
    glColor3f(0.9333, 0.4980, 0.3725);  

    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.1 * height;  

   
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    // رنگ خط مرزی
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز سیاه

    glLineWidth(2);  // ضخامت خط مرز

    // رسم مرز اطراف دکمه
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  // پایین چپ
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  // پایین راست
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);  // بالا راست
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  // بالا چپ
    glEnd();

    // رنگ متن
    glColor3f(0.0, 0.0, 0.0);  // رنگ متن

    // موقعیت متن
    float textX = buttonX1 + (buttonWidth / 2) - 26;
    float textY = buttonY1 + (buttonHeight / 2) - 5;

    // نمایش متن روی دکمه
    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"C = Clear");
}


void lineclearButton(void)
{
    glColor3f(0.9333, 0.4980, 0.3725);  // رنگ پس‌زمینه دکمه (روشن‌تر)

    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    // موقعیت دکمه h زیر دکمه clear
    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.37 * height - buttonHeight - 0.01 * height;  // موقعیت دکمه h

    // رسم دکمه به صورت چهارگوش
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    // رنگ خط مرزی
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز سیاه

    glLineWidth(2);  // ضخامت خط مرز

    // رسم مرز اطراف دکمه
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  // پایین چپ
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  // پایین راست
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);  // بالا راست
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  // بالا چپ
    glEnd();

    // رنگ متن
    glColor3f(0.0, 0.0, 0.0);  // رنگ متن

    // موقعیت متن
    float textX = buttonX1 + (buttonWidth / 2) - 35;  // جابجایی متن در محور x
    float textY = buttonY1 + (buttonHeight / 2) - 5;  // جابجایی متن در محور y

    // نمایش متن روی دکمه
    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"L = lineclear");
}



void brushButton(void)
{
    glColor3f(0.9333, 0.4980, 0.3725); // رنگ پس‌زمینه دکمه (روشن‌تر)

    // ابعاد دکمه
    float buttonWidth = 0.05 * width;
    float buttonHeight = 0.05 * height;

    // موقعیت دکمه جدید بالای دکمه clear
    float buttonX1 = 0.005 * width;
    float buttonY1 = 0.20 * height;  // موقعیت Y کمی بالاتر از دکمه clear

    // رسم دکمه به صورت چهارگوش
    glBegin(GL_QUADS);
    glVertex2f(buttonX1, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1);
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);
    glVertex2f(buttonX1, buttonY1 + buttonHeight);
    glEnd();

    // رنگ خط مرزی
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز سیاه

    glLineWidth(2);  // ضخامت خط مرز

    // رسم مرز اطراف دکمه
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX1, buttonY1);  // پایین چپ
    glVertex2f(buttonX1 + buttonWidth, buttonY1);  // پایین راست
    glVertex2f(buttonX1 + buttonWidth, buttonY1 + buttonHeight);  // بالا راست
    glVertex2f(buttonX1, buttonY1 + buttonHeight);  // بالا چپ
    glEnd();

    // رنگ متن
    glColor3f(0.0, 0.0, 0.0);  // رنگ متن

    // موقعیت متن
    float textX = buttonX1 + (buttonWidth / 2) - 26;  // تنظیم موقعیت افقی متن
    float textY = buttonY1 + (buttonHeight / 2) - 5;  // تنظیم موقعیت عمودی متن

    // نمایش متن روی دکمه
    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"A = Brush");
}







void redButton(void)
{
    // رسم دکمه قرمز
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 10);
    glVertex2f(width - 10, 10);
    glVertex2f(width - 10, 30);
    glVertex2f(width - 70, 30);
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 10);
    glVertex2f(width - 10, 10);
    glVertex2f(width - 10, 30);
    glVertex2f(width - 70, 30);
    glEnd();

    // تنظیم رنگ برای رسم متن
    glColor3f(0.0, 0.0, 0.0);

    // محاسبه مختصات وسط دکمه برای نوشتن "R"
    float textX = width - 70 + (60 / 2) - 5;  // نصف عرض دکمه منهای نصف عرض متن "R"
    float textY = 4 + (20 / 2);           // نصف ارتفاع دکمه به اضافه مقدار تنظیمی برای وسط بودن عمودی

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"R");
}


void greenButton(void)
{
    // رسم دکمه سبز
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 40);
    glVertex2f(width - 10, 40);
    glVertex2f(width - 10, 60);
    glVertex2f(width - 70, 60);
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 40);
    glVertex2f(width - 10, 40);
    glVertex2f(width - 10, 60);
    glVertex2f(width - 70, 60);
    glEnd();

    // تنظیم رنگ برای رسم متن
    glColor3f(0.0, 0.0, 0.0);

    // محاسبه مختصات وسط دکمه برای نوشتن "G"
    float textX = width - 70 + (60 / 2) - 8;  // نصف عرض دکمه منهای عرض تقریبی متن "G"
    float textY = 40 + (20 / 2) - 7;          // نصف ارتفاع دکمه منهای ارتفاع تقریبی متن "G"

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"G");
}


void blueButton(void)
{
    // رسم دکمه آبی
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 70);
    glVertex2f(width - 10, 70);
    glVertex2f(width - 10, 90);
    glVertex2f(width - 70, 90);
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 70);
    glVertex2f(width - 10, 70);
    glVertex2f(width - 10, 90);
    glVertex2f(width - 70, 90);
    glEnd();

    // تنظیم رنگ برای رسم متن
    glColor3f(0.0, 0.0, 0.0);

    // محاسبه مختصات وسط دکمه برای نوشتن "B"
    float textX = width - 70 + (60 / 2) - 8;  // نصف عرض دکمه منهای عرض تقریبی متن "B"
    float textY = 70 + (20 / 2) - 6;          // نصف ارتفاع دکمه منهای ارتفاع تقریبی متن "B"

    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"B");
}


void blackButton(void)
{
    // رسم دکمه
    glColor3f(0.0, 0.0, 0.0);  // رنگ دکمه
    glBegin(GL_QUADS);
    glVertex2f(width - 70, 100);   // گوشه پایین چپ
    glVertex2f(width - 10, 100);   // گوشه پایین راست
    glVertex2f(width - 10, 120);   // گوشه بالای راست
    glVertex2f(width - 70, 120);   // گوشه بالای چپ
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(width - 70, 100);   // گوشه پایین چپ
    glVertex2f(width - 10, 100);   // گوشه پایین راست
    glVertex2f(width - 10, 120);   // گوشه بالای راست
    glVertex2f(width - 70, 120);   // گوشه بالای چپ
    glEnd();

    // رنگ برای متن
    glColor3f(1.0, 1.0, 1.0);  // سفید برای متن

    // موقعیت شروع رسم متن
    float textX = width - 45;   // موقعیت افقی برای رسم متن
    float textY = 105;          // موقعیت عمودی برای رسم متن

    // رسم حرف "S"
    glRasterPos2f(textX, textY);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'S');
}



void increaseButton(void)
{
    glColor3f(0.5, 0.5, 0.5);  // رنگ دکمه (خاکی روشن)

    // ابعاد دکمه
    float buttonWidth = 60;   // عرض دکمه
    float buttonHeight = 20;  // ارتفاع دکمه

    // موقعیت دکمه جدید دقیقا بالای دکمه آبی
    float buttonX = width - 70;  // همان موقعیت X دکمه آبی
    float buttonY = 90 + 70;     // موقعیت Y بالای دکمه آبی

    // رسم دکمه به صورت چهارگوش
    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    // رنگ متن
    glColor3f(0.0, 0.0, 0.0);  // سیاه

    // موقعیت متن
    float textX = buttonX + (buttonWidth / 2) - 5;  // تنظیم موقعیت افقی متن
    float textY = buttonY + (buttonHeight / 2) - 5;  // تنظیم موقعیت عمودی متن

    // نمایش متن روی دکمه
    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"+");
}



void decreaseButton(void)
{
    glColor3f(0.5, 0.5, 0.5);  // رنگ دکمه (خاکی روشن)

    // ابعاد دکمه
    float buttonWidth = 60;   // عرض دکمه
    float buttonHeight = 20;  // ارتفاع دکمه

    // موقعیت دکمه جدید دقیقا بالای دکمه آبی
    float buttonX = width - 70;  // همان موقعیت X دکمه آبی
    float buttonY = 90 + 100;    // موقعیت Y بالای دکمه آبی

    // رسم دکمه به صورت چهارگوش
    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    // رنگ متن
    glColor3f(0.0, 0.0, 0.0);  // سیاه

    // موقعیت متن
    float textX = buttonX + (buttonWidth / 2) - 5;  // تنظیم موقعیت افقی متن
    float textY = buttonY + (buttonHeight / 2) - 5;  // تنظیم موقعیت عمودی متن

    // نمایش متن روی دکمه
    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"-");
}



void saveButton(void)
{
    glColor3f(0.5, 0.5, 0.5);  // رنگ دکمه (خاکی روشن)

    // ابعاد دکمه
    float buttonWidth = 60;   // عرض دکمه
    float buttonHeight = 20;  // ارتفاع دکمه

    // موقعیت دکمه جدید دقیقا بالای دکمه decreaseButton
    float buttonX = width - 70;  // همان موقعیت X دکمه decreaseButton
    float buttonY = 90 + 100 + buttonHeight + 10; // موقعیت Y بالای دکمه decreaseButton (با فاصله)

    // رسم دکمه به صورت چهارگوش
    glBegin(GL_QUADS);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    // رسم مرز سیاه
    glColor3f(0.0, 0.0, 0.0);  // رنگ مرز
    glBegin(GL_LINE_LOOP);
    glVertex2f(buttonX, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY);
    glVertex2f(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2f(buttonX, buttonY + buttonHeight);
    glEnd();

    // رنگ متن
    glColor3f(0.0, 0.0, 0.0);  // سیاه

    // موقعیت متن
    float textX = buttonX + (buttonWidth / 2) - 27;  // تنظیم موقعیت افقی متن برای "Save"
    float textY = buttonY + (buttonHeight / 2) - 5;  // تنظیم موقعیت عمودی متن

    // نمایش متن روی دکمه
    glRasterPos2f(textX, textY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)"W = Save");
}







class Point
{
public:
    Point(int x, int y)
    {
        xVal = x; yVal = y;
    }
    Point() {};
    void setCoords(int x, int y)
    {
        xVal = x; yVal = y;
    }
    void drawPoint(void);
    int getCoordsX()
    {
        return xVal;
    }
    int getCoordsY()
    {
        return yVal;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
private:
    int xVal, yVal;
    float R, G, B;
    static float size;
};

float Point::size = pointSize;


void Point::drawPoint(void)
{
    glColor3f(R, G, B);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(xVal, yVal, 0.0);
    glEnd();
}





class Line
{
public:
    Line(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setPoints(Point p1, Point p2) {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }


    void drawLine(void) {
        glLineWidth(lineThickness);  // تنظیم ضخامت خط
        glBegin(GL_LINES);
        glColor3f(R, G, B);
        glVertex2f(p1.getCoordsX(), p1.getCoordsY());
        glVertex2f(p2.getCoordsX(), p2.getCoordsY());
        glEnd();
        glFlush();
    }


private:
    Point p1;
    Point p2;
    float R, G, B = 0;
};




class Circle
{
public:
    Circle(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    void setPoints(Point p1, Point p2) {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void drawCircle(void)
    {
        float t = 0;
        glBegin(GL_LINE_LOOP);

        glColor3f(R, G, B);
        float R = sqrt(pow(p2.getCoordsX() - p1.getCoordsX(), 2) + pow(p2.getCoordsY() - p1.getCoordsY(), 2));
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numVertices; ++i)
        {
            glVertex3f(p1.getCoordsX() + R * cos(t), p1.getCoordsY() + R * sin(t), 0.0);
            t += 2 * PI / numVertices;
        }
        glEnd();
        glFlush();
    }

private:
    Point p1;
    Point p2;
    float R, G, B = 0;
};




class Square
{
public:
    Square(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    void setPoints(Point p1, Point p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }
    void setColor(float R, float G, float B)
    {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void drawSquare(void)
    {
        glBegin(GL_LINE_LOOP);
        glColor3f(R, G, B);
        glVertex2f(p1.getCoordsX(), p1.getCoordsY());
        glVertex2f(p1.getCoordsX(), p2.getCoordsY());
        glVertex2f(p2.getCoordsX(), p2.getCoordsY());
        glVertex2f(p2.getCoordsX(), p1.getCoordsY());
        glEnd();
        glFlush();
    }

private:
    Point p1;
    Point p2;
    float R, G, B = 0;
};
vector<Point> points;
vector<Point>::iterator pointsIterator;
Point currentPoint;
Point lastClickedPoint;
vector<Line> lines;
vector<Line>::iterator linesIterator;
vector<Circle> circles;
vector<Circle>::iterator circleIterator;
vector<Square> squares;
vector<Square>::iterator squareIterator;




void drawScene(void)
{
    glClearColor(1.0, 1.0, 0.7, 0.0); // Set background color to white
    glClear(GL_COLOR_BUFFER_BIT);     // Clear the color buffer

    // Draw the buttons for selecting the shapes
    drawGrid(30);
    circleButton();
    squareButton();
    lineButton();
    redButton();
    greenButton();
    blueButton();
    blackButton();
    clearButton();  // Draw the "Clear" button
    increaseButton();
    decreaseButton();
    brushButton();
    lineclearButton();
    randomColorButton();
    saveButton();
    eraseButton();



    linesIterator = lines.begin();
    while (linesIterator != lines.end())
    {
        linesIterator->drawLine();  // Draw each line
        linesIterator++;
    }

    pointsIterator = points.begin();
    while (pointsIterator != points.end())
    {
        pointsIterator->drawPoint();  // Draw each point
        pointsIterator++;
    }

    circleIterator = circles.begin();
    while (circleIterator != circles.end())
    {
        circleIterator->drawCircle();  // Draw each circle
        circleIterator++;
    }

    squareIterator = squares.begin();
    while (squareIterator != squares.end())
    {
        squareIterator->drawSquare();  // Draw each square
        squareIterator++;
    }

    // Draw the shape in progress (line, circle, or square)
    switch (shape)
    {
    case 0:  // Line
    {
        Line currentLine(lastClickedPoint, currentPoint);
        currentLine.setColor(R, G, B);
        currentLine.drawLine();
        break;
    }
    case 2:  // Circle
    {
        Circle currentCircle(lastClickedPoint, currentPoint);
        currentCircle.setColor(R, G, B);
        currentCircle.drawCircle();
        break;
    }
    case 3:  // Square
    {
        Square currentSquare(lastClickedPoint, currentPoint);
        currentSquare.setColor(R, G, B);
        currentSquare.drawSquare();
        break;
    }
    }




    if (brushMode) {
        pointsIterator = points.begin();
        while (pointsIterator != points.end()) {

            currentPoint.setColor(R, G, B);
            pointsIterator->drawPoint();
            pointsIterator++;
        }
    }

    glFlush();  // Ensure all OpenGL commands are executed
}



void clearShapes()
{
    points.clear();
    lines.clear();
    circles.clear();
    squares.clear();
    glutPostRedisplay();  // Redraw the screen to reflect the cleared shapes
}



void clearline()
{

    lines.clear();

    glutPostRedisplay();  // Redraw the screen to reflect the cleared shapes
}





void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        exit(0);
        break;
    case '1':
        shape = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case '2':
        shape = 2;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case '3':
        shape = 3;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'r':
        R = 1;
        G = 0;
        B = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'b':
        R = 0;
        G = 0;
        B = 1;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'e':
        R = 1;
        G = 1;
        B = 0.7;
        pointSize = 15;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'g':
        R = 0;
        G = 1;
        B = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 's':
        R = 0;
        G = 0;
        B = 0;
        lastClickedPoint.setCoords(0, 0);
        currentPoint.setCoords(0, 0);
        glutPostRedisplay();
        break;
    case 'c':
        clearShapes();
        break;
    case 'l':
        clearline();
        break;
    case 'a':  // فعال کردن حالت قلم‌مو
        brushMode = !brushMode;
        if (brushMode) {
            std::cout << "Brush mode activated!" << std::endl;
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);  // تغییر به نشانه‌گر قلم‌مو
        }
        else {
            std::cout << "Brush mode deactivated!" << std::endl;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);  // بازگرداندن نشانه‌گر پیش‌فرض
        }
        break;

    case 'w': // 's' key for saving screenshot
        saveScreenshot("screenshot.png");
        glutPostRedisplay();
        break;
    case '+':  // افزایش ضخامت خط
        setLineThickness(lineThickness + 1.0f); // افزایش ضخامت
        break;
    case '-':  // کاهش ضخامت خط
        setLineThickness(lineThickness - 1.0f); // کاهش ضخامت
        break;
       
    case 'q':  // ???? ???? ??? ??????
        randomColorMode = !randomColorMode;
        if (randomColorMode) {
            std::cout << "Random color mode activated!" << std::endl;
        }
        else {
            std::cout << "Random color mode deactivated!" << std::endl;
        }
        glutPostRedisplay();
        break;




    default:
        break;

    }
}


void mouseControl(int button, int state, int x, int y)
{
    int viewportY = height - y;  // تبدیل مختصات به مختصات OpenGL


    // دکمه افزایش ضخامت
    if (x >= width - 70 && x <= width - 10 && viewportY >= 160 && viewportY <= 180) {
        setLineThickness(lineThickness + 1.0f); // افزایش ضخامت
        glutPostRedisplay();
        return;
    }

    // دکمه کاهش ضخامت
    if (x >= width - 70 && x <= width - 10 && viewportY >= 190 && viewportY <= 210) {
        setLineThickness(lineThickness - 1.0f); // کاهش ضخامت
        glutPostRedisplay();
        return;
    }

    // دکمه Save Screenshot
    if (x >= (width - 70) && x <= (width - 10) &&
        viewportY >= (90 + 100 + 20 + 10) &&
        viewportY <= (90 + 100 + 20 + 10 + 20)) {
        std::cout << "Save Screenshot button clicked!" << std::endl;
        saveScreenshot("Paint.jpg"); // ذخیره اسکرین‌شات
        glutPostRedisplay();
        return;
    }


    // دکمه Lineclear
    if (x >= 0.005 * width && x <= 0.055 * width &&
        viewportY >= (0.37 * height - 0.05 * height - 0.01 * height) &&
        viewportY <= (0.37 * height - 0.01 * height)) {
        std::cout << "Lineclear button clicked!" << std::endl;
        clearline(); // پاک کردن خطوط
        glutPostRedisplay(); // بازخوانی صفحه
        return;
    }


    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float buttonX1 = 0.005 * width;
        float buttonX2 = buttonX1 + (0.05 * width);
        float buttonY1 = 0.42 * height;
        float buttonY2 = buttonY1 + (0.05 * height);

        if (x >= buttonX1 && x <= buttonX2 && viewportY >= buttonY1 && viewportY <= buttonY2) {
            std::cout << "Erase button clicked!" << std::endl;
            keyInput('e', 0, 0);  // ???????? ?????? ????? ?? ??????
            return;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // ????? ???? ??? ???? ??? ??????
        if (x >= width - 70 && x <= width - 10 && viewportY >= 130 && viewportY <= 150) {
            std::cout << "Random Color button clicked!" << std::endl;
            randomColorMode = !randomColorMode;  // ????? ????? ???? ??? ??????
            if (randomColorMode) {
                std::cout << "Random color mode activated!" << std::endl;
            }
            else {
                std::cout << "Random color mode deactivated!" << std::endl;
            }
            glutPostRedisplay();
            return;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        if (x >= 0.005 * width && x <= 0.055 * width &&
             viewportY >= 0.20 * height && viewportY <= 0.25 * height) {
            std::cout << "Brush button clicked!" << std::endl;
            brushMode = !brushMode;  // تغییر وضعیت قلم‌مو
            if (brushMode) {
                std::cout << "Brush mode activated!" << std::endl;
            }
            else {
                std::cout << "Brush mode deactivated!" << std::endl;
            }
            glutPostRedisplay();
            return;  // خروج از تابع، کلیک در جای دیگری پردازش نشود
        }

    }


    if (brushMode) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // اضافه کردن نقاط جدید برای brushMode
            lastClickedPoint.setCoords(x, viewportY);
            currentPoint = Point(x, viewportY);
            points.push_back(currentPoint);  // اضافه کردن نقطه به لیست نقاط
            return;
        }
    }



    // بررسی کلیک روی دکمه‌ها
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // دکمه Line
        if (x >= 0.005 * width && x <= 0.055 * width && viewportY >= 0.75 * height && viewportY <= 0.8 * height) {
            shape = 0;  // انتخاب حالت Line
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // دکمه Circle
        if (x >= 0.005 * width && x <= 0.055 * width && viewportY >= 0.85 * height && viewportY <= 0.9 * height) {
            shape = 2;  // انتخاب حالت Circle
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // دکمه Square
        if (x >= 0.005 * width && x <= 0.055 * width && viewportY >= 0.95 * height && viewportY <= 1.0 * height) {
            shape = 3;  // انتخاب حالت Square
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // دکمه Clear
        if (x >= 0.005 * width && x <= 0.055 * width && viewportY >= 0.1 * height && viewportY <= 0.15 * height) {
            clearShapes();  // پاک کردن تمام اشکال
            return;
        }

        // دکمه رنگ قرمز
        if (x >= width - 70 && x <= width - 10 && viewportY >= 10 && viewportY <= 30) {
            R = 1.0f; G = 0.0f; B = 0.0f;  // تنظیم رنگ قرمز
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // دکمه رنگ سبز
        if (x >= width - 70 && x <= width - 10 && viewportY >= 40 && viewportY <= 60) {
            R = 0.0f; G = 1.0f; B = 0.0f;  // تنظیم رنگ سبز
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // دکمه رنگ آبی
        if (x >= width - 70 && x <= width - 10 && viewportY >= 70 && viewportY <= 90) {
            R = 0.0f; G = 0.0f; B = 1.0f;  // تنظیم رنگ آبی
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }

        // دکمه رنگ مشکی
        if (x >= width - 70 && x <= width - 10 && viewportY >= 100 && viewportY <= 120) {
            R = 0.0f; G = 0.0f; B = 0.0f;  // تنظیم رنگ مشکی
            lastClickedPoint.setCoords(0, 0);
            currentPoint.setCoords(0, 0);
            glutPostRedisplay();
            return;
        }
    }

    // در غیر این صورت، باید نقاشی‌های دیگر انجام شوند.
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastClickedPoint.setCoords(x, viewportY);
        currentPoint = Point(x, viewportY);

        // رسم اشکال به حالت عادی
        switch (shape) {
        case 0:  // رسم خط
        {
            Line line(lastClickedPoint, currentPoint);
            line.setColor(R, G, B);
            lines.push_back(line);
            break;
        }
        case 2:  // رسم دایره
        {
            Circle circle(lastClickedPoint, currentPoint);
            circle.setColor(R, G, B);
            circles.push_back(circle);
            break;
        }
        case 3:  // رسم مربع
        {
            Square square(lastClickedPoint, currentPoint);
            square.setColor(R, G, B);
            squares.push_back(square);
            break;
        }
        }

        glutPostRedisplay();  // بازسازی صفحه نمایش
    }

    // در حالت رها کردن موس
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // برای حالت brushMode هیچ عملی نباید انجام شود
        if (brushMode) return;

        // اشکال نهایی بعد از رها کردن دکمه موس باید رسم شوند
        switch (shape) {
        case 0:  // رسم خط
        {
            Line line(lastClickedPoint, currentPoint);
            line.setColor(R, G, B);
            lines.push_back(line);
            break;
        }
        case 2:  // رسم دایره
        {
            Circle circle(lastClickedPoint, currentPoint);
            circle.setColor(R, G, B);
            circles.push_back(circle);
            break;
        }
        case 3:  // رسم مربع
        {
            Square square(lastClickedPoint, currentPoint);
            square.setColor(R, G, B);
            squares.push_back(square);
            break;
        }
        }

        glutPostRedisplay();  // بازسازی صفحه نمایش
    }
}






void mouseMotion(int x, int y)
{

    currentPoint.setCoords(x, height - y);
    Line line(lastClickedPoint, currentPoint);



    if (brushMode) {
        int dx = currentPoint.getCoordsX() - lastClickedPoint.getCoordsX();
        int dy = currentPoint.getCoordsY() - lastClickedPoint.getCoordsY();
        int steps = std::max(abs(dx), abs(dy));
        float stepX = dx / static_cast<float>(steps);
        float stepY = dy / static_cast<float>(steps);

        for (int i = 0; i <= steps; i++) {
            int interpolatedX = lastClickedPoint.getCoordsX() + i * stepX;
            int interpolatedY = lastClickedPoint.getCoordsY() + i * stepY;

            Point interpolatedPoint(interpolatedX, interpolatedY);

            if (randomColorMode) {  
                setNonConflictingRandomColor();
            }

            interpolatedPoint.setColor(R, G, B);
            points.push_back(interpolatedPoint);
        }

        lastClickedPoint = currentPoint;
        points.push_back(currentPoint);
    }



    if (brushMode) {
        int dx = currentPoint.getCoordsX() - lastClickedPoint.getCoordsX();
        int dy = currentPoint.getCoordsY() - lastClickedPoint.getCoordsY();


        int steps = std::max(abs(dx), abs(dy));

        
        float stepX = dx / (float)steps;
        float stepY = dy / (float)steps;

        for (int i = 0; i <= steps; i++) {
            int interpolatedX = lastClickedPoint.getCoordsX() + i * stepX;
            int interpolatedY = lastClickedPoint.getCoordsY() + i * stepY;

            Point interpolatedPoint(interpolatedX, interpolatedY);
            interpolatedPoint.setColor(R, G, B); 

            points.push_back(interpolatedPoint); 
        }

        lastClickedPoint = currentPoint; 
        points.push_back(currentPoint);
    }
    else {
        Line line(lastClickedPoint, currentPoint);
        line.setColor(R, G, B);
        line.drawLine();  
    }

    glutPostRedisplay();
}

void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);


    width = w;
    height = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Left mouse was clicked on the screen , hold it to draw" << endl
        << "click right mouse for going out" << endl;
}




int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(600, 350);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Paint");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glutMouseFunc(mouseControl);

    glutMotionFunc(mouseMotion);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
