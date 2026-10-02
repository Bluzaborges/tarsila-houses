#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <utility>

class Color {
private:
    Uint32 primary;
    Uint32 secondary;

    struct RGB {
        Uint8 r, g, b;
    };

    static RGB hexToRGB(unsigned int hex) {
        return {
            Uint8((hex >> 16) & 0xFF),
            Uint8((hex >> 8)  & 0xFF),
            Uint8(hex & 0xFF)
        };
    }

    static Uint32 mixColors(SDL_Surface* surface, Uint32 c1, Uint32 c2, float t) {
        Uint8 r1, g1, b1, r2, g2, b2;
        SDL_GetRGB(c1, surface->format, &r1, &g1, &b1);
        SDL_GetRGB(c2, surface->format, &r2, &g2, &b2);

        Uint8 r = Uint8(r1 + (r2 - r1) * t);
        Uint8 g = Uint8(g1 + (g2 - g1) * t);
        Uint8 b = Uint8(b1 + (b2 - b1) * t);

        return SDL_MapRGB(surface->format, r, g, b);
    }

public:
    explicit Color(Uint32 primary = 0,
                   Uint32 secondary = 0)
                 : primary(primary),
                   secondary(secondary) {}

    Color(SDL_Surface* surface,
          unsigned int hexPrimary,
          unsigned int hexSecondary)
    {
        RGB n = hexToRGB(hexPrimary);
        RGB d = hexToRGB(hexSecondary);

        primary = SDL_MapRGB(surface->format, n.r, n.g, n.b);
        secondary   = SDL_MapRGB(surface->format, d.r, d.g, d.b);
    }

    Uint32 getPrimary() const { return primary; }
    Uint32 getSecondary()   const { return secondary;   }

