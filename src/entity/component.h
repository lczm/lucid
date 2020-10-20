#pragma once

#include "constants.h"
#include "gl.h"

/*
   A standard place to store components.
   Components should just be defined as POD (Plain Old Data)
   i.e. simply a struct.

  Note : the structs/components that are declared here should have a default
   value.
*/

// PlayerState enum
enum class PLAYER_STATE {
  IDLE1,
  CROUCH,
  RUN,
  JUMP,
  SOMERSAULT,
  FALL,
  SLIDE,
  STAND,
  CORNERGRAB,
  CORNERCLIMB,
  IDLE2,
  ATTACK1,
  ATTACK2,
  ATTACK3,
  HURT,
  DIE,
  CORNERJUMP,
  SWORDDRAW,
  SWORDSHEATHE,
  JUMP2,  // Not exactly too sure what this is...
  FALL2,  // Not exactly too sure what this is...
  WALLCLIMB,

  PUNCH1,
  PUNCH2,
  PUNCH3,
  KICK1,
  KICK2,
  DROPKICK,
  WALK,
  KNOCKED_DOWN,
  RECOVER,
  RUN2,
  CROUCHWALK,
  WALLRUN
};

// Tile Type enum
enum class TILE_TYPES {
  WALL,
  COBBLE1,
  COBBLE2,
  COBBLE3,
  COBBLE4,
  COBBLE5,
  COBBLE6,
  CHAIN,
  MOULDYLEDGE,
  MOULDYFLOOR,
  MOULDYFLOOR2,
  STONELEDGE,
  STONEFLOOR,
  STONEFLOOR2,
  WOODPLATFORM,
  WOODPLATFORMSUPPORT1,
  MOULDYWALL1,
  MOULDYWALL2,
  MOULDYWALL3,
  COBBLEWALL1,
  COBBLESTAND,
  WEIRDTHING,
  WOODPLATFORMSUPPORT2,
  WOODWALL,
  MOULDYBOTTOMLEDGE,
  MOULDYBOTTOM,
  GRASS,
  COBBLEBOTTOMLEDGE,
  COBBLEBOTTOM,
  BLUESQUARE,
  WOODPILLAR,
  REDBIRD1,
  PIPETIP,
  PIPEBEND,
  PIPESHAFT,
  SANDLEDGE,
  SANDFLOOR,
  SANDDRIP,
  REDBIRD2,
  REDBIRD3,
  GREYLINE,
  BLUEBIRD1,
  PIPECONNECTOR1,
  SANDWALL,
  SANDPOOP,
  TORCH1,
  WOODLADDER1,
  STEELLADDER1,
  BLUEBIRD2,
  BLUEBIRD3,
  PIPECONNECTOR2,
  SANDBOTTOMLEDGE,
  SANDBOTTOM,
  TORCH2,
  WOODLADDER2,
  STEELLADDER2,
  BLUEBOX,
  GREENSQUARE,
  GUNTHING,
  GUNBULLETS,
  LAVA1,
  GREYBOX,
  WALLLIGHT,
  CEILINGLIGHT,
  PIPEOUT,
  WATERFALL1,
  BLUEPILLAR,
  FIRETHING,
  LAVA2,
  CLOUD1,
  CLOUD2,
  CLOUD3,
  SPLASH1,
  WATERFALL2,
  SPLASH2,
  PIPEOUTWATERFALL,
  LAVA3,
  CLOUD4,
  CLOUD5,
  CLOUD6,
  WATER1,
  WATER2,
  WATER3,
  WATER4,
  LAVA4,
  VINE1,
  VINE2,
  VINE3,
  WATER5,
  WATER6,
  WATER7,
  WATER8,
  LAVA5
};

// SlimeState enum
enum class SLIME_STATE { IDLE, MOVE, ATTACK, HURT, DIE };

// Direction enum
enum class DIRECTION {
  NORTH,  // I guess this is for when the player goes up the ladder?
  SOUTH,  // I guess this is for when the player goes down the ladder?
  EAST,
  WEST
};

typedef struct Movement {
  float verticalVelocity = 0;
  float horizontalVelocity = 0;

  // there has to be a more proper name for this...?
  float verticalVelocityDecay = 10;
  float horizontalVelocityDecay = 10;

  // This is for the PhysicsSystem to update
  bool colliding = false;
} Movement;

typedef struct PlayerState {
  PLAYER_STATE state = PLAYER_STATE::IDLE1;
} PlayerState;

typedef struct SlimeState {
  SLIME_STATE state = SLIME_STATE::IDLE;
} SlimeState;

typedef struct Transform {
  float x = 0;
  float y = 0;
  float scale = 1;
} Transform;

typedef struct Animation {
  float animCounter = 0;   // Counter on time passed
  float animInterval = 1;  // Counter on the threshold of each animation

  int spriteCurrent = 0;  // Current sprite
  int spriteStart = 0;    // Default for IDLE1, change when needed
  int spriteEnd = 3;      // Default for IDLE1, change when needed
} Animation;

typedef struct Tile {
  TILE_TYPES tileType = TILE_TYPES::WALL;
  int collidable = 0;
} Tile;

typedef struct Map {
  std::vector<TILE_TYPES> tiles = {};
  int rows = 0;
  int cols = 0;
} Map;

typedef struct Image {
  int spriteX = 0;
  int spriteY = 0;

  // These values are passed into the model matrix for transformation
  int imageSlot = 1;
  // These are used to get which sprite in the sheet is being used
  float sheetWidth = 1;
  float sheetHeight = 1;
  float spriteWidth = 1;
  float spriteHeight = 1;

  float scaledWidth = 1;
  float scaledHeight = 1;

  int totalSpriteRow = 1;
  int totalSpriteCol = 1;

  // Set default direction. Generally will be facing EAST. I think West might be
  // the only one that might be commonly used. For west, the RenderSystem can
  // just flip the image
  DIRECTION direction = DIRECTION::EAST;

  // These will hold position, color, texcoords
  std::vector<Vertex> vertices;

  glm::mat4 projection;
  glm::mat4 view;

  Texture* texture = new Texture();
  VertexArray* vertexArray = new VertexArray();
  VertexBuffer* vertexBuffer = new VertexBuffer();
  VertexBufferLayout* vertexBufferLayout = new VertexBufferLayout();
  IndexBuffer* indexBuffer = new IndexBuffer();
  Shader* shader = new Shader();
} Image;

// Just a pointer to a component
typedef struct ImageShared {
  Image* image = new Image();
} ImageShared;
