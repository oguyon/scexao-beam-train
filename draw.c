#include "draw.h"
#include <math.h>

#define DEG2RAD(angle) ((angle) * M_PI / 180.0)

void draw_text(cairo_t *cr, double x, double y, const char *text, double size, int bold) {
    cairo_save(cr);
    // Default text color is white for dark theme, unless caller set it (but here we reset)
    // To support caller setting color, we should probably not set it here, or have a theme arg.
    // For now, hardcode white.
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_select_font_face(cr, "Sans",
        CAIRO_FONT_SLANT_NORMAL,
        bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, size);
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);
    cairo_restore(cr);
}

void draw_rounded_rect_border(cairo_t *cr, double x, double y, double w, double h, double r, double g, double b, const char *label) {
    cairo_save(cr);
    double radius = 20.0;

    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - radius, y + radius, radius, -M_PI / 2, 0);
    cairo_arc(cr, x + w - radius, y + h - radius, radius, 0, M_PI / 2);
    cairo_arc(cr, x + radius, y + h - radius, radius, M_PI / 2, M_PI);
    cairo_arc(cr, x + radius, y + radius, radius, M_PI, 3 * M_PI / 2);
    cairo_close_path(cr);

    cairo_set_source_rgb(cr, r, g, b);
    cairo_set_line_width(cr, 3.0);
    cairo_stroke(cr);

    if (label) {
        draw_text(cr, x + 20, y + h - 15, label, 24, 1); // Bold label at bottom left-ish
    }
    cairo_restore(cr);
}

void draw_beam(cairo_t *cr, Point start, Point end, double r, double g, double b, double width) {
    cairo_save(cr);
    cairo_set_source_rgba(cr, r, g, b, 0.4); // Semi-transparent
    cairo_set_line_width(cr, width);
    cairo_move_to(cr, start.x, start.y);
    cairo_line_to(cr, end.x, end.y);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void draw_arrow(cairo_t *cr, Point start, Point end, double r, double g, double b) {
    // Draws a thick arrow for diagram annotations
    cairo_save(cr);
    cairo_set_source_rgb(cr, r, g, b);

    double angle = atan2(end.y - start.y, end.x - start.x);
    double head_len = 15.0;

    cairo_move_to(cr, start.x, start.y);
    cairo_line_to(cr, end.x, end.y);
    cairo_set_line_width(cr, 4.0);
    cairo_stroke(cr);

    cairo_move_to(cr, end.x, end.y);
    cairo_line_to(cr, end.x - head_len * cos(angle - M_PI/6), end.y - head_len * sin(angle - M_PI/6));
    cairo_line_to(cr, end.x - head_len * cos(angle + M_PI/6), end.y - head_len * sin(angle + M_PI/6));
    cairo_close_path(cr);
    cairo_fill(cr);

    cairo_restore(cr);
}

void draw_component(cairo_t *cr, Component *comp) {
    cairo_save(cr);

    cairo_translate(cr, comp->pos.x, comp->pos.y);
    cairo_rotate(cr, DEG2RAD(comp->rotation));

    cairo_set_source_rgb(cr, comp->color.r, comp->color.g, comp->color.b);

    switch (comp->type) {
        case COMP_BOX:
        case COMP_SOURCE:
            cairo_rectangle(cr, -comp->width/2, -comp->height/2, comp->width, comp->height);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White border
            cairo_set_line_width(cr, 1.0);
            cairo_stroke(cr);
            break;

        case COMP_MIRROR:
            cairo_set_line_width(cr, 3.0);
            // If color is too dark, lighten it? Or just assume caller provides good colors.
            // Mirrors are often gray. Let's force a lighter gray outline if needed or just use current.
            // But if it's black/dark gray it won't show.
            // Let's rely on the passed color, but if it was intended for white bg it might be dark.
            // If the color passed is dark, we might want to invert or lighten.
            // For now, assume caller fixes component colors or they are already distinct.
            cairo_move_to(cr, -comp->width/2, 0);
            cairo_line_to(cr, comp->width/2, 0);
            cairo_stroke(cr);
            break;

        case COMP_LENS:
            cairo_save(cr);
            cairo_scale(cr, 1.0, comp->height/comp->width); // Ellipse
            cairo_arc(cr, 0, 0, comp->width/2, 0, 2*M_PI);
            cairo_restore(cr);
            cairo_set_source_rgba(cr, 0.6, 0.8, 1.0, 0.5); // Light blue glass
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 0.8, 0.8, 1.0); // Light blue outline
            cairo_set_line_width(cr, 1.0);
            cairo_stroke(cr);
            break;

        case COMP_OAP:
            cairo_arc(cr, -comp->width, 0, comp->width, -0.2, 0.2); // Small arc segment
            cairo_set_line_width(cr, 3.0);
            cairo_stroke(cr);
            break;

        case COMP_DM:
            // Wavy line representation
            cairo_move_to(cr, -comp->width/2, 0);
            for(double x = -comp->width/2; x <= comp->width/2; x += 2) {
                cairo_line_to(cr, x, 2 * sin(x/2));
            }
            cairo_set_line_width(cr, 2.0);
            cairo_set_source_rgb(cr, 0.8, 0.8, 0.0); // Gold-ish
            cairo_stroke(cr);
            break;

        case COMP_SPLITTER:
        case COMP_PRISM:
            cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
            cairo_rectangle(cr, -comp->width/2, -comp->height/2, comp->width, comp->height);
            cairo_fill(cr);
            cairo_move_to(cr, -comp->width/2, comp->height/2);
            cairo_line_to(cr, comp->width/2, -comp->height/2);
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White line
            cairo_stroke(cr);
            break;

        case COMP_FILTER:
            cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
            cairo_rectangle(cr, -comp->width/2, -comp->height/2, comp->width, comp->height);
            cairo_fill(cr);
            break;

        default:
            break;
    }

    cairo_restore(cr);

    // Label
    if (comp->label) {
        draw_text(cr, comp->pos.x - 20, comp->pos.y + comp->height/2 + 15, comp->label, 10, 0);
    }
}
