#include <gtk/gtk.h>
#include <cairo.h>
#include "draw.h"

// SCExAO Beam Train Visualization
// Coordinates are approximated based on the reference diagram.

static void draw_ir_bench(cairo_t *cr) {
    // Border for IR Bench
    draw_rounded_rect_border(cr, 10, 580, 1180, 600, 0.6, 0.0, 0.0, "IR BENCH");

    // --- IR PATH ---

    // 1. Input from AO188 (Purple)
    Point ao188_start = {20, 700};
    Point oap1_pos = {280, 740};

    // Periscope (Top Left of IR Bench)
    Component periscope_ir = {COMP_BOX, {60, 650}, 40, 40, 0, "Periscope", {0.2, 0.4, 0.8}};
    draw_component(cr, &periscope_ir);
    draw_text(cr, 30, 630, "To IRCS", 10, 0);

    draw_beam(cr, ao188_start, oap1_pos, 0.6, 0.0, 0.8, 15.0); // Purple beam
    draw_text(cr, 30, 690, "F/14 beam from AO188", 12, 1);

    // Integrating Sphere (Below input)
    Component int_sphere = {COMP_BOX, {80, 800}, 40, 40, 0, "Int. Sphere", {0.3, 0.3, 0.3}};
    draw_component(cr, &int_sphere);
    // Arrow from beam to sphere roughly
    draw_arrow(cr, (Point){150, 720}, (Point){100, 780}, 0.5, 0.5, 0.5);

    // 2. OAP1
    Component oap1 = {COMP_OAP, oap1_pos, 40, 10, 45, "OAP1", {0.8, 0.8, 0.8}};
    draw_component(cr, &oap1);

    // 3. OAP1 -> DM (Yellowish reflection)
    Point dm_pos = {120, 850};
    draw_beam(cr, oap1_pos, dm_pos, 0.6, 0.0, 0.8, 12.0);

    // 4. Deformable Mirror (2k DM)
    Component dm = {COMP_DM, dm_pos, 40, 10, -45, "2k DM", {0.8, 0.8, 0.0}};
    draw_component(cr, &dm);

    // Internal NIR Camera (Below DM)
    Component nir_cam = {COMP_BOX, {120, 950}, 50, 30, 0, "Int. NIR Cam", {0.2, 0.2, 0.2}};
    draw_component(cr, &nir_cam);
    draw_arrow(cr, dm_pos, (Point){120, 930}, 0.5, 0.5, 0.5); // Pickoff representation

    // 5. DM -> OAP2/PIAA
    Point piaa_pos = {400, 850};
    draw_beam(cr, dm_pos, piaa_pos, 0.6, 0.0, 0.8, 12.0);

    // Components on this path (Polarizer, Pupil Mask)
    Component polarizer = {COMP_FILTER, {220, 850}, 8, 20, 90, "Polarizer", {0.8, 0.4, 0.0}};
    draw_component(cr, &polarizer);

    Component pupil_mask = {COMP_FILTER, {260, 850}, 8, 25, 90, "Pupil Mask", {0.8, 0.4, 0.0}};
    draw_component(cr, &pupil_mask);

    // OAP2 / PIAA
    Component piaa = {COMP_LENS, piaa_pos, 30, 15, 90, "OAP2/PIAA", {0.6, 0.8, 1.0}};
    draw_component(cr, &piaa);

    // 6. PIAA -> Dichroic
    Point dichroic_pos = {550, 850};
    draw_beam(cr, piaa_pos, dichroic_pos, 0.6, 0.0, 0.8, 12.0);

    // ADC on this path
    Component adc = {COMP_PRISM, {480, 850}, 20, 15, 0, "ADC", {0.7, 0.7, 1.0}};
    draw_component(cr, &adc);

    // 7. Dichroic Split
    Component dichroic = {COMP_SPLITTER, dichroic_pos, 30, 5, 45, "VIS/IR Dichroic", {0.0, 0.8, 0.0}};
    draw_component(cr, &dichroic);

    // --- IR BRANCH (Continues Right -> Red) ---
    Point lyot_pos = {750, 850};
    draw_beam(cr, dichroic_pos, lyot_pos, 1.0, 0.0, 0.0, 12.0); // Red beam

    // LLOWFS Pickoff (Green box)
    Point llowfs_pickoff_pos = {650, 850};
    Component pickoff_llowfs = {COMP_SPLITTER, llowfs_pickoff_pos, 15, 5, 45, "Pickoff", {0.0, 0.6, 0.0}};
    draw_component(cr, &pickoff_llowfs);

    // Path to LLOWFS (Down)
    Component llowfs = {COMP_BOX, {650, 1000}, 60, 40, 0, "LLOWFS", {0.2, 0.2, 0.2}};
    draw_component(cr, &llowfs);
    draw_beam(cr, llowfs_pickoff_pos, llowfs.pos, 1.0, 0.6, 0.0, 3.0);

    // GLINT Path (Up from pickoff)
    draw_arrow(cr, llowfs_pickoff_pos, (Point){650, 780}, 0.8, 0.2, 0.2);
    draw_text(cr, 620, 770, "To GLINT", 10, 0);

    // Lyot Stop
    Component lyot = {COMP_FILTER, lyot_pos, 10, 20, 90, "Lyot Stop", {0.2, 0.2, 0.8}};
    draw_component(cr, &lyot);

    // Steering Mirror
    Point steer_pos = {850, 850};
    draw_beam(cr, lyot_pos, steer_pos, 1.0, 0.0, 0.0, 12.0);
    Component steer = {COMP_MIRROR, steer_pos, 30, 5, 135, "Steering Mirror", {0.7, 0.7, 0.7}};
    draw_component(cr, &steer);

    // Down to Field Stop
    Point field_stop_pos = {850, 950};
    draw_beam(cr, steer_pos, field_stop_pos, 1.0, 0.0, 0.0, 12.0);

    // Field Stop
    Component fs_ir = {COMP_FILTER, field_stop_pos, 15, 15, 0, "Field Stop", {0.0, 0.0, 0.0}};
    draw_component(cr, &fs_ir);

    // Focal Plane Camera
    Component focal_cam = {COMP_BOX, {800, 950}, 40, 30, 0, "Focal Plane", {0.2, 0.2, 0.2}};
    draw_component(cr, &focal_cam);

    // Split to CHARIS / MEC
    Point split_charis = {950, 950};
    draw_beam(cr, field_stop_pos, split_charis, 1.0, 0.0, 0.0, 12.0);

    // CHARIS Pickoff
    Component charis_pick = {COMP_MIRROR, split_charis, 20, 5, 45, "CHARIS Pick", {0.7, 0.7, 0.7}};
    draw_component(cr, &charis_pick);

    // To CHARIS (Up)
    draw_arrow(cr, split_charis, (Point){950, 880}, 1.0, 0.4, 0.0);
    draw_text(cr, 960, 890, "To CHARIS", 10, 0);

    // To MEC (Right)
    Point split_mec = {1050, 950};
    draw_beam(cr, split_charis, split_mec, 1.0, 0.0, 0.0, 12.0);

    // MEC Pickoff
    Component mec_pick = {COMP_MIRROR, split_mec, 20, 5, 45, "MEC Pick", {0.7, 0.7, 0.7}};
    draw_component(cr, &mec_pick);

    // To MEC (Right/Arrow)
    draw_arrow(cr, split_mec, (Point){1120, 950}, 1.0, 0.4, 0.0);
    draw_text(cr, 1060, 980, "To MEC", 10, 0);

    // REACH Path (Top Right of IR Bench area)
    Component mcf = {COMP_BOX, {1000, 700}, 40, 20, 0, "MCF", {0.7, 0.7, 0.0}};
    draw_component(cr, &mcf);
    draw_text(cr, 1000, 680, "To REACH switchyard", 10, 0);

    // C-RED ONE (Bottom Left)
    Component cred = {COMP_BOX, {250, 1100}, 80, 50, 0, "C-RED ONE", {0.8, 0.8, 0.8}};
    draw_component(cr, &cred);
    draw_arrow(cr, (Point){100, 680}, (Point){200, 1080}, 0.5, 0.5, 0.5); // Abstract connection
}

