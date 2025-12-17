#ifndef DRAW_H
#define DRAW_H

#include <cairo.h>

typedef struct {
    double x;
    double y;
} Point;

typedef enum {
    COMP_BOX,       // Camera, Spectrograph
    COMP_MIRROR,    // Flat mirror
    COMP_LENS,      // Lens
    COMP_OAP,       // Off-axis parabola
    COMP_DM,        // Deformable Mirror
    COMP_SPLITTER,  // Beam splitter / Dichroic
    COMP_PRISM,     // Prism / Wollaston
    COMP_FILTER,    // Filter wheel
    COMP_SOURCE     // Light source
} ComponentType;

typedef struct {
    ComponentType type;
    Point pos;
    double width;
    double height;
    double rotation; // in degrees
    const char *label;
    struct { double r, g, b; } color;
} Component;

void draw_component(cairo_t *cr, Component *comp);
void draw_beam(cairo_t *cr, Point start, Point end, double r, double g, double b, double width);
void draw_arrow(cairo_t *cr, Point start, Point end, double r, double g, double b);
void draw_text(cairo_t *cr, double x, double y, const char *text, double size, int bold);
void draw_rounded_rect_border(cairo_t *cr, double x, double y, double w, double h, double r, double g, double b, const char *label);

#endif
