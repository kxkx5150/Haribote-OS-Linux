int mystrcmp(char *str1, char *str2) {
  int i;
  for (i = 0; i < 20; i++) {
    if (str1[i] == 0x00 && str2[i] == 0x00) return 0;
    if (str1[i] == 0x00) return 1;
    if (str2[i] == 0x00) return 1;
    if (str1[i] != str2[i]) return 1;
  }
  return -1;
}