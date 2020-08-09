#include "Player.hpp"
#include "../gfx/TextureAtlas.hpp"
#include "../ui/Keyboard.hpp"
#include <iostream>

Player::Player(double x, double y): Entity(x, y, PLAYER_WIDTH, PLAYER_HEIGHT)
{	
	loadTextures();
}

Player::Player(): Entity(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT)
{
	loadTextures();
}

Player::~Player()
{
	delete walkingLeftAnimation;
	delete walkingRightAnimation;
}

void Player::loadTextures()
{
	walkingRightAnimation = getAnimation("player_walking_right")->getCopy();
	walkingLeftAnimation = getAnimation("player_walking_left")->getCopy();
}

void Player::update(Tilemap* tilemap)
{
	applyGravity();
	handleInput();
	pdx = getDx();
	pdy = getDy();
	if(pdx < 0) facing = FACING_LEFT;
	else if(pdx > 0) facing = FACING_RIGHT;
	updatePosition(tilemap);
	Animation* currentAnimation = getCurrentAnimation();
	if(currentAnimation->isHalted()) currentAnimation->setFrame(0);
	if((pdx == 0 || !isOnGround()))
	{
		if(!currentAnimation->isHalted()) currentAnimation->halt
			(isJumping() ? Animation::HALT_IMMEDIATE : Animation::HALT_WAIT_NEXT);
	}
	else if(currentAnimation->isHalted()) currentAnimation->unHalt();
	currentAnimation->update();
}

double bdx, bdy, bx, by; // Buffer dx, dy, x, and y
void Player::checkLeftCollision(Rectangle delta, Rectangle tile) 
{ if(delta.intersects(tile)) bdx += (tile.x + tile.width - delta.x); }
void Player::checkRightCollision(Rectangle delta, Rectangle tile)
{ if(delta.intersects(tile)) bdx += (-(delta.x + delta.width - tile.x)); }
void Player::checkTopCollision(Rectangle delta, Rectangle tile)
{ if(delta.intersects(tile)) {bdy += (tile.y + tile.height - delta.y); setDy(0); } }
void Player::checkBottomCollision(Rectangle delta, Rectangle tile)
{ if(delta.intersects(tile)) bdy += (-(delta.y + delta.height - tile.y)); }

void Player::updatePosition(Tilemap* tilemap) // Override
{
	bdx = getDx();
	bdy = getDy();
	bx = getX();
	by = getY();
	#define DELTA_LEFT Rectangle(bx + bdx, by + Y_PADDING, getWidth() / 2 + -bdx * 2, getHeight() - Y_PADDING * 2)
	#define DELTA_RIGHT  Rectangle(bx + getWidth() / 2, by + Y_PADDING, getWidth() / 2 + bdx, getHeight() - Y_PADDING * 2)
	#define DELTA_TOP Rectangle(bx + X_PADDING, by + bdy, getWidth() - X_PADDING * 2, getHeight() / 2 + -bdy * 2)
	#define DELTA_BOTTOM  Rectangle(bx + X_PADDING, by + getHeight() / 2, getWidth() - X_PADDING * 2 , getHeight() / 2 + bdy)
	// Collision on x-axis
	if(getDx() >= 0) 
	{
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_RIGHT.getCenterX(), DELTA_RIGHT.getCenterY())) 
			checkRightCollision(DELTA_RIGHT, tile);
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_LEFT.getCenterX(), DELTA_LEFT.getCenterY())) 
			checkLeftCollision(DELTA_LEFT, tile);
	}
	else
	{
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_LEFT.getCenterX(), DELTA_LEFT.getCenterY())) 
			checkLeftCollision(DELTA_LEFT, tile);
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_RIGHT.getCenterX(), DELTA_RIGHT.getCenterY())) 
			checkRightCollision(DELTA_RIGHT, tile);
	}
	bx += bdx;
	// Collision on y-axis
	if(getDy() >= 0)
	{
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_BOTTOM.getCenterX(), DELTA_BOTTOM.getCenterY())) 
			checkBottomCollision(DELTA_BOTTOM, tile);
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_TOP.getCenterX(), DELTA_TOP.getCenterY())) 
			checkTopCollision(DELTA_TOP, tile);
	}
	else
	{
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_TOP.getCenterX(), DELTA_TOP.getCenterY())) 
			checkTopCollision(DELTA_TOP, tile);
		for(Rectangle tile : tilemap->getRectanglesSurrounding(DELTA_BOTTOM.getCenterX(), DELTA_BOTTOM.getCenterY())) 
			checkBottomCollision(DELTA_BOTTOM, tile);
	}
	by += bdy;
	setPosition(bx, by);
	// Determine whether player is on the ground
	bool isOnGround = false;
	Rectangle groundTest = Rectangle(bx + X_PADDING, by + getHeight() / 2, getWidth() - X_PADDING * 2 , getHeight() / 2 + 1);
	for(Rectangle tile : tilemap->getRectanglesSurrounding(groundTest.x + groundTest.width / 2, groundTest.y + groundTest.height / 2))
		if(groundTest.intersects(tile)) isOnGround = true;
	onGround = isOnGround;	
}

