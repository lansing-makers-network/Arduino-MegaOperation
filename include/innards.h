struct Organ {
    int start;
    int len;
    int sensor;
};

struct Organ NOSE =            {   0, 16, -1 };
struct Organ ANKLEBONE =       {  16, 47,  0 };
struct Organ CHARLIEHORSE =    {  63, 26,  1 };
struct Organ WATERONKNEE =     {  89, 26,  2 };
struct Organ FUNNYBONE =       { 115, 23,  3 };
struct Organ SPARERIBS =       { 138, 31,  4 };
struct Organ WISHBONE =        { 169, 25,  5 };
struct Organ ADAMSAPPLE =      { 194, 21,  6 };
struct Organ BROKENHEART =     { 215, 16,  7 };
struct Organ BUTTERFLIES =     { 231, 25,  8 };
struct Organ BREADBASKET =     { 256, 23,  9 };
struct Organ WRENCHEDANKLE =   { 279, 29, 10 };
struct Organ WRITERSCRAMP =    { 308, 29, 11 };

Organ const BodyMap[] = {
    NOSE,
    ANKLEBONE,
    CHARLIEHORSE,
    WATERONKNEE,
    FUNNYBONE,
    SPARERIBS,
    WISHBONE,
    ADAMSAPPLE,
    BROKENHEART,
    BUTTERFLIES,
    BREADBASKET,
    WRENCHEDANKLE,
    WRITERSCRAMP 
};

const int ORGAN_COUNT = 13;
const int ORGAN_PIXEL_OFFSET = 16;
const int ORGAN_PIXEL_COUNT = (337-16);
const int TOTAL_PIXEL_COUNT = 337;
