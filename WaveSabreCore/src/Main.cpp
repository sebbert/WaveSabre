#include <WaveSabreCore.h>
#include <emscripten/console.h>

int main() {
  char *test = new char[256];
  test[0] = 'H';
  test[1] = 'e';
  test[2] = '\0';
  emscripten_console_log(test);

  new WaveSabreCore::Falcon();
  new WaveSabreCore::Slaughter();
  new WaveSabreCore::Scissor();
  new WaveSabreCore::Leveller();
  new WaveSabreCore::Crusher();
  new WaveSabreCore::Echo();
  new WaveSabreCore::Smasher();
  new WaveSabreCore::Chamber();
  new WaveSabreCore::Twister();
  new WaveSabreCore::Cathedral();
  return 0;
}