// MusicBox NFC — cubic prototype v0.1
// Parametric exterior/interface mock-up. Not a final-fit enclosure.
// Confirm every component dimension and battery power design before printing.
$fn = 48;

part = "assembly"; // [assembly, lower, lid]

box_w = 160;
box_d = 148;
overall_h = 125;
lid_th = 4;
wall = 3.5;
floor_th = 5;
corner_r = 20;

button_d = 27;       // panel cutout; verify against selected switch body
button_z = 88;
button_x = [-55, -18, 18]; // play/pause, previous, next
encoder_x = 55;
encoder_hole_d = 11; // shaft opening; verify chosen encoder
encoder_z = 88;
speaker_r = 31;      // perforation field
speaker_z = 39;
speaker_hole_d = 3.2;
speaker_pitch = 8;

nfc_outer_d = 88;    // alignment ring around broad NFC landing area
nfc_inner_d = 82;
nfc_ring_h = 1.6;

boss_d = 12;
boss_hole_d = 2.7;   // pilot for M3 screw; tune after hardware selection
boss_x = [-66, 66];
boss_y = [-59, 59];

body_h = overall_h - lid_th;
inside_r = corner_r - wall;

module rounded_box(x, y, z, r) {
    translate([r, r, r])
        minkowski() {
            cube([x - 2*r, y - 2*r, z - 2*r]);
            sphere(r=r);
        }
}

module lower_shell() {
    difference() {
        union() {
            // Hollow lower shell; keep a continuous floor and a perimeter ledge.
            difference() {
                rounded_box(box_w, box_d, body_h, corner_r);
                translate([wall, wall, floor_th])
                    rounded_box(
                        box_w - 2*wall,
                        box_d - 2*wall,
                        body_h - floor_th + 1,
                        inside_r
                    );
            }

            // Internal screw posts stay inside the cavity.
            for (x = boss_x)
                for (y = boss_y)
                    translate([box_w/2 + x, box_d/2 + y, floor_th])
                        cylinder(d=boss_d, h=body_h - floor_th - 3);
        }

        // Three large physical button openings on the front.
        for (x = button_x)
            translate([box_w/2 + x, -1, button_z])
                rotate([-90, 0, 0])
                    cylinder(d=button_d, h=wall + 2);

        // Rotary encoder shaft opening. Knob/bezel is external and must be captive.
        translate([box_w/2 + encoder_x, -1, encoder_z])
            rotate([-90, 0, 0])
                cylinder(d=encoder_hole_d, h=wall + 2);

        // Speaker perforations in a circular field.
        for (x = [-24:speaker_pitch:24])
            for (dz = [-24:speaker_pitch:24])
                if (x*x + dz*dz <= speaker_r*speaker_r)
                    translate([box_w/2 + x, -1, speaker_z + dz])
                        rotate([-90, 0, 0])
                            cylinder(d=speaker_hole_d, h=wall + 2);

        // M3 pilot holes through the internal posts.
        for (x = boss_x)
            for (y = boss_y)
                translate([box_w/2 + x, box_d/2 + y, floor_th - 1])
                    cylinder(d=boss_hole_d, h=body_h + 2);
    }
}

module top_lid() {
    difference() {
        union() {
            rounded_box(box_w, box_d, lid_th, corner_r);
            // Raised NFC alignment ring; leave the center flat and non-metallic.
            translate([box_w/2, box_d/2, lid_th])
                difference() {
                    cylinder(d=nfc_outer_d, h=nfc_ring_h);
                    translate([0, 0, -0.1])
                        cylinder(d=nfc_inner_d, h=nfc_ring_h + 0.2);
                }
        }

        // Clearance holes aligned with the four internal posts.
        for (x = boss_x)
            for (y = boss_y)
                translate([box_w/2 + x, box_d/2 + y, -0.1])
                    cylinder(d=3.3, h=lid_th + 0.2);
    }
}

module assembled() {
    lower_shell();
    translate([0, 0, body_h])
        top_lid();
}

if (part == "lower") {
    lower_shell();
} else if (part == "lid") {
    top_lid();
} else {
    assembled();
}
