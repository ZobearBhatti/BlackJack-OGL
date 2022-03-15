#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

void initTextRend();

void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

#endif