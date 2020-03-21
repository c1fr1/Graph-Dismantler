#pragma once

typedef struct {
  int codePoint, x, y, width, height, originX, originY, advance;
} Character;
//https://evanw.github.io/font-texture-generator/example-webgl/
typedef struct {
  const char *name;
  int size, bold, italic, width, height, characterCount;
  Character *characters;
} Font;

static Character hackCharacters[] = {
  {' ', 863, 118, 3, 3, 1, 1, 39},
  {'!', 45, 67, 13, 51, -13, 48, 39},
  {'"', 703, 118, 20, 20, -9, 48, 39},
  {'#', 978, 67, 41, 48, 1, 47, 39},
  {'$', 171, 0, 32, 60, -4, 50, 39},
  {'%', 0, 118, 39, 47, 0, 46, 39},
  {'&', 465, 0, 40, 51, 0, 49, 39},
  {'\'', 723, 118, 8, 20, -15, 48, 39},
  {'(', 203, 0, 17, 59, -8, 50, 39},
  {')', 220, 0, 17, 59, -14, 50, 39},
  {'*', 582, 118, 33, 34, -3, 46, 39},
  {'+', 461, 118, 36, 36, -1, 39, 39},
  {',', 615, 118, 15, 26, -11, 12, 39},
  {'-', 798, 118, 29, 8, -5, 23, 39},
  {'.', 785, 118, 13, 13, -13, 11, 39},
  {'/', 276, 0, 33, 55, -2, 48, 39},
  {'0', 642, 0, 33, 51, -3, 49, 39},
  {'1', 889, 67, 30, 49, -6, 48, 39},
  {'2', 97, 67, 32, 50, -3, 49, 39},
  {'3', 675, 0, 33, 51, -3, 49, 39},
  {'4', 592, 67, 35, 49, -2, 48, 39},
  {'5', 129, 67, 32, 50, -3, 48, 39},
  {'6', 708, 0, 33, 51, -3, 49, 39},
  {'7', 161, 67, 32, 50, -3, 49, 39},
  {'8', 741, 0, 33, 51, -3, 49, 39},
  {'9', 540, 0, 34, 51, -2, 49, 39},
  {':', 497, 118, 13, 35, -13, 36, 39},
  {';', 29, 67, 16, 51, -11, 35, 39},
  {'<', 510, 118, 36, 34, -1, 37, 39},
  {'=', 667, 118, 36, 20, -1, 30, 39},
  {'>', 546, 118, 36, 34, -1, 37, 39},
  {'?', 437, 0, 28, 52, -6, 50, 39},
  {'@', 237, 0, 39, 56, 1, 45, 39},
  {'A', 366, 67, 39, 49, 0, 48, 39},
  {'B', 662, 67, 33, 49, -4, 48, 39},
  {'C', 840, 0, 32, 51, -3, 49, 39},
  {'D', 695, 67, 33, 49, -3, 48, 39},
  {'E', 858, 67, 31, 49, -5, 48, 39},
  {'F', 919, 67, 30, 49, -6, 48, 39},
  {'G', 574, 0, 34, 51, -2, 49, 39},
  {'H', 728, 67, 33, 49, -3, 48, 39},
  {'I', 949, 67, 29, 49, -5, 48, 39},
  {'J', 256, 67, 29, 50, -2, 48, 39},
  {'K', 482, 67, 37, 49, -3, 48, 39},
  {'L', 794, 67, 32, 49, -5, 48, 39},
  {'M', 556, 67, 36, 49, -1, 48, 39},
  {'N', 761, 67, 33, 49, -3, 48, 39},
  {'O', 608, 0, 34, 51, -2, 49, 39},
  {'P', 826, 67, 32, 49, -4, 48, 39},
  {'Q', 134, 0, 37, 61, -2, 49, 39},
  {'R', 519, 67, 37, 49, -3, 48, 39},
  {'S', 774, 0, 33, 51, -3, 49, 39},
  {'T', 58, 67, 39, 50, 0, 49, 39},
  {'U', 193, 67, 32, 50, -3, 48, 39},
  {'V', 444, 67, 38, 49, 0, 48, 39},
  {'W', 285, 67, 41, 49, 1, 48, 39},
  {'X', 326, 67, 40, 49, 1, 48, 39},
  {'Y', 405, 67, 39, 49, 0, 48, 39},
  {'Z', 627, 67, 35, 49, -2, 48, 39},
  {'[', 112, 0, 22, 62, -9, 53, 39},
  {'\\', 309, 0, 33, 55, -3, 48, 39},
  {']', 89, 0, 23, 62, -7, 53, 39},
  {'^', 630, 118, 37, 20, -1, 48, 39},
  {'_', 827, 118, 36, 6, -1, -5, 39},
  {'`', 767, 118, 18, 14, -7, 53, 39},
  {'a', 73, 118, 33, 39, -3, 37, 39},
  {'b', 406, 0, 31, 52, -5, 50, 39},
  {'c', 139, 118, 30, 39, -4, 37, 39},
  {'d', 342, 0, 32, 52, -2, 50, 39},
  {'e', 39, 118, 34, 39, -2, 37, 39},
  {'f', 935, 0, 30, 51, -4, 50, 39},
  {'g', 374, 0, 32, 52, -3, 38, 39},
  {'h', 995, 0, 29, 51, -5, 50, 39},
  {'i', 0, 67, 29, 51, -7, 50, 39},
  {'j', 8, 0, 22, 64, -6, 50, 39},
  {'k', 807, 0, 33, 51, -6, 50, 39},
  {'l', 965, 0, 30, 51, -4, 50, 39},
  {'m', 235, 118, 35, 38, -2, 37, 39},
  {'n', 270, 118, 29, 38, -5, 37, 39},
  {'o', 106, 118, 33, 39, -3, 37, 39},
  {'p', 872, 0, 32, 51, -4, 37, 39},
  {'q', 904, 0, 31, 51, -3, 37, 39},
  {'r', 328, 118, 28, 38, -8, 37, 39},
  {'s', 169, 118, 29, 39, -5, 37, 39},
  {'t', 225, 67, 31, 50, -3, 49, 39},
  {'u', 299, 118, 29, 38, -5, 36, 39},
  {'v', 397, 118, 35, 37, -2, 36, 39},
  {'w', 356, 118, 41, 37, 1, 36, 39},
  {'x', 198, 118, 37, 38, -1, 37, 39},
  {'y', 505, 0, 35, 51, -2, 37, 39},
  {'z', 432, 118, 29, 37, -5, 36, 39},
  {'{', 30, 0, 30, 62, -2, 53, 39},
  {'|', 0, 0, 8, 67, -15, 50, 39},
  {'}', 60, 0, 29, 62, -7, 53, 39},
  {'~', 731, 118, 36, 17, -1, 27, 39},
};

static Font hackFont = {"Hack", 64, 0, 0, 1024, 256, 95, hackCharacters};
