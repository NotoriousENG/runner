#pragma once

// resource paths:

#ifndef SHIP_IT
#ifdef SHARED_GAME
#define ASSETS_BASE_PATH "../../assets/"
#else
#define ASSETS_BASE_PATH "../assets/"
#endif
#else
#define ASSETS_BASE_PATH "assets/"
#endif

// C Macro to concatenate file path to assets base path
#define ASSETS_PATH(file) (ASSETS_BASE_PATH file)

#define RES_FONT_VERA ASSETS_PATH("fonts/Vera.ttf")

#define RES_MODEL_CUBE ASSETS_PATH("models/cube.glb")

#define RES_MODEL_ANIMATED ASSETS_PATH("models/Test/test.glb")

#define RES_SHADER_SPRITE_VERT ASSETS_PATH("shaders/sprite.vert")
#define RES_SHADER_SPRITE_FRAG ASSETS_PATH("shaders/sprite.frag")

#define RES_SHADER_MODEL_VERT ASSETS_PATH("shaders/mesh.vert")
#define RES_SHADER_MODEL_FRAG ASSETS_PATH("shaders/mesh.frag")