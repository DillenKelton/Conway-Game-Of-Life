#include <windows.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include "freeglut\include\GL\glut.h"
#include "freeglut\include\GL\freeglut.h"
#include "soil\SOIL.h"
#include "Game.h"

std::vector<Object_2D*> Game::objectList;
int Game::timerDelay = SIMULATION_DELAY;
int Game::gridArray[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {0};

int Game::stageCounter = 0;

rgba_color Game::emptyColor = BLACK;
rgba_color Game::aliveColor = GREEN;
rgba_color Game::deadColor = RED;

bool Game::paused = true;

std::unordered_map<char, texture_data> Game::bitmapFontMap;

const char Game::BITMAP_FONT_ARRAY[BITMAP_FONT_ROWS][BITMAP_FONT_COLUMNS] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', ' ', ':',
    '\''
};

const presetGrid Game::presets[] = { EMPTY_GRID, BOBCAT_GRID, CLOCK_GRID, HAPPYFACE_GRID, SHAPES_GRID, TXST_GRID };

int Game::selectedPreset = 0;

void Game::start(int* argc, char** argv)
{
    //basic initialization
    glutInit(argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Team Lrr's Game of Life");

    glEnable(GL_TEXTURE_2D); //enable 2D textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //setup the grid cells
    for(int row = 0; row < MAX_GRID_HEIGHT; row++)
    {
        for(int column = 0; column < MAX_GRID_WIDTH; column++)
        {
            objectList.push_back(new Object_2D(CELL_WIDTH, CELL_HEIGHT, 0 + (CELL_WIDTH * column), 0 + (CELL_HEIGHT * row), emptyColor, {0}, cellClick));
        }
    }

    //setup the UI objects
    objectList.push_back(new Object_2D(1600, 300, 0, 600, WHITE, { loadTexture("img\\menubar.jpg"), 1, 1, 0, 0 }));
    objectList.push_back(new Object_2D(50, 50, 540, 750, WHITE, { loadTexture("img\\pausebutton.jpg"), 1, 1, 0, 0 }, pauseClick));
    objectList.push_back(new Object_2D(50, 50, 640, 750, WHITE, { loadTexture("img\\rewindbutton.jpg"), 1, 1, 0, 0 }, halfSpeedClick));
    objectList.push_back(new Object_2D(50, 50, 740, 750, WHITE, { loadTexture("img\\playbutton.jpg"), 1, 1, 0, 0 }, playClick));
    objectList.push_back(new Object_2D(50, 50, 840, 750, WHITE, { loadTexture("img\\fastforwardbutton.jpg"), 1, 1, 0, 0 }, doubleSpeedClick));
    objectList.push_back(new Object_2D(50, 50, 940, 750, WHITE, { loadTexture("img\\nextbutton.jpg"), 1, 1, 0, 0 }, nextClick));
    objectList.push_back(new Object_2D(50, 50, 1040, 750, WHITE, { loadTexture("img\\resetbutton.jpg"), 1, 1, 0, 0 }, resetClick));
    objectList.push_back(new Object_2D(50, 50, 1450, 650, WHITE, { loadTexture("img\\nextbutton.jpg"), 1, 1, 0, 0 }, nextPresetClick));
    objectList.push_back(new Object_2D(50, 50, 1250, 650, WHITE, { loadTexture("img\\prevbutton.jpg"), 1, 1, 0, 0 }, previousPresetClick));
    objectList.push_back(new Object_2D(160, 250, 200, 650, WHITE, { loadTexture("img\\lrr.png"), 1, 1, 0, 0 }));

    //empty color picks
    objectList.push_back(new Object_2D(30, 30, 20, 635, WHITE, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 50, 635, GREEN, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 80, 635, BLUE, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 110, 635, PURPLE, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 20, 665, BLACK, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 50, 665, RED, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 80, 665, ORANGE, {0}, emptyColorClick));
    objectList.push_back(new Object_2D(30, 30, 110, 665, YELLOW, {0}, emptyColorClick));

    //alive color picks
    objectList.push_back(new Object_2D(30, 30, 20, 735, WHITE, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 50, 735, GREEN, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 80, 735, BLUE, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 110, 735, PURPLE, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 20, 765, BLACK, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 50, 765, RED, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 80, 765, ORANGE, {0}, aliveColorClick));
    objectList.push_back(new Object_2D(30, 30, 110, 765, YELLOW, {0}, aliveColorClick));

    //dead color picks
    objectList.push_back(new Object_2D(30, 30, 20, 835, WHITE, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 50, 835, GREEN, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 80, 835, BLUE, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 110, 835, PURPLE, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 20, 865, BLACK, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 50, 865, RED, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 80, 865, ORANGE, {0}, deadColorClick));
    objectList.push_back(new Object_2D(30, 30, 110, 865, YELLOW, {0}, deadColorClick));

    //setup bitmap font for text drawing
    initializeBitmapFontMap();

    glutDisplayFunc(display); //display callback
    glutReshapeFunc(resize); //resize callback
    glutMouseFunc(mouse); //mouse callback
    glutTimerFunc(timerDelay, timer, 0); //timer callback

    glutMainLoop(); //main loop
}

void Game::renderObject(Object_2D* obj)
{
    glBindTexture(GL_TEXTURE_2D, obj->getTextureData().textureID); //texture must be bound before we start drawing vertices

    glBegin(GL_TRIANGLES);

    rgba_color color = obj->getColor();
    glColor4f(color.red, color.green, color.blue, color.alpha);

    //triangle 1
    glTexCoord2f(obj->getTextureData().texCoordX, obj->getTextureData().texCoordY);
    glVertex2f(obj->getPosX(), obj->getPosY());
    glTexCoord2f(obj->getTextureData().texCoordX + obj->getTextureData().textureWidth, obj->getTextureData().texCoordY + obj->getTextureData().textureHeight);
    glVertex2f(obj->getPosX() + obj->getWidth(), obj->getPosY() + obj->getHeight());
    glTexCoord2f(obj->getTextureData().texCoordX, obj->getTextureData().texCoordY + obj->getTextureData().textureHeight);
    glVertex2f(obj->getPosX(), obj->getPosY() + obj->getHeight());

    //triangle 2
    glTexCoord2f(obj->getTextureData().texCoordX, obj->getTextureData().texCoordY);
    glVertex2f(obj->getPosX(), obj->getPosY());
    glTexCoord2f(obj->getTextureData().texCoordX + obj->getTextureData().textureWidth, obj->getTextureData().texCoordY);
    glVertex2f(obj->getPosX() + obj->getWidth(), obj->getPosY());
    glTexCoord2f(obj->getTextureData().texCoordX + obj->getTextureData().textureWidth, obj->getTextureData().texCoordY + obj->getTextureData().textureHeight);
    glVertex2f(obj->getPosX() + obj->getWidth(), obj->getPosY() + obj->getHeight());

    glEnd();
}

void Game::display()
{
    //clear color buffer (background) to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //render

    for(Object_2D* obj : objectList)
    {
        renderObject(obj);
    }

    drawText("Team Lrr's Game of Life", 250, 615, 30, 30);
    drawText("Stage Counter: " + std::to_string(stageCounter), 1250, 850, 16, 16);
    drawText(presets[selectedPreset].name, 1315, 670, 16, 16);
    drawText("Empty Cell Color:", 5, 615, 10, 10);
    drawText("Live Cell Color:", 5, 715, 10, 10);
    drawText("Dead Cell Color:", 5, 815, 10, 10);

    glFlush();
}

void Game::resize(int w, int h)
{
    w = WINDOW_WIDTH;
    h = WINDOW_HEIGHT;
    glutReshapeWindow(w, h); //forces window to stay current size

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
}

void Game::mouse(int button, int state, int x, int y)
{
    for(Object_2D* obj : objectList)
    {
        if(intersects(obj->getPosX(), obj->getPosY(), obj->getWidth(), obj->getHeight(), x, y))
        {
            obj->click(obj, button, state);
        }
    }
}

void Game::cellClick(Object_2D* obj, int button, int state)
{
    //this is a slight workaround, as this function needs to alter the state of the clicked cell, but that data is not in obj
    //there might be a better way to get the gridArray coords of the object, but this will work for now

    if(paused && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        int count = 0;
        for(int row = 0; row < MAX_GRID_HEIGHT; row++)
        {
            for(int column = 0; column < MAX_GRID_WIDTH; column++)
            {
                if(objectList.at(count++) == obj)
                {
                    switch(gridArray[row][column])
                    {
                        case Empty:
                            gridArray[row][column] = Alive;
                            break;
                        case Alive:
                            gridArray[row][column] = Dead;
                            break;
                        case Dead:
                            gridArray[row][column] = Empty;
                            break;
                    }

                    updateCells();
                    glutPostRedisplay();
                    return;
                }
            }
        }
    }
}

void Game::pauseClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        paused = true;
    }
}

