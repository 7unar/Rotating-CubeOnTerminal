#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define WIDTH  80
#define HEIGHT 44
#define backgroundASCIICode ' '

float A, B, C;
const float cubeWidth = 40.0f;
int distanceFromCam = 100;
float K1 = 40.0f;

float zBuffer[WIDTH * HEIGHT];
char  buffer[WIDTH * HEIGHT];

float calculateX(float i, float j, float k) {
    return j * sin(A) * sin(B) * sin(C) - k * cos(A) * sin(B) * cos(C)
         + j * cos(A) * sin(C) + k * sin(A) * sin(C)
         + i * cos(B) * cos(C);
}

float calculateY(float i, float j, float k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C)
         - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C)
         - i * cos(B) * sin(C);
}

float calculateZ(float i, float j, float k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B)
         + i * sin(B);
}

void calculateForPoint(float i, float j, float k, char ch) {
    float x = calculateX(i, j, k);
    float y = calculateY(i, j, k);
    float z = calculateZ(i, j, k) + distanceFromCam;
    float ooz = 1.0f / z;
    int xp = (int)(WIDTH  / 2.0f + K1 * ooz * x * 2.0f);
    int yp = (int)(HEIGHT / 2.0f + K1 * ooz * y);
    int idx = xp + yp * WIDTH;
    if (idx >= 0 && idx < WIDTH * HEIGHT) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    printf("\x1b[2J");
    while (1) {
        memset(buffer,  backgroundASCIICode, WIDTH * HEIGHT);
        memset(zBuffer, 0, WIDTH * HEIGHT * sizeof(float));

        float start = -cubeWidth / 2.0f;
        float end   =  cubeWidth / 2.0f;
        float step  = 0.15f;
        for (float i = start; i < end; i += step) {
            for (float j = start; j < end; j += step) {
                calculateForPoint(i, j, -cubeWidth / 2.0f, '@');
                calculateForPoint(cubeWidth / 2, j, i, '#');
                calculateForPoint(-cubeWidth / 2, j, -i, '$');
                calculateForPoint(-i, j, cubeWidth / 2, '%');
                calculateForPoint(i, -cubeWidth / 2, -j, '^');
                calculateForPoint(i, cubeWidth / 2, j, '&');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < WIDTH * HEIGHT; k++) {
            putchar(k % WIDTH ? buffer[k] : 10);
        }

        A += 0.05f;
        B += 0.05f;
        C += 0.01f;
        usleep(8000 * 2);
    }
    return 0;
}