static void draw_visible_bench(cairo_t *cr) {
    draw_rounded_rect_border(cr, 10, 200, 1180, 350, 0.6, 0.0, 0.0, "VISIBLE BENCH");

    // Input from IR Bench Dichroic (550, 850)
    Point entry_start = {550, 580}; // Where it enters the box
    Point wfs_split = {550, 480};

    draw_beam(cr, (Point){550, 850}, wfs_split, 0.0, 1.0, 0.0, 10.0); // Green beam coming up
    draw_text(cr, 560, 600, "Visible light (<950nm)", 10, 0);

    // WFS Splitter
    Component wfs_bs = {COMP_SPLITTER, wfs_split, 30, 5, -45, "WFS Splitter", {0.5, 0.5, 0.5}};
    draw_component(cr, &wfs_bs);

    // --- PYRAMID WFS PATH (Right) ---
    Point pyr_lens_pos = {650, 480};
    draw_beam(cr, wfs_split, pyr_lens_pos, 0.0, 1.0, 0.0, 8.0);

    // Field Stop
    Component fs_vis = {COMP_FILTER, {600, 480}, 5, 15, 90, "FS", {0.8, 0.0, 0.0}};
    draw_component(cr, &fs_vis);

    // Lens
    Component lens_vis = {COMP_LENS, pyr_lens_pos, 10, 30, 0, "Lens", {0.6, 0.8, 1.0}};
    draw_component(cr, &lens_vis);

    // Pyramid WFS Camera
    Component pyr_cam = {COMP_BOX, {750, 480}, 50, 40, 0, "Pyramid WFS", {0.1, 0.1, 0.1}};
    draw_component(cr, &pyr_cam);

    // --- VAMPIRES PATH (Left/Up) ---
    // Reflected by WFS Splitter?
    // Wait, the diagram shows the beam splitting.
    // Let's assume Transmitted = VAMPIRES (Up), Reflected = WFS (Right).
    // Actually the diagram has a "Periscope" on the left side of the Visible bench.

    // Let's trace back from VAMPIRES.
    // The beam goes Left from the WFS splitter in the diagram?
    // No, standard SCExAO:
    // Dichroic -> Up -> WFS Splitter.
    // Transmitted -> VAMPIRES. Reflected -> PyWFS.
    // OR vice versa.

    // Looking at the drawing:
    // "Visible light" arrow goes UP.
    // Hits a splitter.
    // Right -> Pyramid WFS.
    // Left -> Periscope -> VAMPIRES.

    Point peri_in = {550, 480};
    Point peri_out = {100, 480}; // Goes left

    draw_beam(cr, peri_in, peri_out, 0.0, 1.0, 0.0, 10.0);

    // Periscope
    Component periscope_vis = {COMP_BOX, {80, 480}, 40, 40, 0, "Periscope", {0.2, 0.4, 0.8}};
    draw_component(cr, &periscope_vis);

    // Up from Periscope
    Point top_peri = {80, 300};
    draw_beam(cr, (Point){80, 480}, top_peri, 0.0, 1.0, 0.0, 10.0);

    // Right from Top Periscope
    Point vamp_split = {450, 300};
    draw_beam(cr, top_peri, vamp_split, 0.0, 1.0, 0.0, 10.0);

    // Pupil Viewing Camera (Branch off top periscope path)
    Component pupil_cam = {COMP_BOX, {180, 250}, 40, 30, 0, "Pupil Cam", {0.2, 0.2, 0.2}};
    draw_component(cr, &pupil_cam);
    draw_beam(cr, (Point){180, 300}, pupil_cam.pos, 0.0, 1.0, 0.0, 4.0); // Pickoff

    // VAMPIRES / FIRST Splitter
    Component v_split = {COMP_SPLITTER, vamp_split, 20, 5, 45, "VAMP/FIRST", {0.7, 0.7, 0.7}};
    draw_component(cr, &v_split);

    // FIRST Injection (Up)
    draw_arrow(cr, vamp_split, (Point){450, 220}, 1.0, 0.6, 0.0);
    draw_rounded_rect_border(cr, 400, 210, 100, 50, 1.0, 0.5, 0.0, "FIRST INJ");

    // VAMPIRES Path (Right)
    Point bsplit_pos = {800, 300};
    draw_beam(cr, vamp_split, bsplit_pos, 0.0, 1.0, 0.0, 10.0);

    // HWP / QWP on path
    Component hwp = {COMP_FILTER, {600, 300}, 5, 20, 90, "HWP", {0.8, 0.0, 0.0}};
    draw_component(cr, &hwp);

    // VAMPIRES Splitter (PBS?)
    Component pbs = {COMP_SPLITTER, bsplit_pos, 20, 20, 0, "PBS", {0.5, 0.5, 1.0}};
    draw_component(cr, &pbs);

    // Camera 1 (Top Branch)
    Point cam1_pos = {1000, 250};
    draw_beam(cr, bsplit_pos, cam1_pos, 0.0, 1.0, 0.0, 8.0);
    Component vcam1 = {COMP_BOX, cam1_pos, 40, 40, 0, "Cam 1", {0.1, 0.1, 0.1}};
    draw_component(cr, &vcam1);

    // Camera 2 (Bottom Branch)
    Point cam2_pos = {1000, 350};
    draw_beam(cr, bsplit_pos, cam2_pos, 0.0, 1.0, 0.0, 8.0);
    Component vcam2 = {COMP_BOX, cam2_pos, 40, 40, 0, "Cam 2", {0.1, 0.1, 0.1}};
    draw_component(cr, &vcam2);

    // MBI (Mirror)
    Component mbi = {COMP_MIRROR, {900, 275}, 20, 5, 45, "MBI", {0.6, 0.6, 1.0}};
    draw_component(cr, &mbi);
}

