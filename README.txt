# Virus Transmission Simulation 🦠

A C++ interactive application simulating the spread of a virus through a dynamic network of nodes. Developed using the SGG (Simple Graphics Library) for the Computer Programming course (2025-2026).

## 👥 Team Members
* ELENI EFTHYMIOPOULOU (Student ID: 3240052)
---

## 📖 Project Overview
This application visualizes the SIRS model (Susceptible-Infected-Recovered-Susceptible) in real-time. Nodes move freely within the canvas, forming dynamic connections (edges) based on proximity. The virus spreads through these connections based on probabilistic logic controlled by the user.

### Key Features
* Dynamic Graph Network: Nodes automatically connect to neighbors within a specific radius.
* Real-time Infection: Visual feedback for health states:
    * 🟢 Green: Healthy (Susceptible)
    * 🔴 Red: Infected
    * 🟡 Yellow: Recovered (Temporary Immunity)
* Interactive UI:
    * Pause/Resume: Stop time to analyze the network.
    * Sliders: Adjust Infection Rate, Recovery Rate, and Immunity Loss Rate in real-time.
* User Interaction:
    * Drag & Drop: Move nodes to break or create clusters.
    * Manual Infection: Right-click a node to make it "Patient Zero".

---

## 🎮 Controls
 Left Mouse Click (Hold) -> Drag and move nodes around the screen.
 Right Mouse Click -> Infect a specific Healthy node immediately.
 UI Buttons/Sliders -> Click and drag to adjust simulation parameters.

---

## 🏗️ Technical Implementation
This project adheres to the strict architectural requirements of the assignment:

### 1. Architecture & Design Patterns
* Singleton Pattern: The `GlobalState` class manages the game loop, data, and global settings, ensuring a single point of access.
* Game Loop: Implemented via `init()`, `update(dt)`, and `draw()` callbacks.

### 2. Object-Oriented Programming (OOP)
* Inheritance & Polymorphism:
    * `Widget` (Base Class) → `Button`, `Slider` (Derived Classes).
    * `Node` class prepared for future extension.
    * Virtual functions (`update`, `draw`) used for polymorphic behavior.
* Encapsulation: Private member variables with public Getters/Setters.

### 3. Memory Management & STL
* Dynamic Allocation: All Nodes and UI Widgets are allocated on the heap using `new` and properly cleaned up in the destructor using `delete` to prevent memory leaks.
* STL Containers: Used `std::vector` and `std::list` to manage collections of nodes and neighbors efficiently.

---

## 📂 Project Structure
```text
/bin
  /assets         <-- Contains font.ttf (REQUIRED)
  virus_sim.exe
/test
  main.cpp        <-- Entry point, SGG callbacks
  GlobalState.h   <-- Singleton Manager
  GlobalState.cpp
  Node.h          <-- Agent logic
  Node.cpp
  UI.h            <-- Widget, Button, Slider classes
README.md

