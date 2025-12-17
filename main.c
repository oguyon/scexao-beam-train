#include <gtk/gtk.h>
#include <cairo.h>
#include "draw.h"

// Define drawing constants
#define IR_BENCH_Y 600
#define VIS_BENCH_Y 350
#define FIRST_BENCH_Y 100
#define CAL_SOURCE_Y 100

static void draw_ir_bench(cairo_t *cr) {
    // Border
    draw_rounded_rect_border(cr, 10, 480, 1180, 700, 0.6, 0.0, 0.0, "IR BENCH");

    // Periscope (Top Left)
    Component periscope_ir = {COMP_BOX, {60, 520}, 40, 40, 0, "Periscope", {0.2, 0.4, 0.8}};
    draw_component(cr, &periscope_ir);
    draw_text(cr, 40, 500, "To IRCS", 10, 0);

    // Input Beam (Purple from AO188)
    Point ao188_start = {20, 550};
    Point oap1_pos = {300, 580};
    draw_beam(cr, ao188_start, oap1_pos, 0.5, 0.0, 0.5, 20.0); // Purple beam
    draw_text(cr, 30, 540, "F/14 beam from AO188", 12, 0);

    // OAP1
    Component oap1 = {COMP_OAP, oap1_pos, 40, 10, 45, "OAP1", {0.5, 0.5, 0.5}};
    draw_component(cr, &oap1);

    // Beam to DM
    Point dm_pos = {150, 650};
    draw_beam(cr, oap1_pos, dm_pos, 0.5, 0.0, 0.5, 15.0);

    // Deformable Mirror
    Component dm = {COMP_DM, dm_pos, 40, 10, -45, "Deformable Mirror", {0.8, 0.8, 0.0}};
    draw_component(cr, &dm);

    // Beam to PIAA
    Point piaa_pos = {400, 650};
    draw_beam(cr, dm_pos, piaa_pos, 0.5, 0.0, 0.5, 15.0);

    // Polarizer, Pupil mask (simplified as boxes/lines on path)
    Component polarizer = {COMP_FILTER, {250, 650}, 10, 20, 90, "Polarizer", {0.8, 0.4, 0.0}};
    draw_component(cr, &polarizer);

    // PIAA Lenses
    Component piaa = {COMP_LENS, piaa_pos, 30, 10, 90, "PIAA", {0.6, 0.8, 1.0}};
    draw_component(cr, &piaa);

    // Beam to Dichroic
    Point dichroic_pos = {500, 650};
    draw_beam(cr, piaa_pos, dichroic_pos, 0.5, 0.0, 0.5, 15.0);

    // Dichroic
    Component dichroic = {COMP_SPLITTER, dichroic_pos, 30, 5, 45, "Dichroic", {0.0, 0.8, 0.0}};
    draw_component(cr, &dichroic);

    // Split to Visible Bench (Green)
    Point vis_entry = {500, 450};
    draw_beam(cr, dichroic_pos, vis_entry, 0.0, 1.0, 0.0, 10.0);
    draw_arrow(cr, (Point){500, 630}, (Point){500, 500}, 0.0, 0.8, 0.0);
    draw_text(cr, 510, 550, "Visible light (<950 nm)", 10, 0);

    // IR Path continues (Red)
    Point lyot_pos = {700, 650};
    draw_beam(cr, dichroic_pos, lyot_pos, 1.0, 0.0, 0.0, 15.0);

    // Photonics Pickoff (Green box on path)
    Component phot_pickoff = {COMP_SPLITTER, {600, 650}, 20, 5, 45, "Photonics pickoff", {0.0, 0.6, 0.0}};
    draw_component(cr, &phot_pickoff);

    // Lyot Mask
    Component lyot = {COMP_FILTER, lyot_pos, 10, 20, 90, "Lyot mask", {0.2, 0.2, 0.8}};
    draw_component(cr, &lyot);

    // Steering Mirror
    Point steer_pos = {800, 650};
    draw_beam(cr, lyot_pos, steer_pos, 1.0, 0.0, 0.0, 15.0);
    Component steer = {COMP_MIRROR, steer_pos, 30, 5, 135, "Steering mirror", {0.7, 0.7, 0.7}};
    draw_component(cr, &steer);

    // Down to Field Stop
    Point field_stop_pos = {800, 750};
    draw_beam(cr, steer_pos, field_stop_pos, 1.0, 0.0, 0.0, 15.0);

    // To CHARIS
    Point charis_split_pos = {900, 750};
    draw_beam(cr, field_stop_pos, charis_split_pos, 1.0, 0.0, 0.0, 15.0);
    Component charis_pickoff = {COMP_MIRROR, charis_split_pos, 20, 5, 45, "CHARIS pickoff", {0.7, 0.7, 0.7}};
    draw_component(cr, &charis_pickoff);

    draw_arrow(cr, charis_split_pos, (Point){900, 680}, 1.0, 0.5, 0.0);
    draw_text(cr, 910, 700, "Beam to CHARIS", 10, 0);

    // To MEC
    Point mec_split_pos = {1000, 750};
    draw_beam(cr, charis_split_pos, mec_split_pos, 1.0, 0.0, 0.0, 15.0);
    Component mec_pickoff = {COMP_MIRROR, mec_split_pos, 20, 5, 45, "MEC pickoff", {0.7, 0.7, 0.7}};
    draw_component(cr, &mec_pickoff);

    draw_arrow(cr, mec_split_pos, (Point){1100, 750}, 1.0, 0.5, 0.0);
    draw_text(cr, 1020, 770, "Beam to MEC", 10, 0);

    // Low Order WFS Camera (Below Dichroic area)
    Component llowfs = {COMP_BOX, {650, 850}, 60, 40, 0, "LLOWFS camera", {0.2, 0.2, 0.2}};
    draw_component(cr, &llowfs);

    // Connect LLOWFS (approximate path)
    draw_beam(cr, (Point){600, 650}, llowfs.pos, 1.0, 0.6, 0.6, 2.0); // Thin line for pickoff

    // C-RED ONE Spectrograph (Bottom Left)
    Component cred_one = {COMP_BOX, {200, 1000}, 80, 50, 0, "C-RED ONE", {0.8, 0.8, 0.8}};
    draw_component(cr, &cred_one);

    // Path from Periscope/IR to C-RED? The diagram is complex here.
    // It shows "To C-RED ONE" arrow from the Periscope area.
    draw_arrow(cr, (Point){60, 540}, (Point){150, 1000}, 0.8, 0.8, 0.0); // Approximate path
}

