#include <SDL2/SDL.h>

#include <memory>

// SDL Deleters
struct SDL_Deleter {
  void operator()(SDL_Surface*  ptr) { if (ptr) SDL_FreeSurface(ptr); }
  void operator()(SDL_Texture*  ptr) { if (ptr) SDL_DestroyTexture(ptr); }
  void operator()(SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
  void operator()(SDL_Window*   ptr) { if (ptr) SDL_DestroyWindow(ptr); }
  void operator()(SDL_RWops*    ptr) { if (ptr) SDL_RWclose(ptr); }
};
 
// Unique Pointers
using SurfaceUniquePtr  = std::unique_ptr<SDL_Surface,  SDL_Deleter>;
using TextureUniquePtr  = std::unique_ptr<SDL_Texture,  SDL_Deleter>;
using RendererUniquePtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
using WindowUniquePtr   = std::unique_ptr<SDL_Window,   SDL_Deleter>;
using RWopsUniquePtr    = std::unique_ptr<SDL_RWops,    SDL_Deleter>;


template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter : public std::shared_ptr<T>
{
  explicit shared_ptr_with_deleter(T* t = nullptr)
      : std::shared_ptr<T>(t, D()) {}
 
  void reset(T* t = nullptr) {
    std::shared_ptr<T>::reset(t, D());
  }
};

using SurfacePtr  = shared_ptr_with_deleter<SDL_Surface,  SDL_Deleter>;
using TexturePtr  = shared_ptr_with_deleter<SDL_Texture,  SDL_Deleter>;
using RendererPtr = shared_ptr_with_deleter<SDL_Renderer, SDL_Deleter>;
using WindowPtr   = shared_ptr_with_deleter<SDL_Window,   SDL_Deleter>;
using RWopsPtr    = shared_ptr_with_deleter<SDL_RWops,    SDL_Deleter>;
