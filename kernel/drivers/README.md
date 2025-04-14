# drivers

Contains drivers functions for use by the kernel (Machine or supervisor mode). Initially these will just be wrappers for ESP32-C3 functions but will eventually be adapted for other hardware.

Structure will evolve to something like:

└── drivers
    ├── esp32c3
    ├── milkvduo #(or similar)
    └── common
