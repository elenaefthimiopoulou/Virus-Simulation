#pragma once
#include <vector>
#include "Node.h"
#include "UI.h"
#include <sgg/graphics.h>

class GlobalState {
private:
    // 1. Singleton: Static instance
    static GlobalState* instance;

    // 2. Data Structures 
    std::vector<Node*> all_nodes;
    std::vector<Widget*> ui_widgets;
    float transmission_rate;
    float recovery_rate;
    float immunity_loss_rate;

    bool is_paused = false;

    Node* selected_node = nullptr;
    // Private Constructor
    GlobalState();

public:
    // 1. Singleton: Static Accessor
    static GlobalState* getInstance();

    // Delete copy constructor and assignment operator
    GlobalState(GlobalState const&) = delete;
    void operator=(GlobalState const&) = delete;

    // 2. Basic Methods (from requirements)
    void init();                // Create the graph
    void update(float dt);      // Move and infect
    void draw();                // Draw nodes and lines

    // Helper to cleanup memory
    ~GlobalState();
};
