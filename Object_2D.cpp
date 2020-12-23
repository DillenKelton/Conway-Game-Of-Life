#include "Object_2D.h"

Object_2D::Object_2D(float width, float height, float pos_x, float pos_y, rgba_color color, texture_data textureData, void (*clickCallback)(Object_2D*, int, int))
{
    this->width = width;
    this->height = height;
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->color = color;
    this->texData = textureData;
    this->clickCallback = clickCallback;
}

void Object_2D::setWidth(float width)
{
    this->width = width;
}

void Object_2D::setHeight(float height)
{
    this->height = height;
}

void Object_2D::setPosX(float pos_x)
{
    this->pos_x = pos_x;
}

void Object_2D::setPosY(float pos_y)
{
    this->pos_y = pos_y;
}

void Object_2D::setColor(rgba_color color)
{
    this->color = color;
}

void Object_2D::setTextureData(texture_data textureData)
{
    this->texData = textureData;
}

void Object_2D::createClickCallback(void (*callback)(Object_2D*, int, int))
{
    clickCallback = callback;
}

float Object_2D::getWidth()
{
    return width;
}

float Object_2D::getHeight()
{
    return height;
}

float Object_2D::getPosX()
{
    return pos_x;
}

float Object_2D::getPosY()
{
    return pos_y;
}

rgba_color Object_2D::getColor()
{
    return color;
}

texture_data Object_2D::getTextureData()
{
    return texData;
}

void Object_2D::click(Object_2D* obj, int button, int state)
{
    if(clickCallback != nullptr)
    {
        clickCallback(obj, button, state);
    }
}