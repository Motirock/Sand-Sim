#include "Player.h"

Player::Player() {

}

Player::~Player() {}

void Player::update(Input &input) {
    if (input.upPressed)
        y -= speed;
    if (input.downPressed)
        y += speed;
    if (input.leftPressed)
        x -= speed;
    if (input.rightPressed)
        x += speed;
}

void Player::draw() {

}