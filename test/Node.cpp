#include <iostream>
#include <sgg\graphics.h>
#include "Node.h"
#include <cstdlib> // Needed for rand() and RAND_MAX


void Node::draw() {//sick(red)
		graphics::Brush br;
		if (status == INFECTED) {
			br.fill_color[0] = 1.0f;
			br.fill_color[1] = 0.0f;
			br.fill_color[2] = 0.0f;
			
		}
		else if (status == HEALTHY) {//healthy(green)
			// Set fill color to Green (RGB: 0, 1, 0)
			br.fill_color[0] = 0.0f; // Red off
			br.fill_color[1] = 1.0f; // Green on
			br.fill_color[2] = 0.0f; // Blue off

		}
		else if (status == RECOVERED) { //recovered(yellow)
			br.fill_color[0] = 1.0f; // Red: Full
			br.fill_color[1] = 1.0f; // Green: Full
			br.fill_color[2] = 0.0f; // Blue: Off

			br.outline_color[0] = 1.0f;
			br.outline_color[1] = 1.0f;
			br.outline_color[2] = 1.0f;
		}
		// --- Common Visuals ---
		br.outline_color[0] = 1.0f;
		br.outline_color[1] = 1.0f;
		br.outline_color[2] = 1.0f;
		br.outline_width = 2.0f;
		br.fill_opacity = 0.5f;

		// Use member variable 'highlighted' 
		br.outline_opacity = highlighted ? 1.0f : 0.0f;


		float p[4][2];
		p[0][0] = 0.0f; p[0][1] = 0.0f;
		p[3][0] = x; p[3][1] = y;
		p[1][0] = 15; p[1][1] = 0.0f;
		p[2][0] = x; p[2][1] = y - 15;

		graphics::Brush bs;

		float theta = sinf(graphics::getGlobalTime() / 100.f);

		graphics::setOrientation(15.f * theta);
		graphics::setScale(theta * 0.2f + 1.0f, theta * 0.2f + 1.0f);

		graphics::drawDisk(x + 1, y + 2, radius + 1, br);
		br.outline_opacity = highlighted ? 1.0f : 0.5f;
		br.texture = " ";
		br.fill_opacity = 1.0f;

		graphics::resetPose();
}

void Node::update(float dt) {

	x += vx * dt;
	y += vy * dt;

	// Bounce off screen edges (assuming 800x600 window)
	if (x < 0 || x > 800) vx = -vx;
	if (y < 0 || y > 600) vy = -vy;


}
	