void Game::playClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        timerDelay = SIMULATION_DELAY;
        paused = false;
    }
}

void Game::halfSpeedClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        timerDelay = SIMULATION_DELAY * 2;
        paused = false;
    }
}

void Game::doubleSpeedClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        timerDelay = SIMULATION_DELAY / 2;
        paused = false;
    }
}

void Game::nextClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        stageCounter++;
        simulateGeneration();
    }
}

void Game::resetClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        paused = true;
        stageCounter = 0; //reset stage counter

        //reset grid to 0
        for(int row = 0; row < MAX_GRID_HEIGHT; row++)
        {
            for(int column = 0; column < MAX_GRID_WIDTH; column++)
            {
                gridArray[row][column] = 0;
            }
        }

        updateCells();
        glutPostRedisplay();
    }
}

void Game::emptyColorClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && paused)
    {
        if(obj->getColor() != aliveColor && obj->getColor() != deadColor) //can't use same color for two cell types
        {
            emptyColor = obj->getColor();
            updateCells();
            glutPostRedisplay();
        }
    }
}

void Game::aliveColorClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && paused)
    {
        if(obj->getColor() != emptyColor && obj->getColor() != deadColor) //can't use same color for two cell types
        {
            aliveColor = obj->getColor();
            updateCells();
            glutPostRedisplay();
        }
    }
}

