#include "stdafx.h"
#include "Maze.h"
#include "instantcg.h"

using namespace InstantCG;

static constexpr auto MAP_WIDTH = 24;
static constexpr auto MAP_HEIGHT = 24;

static constexpr int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
    ,{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void Maze::run()
{
    double posX = MAP_WIDTH / 2, posY = MAP_HEIGHT / 2; // x and y start position
    double dirX = -1, dirY = 0; // initial direction vector
    double planeX = 0, planeY = 0.66; // the 2d raycaster version of camera plane

    double time = 0; // time of current frame
    double oldTime; // time of previous frame

    screen(512, 384, 0, "Raycaster");

    while (!done(false)) {
        for (auto x = 0; x < w; x++) {
            // calculate ray position and direction
            auto cameraX = 2 * x / double(w) - 1; // x-coordinate in camera space
            auto rayPosX = posX;
            auto rayPosY = posY;
            auto rayDirX = dirX + planeX * cameraX;
            auto rayDirY = dirY + planeY * cameraX;
            // which box of the map we're in
            auto mapX = int(rayPosX);
            auto mapY = int(rayPosY);

            // length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            // length of ray from one x or y-side to next x or y-side
            auto deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            auto deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
            double perpWallDist;

            // what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            auto hit = 0; // was there a wall hit?
            auto side = 0; // was a NS or a EW wall hit?
            // calculate step and initial sideDist
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (rayPosX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
            }

            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (rayPosY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
            }

            // perform DDA
            while (hit == 0) {
                // jump to next map square, OR in x-direction, OR in y-direction
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                // Check if ray has hit a wall
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            // Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
            if (side == 0)
                perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
            else
                perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

            // Calculate height of line to draw on screen
            auto lineHeight = abs(int(h / perpWallDist));

            // calculate lowest and highest pixel to fill in current stripe
            auto drawStart = std::max(0, -lineHeight / 2 + h / 2);
            auto drawEnd = std::min(h - 1, lineHeight / 2 + h / 2);

            // choose wall color
            ColorRGB color;
            switch (worldMap[mapX][mapY]) {
            case 1:
                color = RGB_Red;
                break; // red
            case 2:
                color = RGB_Green;
                break; // green
            case 3:
                color = RGB_Blue;
                break; // blue
            case 4:
                color = RGB_White;
                break; // white
            default:
                color = RGB_Magenta;
                break; // yellow
            }

            // give x and y sides different brightness
            if (side == 1) {
                color = color / 2;
            }

            // draw the pixels of the stripe as a vertical line
            verLine(x, drawStart, drawEnd, color);
        }

        // timing for input and FPS counter
        oldTime = time;
        time = getTicks();
        auto frameTime = (time - oldTime) / 1000.0; // frameTime is the time this frame has taken, in seconds
        
        redraw();
        cls();

        // speed modifiers
        auto moveSpeed = frameTime * 5.0; // the constant value is in squares/second
        auto rotSpeed = frameTime * 3.0; // the constant value is in radians/second

        readKeys();

        // move forward if no wall in front of you
        if (keyDown(SDLK_UP)) {
            if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
        }
        // move backwards if no wall behind you
        if (keyDown(SDLK_DOWN)) {
            if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0) posX -= dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
        }
        // rotate to the right
        if (keyDown(SDLK_RIGHT)) {
            // both camera direction and camera plane must be rotated
            auto oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            auto oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }
        // rotate to the left
        if (keyDown(SDLK_LEFT)) {
            // both camera direction and camera plane must be rotated
            auto oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            auto oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
    }
}

