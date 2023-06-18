#pragma once


namespace DI{
 
  // Base class for drawing object such as models, primitives, interfaces ...
  class Drawable {
    friend class RendererSingleton;
    protected:
      Drawable(){}
      virtual ~Drawable(){}
      virtual void Draw() = 0;
  };

}
