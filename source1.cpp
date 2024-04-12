#include "SFML/Graphics.hpp"
#include "player.hpp"
#include "ItemStack.hpp"
#include "world.hpp"
#include "game.hpp"

int main() {
    // SETUP WINDOW AND CAMERA
    World world;
    Game game(world);

    game.gameLoop();
    // DEFINE TEXTURES AND SPRITES
    /* SETUP PLAYER SPRITE
        NOW IN PLAYER.H
             \ /
              V
    txt_player.loadFromFile("nig.png");
    spr_player.setTexture(txt_player);
    spr_player.setPosition(viewSize.x * 0.8 + 60000, viewSize.y * 0.747); /// 60000 TO SRODEK MAPY
    */

    return 0;
}