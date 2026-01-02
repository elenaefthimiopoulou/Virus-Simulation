#pragma once
#include <sgg/graphics.h>
#include <string>

using namespace std;

// --- 1. Base Class ---
class Widget {
protected:
    float x, y, w, h;
    string title;

public:
    Widget(float x, float y, float w, float h, string title)
        : x(x), y(y), w(w), h(h), title(title) {
    }

    virtual ~Widget() {}
    virtual void draw() = 0;
    virtual void update(graphics::MouseState& ms) = 0;

    bool contains(float mx, float my) {
        return mx >= x - w / 2 && mx <= x + w / 2 && my >= y - h / 2 && my <= y + h / 2;
    }
};

// --- 2. Button Class ---
class Button : public Widget {
    bool* target_var;
public:
    Button(float x, float y, float w, float h, string title, bool* target)
        : Widget(x, y, w, h, title), target_var(target) {
    }

    void draw() override {
        graphics::Brush br;

        // Background Color (Red/Green)
        if (*target_var) {
            br.fill_color[0] = 1.0f; br.fill_color[1] = 0.5f; br.fill_color[2] = 0.5f;
        }
        else {
            br.fill_color[0] = 0.5f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.5f;
        }
        br.outline_opacity = 1.0f;
        graphics::drawRect(x, y, w, h, br);

        // --- TEXT DRAWING ---
        graphics::Brush text_br;
        // FORCE WHITE COLOR
        text_br.fill_color[0] = 1.0f;
        text_br.fill_color[1] = 1.0f;
        text_br.fill_color[2] = 1.0f;
        text_br.fill_opacity = 1.0f;

        string label = *target_var ? "RESUME" : "PAUSE";
        graphics::drawText(x - 30, y + 5, 20, label, text_br);
    }

    void update(graphics::MouseState& ms) override {
        float mx = graphics::windowToCanvasX((float)ms.cur_pos_x);
        float my = graphics::windowToCanvasY((float)ms.cur_pos_y);

        if (contains(mx, my) && ms.button_left_pressed) {
            *target_var = !(*target_var);
        }
    }
};

// --- 3. Slider Class ---
class Slider : public Widget {
    float* target_var;
    float min_val, max_val;

public:
    Slider(float x, float y, float w, float h, string title, float* target, float minv, float maxv)
        : Widget(x, y, w, h, title), target_var(target), min_val(minv), max_val(maxv) {
    }

    void draw() override {
        graphics::Brush br;

        // --- 1. Draw Title (WHITE) ---
        br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f;
        graphics::drawText(x - w / 2, y - 20, 15, title, br);

        // --- 2. Draw Value Number (WHITE) ---
        string val_str = to_string(*target_var).substr(0, 4);
        graphics::drawText(x + w / 2 + 10, y + 5, 15, val_str, br);

        // --- 3. Draw Bar (Gray) ---
        br.fill_color[0] = 0.5f; br.fill_color[1] = 0.5f; br.fill_color[2] = 0.5f;
        graphics::drawRect(x, y, w, 5, br);

        // --- 4. Draw Handle (White) ---
        float percentage = (*target_var - min_val) / (max_val - min_val);
        float handle_x = (x - w / 2) + percentage * w;

        br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 1.0f;
        graphics::drawDisk(handle_x, y, 10, br);
    }

    void update(graphics::MouseState& ms) override {
        float mx = graphics::windowToCanvasX((float)ms.cur_pos_x);
        float my = graphics::windowToCanvasY((float)ms.cur_pos_y);

        if (ms.button_left_down && mx >= x - w / 2 && mx <= x + w / 2 && my >= y - 20 && my <= y + 20) {
            float percentage = (mx - (x - w / 2)) / w;
            if (percentage < 0.0f) percentage = 0.0f;
            if (percentage > 1.0f) percentage = 1.0f;
            *target_var = min_val + percentage * (max_val - min_val);
        }
    }
};
