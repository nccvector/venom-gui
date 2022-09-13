#include "VenomGui/VenomGui.h"

int main()
{
  Venom::VenomGui* vg = new Venom::VenomGui();
  vg->Initialize();
  vg->Run();

  return 0;
}