void Game::deadColorClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && paused)
    {
        if(obj->getColor() != emptyColor && obj->getColor() != aliveColor) //can't use same color for two cell types
        {
            deadColor = obj->getColor();
            updateCells();
            glutPostRedisplay();
        }
    }
}

void Game::previousPresetClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && paused)
    {
        if((selectedPreset - 1) < 0)
        {
            selectedPreset = (sizeof(presets) / sizeof(presetGrid)) - 1;
        }
        else
        {
            selectedPreset--;
        }

        loadPreset();
    }
}

void Game::nextPresetClick(Object_2D* obj, int button, int state)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && paused)
    {
        if((selectedPreset + 1) >= (sizeof(presets) / sizeof(presetGrid)))
        {
            selectedPreset = 0;
        }
        else
        {
            selectedPreset++;
        }

        loadPreset();
    }
}

void Game::timer(int val)
{
    if(!paused)
    {
        stageCounter++;
        simulateGeneration();
    }

    glutTimerFunc(timerDelay, timer, 0); //set next timer
}

void Game::simulateGeneration()
{
    //simulate next generation of cells
    int tempArray[MAX_GRID_HEIGHT][MAX_GRID_WIDTH] = {0};

    for(int row = 0; row < MAX_GRID_HEIGHT; row++)
    {
        for(int column = 0; column < MAX_GRID_WIDTH; column++)
        {
            //count neighbors
            int neighbors = 0;

            int topLeftNeighborRow = row - 1;
            int topLeftNeighborColumn = column - 1;

            int topNeighborRow = row - 1;
            int topNeighborColumn = column;

            int topRightNeighborRow = row - 1;
            int topRightNeighborColumn = column + 1;

            int leftNeighborRow = row;
            int leftNeighborColumn = column - 1;

            int rightNeighborRow = row;
            int rightNeighborColumn = column + 1;

            int bottomLeftNeighborRow = row + 1;
            int bottomLeftNeighborColumn = column - 1;
            
            int bottomNeighborRow = row + 1;
            int bottomNeighborColumn = column;

            int bottomRightNeighborRow = row + 1;
            int bottomRightNeighborColumn = column + 1;

            if(topLeftNeighborRow >= 0 && topLeftNeighborColumn >= 0 && gridArray[topLeftNeighborRow][topLeftNeighborColumn] == Alive) //top left neighbor
            {
                neighbors++;
            }

            if(topNeighborRow >= 0 && gridArray[topNeighborRow][topNeighborColumn] == Alive) //top neighbor
            {
                neighbors++;
            }

            if(topRightNeighborRow >= 0 && topRightNeighborColumn < MAX_GRID_WIDTH && gridArray[topRightNeighborRow][topRightNeighborColumn] == Alive) //top right neighbor
            {
                neighbors++;
            }

            if(leftNeighborColumn >= 0 && gridArray[leftNeighborRow][leftNeighborColumn] == Alive) //left neighbor
            {
                neighbors++;
            }

            if(rightNeighborColumn < MAX_GRID_WIDTH && gridArray[rightNeighborRow][rightNeighborColumn] == Alive) //right neighbor
            {
                neighbors++;
            }

            if(bottomLeftNeighborRow < MAX_GRID_HEIGHT && bottomLeftNeighborColumn >= 0 && gridArray[bottomLeftNeighborRow][bottomLeftNeighborColumn] == Alive) //bottom left neighbor
            {
                neighbors++;
            }

            if(bottomNeighborRow < MAX_GRID_HEIGHT && gridArray[bottomNeighborRow][bottomNeighborColumn] == Alive) //bottom neighbor
            {
                neighbors++;
            }

            if(bottomRightNeighborRow < MAX_GRID_HEIGHT && bottomRightNeighborColumn < MAX_GRID_WIDTH && gridArray[bottomRightNeighborRow][bottomRightNeighborColumn] == Alive) //bottom right neighbor
            {
                neighbors++;
            }

            //apply game logic and update cell state into temp array

            if(gridArray[row][column] == Alive)
            {
                if(neighbors < 2 || neighbors > 3) //death from under or overpopulation
                {
                    tempArray[row][column] = Dead;
                    continue;
                }
            }
            else if(gridArray[row][column] == Dead || gridArray[row][column] == Empty)
            {
                if(neighbors == 3) //dead (or empty) cell with exactly 3 live neighbors comes to life from reproduction
                {
                    tempArray[row][column] = Alive;
                    continue;
                }
            }

            tempArray[row][column] = gridArray[row][column]; //retain state
        }
    }

    //gridArray copies data from tempArray
    for(int row = 0; row < MAX_GRID_HEIGHT; row++)
    {
        for(int column = 0; column < MAX_GRID_WIDTH; column++)
        {
            gridArray[row][column] = tempArray[row][column];
        }
    }

    updateCells();
    glutPostRedisplay(); //request redisplay
}

