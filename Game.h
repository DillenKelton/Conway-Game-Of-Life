#include <vector>
#include <unordered_map>
#include <iostream>
#include "Object_2D.h"
#include "Presets.h"

class Game
{
    public:

        static void start(int* argc, char** argv);

    private:

        enum Cell_State
        {
            Empty, Alive, Dead
        };

        const static int WINDOW_WIDTH = 1600;
        const static int WINDOW_HEIGHT = 900;

        const static int MAX_GRID_WIDTH = 320;
        const static int MAX_GRID_HEIGHT = 120;

        const static int CELL_WIDTH = 5;
        const static int CELL_HEIGHT = 5;

        const static int SIMULATION_DELAY = 500;

        const static int BITMAP_FONT_WIDTH = 8;
        const static int BITMAP_FONT_HEIGHT = 8;
        
        const static int BITMAP_FONT_COLUMNS = 8;
        const static int BITMAP_FONT_ROWS = 9;

        const static char BITMAP_FONT_ARRAY[BITMAP_FONT_ROWS][BITMAP_FONT_COLUMNS];

        static int timerDelay;
        static int stageCounter;

        static std::vector<Object_2D*> objectList;
        static int gridArray[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];

        static rgba_color emptyColor;
        static rgba_color aliveColor;
        static rgba_color deadColor;

        static bool paused;

        static std::unordered_map<char, texture_data> bitmapFontMap;

        static const presetGrid presets[];

        static int selectedPreset;

        static void renderObject(Object_2D* obj);

        static void display();

        static void resize(int w, int h);

        static void mouse(int button, int state, int x, int y);

        static void cellClick(Object_2D* obj, int button, int state);

        static void pauseClick(Object_2D* obj, int button, int state);

        static void playClick(Object_2D* obj, int button, int state);

        static void halfSpeedClick(Object_2D* obj, int button, int state);

        static void doubleSpeedClick(Object_2D* obj, int button, int state);

        static void nextClick(Object_2D* obj, int button, int state);

        static void resetClick(Object_2D* obj, int button, int state);

        static void emptyColorClick(Object_2D* obj, int button, int state);

        static void aliveColorClick(Object_2D* obj, int button, int state);

        static void deadColorClick(Object_2D* obj, int button, int state);

        static void previousPresetClick(Object_2D* obj, int button, int state);

        static void nextPresetClick(Object_2D* obj, int button, int state);

        static void timer(int val);

        static void simulateGeneration();

        static void updateCells();

        static void loadPreset();

        static GLuint loadTexture(const char* imagePath);

        static bool intersects(int x1, int y1, int width, int height, int x2, int y2);

        static void initializeBitmapFontMap();

        static void drawText(std::string text, float startx, float starty, float width, float height);

};