void Player::render()
{
	Texture* texture;
	// Determine which texture to draw
	if(getCurrentAnimation()->isHalted())
	{
		if(isFacingRight())
		{
			if(!isOnGround()) texture = getTexture("player_right_jump");
			else texture = getTexture("player_right_stand");	
		} 
		if(isFacingLeft())
		{
			if(!isOnGround()) texture = getTexture("player_left_jump");
			else texture = getTexture("player_left_stand");	
		}
	}
	else texture = getCurrentAnimation()->getCurrentFrame(); 
	double correctedX = absv(getDx()) > 0 ? getX() : (int) getX(), correctedY = absv(getDy()) > 0 ? getY() : (int) getY(); // Fix jitter
	int xOffset = isFacingRight() ? getWidth() - texture->width : 0; // Make sure texture fits in collision box
	Graphics::drawTextureTransparent(texture, correctedX + xOffset, correctedY, Graphics::TRANSPARENT, true);
}

void Player::handleInput()
{
	if(Keyboard::keyPressed(SDLK_LEFT))
	{
		if(getDx() > -MAX_SPEED) addDx(-ACCELERATION);	
		attemptingMove = true;
	} 
	else if(Keyboard::keyPressed(SDLK_RIGHT))
	{
		if(getDx() < MAX_SPEED) addDx(ACCELERATION);
		attemptingMove = true;
	} else
	{
		if(absv(getDx()) < DECCELERATION) setDx(0);
		if(getDx() > 0) addDx(-DECCELERATION);
		if(getDx() < 0) addDx(DECCELERATION);
	}
	if(!Keyboard::keyPressed(SDLK_LEFT) && !Keyboard::keyPressed(SDLK_RIGHT)) attemptingMove = false;
	if((Keyboard::keyPressed(SDLK_z) && isOnGround()) || Keyboard::keyPressed(SDLK_a)) jump();
}

void Player::jump()
{
	setDy(-JUMP_VELOCITY);
}

void Player::applyGravity()
{
	if(getDy() < TERMINAL_VELOCITY && (!isOnGround() || getDy() < GRAVITATIONAL_ACCELERATION)) addDy(GRAVITATIONAL_ACCELERATION);
	if(isOnGround()) setDy(GRAVITATIONAL_ACCELERATION);
}

Animation* Player::getCurrentAnimation() { return (isFacingRight() ? walkingRightAnimation : walkingLeftAnimation); }
bool Player::isFacingRight() { return (facing == FACING_RIGHT); }
bool Player::isFacingLeft() { return (facing == FACING_LEFT); }
bool Player::isJumping() { return (!isOnGround() && pdy < 0); }
bool Player::isOnGround() { return onGround; }
