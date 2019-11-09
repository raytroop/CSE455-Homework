#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float sum = 0;
    for (int i = 0; i < im.w * im.h * im.c; ++i)
        sum +=im.data[i];


    for (int i = 0; i < im.w * im.h * im.c; ++i)
        if (sum != 0)
            im.data[i] /= sum;
        else
            im.data[i] = 1.0 / im.w * im.h;
}

image make_box_filter(int w)
{
    // TODO
    image im = make_image(w, w, 1);
    for (int i = 0; i < w * w; ++i)
        im.data[i] = 1.0;

    l1_normalize(im);
    return im;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == 1 || filter.c == im.c);
    image copy = make_image(im.w, im.h, im.c);
    for (int c = 0; c < copy.c; ++c) {
        for (int y = 0; y < copy.h; ++y) {
            for (int x = 0; x < copy.w; ++x) {
                int cf;
                if (filter.c == 1)
                    cf = 0;
                else
                    cf = c;

                float val = 0.0;
                for (int yf = 0; yf < filter.h; ++yf) {
                    for (int xf = 0; xf < filter.w; ++xf) {
                        int x0 = x - filter.w / 2;
                        int y0 = y - filter.h / 2;
                        val += get_pixel(filter, xf, yf, cf) * get_pixel(im, x0 + xf, y0 + yf, c);
                    }
                }

                copy.data[c * copy.w * copy.h + y * copy.w + x] = val;
            }
        }
    }

    if (!preserve) {
        image ans = make_image(im.w, im.h, 1);
        for (int y = 0; y < ans.h; ++y)
            for (int x = 0; x < ans.w; ++x) {
                int idx = y * im.w + x;
                float val = 0.0;
                for (int c = 0; c < copy.c; ++c)
                    val += copy.data[c * copy.w * copy.h + idx];

                ans.data[idx] = val;
            }

        return ans;
    }

    return copy;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 4;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;

    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 5;
    filter.data[5] = -1;
    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;

    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = -2;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;
    filter.data[6] = 0;
    filter.data[7] = 1;
    filter.data[8] = 2;

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    int w = (int)roundf(sigma * 6) + 1;
    if (w % 2 == 0)
        w++;
    image filter = make_box_filter(w);
    int xc = (int)w/2;
    int yc = xc;

    for (int y = 0; y < w; ++y)
        for (int x = 0; x < w; ++x) {
            float value = 1 / (sigma*sigma*TWOPI);
            float power = -((x-xc)*(x-xc)+(y-yc)*(y-yc)) / (sigma*sigma*2);
            filter.data[x + y*w] = value * exp(power);
        }

    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image copy = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; ++i)
        copy.data[i] = a.data[i] + b.data[i];

    return copy;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image copy = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; ++i)
        copy.data[i] = a.data[i] - b.data[i];

    return copy;
}

image make_gx_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = -1;
    filter.data[1] = 0;
    filter.data[2] = 1;
    filter.data[3] = -2;
    filter.data[4] = 0;
    filter.data[5] = 2;
    filter.data[6] = -1;
    filter.data[7] = 0;
    filter.data[8] = 1;

    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_box_filter(3);
    filter.data[0] = -1;
    filter.data[1] = -2;
    filter.data[2] = -1;
    filter.data[3] = 0;
    filter.data[4] = 0;
    filter.data[5] = 0;
    filter.data[6] = 1;
    filter.data[7] = 2;
    filter.data[8] = 1;

    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float minval = im.data[0];
    float maxval = im.data[0];

    for (int i = 1; i < im.w * im.h * im.c; ++i) {
        maxval = fmaxf(maxval, im.data[i]);
        minval = fminf(minval, im.data[i]);
    }

    float range = maxval - minval;
    if (range == 0)
        for (int i = 0; i < im.w * im.h * im.c; ++i)
            im.data[i] = 0.0;
    else
        for (int i = 0; i < im.w * im.h * im.c; ++i)
            im.data[i] = (im.data[i] - minval) / range;

}

image *sobel_image(image im)
{
    // TODO
    image filter_gx = make_gx_filter();
    image filter_gy = make_gy_filter();
    image gx = convolve_image(im, filter_gx, 0);
    image gy = convolve_image(im, filter_gy, 0);
    image *ret = calloc(2, sizeof(image));
    ret[0].w = im.w;
    ret[0].h = im.h;
    ret[0].c = 1;
    ret[0].data = calloc(im.w * im.h, sizeof(float));
    ret[1].w = im.w;
    ret[1].h = im.h;
    ret[1].c = 1;
    ret[1].data = calloc(im.w * im.h, sizeof(float));

/*  `malloc` also work

    int airbornePlanes;
    Flight *planes;
    printf(“How many planes are in the air?”);
    scanf(“%d”, &airbornePlanes);
    planes = (Flight*)malloc(sizeof(Flight) * airbornePlanes);
    if (planes == NULL) {
        printf(“Error in allocating the data array.\n”);
        ...
    }
    planes[0].altitude = ...    //Note: Can use array notation or pointer notation.
    */

    for (int i = 0; i < im.w * im.h; i++) {
        float magnitude = sqrtf(gx.data[i]*gx.data[i] + gy.data[i]*gy.data[i]);
        float gradient = atan2(gy.data[i], gx.data[i]);
        ret[0].data[i] = magnitude;
        ret[1].data[i] = gradient;
    }
    return ret;
}
/*
the magnitude to specify the saturation
the angle to specify the hue
*/
image colorize_sobel(image im)
{
    // TODO
    image* ret = sobel_image(im);
    feature_normalize(ret[0]);
    feature_normalize(ret[1]);
    image color = make_image(im.w, im.h, 3);
    memcpy(color.data, ret[1].data, sizeof(float) * im.w * im.h);
    memcpy(color.data + im.w * im.h, ret[0].data, sizeof(float) * im.w * im.h);
    memcpy(color.data + 2 * im.w * im.h, ret[0].data, sizeof(float) * im.w * im.h);

    hsv_to_rgb(color);

    return convolve_image(color, make_gaussian_filter(1), 1);
}
