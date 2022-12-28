unsigned long myrand(unsigned long seed) {
  static unsigned long rand;

  if (seed) {
    rand = seed;
  }
  rand *= 1234567;
  rand += 1397;

  return rand;
}