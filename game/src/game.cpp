#include "game.hpp"
#include "resource-paths.hpp"

#include <SDL.h>
#include <asset-manager.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <input.hpp>

static Game *game; // this is dirty but it works for now

#ifdef SHARED_GAME
#include <cassert>
#include <cr.h>

static int loaded_timestamp = 0;

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation) {
  assert(ctx);

  gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);
  // get a random int
  switch (operation) {
  case CR_LOAD:
    loaded_timestamp = SDL_GetTicks();
    game = new Game();
    game->init((SharedData *)ctx->userdata);
    return printf("loaded %i\n", loaded_timestamp);
  case CR_UNLOAD:
    game->unload();
    return printf("unloaded %i\n", loaded_timestamp);
  case CR_CLOSE:
    game->close();
    delete game;
    return printf("closed %i\n", loaded_timestamp);
  case CR_STEP:
    return game->update();
  }
  return 0;
}
#endif

Game::Game() { game = this; }

Game::~Game() {}

int Game::init(SharedData *shared_data) {
  SDL_Log("Game init");
  SDL_SetWindowTitle(SDL_GL_GetCurrentWindow(), "Runner");

  // map the text_input_buffer
  InputManager::SetTextInputBuffer(&shared_data->text_input_buffer[0]);
  // Get current window size
  int w, h;
  SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &w, &h);
  this->spriteBatcher = std::make_unique<SpriteBatch>(
      glm::vec2(w, h), RES_SHADER_SPRITE_VERT, RES_SHADER_SPRITE_FRAG);

  this->meshRenderer = std::make_unique<MeshRenderer>(RES_SHADER_MODEL_VERT,
                                                      RES_SHADER_MODEL_FRAG);

  this->mixer = std::make_unique<Mixer>();

  this->font = AssetManager<Font>::getFont(RES_FONT_VERA, 32);
  this->model = AssetManager<Model>::get(RES_MODEL_ANIMATED);

  glm::vec2 center = glm::vec2(w / 2, h / 2);
  SDL_Rect bounds = {0, 0, w, h};

  this->spriteBatcher->UpdateCamera(center,
                                    bounds); // this is needed for sprites/text

  // set clear color to test sky blue
  glClearColor(0.53f, 0.81f, 0.98f, 1.0f);

  return 0;
}

int Game::update() {

  float time = SDL_GetTicks() / 1000.0f;
  float delta = time - this->lastTime;
  this->lastTime = time;

  // INPUT:
  int num_keys;
  const Uint8 *key_state = SDL_GetKeyboardState(&num_keys);
  InputManager::Update(key_state, num_keys);

  // LOGIC:

  // RENDER:

  // set the camera
  glm::mat4 view =
      glm::lookAt(this->camPos, glm::vec3(0.0f, 0.0f, 0.0f), this->camUp);
  this->meshRenderer->SetViewMatrix(view);

  for (const auto &mesh : this->model->getMeshes()) {
    this->meshRenderer->DrawMesh(mesh.get(), mesh->model);
  }

  this->font->RenderText(this->spriteBatcher.get(), "Hello World!",
                         glm::vec2(0, 0), glm::vec2(1.0f),
                         glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

  // draw all sprites in the batch (note text is also a sprite)
  this->spriteBatcher->Flush();

  return 0;
}

int Game::unload() { return 0; }

int Game::close() {
  // clean up gl stuff
  return 0;
}