static void draw_calibration_source(cairo_t *cr) {
    draw_rounded_rect_border(cr, 10, 10, 480, 180, 0.6, 0.0, 0.0, "CALIBRATION SOURCE");

    // Sources
    Component s1 = {COMP_SOURCE, {60, 40}, 100, 25, 0, "SuperK 2W", {1.0, 1.0, 1.0}};
    draw_component(cr, &s1);
    Component s2 = {COMP_SOURCE, {60, 80}, 100, 25, 0, "SuperK 0.2W", {1.0, 1.0, 1.0}};
    draw_component(cr, &s2);
    Component s3 = {COMP_SOURCE, {60, 120}, 100, 25, 0, "Red Laser", {1.0, 0.8, 0.8}};
    draw_component(cr, &s3);

    // Combiner
    Point comb = {300, 80};
    draw_beam(cr, (Point){110, 40}, comb, 1.0, 0.9, 0.0, 2.0);
    draw_beam(cr, (Point){110, 80}, comb, 1.0, 0.9, 0.0, 2.0);
    draw_beam(cr, (Point){110, 120}, comb, 1.0, 0.9, 0.0, 2.0);

    // Filter Wheel
    Component fw = {COMP_FILTER, {200, 80}, 5, 30, 90, "FW", {0.5, 0.5, 0.5}};
    draw_component(cr, &fw);

    // Output
    draw_arrow(cr, comb, (Point){450, 60}, 1.0, 0.6, 0.0);
    draw_text(cr, 350, 50, "To Flat", 10, 0);
    draw_arrow(cr, comb, (Point){450, 100}, 1.0, 0.6, 0.0);
    draw_text(cr, 350, 120, "To Focus", 10, 0);
}