static void draw_visible_bench(cairo_t *cr) {
    // Border
    draw_rounded_rect_border(cr, 10, 200, 1180, 260, 0.6, 0.0, 0.0, "VISIBLE BENCH");

    // Input from IR Bench (Dichroic)
    Point entry_point = {500, 450}; // Matches output from IR bench

    // Path to WFS Splitter
    Point wfs_split_pos = {500, 400};
    draw_beam(cr, entry_point, wfs_split_pos, 0.0, 1.0, 0.0, 10.0);

    // WFS Beamsplitter
    Component wfs_bs = {COMP_SPLITTER, wfs_split_pos, 30, 5, -45, "WFS splitter", {0.5, 0.5, 0.5}};
    draw_component(cr, &wfs_bs);

    // Path to Pyramid WFS (Reflected right)
    Point pyr_cam_pos = {700, 400};
    draw_beam(cr, wfs_split_pos, pyr_cam_pos, 0.0, 1.0, 0.0, 8.0);

    // Components on WFS path
    Component field_stop = {COMP_FILTER, {550, 400}, 5, 15, 90, "Field stops", {0.8, 0.0, 0.0}};
    draw_component(cr, &field_stop);

    Component coll_lens = {COMP_LENS, {600, 400}, 10, 30, 0, "Coll. lens", {0.6, 0.8, 1.0}};
    draw_component(cr, &coll_lens);

    // Pyramid WFS Camera
    Component pyr_cam = {COMP_BOX, pyr_cam_pos, 40, 40, 0, "Pyramid WFS", {0.1, 0.1, 0.1}};
    draw_component(cr, &pyr_cam);

    // Path to VAMPIRES (Transmitted up/left)
    // The diagram shows it going left actually. Let's adjust.
    // In diagram: Input from bottom (IR), goes UP.
    // Diagram: Visible bench is above IR bench.
    // Input is "Visible light (<950 nm)" arrow from IR bench.

    // Periscope (Blue box) on Left
    Component periscope = {COMP_BOX, {50, 300}, 50, 50, 0, "Periscope", {0.2, 0.4, 0.8}};
    draw_component(cr, &periscope);

    // Main Green Beam from Periscope
    Point peri_pos = {50, 300};
    Point wfs_bs_actual = {250, 300};
    draw_beam(cr, peri_pos, wfs_bs_actual, 0.0, 1.0, 0.0, 12.0);

    // WFS Splitter (Red line)
    Component wfs_bs_real = {COMP_SPLITTER, wfs_bs_actual, 5, 30, 0, "WFS BS", {0.8, 0.0, 0.0}};
    draw_component(cr, &wfs_bs_real);

    // Down to Pyramid WFS
    Point pyr_wfs_final = {250, 400};
    draw_beam(cr, wfs_bs_actual, pyr_wfs_final, 0.0, 1.0, 0.0, 8.0);
    Component pyr_wfs_cam = {COMP_BOX, pyr_wfs_final, 50, 40, 0, "Pyramid WFS", {0.1, 0.1, 0.1}};
    draw_component(cr, &pyr_wfs_cam);

    // Continue right to VAMPIRES
    Point vamp_split_pos = {450, 300};
    draw_beam(cr, wfs_bs_actual, vamp_split_pos, 0.0, 1.0, 0.0, 10.0);

    // VAMPIRES/FIRST Splitter
    Component vamp_split = {COMP_SPLITTER, vamp_split_pos, 20, 5, 45, "VAMPIRES/FIRST", {0.7, 0.7, 0.7}};
    draw_component(cr, &vamp_split);

    // Up to FIRST Injection (Dashed Box)
    Point first_inj_pos = {450, 220};
    draw_beam(cr, vamp_split_pos, first_inj_pos, 1.0, 0.8, 0.0, 5.0); // Orange beam
    draw_rounded_rect_border(cr, 400, 210, 300, 60, 1.0, 0.6, 0.0, "FIRST INJECTION");

    // Right to VAMPIRES Cameras
    Point vamp_cam1_pos = {1100, 250};
    Point vamp_cam2_pos = {1100, 350};

    // Path splits
    draw_beam(cr, vamp_split_pos, (Point){800, 300}, 0.0, 1.0, 0.0, 10.0);

    // Selectable Beamsplitter
    Point sel_bs_pos = {900, 300};
    draw_beam(cr, (Point){800, 300}, sel_bs_pos, 0.0, 1.0, 0.0, 10.0);
    Component sel_bs = {COMP_SPLITTER, sel_bs_pos, 20, 20, 0, "Selectable BS", {0.5, 0.5, 1.0}};
    draw_component(cr, &sel_bs);

    // To Cam 1
    draw_beam(cr, sel_bs_pos, vamp_cam1_pos, 0.0, 1.0, 0.0, 8.0);
    Component cam1 = {COMP_BOX, vamp_cam1_pos, 40, 40, 0, "VAMP 1", {0.1, 0.1, 0.1}};
    draw_component(cr, &cam1);

    // To Cam 2
    draw_beam(cr, sel_bs_pos, vamp_cam2_pos, 0.0, 1.0, 0.0, 8.0);
    Component cam2 = {COMP_BOX, vamp_cam2_pos, 40, 40, 0, "VAMP 2", {0.1, 0.1, 0.1}};
    draw_component(cr, &cam2);
}

