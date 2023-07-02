struct Organ {
    int start;
    int len;
    int sensor;
    bool clockwise;
};

struct Organ NOSE =             {   0, 16, -1, false };
struct Organ WRITERSCRAMP =     {  16, 29, 11,  true };
struct Organ WRENCHEDANKLE =    {  46, 28, 10,  true };
struct Organ BREADBASKET =      {  75, 22,  9,  true };
struct Organ BUTTERFLIES =      {  98, 23,  8,  true };
struct Organ BROKENHEART =      { 122, 15,  7,  true };
struct Organ ADAMSAPPLE =       { 138, 21,  6,  true };
struct Organ WISHBONE =         { 159, 26,  5,  true };
struct Organ SPARERIBS =        { 186, 28,  4, false };
struct Organ FUNNYBONE =        { 215, 21,  3,  true };
struct Organ WATERONKNEE =      { 237, 25,  2,  true };
struct Organ CHARLIEHORSE =     { 263, 25,  1,  true };
struct Organ ANKLEBONE =        { 289, 46,  0,  true };

const int ORGAN_COUNT = 13;

Organ const BodyMap[] = {
    NOSE,
    WRITERSCRAMP,
    WRENCHEDANKLE,
    BREADBASKET,
    BUTTERFLIES,
    BROKENHEART,
    ADAMSAPPLE,
    WISHBONE,
    SPARERIBS,
    FUNNYBONE,
    WATERONKNEE,
    CHARLIEHORSE,
    ANKLEBONE
};