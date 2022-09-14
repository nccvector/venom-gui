#include "imgui.h"

namespace Venom
{
class Widget
{
 public:
  Widget(std::string name) : _name(name) {}
  ~Widget() {}

  // To be called by application...
  void Draw()
  {
    _Begin();
    _DrawContent();
    _End();
  }

 private:
  void _Begin() { ImGui::Begin(_name); }
  void _DrawContent() { ImGui::Text("some widget content"); }
  void _End() { ImGui::End(); }

 private:
  std::string _name;
};
}  // namespace Venom