static void draw_first_recombination_bench(cairo_t *cr) {
    draw_rounded_rect_border(cr, 500, 10, 690, 180, 0.6, 0.0, 0.0, "FIRST RECOMBINATION BENCH");

    // PIC
    draw_text(cr, 520, 40, "PIC", 12, 1);
    draw_beam(cr, (Point){550, 40}, (Point){1100, 40}, 1.0, 0.8, 0.0, 4.0);
    Component dl = {COMP_BOX, {650, 40}, 60, 30, 0, "Delay", {0.8, 0.8, 0.8}};
    draw_component(cr, &dl);
    Component chip = {COMP_BOX, {800, 40}, 50, 20, 0, "Chip", {0.9, 0.9, 0.9}};
    draw_component(cr, &chip);
    Component cam1 = {COMP_BOX, {1150, 40}, 30, 30, 0, "Cam", {0.1, 0.1, 0.1}};
    draw_component(cr, &cam1);

    // PL
    draw_text(cr, 520, 90, "PL", 12, 1);
    draw_beam(cr, (Point){550, 90}, (Point){1100, 90}, 1.0, 0.8, 0.0, 4.0);

    // FIZ
    draw_text(cr, 520, 140, "FIZ", 12, 1);
    draw_beam(cr, (Point){550, 140}, (Point){1100, 140}, 1.0, 0.8, 0.0, 4.0);
    Component mla = {COMP_LENS, {700, 140}, 10, 20, 0, "MLA", {0.6, 0.8, 1.0}};
    draw_component(cr, &mla);
    Component cam2 = {COMP_BOX, {1150, 140}, 30, 30, 0, "Cam", {0.1, 0.1, 0.1}};
    draw_component(cr, &cam2);
}

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    // Clear background
    cairo_set_source_rgb(cr, 0.05, 0.05, 0.05); // Very dark grey
    cairo_paint(cr);

    // Title
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    draw_text(cr, 20, 570, "SCExAO Beam Train", 20, 1); // Placed near middle left

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
    gtk_window_set_default_size(GTK_WINDOW(window), 1250, 900);

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
