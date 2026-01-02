#pragma once
#include <list>
#include <iostream> 
#include <cmath>

using namespace std;

// Definition of the State enum (Health status)
enum State {
    HEALTHY,
    INFECTED,
    RECOVERED
};

class Node {
protected:
    int id;
    float x, y;
    State status;
    list<Node*> neighbors; // Adjacency list
    float vx, vy; // Velocity X and Y

    // Radius is needed for drawing the circle and checking collision
    float radius;

    bool highlighted = false;

public:
    // Constructor
    Node(int id, float x, float y) : id(id), x(x), y(y) {
        status = HEALTHY; // Default state
        radius = 25.0f;    // Default size (adjust based on your SGG scale)
        vx = (rand() % 100 - 50) / 10.0f * 2.0f;
        vy = (rand() % 100 - 50) / 10.0f * 2.0f;
    }
    Node* infected_by = nullptr;
    void setPosition(float new_x, float new_y) { x = new_x; y = new_y; }

    // Virtual Destructor (Essential for a base class)
    virtual ~Node() {}

    // Polymorphic Methods
    virtual void draw();            // Implementation is in Node.cpp
    virtual void update(float dt);  // Implementation is in Node.cpp

    // Interaction Method
    bool checkCollision(float px, float py) {
        float dist = (float)sqrt(pow(x - px, 2) + pow(y - py, 2));

        if (dist <= radius) {
            highlighted = true; // Enable hover effect
            return true;
        }
        else {
            highlighted = false; // Disable hover effect
            return false;
        }
    }

    // Helper to add neighbors
    void addNeighbor(Node* node) {
        neighbors.push_back(node);
    }

    // Getters and Setters
    void setStatus(State s) { status = s; }
    State getStatus() const { return status; }
    float getX() const { return x; }
    float getY() const { return y; }
};


