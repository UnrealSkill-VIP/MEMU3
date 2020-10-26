#include "../headers/Manager.h"
#include "../headers/Utils.h"
#include "../headers/Overlay.h"

#include <condition_variable>
#include <mutex>

std::mutex exit_mutex;
std::mutex pause_mutex;
std::condition_variable exit_condition;
std::condition_variable pause_condition;

Manager::~Manager() {
}

bool Manager::is_running() const {
    return running;
}


Manager::Manager(const int width, const int height, const int offsetLeft, const int offsetTop, const Coords &pFarHeadOffset, const Coords &pCloseHeadOffset,
                 const float &pSensitivity, const float &pStrength) : running(false), exitRequested(false), screenshot(ScreenshotData(width, height)), farHeadOffset(pFarHeadOffset),
                                           closeHeadOffset(pCloseHeadOffset), sensitivity(pSensitivity), strength(pStrength) {
    RECT desktop;
    const auto hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    screenSize.x = desktop.right;
    screenSize.y = desktop.bottom;
    int left = screenSize.x / 2 + offsetLeft;
    int top = screenSize.y / 2 + offsetTop;
    region = Rect(width, height, left, top);
    median.x = region.left - screenSize.x / 2;
    median.y = region.top + region.height - screenSize.y / 2;
    enemyCoords = Coords();
}

bool Manager::is_exit_requested() const {
    return exitRequested;
}


void Manager::update_enemy_coords_with_local_coords(int x, int y) {
    auto dv = (clamp(((float) lastKnownBarSize.x / 2.0f + (float) lastKnownBarSize.y), 5.0f, 15.0f) - 5.0f) / 10.0f;
    enemyCoords.set((int) lerp(dv, (float) closeHeadOffset.x, (float) farHeadOffset.x) + median.x + x,
                    (int) lerp(dv, (float) closeHeadOffset.y, (float) farHeadOffset.y) + median.y - y);
}

bool Manager::is_crosshair_over_enemy() const {
    return enemyCoords.length <= 15.0f;
}

void Manager::update_enemy_coords_with_local_coords(Coords coords) {
    update_enemy_coords_with_local_coords(coords.x, coords.y);
}

void Manager::increase_sensitivity() {
    sensitivity = min(sensitivity + 0.1f, 25.0f);
    Overlay::toggle_render();
    Overlay::show_hint("Sensitivity: " + to_string(sensitivity));
}

void Manager::decrease_sensitivity() {
    sensitivity = max(sensitivity - 0.1f, 0.1f);
    Overlay::toggle_render();
    Overlay::show_hint("Sensitivity: " + to_string(sensitivity));
}

void Manager::increase_aim_strength() {
    strength = min(strength + 0.5f, 10.0f);
    Overlay::toggle_render();
    Overlay::show_hint("Strength: " + to_string(strength));
}

void Manager::decrease_aim_strength() {
    strength = max(strength - 0.5f, 0.0f);
    Overlay::toggle_render();
    Overlay::show_hint("Strength: " + to_string(strength));
}

void Manager::toggle_mode() {
    mode = (Mode) ((((int) mode) + 1) % sizeof(Mode));
    switch (mode) {
        case hanzo:
            Overlay::show_hint("Mode: Hanzo");
            break;
        case aim:
            Overlay::show_hint("Mode: Aim Assist");
            break;
        case flick:
            Overlay::show_hint("Mode: Flickshots");
            break;
        case trigger:
            Overlay::show_hint("Mode: Triggerbot");
            break;
    }
    Overlay::toggle_render();
}

void Manager::stop_thread_until_exit() const {
    std::unique_lock<std::mutex> lck(exit_mutex);
    while (!is_exit_requested()) exit_condition.wait(lck);
}

void Manager::pause_thread_if_not_running() const {
    std::unique_lock<std::mutex> lck(pause_mutex);
    while (!is_running()) pause_condition.wait(lck);
}

void Manager::request_exit() {
    std::unique_lock<std::mutex> lck(exit_mutex);
    running = false;
    exitRequested = true;
    exit_condition.notify_all();
}

void Manager::set_running(const bool &state) {
    std::unique_lock<std::mutex> lck(pause_mutex);
    running = state;
    pause_condition.notify_all();
    Overlay::show_hint(running?"Running":"Paused");
}