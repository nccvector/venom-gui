#include "Widget.h"

namespace Venom
{
class Viewport : public Widget
{
 public:
  Viewport(std::string name = "Viewport", unsigned int textureId = 0,
           ImVec2 textureSize = {800, 600})
      : _textureId(textureId), _textureSize(textureSize), Widget(name)
  {
  }
  ~Viewport() {}

  void SetTextureId(unsigned int textureId) { _textureId = textureId; }
  void SetTextureSize(ImVec2 textureSize) { _textureSize = textureSize; }

 private:
  void _DrawContent() override { ImGui::Image((void*)textureId, textureSize); }

 private:
  unsigned int _textureId;
  ImVec2 _textureSize;
};
}  // namespace Venom