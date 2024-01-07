#include <iostream>

#include "model.hpp"
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    if (std::abs(y1 - y0) <= std::abs(x0 - x1)) {
        // slope is less than 1; < pi/4
        float m = (y1 - y0) / ((float)(x1 - x0));
        float c = y0 - x0 * (m);
        for (int x = x0; x < x1; x++) {
            int y = m * x + c;
            image.set(x, y, color);
        }
    } else {
        // steep slope then render vertically
        float mp = ((float)(x1 - x0)) / (y1 - y0);
        float cp = x0 - y0 * (mp);
        for (int y = y0; y < y1; y++) {
            int x = mp * y + cp;
            image.set(x, y, color);
        }
    }
}

int main(int argc, char **argv) {
    auto width = 400;
    auto height = 400;
    TGAImage image(width, height, TGAImage::RGB);
    Model *model = new Model("./obj/african_head.obj");

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            line(x0, y0, x1, y1, image, red);
        }
    }
    image.flip_vertically(); // i want to have the origin at the left bottom
                             // corner of the image
    image.write_tga_file("output.tga");

    return 0;
}