    static Color fromName(SDL_Surface* surface, const std::string& name) {

        std::string key = name;
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        static const std::unordered_map<std::string,
            std::pair<unsigned int, unsigned int>> colorTable = {
            { "aliceblue",            { 0xF0F8FF, 0x0487E2 } },
            { "antiquewhite",         { 0xFAEBD7, 0xE5D8C7 } },
            { "aqua",                 { 0x00FFFF, 0x0487E2 } },
            { "aquamarine",           { 0x7FFFD4, 0x30FFB6 } },
            { "azure",                { 0xF0FFFF, 0x3DFFFF } },
            { "beige",                { 0xF5F5DC, 0xF4F495 } },
            { "bisque",               { 0xFFE4C4, 0xFFC584 } },
            { "black",                { 0x000000, 0x000000 } },
            { "blanchedalmond",       { 0xFFEBCD, 0xFFCF8C } },
            { "blue",                 { 0x65C2F5, 0x0463CA } },
            { "blueviolet",           { 0x8A2BE2, 0x7D28CC } },
            { "brown",                { 0xAD782C, 0x7C501F } },
            { "burlywood",            { 0xDEB887, 0x7F5120 } },
            { "cadetblue",            { 0x5F9EA0, 0x477777 } },
            { "chartreuse",           { 0x7FFF00, 0x62C400 } },
            { "chocolate",            { 0xD2691E, 0x9E4D17 } },
            { "coral",                { 0xFF7F50, 0xBF5F3C } },
            { "cornflowerblue",       { 0x6495ED, 0x4B71B1 } },
            { "cornsilk",             { 0xFFF8DC, 0xBFBCA5 } },
            { "crimson",              { 0xDC143C, 0xA8102D } },
            { "cyan",                 { 0x8ED4B2, 0x41918E } },
            { "darkblue",             { 0x00008B, 0x000068 } },
            { "darkcyan",             { 0x008B8B, 0x006868 } },
            { "darkgoldenrod",        { 0xB8860B, 0x8A6408 } },
            { "darkgray",             { 0xA9A9A9, 0x7F7F7F } },
            { "darkgreen",            { 0x006400, 0x004B00 } },
            { "darkkhaki",            { 0xBDB76B, 0x8E8950 } },
            { "darkmagenta",          { 0x8B008B, 0x680068 } },
            { "darkolivegreen",       { 0x556B2F, 0x415124 } },
            { "darkorange",           { 0xFF8C00, 0xBF6900 } },
            { "darkorchid",           { 0x9932CC, 0x732599 } },
            { "darkred",              { 0x8B0000, 0x680000 } },
            { "darksalmon",           { 0xE9967A, 0xAF705C } },
            { "darkseagreen",         { 0x8FBC8F, 0x6B8D6B } },
            { "darkslateblue",        { 0x483D8B, 0x362E68 } },
            { "darkslategray",        { 0x2F4F4F, 0x233B3B } },
            { "darkturquoise",        { 0x00CED1, 0x009B9C } },
            { "darkviolet",           { 0x9400D3, 0x6F009F } },
            { "deeppink",             { 0xFF1493, 0xBF0F6F } },
            { "deepskyblue",          { 0x00BFFF, 0x008FBF } },
            { "dimgray",              { 0x696969, 0x4F4F4F } },
            { "dodgerblue",           { 0x1E90FF, 0x166CC0 } },
            { "firebrick",            { 0xB22222, 0x861919 } },
            { "floralwhite",          { 0xFFFAF0, 0xBFBBA8 } },
            { "forestgreen",          { 0x228B22, 0x196919 } },
            { "fuchsia",              { 0xFF00FF, 0xBF00BF } },
            { "gainsboro",            { 0xDCDCDC, 0xA5A5A5 } },
            { "ghostwhite",           { 0xF8F8FF, 0xBABABF } },
            { "gold",                 { 0xFFD700, 0xBFA200 } },
            { "goldenrod",            { 0xDAA520, 0xA27D18 } },
            { "gray",                 { 0x808080, 0x606060 } },
            { "green",                { 0xB9D532, 0x415F2D } },
            { "greenyellow",          { 0xADFF2F, 0x82BF23 } },
            { "honeydew",             { 0xF0FFF0, 0xB3BFB3 } },
            { "hotpink",              { 0xFF69B4, 0xBF4E86 } },
            { "indianred",            { 0xCD5C5C, 0x9A4545 } },
            { "indigo",               { 0x4B0082, 0x380061 } },
            { "ivory",                { 0xFFFFF0, 0xBFBFB3 } },
            { "khaki",                { 0xF0E68C, 0xB3AA69 } },
            { "lavender",             { 0xE6E6FA, 0xACACBC } },
            { "lavenderblush",        { 0xFFF0F5, 0xBFB4B7 } },
            { "lawngreen",            { 0x7CFC00, 0x5DBE00 } },
            { "lemonchiffon",         { 0xFFFACD, 0xBFBCA0 } },
            { "lightblue",            { 0xADD8E6, 0x82A2AC } },
            { "lightcoral",           { 0xF08080, 0xB46060 } },
            { "lightcyan",            { 0xE0FFFF, 0xA8BFBF } },
            { "lightgoldenrodyellow", { 0xFAFAD2, 0xBDBDA0 } },
            { "lightgray",            { 0xD3D3D3, 0x9F9F9F } },
            { "lightgreen",           { 0x90EE90, 0x6CB26C } },
            { "lightpink",            { 0xFFB6C1, 0xBF8890 } },
            { "lightsalmon",          { 0xFFA07A, 0xBF785C } },
            { "lightseagreen",        { 0x20B2AA, 0x188580 } },
            { "lightskyblue",         { 0x87CEFA, 0x659BBA } },
            { "lightslategray",       { 0x778899, 0x5A6673 } },
            { "lightsteelblue",       { 0xB0C4DE, 0x8493A6 } },
            { "lightyellow",          { 0xFFFFE0, 0xBFBFA8 } },
            { "lime",                 { 0x00FF00, 0x00BF00 } },
            { "limegreen",            { 0x32CD32, 0x259A25 } },
            { "linen",                { 0xFAF0E6, 0xBDB3AC } },
            { "magenta",              { 0xFF00FF, 0xBF00BF } },
            { "maroon",               { 0x800000, 0x600000 } },
            { "mediumaquamarine",     { 0x66CDAA, 0x4D9A80 } },
            { "mediumblue",           { 0x0000CD, 0x00009A } },
            { "mediumorchid",         { 0xBA55D3, 0x8C40A0 } },
            { "mediumpurple",         { 0x9370DB, 0x6F54A5 } },
            { "mediumseagreen",       { 0x3CB371, 0x2D8655 } },
            { "mediumslateblue",      { 0x7B68EE, 0x5D4EB2 } },
            { "mediumspringgreen",    { 0x00FA9A, 0x00BA73 } },
            { "mediumturquoise",      { 0x48D1CC, 0x369D99 } },
            { "mediumvioletred",      { 0xC71585, 0x951064 } },
            { "midnightblue",         { 0x191970, 0x121254 } },
            { "mintcream",            { 0xF5FFFA, 0xB8BFB9 } },
            { "mistyrose",            { 0xFFE4E1, 0xBFB0AC } },
            { "moccasin",             { 0xFFE4B5, 0xBFAC88 } },
            { "navajowhite",          { 0xFFDEAD, 0xBFAB82 } },
            { "navy",                 { 0x000080, 0x000060 } },
            { "oldlace",              { 0xFDF5E6, 0xBEB7AC } },
            { "olive",                { 0x808000, 0x606000 } },
            { "olivedrab",            { 0x6B8E23, 0x506B1A } },
            { "orange",               { 0xF88C34, 0xC3521B } },
            { "orangered",            { 0xFF4500, 0xBF3400 } },
            { "orchid",               { 0xDA70D6, 0xA352A0 } },
            { "palegoldenrod",        { 0xEEE8AA, 0xB2AF80 } },
            { "palegreen",            { 0x98FB98, 0x72BC72 } },
            { "paleturquoise",        { 0xAFEEEE, 0x83B2B2 } },
            { "palevioletred",        { 0xDB7093, 0xA8576F } },
            { "papayawhip",           { 0xFFEFD5, 0xBFB39F } },
            { "peachpuff",            { 0xFFDAB9, 0xBF9F8A } },
            { "peru",                 { 0xCD853F, 0x9A632F } },
            { "pink",                 { 0xF6808B, 0xC33959 } },
            { "plum",                 { 0xDDA0DD, 0xA879A8 } },
            { "powderblue",           { 0xB0E0E6, 0x84A8AC } },
            { "purple",               { 0x800080, 0x600060 } },
            { "rebeccapurple",        { 0x663399, 0x4D2673 } },
            { "red",                  { 0xFF0000, 0xBF0000 } },
            { "rosybrown",            { 0xBC8F8F, 0x8D6B6B } },
            { "royalblue",            { 0x4169E1, 0x3150A9 } },
            { "saddlebrown",          { 0x8B4513, 0x68340E } },
            { "salmon",               { 0xFA8072, 0xBB6055 } },
            { "sandybrown",           { 0xF4A460, 0xB97C48 } },
            { "seagreen",             { 0x2E8B57, 0x236943 } },
            { "seashell",             { 0xFFF5EE, 0xBFB9B2 } },
            { "sienna",               { 0xA0522D, 0x784020 } },
            { "silver",               { 0xC0C0C0, 0x909090 } },
            { "skyblue",              { 0x87CEEB, 0x659BBA } },
            { "slateblue",            { 0x6A5ACD, 0x50449A } },
            { "slategray",            { 0x708090, 0x546067 } },
            { "snow",                 { 0xFFFAFA, 0xBFBABA } },
            { "springgreen",          { 0x00FF7F, 0x00BF5F } },
            { "steelblue",            { 0x4682B4, 0x355F88 } },
            { "tan",                  { 0xD2B48C, 0x9E8769 } },
            { "teal",                 { 0x008080, 0x006060 } },
            { "thistle",              { 0xD8BFD8, 0xA391A3 } },
            { "tomato",               { 0xFF6347, 0xBF4A35 } },
            { "turquoise",            { 0x40E0D0, 0x30A89C } },
            { "violet",               { 0xEE82EE, 0xB261B2 } },
            { "wheat",                { 0xF5DEB3, 0xB7A986 } },
            { "white",                { 0xFFFFFF, 0xBFBFBF } },
            { "whitesmoke",           { 0xF5F5F5, 0xB8B8B8 } },
            { "yellow",               { 0xFFCD00, 0xDBAF00 } },
            { "yellowgreen",          { 0x9ACD32, 0x739A25 } }
        };

        auto colorPair = colorTable.find(key);
        if (colorPair != colorTable.end())
            return Color(surface, colorPair->second.first, colorPair->second.second);

        return Color(surface, 0xC8C8C8, 0x646464);
    }

    Uint32 getVariant(SDL_Surface* surface, float t) const {
        return mixColors(surface, primary, secondary, t);
    }
};

#endif
