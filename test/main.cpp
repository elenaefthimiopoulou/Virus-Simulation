#include <iostream>
#include <sgg\graphics.h>
#include "Node.h"
#include "GlobalState.h"
#include <ctime> // For time() to seed random numbers

// 1. The Draw Callback
void draw() {

    graphics::setFont("assets\\font.ttf");
    // A. Clear the background (Draw a big dark gray rectangle)
    graphics::Brush br;
    br.fill_color[0] = 0.1f;
    br.fill_color[1] = 0.1f;
    br.fill_color[2] = 0.1f;
    br.outline_opacity = 0.0f; // No outline

    // Assuming canvas size is 1000x800. 
    // Center is (500, 400), width 1000, height 800
    graphics::drawRect(500, 400, 1000, 800, br);

    // B. Tell the GlobalState to draw all nodes and lines
    GlobalState::getInstance()->draw();
}

// 2. The Update Callback
void update(float ms) {
    // Convert milliseconds to seconds (dt) for smoother physics/math
    float dt = ms / 1000.0f;

    // Delegate logic to the Singleton
    GlobalState::getInstance()->update(dt);
}

// 3. Main Function
int main(int argc, char** argv) {
    // Initialize Random Seed (Crucial for random movement and start positions)
    srand((unsigned int)time(nullptr));

    // A. Create the Window
    // Width: 1000, Height: 800, Title: "Virus Simulation"
    graphics::createWindow(1000, 800, "Virus Simulation");

    // B. Initialize your Logic (Create nodes, etc.)
    GlobalState::getInstance()->init();

    // C. Register SGG Callbacks
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // D. specific Canvas Setup
    // Defines the "Game World" coordinates.
    // (0,0) is top-left, (1000,800) is bottom-right.
    graphics::setCanvasSize(1000, 800);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    // E. Start the Loop (This blocks until the window is closed)
    graphics::startMessageLoop();

    return 0;
}