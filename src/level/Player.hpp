#ifndef PLAYER_H
#define PLAYER_H

#include "../gfx/TextureAtlas.hpp"
#include "../gfx/Graphics.hpp"
#include "../gfx/Texture.hpp"
#include "../ui/Keyboard.hpp"
#include "Camera.hpp"
#include "Tilemap.hpp"
#include "Entity.hpp"

const int PLAYER_WIDTH = 15;
const int PLAYER_HEIGHT = 33;
// 'Padding' is the space in between the horizontal and verticle collision boxes
const int X_PADDING = 2;
const int Y_PADDING = 2;
// Physics constants
const double ACCELERATION = 0.075;
const double DECCELERATION = 0.1;
const double MAX_SPEED = 1.75;
const double JUMP_VELOCITY = 3.45;
const double TERMINAL_VELOCITY = 2.75;
const double GRAVITATIONAL_ACCELERATION = 0.125;

class Player: public Entity
{
	private:	
		bool onGround = false, attemptingMove = false;
		enum facing_t { FACING_LEFT, FACING_RIGHT } facing = FACING_RIGHT;
		long runningTime = 0, groundTime = 0, airTime = 0;
		double pdx = 0.0, pdy = 0.0; // Dx and Dy before collision handling
		Animation* walkingRightAnimation, *walkingLeftAnimation;
		Animation* getCurrentAnimation();
		void checkLeftCollision(Rectangle delta, Rectangle tile);
		void checkRightCollision(Rectangle delta, Rectangle tile);
		void checkTopCollision(Rectangle delta, Rectangle tile);
		void checkBottomCollision(Rectangle delta, Rectangle tile);
		void loadTextures();
		void handleInput();
		void updatePosition(Tilemap* tilemap); // Override
		void applyGravity();
	public:	
		Player(double x, double y);
		Player();
		~Player();
		void update(Tilemap* tilemap);
		void render(Graphics* graphics, Camera* camera);
		bool isFacingRight();
		bool isFacingLeft();
		bool isOnGround();
		bool isJumping();
		void jump();

};

#endif