static void draw_calibration_source(cairo_t *cr) {
    draw_rounded_rect_border(cr, 10, 10, 480, 180, 0.6, 0.0, 0.0, "CALIBRATION SOURCE");

    // Light Sources
    Component superk1 = {COMP_BOX, {80, 40}, 120, 30, 0, "2W Fianium SuperK", {1.0, 1.0, 1.0}};
    draw_component(cr, &superk1);

    Component superk2 = {COMP_BOX, {80, 90}, 120, 30, 0, "0.2W NKT SuperK", {1.0, 1.0, 1.0}};
    draw_component(cr, &superk2);

    Component red_laser = {COMP_BOX, {80, 140}, 120, 30, 0, "Red alignment laser", {1.0, 1.0, 1.0}};
    draw_component(cr, &red_laser);

    // Fibers (Yellow) to combiner
    Point p1 = {140, 40};
    Point p2 = {140, 90};
    Point p3 = {140, 140};
    Point combiner = {300, 90};

    draw_beam(cr, p1, combiner, 1.0, 0.8, 0.0, 3.0);
    draw_beam(cr, p2, combiner, 1.0, 0.8, 0.0, 3.0);
    draw_beam(cr, p3, combiner, 1.0, 0.8, 0.0, 3.0);

    // Output arrows
    draw_arrow(cr, combiner, (Point){450, 70}, 1.0, 0.5, 0.0);
    draw_text(cr, 350, 60, "To SCExAO flat", 10, 0);

    draw_arrow(cr, combiner, (Point){450, 110}, 1.0, 0.5, 0.0);
    draw_text(cr, 350, 130, "To SCExAO focus", 10, 0);
}

