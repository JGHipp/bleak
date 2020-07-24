#include "Player.hpp"
#include "../gfx/TextureAtlas.hpp"
#include <iostream>

Player::Player(double x, double y): Entity(x, y, PLAYER_WIDTH, PLAYER_HEIGHT), 
	onGround(false), attemptingMove(false), facing(1), runningTime(0), airTime(0)
{	
	loadTextures();
}

Player::Player(): Entity(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT), 
	onGround(false), attemptingMove(false), facing(1), runningTime(0), airTime(0)
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

void Player::update(Keyboard* keyboard, Tilemap* tilemap)
{
	applyGravity();
	handleInput(keyboard);
	pdx = getDx();
	pdy = getDy();
	if(pdx == 0) runningTime = 0;
	else runningTime++;
	/* */if(pdx < 0) facing = 0;
	else if(pdx > 0) facing = 1;
	/* */if(pdx == 0 && facing == 1) walkingRightAnimation->setFrame(0);
	else if(pdx == 0 && facing == 0) walkingLeftAnimation->setFrame(0);
	updatePosition(tilemap);
	if(isOnGround())
	{
		groundTime++;
		airTime = 0;
	}
	else
	{
		airTime++;
		groundTime = 0;	
	} 
	/* */if(facing == 1 && isOnGround() && groundTime >= ANIM_MIN_GROUND_TIME) 	walkingRightAnimation->update();
	else if(facing == 0 && isOnGround() && groundTime >= ANIM_MIN_GROUND_TIME) 	walkingLeftAnimation->update();
	/* */if(facing == 1 && !isOnGround() && airTime >= ANIM_MIN_AIR_TIME) 		walkingRightAnimation->setFrame(2);
	else if(facing == 0 && !isOnGround() && airTime >= ANIM_MIN_AIR_TIME) 		walkingLeftAnimation->setFrame(2);
}

// Collision was hell to figure out. Took 7 hours to finally get right. 
double bdx, bdy, bx, by; // Buffer dx, dy, x, and y
void Player::checkLeftCollision(Rectangle delta, Rectangle* tile) 
{ if(delta.intersects(*tile)) bdx += (tile->x + tile->width - delta.x); }
void Player::checkRightCollision(Rectangle delta, Rectangle* tile)
{ if(delta.intersects(*tile)) bdx += (-(delta.x + delta.width - tile->x)); }
void Player::checkTopCollision(Rectangle delta, Rectangle* tile)
{ if(delta.intersects(*tile)) {bdy += (tile->y + tile->height - delta.y); setDy(0); } }
void Player::checkBottomCollision(Rectangle delta, Rectangle* tile)
{ if(delta.intersects(*tile)) bdy += (-(delta.y + delta.height - tile->y)); }

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
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_RIGHT.getCenterX(), DELTA_RIGHT.getCenterY())) 
			checkRightCollision(DELTA_RIGHT, tile);
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_LEFT.getCenterX(), DELTA_LEFT.getCenterY())) 
			checkLeftCollision(DELTA_LEFT, tile);
	}
	else
	{
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_LEFT.getCenterX(), DELTA_LEFT.getCenterY())) 
			checkLeftCollision(DELTA_LEFT, tile);
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_RIGHT.getCenterX(), DELTA_RIGHT.getCenterY())) 
			checkRightCollision(DELTA_RIGHT, tile);
	}
	bx += bdx;
	// Collision on y-axis
	if(getDy() >= 0)
	{
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_BOTTOM.getCenterX(), DELTA_BOTTOM.getCenterY())) 
			checkBottomCollision(DELTA_BOTTOM, tile);
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_TOP.getCenterX(), DELTA_TOP.getCenterY())) 
			checkTopCollision(DELTA_TOP, tile);
	}
	else
	{
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_TOP.getCenterX(), DELTA_TOP.getCenterY())) 
			checkTopCollision(DELTA_TOP, tile);
		for(Rectangle* tile : tilemap->getRectanglesSurrounding(DELTA_BOTTOM.getCenterX(), DELTA_BOTTOM.getCenterY())) 
			checkBottomCollision(DELTA_BOTTOM, tile);
	}
	by += bdy;
	setPosition(bx, by); // Update real position with buffer
	// Determine whether player is on the ground
	bool isOnGround = false;
	Rectangle groundTest = Rectangle(bx + X_PADDING, by + getHeight() / 2, getWidth() - X_PADDING * 2 , getHeight() / 2 + 1);
	for(Rectangle* tile : tilemap->getRectanglesSurrounding(groundTest.x + groundTest.width / 2, groundTest.y + groundTest.height / 2))
		if(groundTest.intersects(*tile)) isOnGround = true;
	onGround = isOnGround;	
}

void Player::render(Graphics* graphics, Camera* camera)
{
	Texture* texture;
	if((!attemptingMove || runningTime < ANIM_MIN_RUN_TIME) && isOnGround() && groundTime > ANIM_MIN_GROUND_TIME) 
		texture = (isFacingRight() ? getTexture("player_right_stand") : getTexture("player_left_stand"));
	else texture = (isFacingRight() ? walkingRightAnimation->getCurrentFrame() : walkingLeftAnimation->getCurrentFrame());
	graphics->drawTexture(texture, getX(), getY(), 0xFF00FF, camera);
}

void Player::handleInput(Keyboard* keyboard)
{
	if(keyboard->keyPressed(SDLK_LEFT))
	{
		if(getDx() > -MAX_SPEED) addDx(-ACCELERATION);	
		attemptingMove = true;
	} 
	else if(keyboard->keyPressed(SDLK_RIGHT))
	{
		if(getDx() < MAX_SPEED) addDx(ACCELERATION);
		attemptingMove = true;
	} else
	{
		if(absv(getDx()) < DECCELERATION) setDx(0);
		if(getDx() > 0) addDx(-DECCELERATION);
		if(getDx() < 0) addDx(DECCELERATION);
	}
	if(!keyboard->keyPressed(SDLK_LEFT) && !keyboard->keyPressed(SDLK_RIGHT)) attemptingMove = false;
	if((keyboard->keyPressed(SDLK_z) && isOnGround()) || keyboard->keyPressed(SDLK_a)) jump();
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

bool Player::isFacingRight() { return facing; }
bool Player::isFacingLeft() { return !facing; }
bool Player::isOnGround() { return onGround; }
