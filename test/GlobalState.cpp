#include "GlobalState.h"
#include <cmath> // Needed for sqrt() and distance calculations

// Initialize static member
GlobalState* GlobalState::instance = nullptr;

GlobalState::GlobalState() {
    transmission_rate = 0.5f; // 50% chance per second (adjustable)
    recovery_rate = 0.1f;     // 10% chance per second
    immunity_loss_rate = 0.05f; //5% chance per second to lose immunity
}

GlobalState* GlobalState::getInstance() {
    if (instance == nullptr) {
        instance = new GlobalState();
    }
    return instance;
}

void GlobalState::init() {
    // Initialize 20 nodes with random positions
    for (int i = 0; i < 20; i++) {
        // Random X between 50 and 950 (keeping some margin)
        float rx = (float)(rand() % 900 + 50);
        float ry = (float)(rand() % 700 + 50);

        Node* newNode = new Node(i, rx, ry);

        // Logic for "Patient Zero" (The first node is infected)
        if (i == 0) newNode->setStatus(INFECTED);

        all_nodes.push_back(newNode);
    }
    // 1. Pause Button (Top Left)
    // Pass the address of 'is_paused'
    ui_widgets.push_back(new Button(100, 50, 150, 40, "Pause", &is_paused));

    // 2. Transmission Slider (Top Right)
    // Pass address of 'transmission_rate', Range 0.0 to 1.0
    ui_widgets.push_back(new Slider(800, 50, 200, 20, "Infection Rate", &transmission_rate, 0.0f, 1.0f));

    // 3. Recovery Slider (Below Transmission)
    ui_widgets.push_back(new Slider(800, 100, 200, 20, "Recovery Rate", &recovery_rate, 0.0f, 0.5f));

    ui_widgets.push_back(new Slider(800, 150, 200, 20, "Immunity Loss", &immunity_loss_rate, 0.0f, 0.2f));
}

void GlobalState::update(float dt) {
    
    // 1. UPDATE NODE MOVEMENT
    
    for (auto node : all_nodes) {
        // Only update physics if the node is NOT being dragged by the user
        if (node != selected_node) {
            node->update(dt);
        }
        if (node->getStatus() == INFECTED) {
            // Calculate chance based on your slider (recovery_rate)
            // We multiply by dt so it works smoothly regardless of framerate
            float recovery_chance = recovery_rate * dt;

            // Roll the dice (0.0 to 1.0)
            float roll = (float)rand() / RAND_MAX;

            if (roll < recovery_chance) {
                node->setStatus(RECOVERED);
            }
            else if (node->getStatus() == RECOVERED) {
                float immunity_loss_chance = immunity_loss_rate * dt;

                // Roll the dice
                if ((float)rand() / RAND_MAX < immunity_loss_chance) {
                    node->setStatus(HEALTHY); // Become susceptible again
                    node->infected_by = nullptr; // clear the "who infected me" trace
                }
            }
        }
    }
    
    // 2. USER INTERACTION (INPUT HANDLING)
    
    graphics::MouseState ms;
    graphics::getMouseState(ms); // Get current mouse status

    // Convert mouse screen coordinates to canvas coordinates
    float mx = graphics::windowToCanvasX(ms.cur_pos_x);
    float my = graphics::windowToCanvasY(ms.cur_pos_y);

    // 1. Update UI (Always do this, even if paused, so we can click "Resume")
    for (auto widget : ui_widgets) {
        widget->update(ms);
    }

    // 2. If Paused, STOP here. Do not update nodes.
    if (is_paused) return;

    // --- LEFT CLICK: DRAG & DROP ---
    if (ms.button_left_down) {
        if (selected_node == nullptr) {
            // If nothing is selected, check if we clicked on a node
            for (auto node : all_nodes) {
                if (node->checkCollision(mx, my)) {
                    selected_node = node;
                    break;
                }
            }
        }
        else {
            // If a node is selected, move it to the mouse position
            selected_node->setPosition(mx, my);
        }
    }
    else {
        // Release the node when the mouse button is released
        selected_node = nullptr;
    }

    // --- RIGHT CLICK: INFECT NODE ---
    if (ms.button_right_pressed) {
        for (auto node : all_nodes) {
            if (node->checkCollision(mx, my)) {
                //Only infect if they are currently HEALTHY
                if (node->getStatus() == HEALTHY) {
                    node->setStatus(INFECTED);
                }
            }
        }
    }

    
    // 3. INFECTION SPREAD & NEIGHBOR DISCOVERY
    
    float connection_distance = 150.0f; // Distance to consider nodes as "neighbors"

    for (size_t i = 0; i < all_nodes.size(); i++) {
        for (size_t j = i + 1; j < all_nodes.size(); j++) {
            Node* n1 = all_nodes[i];
            Node* n2 = all_nodes[j];

            // Calculate Euclidean distance
            float dx = n1->getX() - n2->getX();
            float dy = n1->getY() - n2->getY();
            float dist = sqrt(dx * dx + dy * dy);

            if (dist < connection_distance) {
                // Determine if transmission happens
                if (n1->getStatus() == INFECTED && n2->getStatus() == HEALTHY) {
                    // Roll the dice for infection
                    if ((rand() % 100) < (transmission_rate * 100 * dt)) {
                        n2->setStatus(INFECTED);
                        n2->infected_by = n1; // Record who caused the infection
                    }
                }
                else if (n2->getStatus() == INFECTED && n1->getStatus() == HEALTHY) {
                    if ((rand() % 100) < (transmission_rate * 100 * dt)) {
                        n1->setStatus(INFECTED);
                        n1->infected_by = n2; // Record who caused the infection
                    }
                }
            }
        }
    }
   

}