static void draw_first_recombination_bench(cairo_t *cr) {
    draw_rounded_rect_border(cr, 500, 10, 690, 180, 0.6, 0.0, 0.0, "FIRST RECOMBINATION BENCH");

    // Inputs
    draw_arrow(cr, (Point){510, 40}, (Point){560, 40}, 1.0, 0.5, 0.0);
    draw_text(cr, 510, 30, "PIC", 10, 1);

    draw_arrow(cr, (Point){510, 90}, (Point){560, 90}, 1.0, 0.5, 0.0);
    draw_text(cr, 510, 80, "PL", 10, 1);

    draw_arrow(cr, (Point){510, 140}, (Point){560, 140}, 1.0, 0.5, 0.0);
    draw_text(cr, 510, 130, "FIZ", 10, 1);

    // PIC Path
    Component delay_lines = {COMP_BOX, {650, 40}, 80, 40, 0, "Delay Lines", {0.8, 0.8, 0.8}};
    draw_component(cr, &delay_lines);
    draw_beam(cr, (Point){560, 40}, (Point){610, 40}, 1.0, 0.8, 0.0, 2.0);

    Component chip = {COMP_BOX, {800, 40}, 60, 20, 0, "Chip", {0.9, 0.9, 0.9}};
    draw_component(cr, &chip);
    draw_beam(cr, (Point){690, 40}, (Point){770, 40}, 1.0, 0.8, 0.0, 2.0);

    Component sci_cam1 = {COMP_BOX, {1100, 40}, 40, 40, 0, "Sci Cam", {0.1, 0.1, 0.1}};
    draw_component(cr, &sci_cam1);
    draw_beam(cr, (Point){830, 40}, (Point){1080, 40}, 0.6, 0.6, 1.0, 5.0); // Light blue beam

    // FIZ Path
    Component mla = {COMP_LENS, {650, 140}, 10, 30, 0, "MLA", {0.6, 0.8, 1.0}};
    draw_component(cr, &mla);

    Component ana_tel = {COMP_LENS, {800, 140}, 10, 40, 0, "Anamorphic", {0.6, 0.8, 1.0}};
    draw_component(cr, &ana_tel);

    Component sci_cam2 = {COMP_BOX, {1100, 140}, 40, 40, 0, "Sci Cam", {0.1, 0.1, 0.1}};
    draw_component(cr, &sci_cam2);

    draw_beam(cr, (Point){560, 140}, (Point){1080, 140}, 0.6, 0.6, 1.0, 5.0);
}

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    // Clear background
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // Draw title
    draw_text(cr, 10, 30, "SCExAO Beam Train Schematic", 24, 1);

    draw_calibration_source(cr);
    draw_first_recombination_bench(cr);
    draw_visible_bench(cr);
    draw_ir_bench(cr);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *drawing_area;
    GtkWidget *scrolled_window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "SCExAO Beam Train");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 800);

    scrolled_window = gtk_scrolled_window_new();
    gtk_window_set_child(GTK_WINDOW(window), scrolled_window);

    drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(drawing_area), 1200);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(drawing_area), 1200);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_function, NULL, NULL);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), drawing_area);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.scexao.beamtrain", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
