#include "freeglut\include\GL\glut.h"
#include "freeglut\include\GL\freeglut.h"
#include "Colors.h"
#include "Texture.h"

class Object_2D
{
    public:

        Object_2D(float width, float height, float pos_x, float pos_y, rgba_color color, texture_data texData, void (*clickCallback)(Object_2D*, int, int) = nullptr);

        void setWidth(float width);

        void setHeight(float height);

        void setPosX(float pos_x);

        void setPosY(float pos_y);

        void setColor(rgba_color color);

        void setTextureData(texture_data textureData);

        void createClickCallback(void (*callback)(Object_2D*, int, int));

        float getWidth();

        float getHeight();

        float getPosX();

        float getPosY();

        rgba_color getColor();

        texture_data getTextureData();

        void click(Object_2D* obj, int button, int state);

    private:

        float width;
        float height;
        float pos_x;
        float pos_y;
        rgba_color color;
        texture_data texData;
        void (*clickCallback)(Object_2D*, int, int); //parameters: pointer to the clicked object, button (left/right/middle), state (up/down)
};