void Game::updateCells()
{
    //Uses gridArray to update the graphical state of the grid
    int count = 0;
    for(int row = 0; row < MAX_GRID_HEIGHT; row++)
    {
        for(int column = 0; column < MAX_GRID_WIDTH; column++)
        {
            switch(gridArray[row][column])
            {
                case Empty:
                    objectList.at(count++)->setColor(emptyColor);
                    break;
                case Alive:
                    objectList.at(count++)->setColor(aliveColor);
                    break;
                case Dead:
                    objectList.at(count++)->setColor(deadColor);
                    break;
            }
        }
    }
}

void Game::loadPreset()
{
    for(int row = 0; row < MAX_GRID_HEIGHT; row++)
    {
        for(int column = 0; column < MAX_GRID_WIDTH; column++)
        {
            gridArray[row][column] = presets[selectedPreset].grid[row][column];
        }
    }

    updateCells();
    glutPostRedisplay();
}

GLuint Game::loadTexture(const char* imagePath)
{
    int width, height;
    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned char* img = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGBA); //load image data from file
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    SOIL_free_image_data(img); //free up image data

    return textureID;
}

bool Game::intersects(int x1, int y1, int width, int height, int x2, int y2)
{
    //x2 must be >= x1 & x2 must be <= (x1 + width)
    //y2 must be >= y1 & y2 must be <= (y1 + height)

    if(x2 >= x1 && x2 <= (x1 + width) && y2 >= y1 && y2 <= (y1 + height))
    {
        return true;
    }

    return false;
}

void Game::initializeBitmapFontMap()
{
    GLuint bitmapFontID = loadTexture("img\\bitmapFont.png");
    float bitmapFileWidth = BITMAP_FONT_WIDTH * BITMAP_FONT_COLUMNS;
    float bitmapFileHeight = BITMAP_FONT_HEIGHT * BITMAP_FONT_ROWS;
    float bitmapFontWidthRatio = (float)BITMAP_FONT_WIDTH / bitmapFileWidth;
    float bitmapFontHeightRatio = (float)BITMAP_FONT_HEIGHT / bitmapFileHeight;

    for(int rows = 0; rows < BITMAP_FONT_ROWS; rows++)
    {
        for(int columns = 0; columns < BITMAP_FONT_COLUMNS; columns++)
        {
            float bitmapFontXRatio = (float)(columns * BITMAP_FONT_WIDTH) / bitmapFileWidth;
            float bitmapFontYRatio = (float)(rows * BITMAP_FONT_HEIGHT) / bitmapFileHeight;
            bitmapFontMap[BITMAP_FONT_ARRAY[rows][columns]] = { bitmapFontID, bitmapFontWidthRatio, bitmapFontHeightRatio, bitmapFontXRatio, bitmapFontYRatio };
        }
    }
}

void Game::drawText(std::string text, float startx, float starty, float width, float height)
{
    for(char c : text)
    {
        Object_2D obj(width, height, startx, starty, WHITE, bitmapFontMap[c]);
        renderObject(&obj);
        startx += width;
    }
}