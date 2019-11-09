#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);;
}

/*
given: a * x_new + b = x_org

a * (-0.5) + b = -0.5
a * (w_new - 0.5) + b = w_org - 0.5
==>
a = w_org / w_new
b = 0.5*a - 0.5
*/
image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image copy = make_image(w, h, im.c);
    float ax = (float)im.w / w;
    float bx = 0.5*ax - 0.5;
    float ay = (float)im.h / h;
    float by = 0.5*ay - 0.5;

    for (int c = 0; c < im.c; ++c)
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) {
                copy.data[c * w * h + y * w + x] = nn_interpolate(im, ax * x + bx, ay * y + by, c);
            }
    return copy;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    int left = floorf(x);
    int right = ceilf(x);
    int top = floorf(y);
    int bottom = ceilf(y);
    float v1 = get_pixel(im, left, top, c);
    float v2 = get_pixel(im, right, top, c);
    float v3 = get_pixel(im, left, bottom, c);
    float v4 = get_pixel(im, right, bottom, c);
    float d1 = bottom - y;
    float d2 = y - top;
    float d3 = x - left;
    float d4 = right - x;
    float q1 = v1*d1 + v3*d2;
    float q2 = v2*d1 + v4*d2;
    float q = q2*d3 + q1*d4;

    return q;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image copy = make_image(w, h, im.c);
    float ax = (float)im.w / w;
    float bx = 0.5*ax - 0.5;
    float ay = (float)im.h / h;
    float by = 0.5*ay - 0.5;

    for (int c = 0; c < im.c; ++c)
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) {
                copy.data[c * w * h + y * w + x] = bilinear_interpolate(im, ax * x + bx, ay * y + by, c);
            }
    return copy;
}