void GlobalState::draw() {
    
    // LAYER 1: NEIGHBOR CONNECTIONS (Background Lines)
    
    graphics::Brush line_brush;
    line_brush.outline_opacity = 0.3f; // Faint lines
    line_brush.outline_width = 1.0f;
    line_brush.outline_color[0] = 1.0f;
    line_brush.outline_color[1] = 1.0f;
    line_brush.outline_color[2] = 1.0f;

    float connection_distance = 150.0f;

    for (size_t i = 0; i < all_nodes.size(); i++) {
        for (size_t j = i + 1; j < all_nodes.size(); j++) {
            Node* n1 = all_nodes[i];
            Node* n2 = all_nodes[j];

            float dx = n1->getX() - n2->getX();
            float dy = n1->getY() - n2->getY();
            float dist = sqrt(dx * dx + dy * dy);

            // Draw line if they are close enough
            if (dist < connection_distance) {
                graphics::drawLine(n1->getX(), n1->getY(), n2->getX(), n2->getY(), line_brush);
            }
        }
    }

    
    // LAYER 2: INFECTION TRACE (Who infected whom)
    
    graphics::Brush trace_brush;
    trace_brush.outline_color[0] = 1.0f; // Red
    trace_brush.outline_color[1] = 0.0f;
    trace_brush.outline_color[2] = 0.0f;
    trace_brush.outline_opacity = 0.8f;
    trace_brush.outline_width = 3.0f; // Thick line

    for (auto node : all_nodes) {
        // If this node was infected by someone, draw a line to them
        if (node->infected_by != nullptr) {
            graphics::drawLine(node->getX(), node->getY(),
                node->infected_by->getX(), node->infected_by->getY(),
                trace_brush);
        }
    }

    
    // LAYER 3: NODES (Foreground)
    for (auto node : all_nodes) {
        node->draw();
    }
    for (auto widget : ui_widgets) {
        widget->draw();
    }
}

GlobalState::~GlobalState() {
    // Cleanup: Delete all dynamically allocated nodes
    for (auto node : all_nodes) {
        delete node;
    }
    all_nodes.clear();
    //Delete UI
    for (auto widget : ui_widgets) delete widget;
    ui_widgets.clear();
}