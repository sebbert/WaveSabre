// This library file is not used, but seems to be the only way to tell emscripten which symbols we want to import.

addToLibrary({
  WaveSabre_Alloc: function (size) {},
  WaveSabre_BeginUncompressSample: function (s, d, z) {},
});
