#pragma once

#include "../headers/Manager.h"
#include "../headers/InputController.h"
#include "../headers/Coords.h"
#include <vector>
#include <atomic>

class AimAssistant {
#define COLOR_HASHTABLE_SIZE (0xFFFFFF + 1) / 8
public:
    AimAssistant(class Manager &pManager);
private:
    const int IGNORED_BORDER_SIZE = 5;
    const int SCANNING_THRESHOLD_PERCENT = 80;
    const int CHECK_COEFFICIENT = 100 / (IGNORED_BORDER_SIZE * IGNORED_BORDER_SIZE);
    BYTE hashTable[COLOR_HASHTABLE_SIZE];
    Manager &manager;
    InputController input;
    std::atomic_bool suspendThreads = false;
    std::atomic_int threadCount = 0;
    void main_thread();
    void input_thread();
    void handle_screenshot();
    bool probe_region_spiral(int &offset, const int side = 25) const;
    bool locate_healthbar_handle_left(int &offset) const;
    bool probe_healthbar_brute() const;
    void initialize_color_table(const std::vector<RGBQUAD> &pColors, const bool pUseCacheFile = true);
    bool probe_color(const RGBQUAD &pColor) const;
    std::string hashtable_name(const std::vector<RGBQUAD> &pColors);
    bool dump_table(std::string &tablename) const;
    bool restore_table(std::string &tablename) const;
    Coords offset_to_coords(const int &offset) const;
    int coords_to_offset(const Coords &coords) const;
    int coords_to_offset(const int &pX, const int &pY) const;
    bool probe_all_points_diagonal(const int &, const int = 5) const;
    bool probe_any_point_left(const int &, const int = 5) const;
    bool probe_handle(const int &) const;
    void find_healthbar_height();
    void find_healthbar_width();
    void move_by_smoothed(const Coords &coords);
    void flick_and_shot(const Coords &coords);
    void flick_and_release(const Coords &coords);
    void aim_handler();
    void trigger_handler() const;
    void flick_handler();
    void hanzo_handler();
    void terminate_threads();
    void apply_modifiers_common(Coords &coords) const;
    static void apply_modifiers_hanzo(Coords &coords);
    void apply_modifiers_smooth(Coords &coords) const;
};