/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#include <stdarg.h>
#include <texts/TypedTextDatabase.hpp>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>

uint16_t touchgfx::Font::getStringWidth(const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(TEXT_DIRECTION_LTR, text, pArg);
    va_end(pArg);
    return width;
}

uint16_t touchgfx::Font::getStringWidth(touchgfx::TextDirection textDirection, const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(textDirection, text, pArg);
    va_end(pArg);
    return width;
}

touchgfx::Unicode::UnicodeChar touchgfx::TextProvider::getNextLigature(TextDirection direction)
{
    if (fontGsubTable && nextCharacters.peekChar())
    {
        substituteGlyphs();
        if (nextCharacters.peekChar(1) == 0x093F) //Hindi I-matra
        {
            nextCharacters.replaceAt1(nextCharacters.peekChar());
            nextCharacters.replaceAt0(0x093F);
        }
    }
    return getNextChar();
}

void touchgfx::TextProvider::initializeInternal()
{
    fillInputBuffer();
}

void touchgfx::LCD::drawString(touchgfx::Rect widgetArea, const touchgfx::Rect& invalidatedArea, const touchgfx::LCD::StringVisuals& stringVisuals, const touchgfx::Unicode::UnicodeChar* format, ...)
{
    va_list pArg;
    va_start(pArg, format);
    drawStringLTR(widgetArea, invalidatedArea, stringVisuals, format, pArg);
    va_end(pArg);
}

//Default typed text database
extern const touchgfx::TypedText::TypedTextData* const typedTextDatabaseArray[];

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const touchgfx::Unicode::UnicodeChar texts_all_languages[] TEXT_LOCATION_FLASH_ATTRIBUTE =
{
    0x4c, 0x6f, 0x72, 0x65, 0x6d, 0x20, 0x69, 0x70, 0x73, 0x75, 0x6d, 0x20, 0x64, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x73, 0x69, 0x74, 0x20, 0x61, 0x6d, 0x65, 0x74, 0x2c, 0x20, 0x63, 0x6f, 0x6e, 0x73, 0x65, 0x74, 0x65, 0x74, 0x75, 0x72, 0x20, 0x73, 0x61, 0x64, 0x69, 0x70, 0x73, 0x63, 0x69, 0x6e, 0x67, 0x20, 0x65, 0x6c, 0x69, 0x74, 0x72, 0x2c, 0x20, 0x73, 0x65, 0x64, 0x20, 0x64, 0x69, 0x61, 0x6d, 0xa, 0x6e, 0x6f, 0x6e, 0x75, 0x6d, 0x79, 0x20, 0x65, 0x69, 0x72, 0x6d, 0x6f, 0x64, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x76, 0x69, 0x64, 0x75, 0x6e, 0x74, 0x20, 0x75, 0x74, 0x20, 0x6c, 0x61, 0x62, 0x6f, 0x72, 0x65, 0x20, 0x65, 0x74, 0x20, 0x64, 0x6f, 0x6c, 0x6f, 0x72, 0x65, 0x20, 0x6d, 0x61, 0x67, 0x6e, 0x61, 0x20, 0x61, 0x6c, 0x69, 0x71, 0x75, 0x79, 0x61, 0x6d, 0x20, 0xa, 0x65, 0x72, 0x61, 0x74, 0x2c, 0x20, 0x73, 0x65, 0x64, 0x20, 0x64, 0x69, 0x61, 0x6d, 0x20, 0x76, 0x6f, 0x6c, 0x75, 0x70, 0x74, 0x75, 0x61, 0x2e, 0x20, 0x41, 0x74, 0x20, 0x76, 0x65, 0x72, 0x6f, 0x20, 0x65, 0x6f, 0x73, 0x20, 0x65, 0x74, 0x20, 0x61, 0x63, 0x63, 0x75, 0x73, 0x61, 0x6d, 0x20, 0x65, 0x74, 0x20, 0x6a, 0x75, 0x73, 0x74, 0x6f, 0x20, 0x64, 0x75, 0x6f, 0x20, 0x64, 0x6f, 0x6c, 0x6f, 0x72, 0x65, 0x73, 0x20, 0xa, 0x65, 0x74, 0x20, 0x65, 0x61, 0x20, 0x72, 0x65, 0x62, 0x75, 0x6d, 0x2e, 0x20, 0x53, 0x74, 0x65, 0x74, 0x20, 0x63, 0x6c, 0x69, 0x74, 0x61, 0x20, 0x6b, 0x61, 0x73, 0x64, 0x20, 0x67, 0x75, 0x62, 0x65, 0x72, 0x67, 0x72, 0x65, 0x6e, 0x2c, 0x20, 0x6e, 0x6f, 0x20, 0x73, 0x65, 0x61, 0x20, 0x74, 0x61, 0x6b, 0x69, 0x6d, 0x61, 0x74, 0x61, 0x20, 0x73, 0x61, 0x6e, 0x63, 0x74, 0x75, 0x73, 0x20, 0xa, 0x65, 0x73, 0x74, 0x20, 0x4c, 0x6f, 0x72, 0x65, 0x6d, 0x20, 0x69, 0x70, 0x73, 0x75, 0x6d, 0x20, 0x64, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x73, 0x69, 0x74, 0x20, 0x61, 0x6d, 0x65, 0x74, 0x2e, 0x20, 0x4c, 0x6f, 0x72, 0x65, 0x6d, 0x20, 0x69, 0x70, 0x73, 0x75, 0x6d, 0x20, 0x64, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x73, 0x69, 0x74, 0x20, 0x61, 0x6d, 0x65, 0x74, 0x2e, 0x0, // @0 "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam?nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam ?erat, sed diam voluptua. At vero eos et accusam et justo duo dolores ?et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus ?est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet."
    0x53, 0x74, 0x61, 0x72, 0x74, 0x20, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x4f, 0x70, 0x65, 0x6e, 0x20, 0x4c, 0x69, 0x64, 0x20, 0x61, 0x72, 0x65, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x67, 0x69, 0x76, 0x65, 0x6e, 0x2e, 0xa, 0x50, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x20, 0x69, 0x66, 0x3a, 0xa, 0x2d, 0x20, 0x53, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x20, 0x74, 0x6f, 0x6f, 0x20, 0x68, 0x69, 0x67, 0x68, 0x2c, 0x20, 0x70, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x77, 0x61, 0x69, 0x74, 0x20, 0x74, 0x6f, 0x20, 0x63, 0x6f, 0x6f, 0x6c, 0x20, 0x64, 0x6f, 0x77, 0x6e, 0x2e, 0xa, 0x2d, 0x20, 0x4c, 0x69, 0x66, 0x74, 0x20, 0x6f, 0x72, 0x20, 0x6c, 0x69, 0x64, 0x20, 0x69, 0x73, 0x20, 0x63, 0x61, 0x6c, 0x69, 0x62, 0x72, 0x61, 0x74, 0x69, 0x6e, 0x67, 0xa, 0x2d, 0x20, 0x4c, 0x69, 0x64, 0x20, 0x69, 0x73, 0x20, 0x63, 0x75, 0x72, 0x72, 0x65, 0x6e, 0x74, 0x6c, 0x79, 0x20, 0x6d, 0x6f, 0x76, 0x69, 0x6e, 0x67, 0x0, // @327 "Start conditions for Open Lid are not given.?Please check if:?- Solder medium temperature too high, please wait to cool down.?- Lift or lid is calibrating?- Lid is currently moving"
    0x53, 0x74, 0x61, 0x72, 0x74, 0x20, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x52, 0x65, 0x66, 0x6c, 0x6f, 0x77, 0x20, 0x61, 0x72, 0x65, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x67, 0x69, 0x76, 0x65, 0x6e, 0x2e, 0xa, 0x50, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6b, 0x20, 0x69, 0x66, 0x3a, 0xa, 0x2d, 0x20, 0x53, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x66, 0x69, 0x6c, 0x65, 0x20, 0x73, 0x65, 0x6c, 0x65, 0x63, 0x74, 0x65, 0x64, 0xa, 0x2d, 0x20, 0x4c, 0x69, 0x64, 0x20, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x6c, 0x79, 0x20, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0x64, 0xa, 0x2d, 0x20, 0x43, 0x6f, 0x6f, 0x6c, 0x61, 0x6e, 0x74, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x20, 0x69, 0x73, 0x20, 0x75, 0x6e, 0x64, 0x65, 0x72, 0x20, 0x36, 0x30, 0xb0, 0x43, 0x20, 0x2f, 0x20, 0x31, 0x34, 0x30, 0xb0, 0x46, 0x0, // @508 "Start conditions for Reflow are not given.?Please check if:?- Solder profile selected?- Lid is properly closed?- Coolant temperature is under 60?C / 140?F"
    0x55, 0x6e, 0x64, 0x65, 0x72, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x20, 0x70, 0x72, 0x6f, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x6f, 0x72, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x20, 0x70, 0x72, 0x6f, 0x74, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x65, 0x64, 0x2e, 0xa, 0x50, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x6d, 0x61, 0x6e, 0x75, 0x61, 0x6c, 0x2e, 0x20, 0x44, 0x6f, 0x6e, 0x27, 0x74, 0x20, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x6f, 0x66, 0x66, 0x20, 0x56, 0x61, 0x70, 0x6f, 0x72, 0x20, 0x50, 0x68, 0x61, 0x73, 0x65, 0x20, 0x4f, 0x6e, 0x65, 0x2e, 0x0, // @663 "Under temperature protection or over temperature protection detected.?Please read manual. Don't turn off Vapor Phase One."
    0x41, 0x6e, 0x20, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x20, 0x6f, 0x63, 0x63, 0x75, 0x72, 0x65, 0x64, 0x20, 0x69, 0x6e, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6f, 0x72, 0x65, 0x20, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x6c, 0x65, 0x72, 0x2e, 0xa, 0x54, 0x68, 0x65, 0x20, 0x64, 0x69, 0x73, 0x70, 0x6c, 0x61, 0x79, 0x65, 0x64, 0x20, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x73, 0x20, 0x6d, 0x69, 0x67, 0x68, 0x74, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6e, 0x63, 0x6f, 0x72, 0x72, 0x65, 0x63, 0x74, 0x2e, 0x0, // @785 "An error occured in the communication with the core controller.?The displayed values might be incorrect."
    0x4c, 0x69, 0x64, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x63, 0x75, 0x72, 0x72, 0x65, 0x6e, 0x74, 0x20, 0x65, 0x76, 0x65, 0x6e, 0x74, 0x20, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x65, 0x64, 0x2e, 0x20, 0x4c, 0x69, 0x64, 0x20, 0x77, 0x61, 0x73, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x65, 0x64, 0x2c, 0x20, 0x70, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x72, 0x65, 0x6d, 0x6f, 0x76, 0x65, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x61, 0x67, 0x65, 0x2e, 0xa, 0x41, 0x6e, 0x64, 0x20, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x20, 0x63, 0x79, 0x63, 0x6c, 0x65, 0x20, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x2e, 0x0, // @890 "Lid over current event detected. Lid was blocked, please remove blockage.?And Power cycle device."
    0x4c, 0x69, 0x64, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x63, 0x75, 0x72, 0x72, 0x65, 0x6e, 0x74, 0x20, 0x65, 0x76, 0x65, 0x6e, 0x74, 0x20, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x65, 0x64, 0x2e, 0xa, 0x4c, 0x69, 0x64, 0x20, 0x77, 0x61, 0x73, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x65, 0x64, 0x2c, 0x20, 0x70, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x72, 0x65, 0x6d, 0x6f, 0x76, 0x65, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x61, 0x67, 0x65, 0x2e, 0xa, 0x41, 0x6e, 0x64, 0x20, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x20, 0x63, 0x79, 0x63, 0x6c, 0x65, 0x20, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x2e, 0x0, // @988 "Lid over current event detected.?Lid was blocked, please remove blockage.?And Power cycle device."
    0x48, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x20, 0x73, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x62, 0x6c, 0x65, 0x6d, 0x20, 0x64, 0x65, 0x74, 0x65, 0x63, 0x74, 0x65, 0x64, 0x2c, 0x20, 0x70, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x6d, 0x61, 0x6e, 0x75, 0x61, 0x6c, 0x2e, 0xa, 0x44, 0x6f, 0x6e, 0x27, 0x74, 0x20, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x6f, 0x66, 0x66, 0x20, 0x56, 0x61, 0x70, 0x6f, 0x72, 0x20, 0x50, 0x68, 0x61, 0x73, 0x65, 0x20, 0x4f, 0x6e, 0x65, 0x2e, 0x0, // @1086 "Heater temperature sensor problem detected, please read manual.?Don't turn off Vapor Phase One."
    0x53, 0x65, 0x6c, 0x66, 0x74, 0x65, 0x73, 0x74, 0x20, 0x69, 0x6e, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 0x65, 0x73, 0x73, 0x2e, 0x2e, 0x2e, 0xa, 0x54, 0x68, 0x69, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x74, 0x61, 0x6b, 0x65, 0x20, 0x66, 0x65, 0x77, 0x20, 0x6d, 0x69, 0x6e, 0x75, 0x74, 0x65, 0x73, 0x0, // @1182 "Selftest in progress...?This will take few minutes"
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0xe4, 0xf6, 0xfc, 0x2e, 0x2d, 0x5f, 0x2f, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x0, // @1233 "abcdefghijklmnopqrstuvwxyz???.-_/1234567890"
    0x43, 0x6f, 0x6f, 0x6c, 0x69, 0x6e, 0x67, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x3a, 0x20, 0x2, 0x20, 0xb0, 0x2, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0, // @1277 "Cooling System: <> ?<>     "
    0x47, 0x61, 0x6c, 0x64, 0x65, 0x6e, 0x3a, 0x20, 0x2, 0x20, 0xb0, 0x2, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0, // @1303 "Galden: <> ?<>        "
    0x53, 0x74, 0x61, 0x72, 0x74, 0x20, 0x52, 0x65, 0x66, 0x6c, 0x6f, 0x77, 0x20, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x6d, 0x65, 0x74, 0x0, // @1324 "Start Reflow condition not met"
    0x50, 0x43, 0x42, 0x3a, 0x20, 0x2, 0x20, 0xb0, 0x2, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0, // @1355 "PCB: <> ?<>        "
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xc4, 0xd6, 0xdc, 0x0, // @1373 "ABCDEFGHIJKLMNOPQRSTUVWXYZ???"
    0x4c, 0x69, 0x64, 0x20, 0x4f, 0x70, 0x65, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x6d, 0x65, 0x74, 0x0, // @1403 "Lid Open condition not met"
    0x43, 0x6f, 0x72, 0x65, 0x20, 0x43, 0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x0, // @1430 "Core Communication Error"
    0x4f, 0x70, 0x65, 0x6e, 0x20, 0x74, 0x65, 0x6d, 0x70, 0x65, 0x72, 0x61, 0x74, 0x75, 0x72, 0x65, 0x0, // @1455 "Open temperature"
    0x43, 0x6f, 0x6f, 0x6c, 0x61, 0x6e, 0x74, 0x20, 0x55, 0x54, 0x50, 0x2f, 0x4f, 0x54, 0x50, 0x0, // @1472 "Coolant UTP/OTP"
    0x50, 0x72, 0x6f, 0x66, 0x69, 0x6c, 0x65, 0x20, 0x61, 0x62, 0x6f, 0x72, 0x74, 0x65, 0x64, 0x0, // @1488 "Profile aborted"
    0x53, 0x74, 0x61, 0x72, 0x74, 0x20, 0x53, 0x65, 0x6c, 0x66, 0x74, 0x65, 0x73, 0x74, 0x3f, 0x0, // @1504 "Start Selftest?"
    0x54, 0x69, 0x6d, 0x65, 0x20, 0x69, 0x6e, 0x20, 0x53, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x73, 0x0, // @1520 "Time in Seconds"
    0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x31, 0x2e, 0x30, 0x2e, 0x36, 0x0, // @1536 "Version: 1.0.6"
    0x44, 0x69, 0x73, 0x70, 0x6c, 0x61, 0x79, 0x65, 0x64, 0x20, 0x75, 0x6e, 0x69, 0x74, 0x0, // @1551 "Displayed unit"
    0x53, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x66, 0x69, 0x6c, 0x65, 0x0, // @1566 "Solder profile"
    0x41, 0x72, 0x65, 0x20, 0x79, 0x6f, 0x75, 0x20, 0x73, 0x75, 0x72, 0x65, 0x3f, 0x0, // @1581 "Are you sure?"
    0x43, 0x61, 0x6e, 0x63, 0x65, 0x6c, 0x20, 0x52, 0x65, 0x66, 0x6c, 0x6f, 0x77, 0x0, // @1595 "Cancel Reflow"
    0x56, 0x61, 0x70, 0x6f, 0x72, 0x20, 0x50, 0x68, 0x61, 0x73, 0x65, 0x0, // @1609 "Vapor Phase"
    0x48, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20, 0x4f, 0x54, 0x50, 0x0, // @1621 "Heater OTP"
    0x4d, 0x6f, 0x64, 0x61, 0x6c, 0x54, 0x69, 0x74, 0x6c, 0x65, 0x0, // @1632 "ModalTitle"
    0x74, 0x65, 0x73, 0x74, 0x2e, 0x63, 0x73, 0x76, 0x0, // @1643 "test.csv"
    0x4e, 0x65, 0x77, 0x20, 0x54, 0x65, 0x78, 0x74, 0x0, // @1652 "New Text"
    0x53, 0x65, 0x6c, 0x66, 0x74, 0x65, 0x73, 0x74, 0x0, // @1661 "Selftest"
    0xb0, 0x2, 0x0, // @1670 "?<>"
    0x4c, 0x69, 0x64, 0x20, 0x4f, 0x43, 0x50, 0x0, // @1673 "Lid OCP"
    0x53, 0x44, 0x20, 0x63, 0x61, 0x72, 0x64, 0x0, // @1681 "SD card"
    0x37, 0x30, 0x0, // @1689 "70"
    0x33, 0x30, 0x0, // @1692 "30"
    0x43, 0x0 // @1695 "C"
};
TEXT_LOCATION_FLASH_PRAGMA
KEEP extern uint32_t const indicesGb[] TEXT_LOCATION_FLASH_ATTRIBUTE;

//array holding dynamically installed languages
struct TranslationHeader
{
    uint32_t offset_to_texts;
    uint32_t offset_to_indices;
    uint32_t offset_to_typedtext;
};
static const TranslationHeader* languagesArray[1] = { 0 };

//Compiled and linked in languages
static const uint32_t* const staticLanguageIndices[] =
{
    indicesGb
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
static const touchgfx::Unicode::UnicodeChar* currentLanguagePtr = 0;
static const uint32_t* currentLanguageIndices = 0;

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    const touchgfx::TypedText::TypedTextData* currentLanguageTypedText = 0;
    if (id < 1)
    {
        if (languagesArray[id] != 0)
        {
            //dynamic translation is added
            const TranslationHeader* translation = languagesArray[id];
            currentLanguagePtr = (const touchgfx::Unicode::UnicodeChar*)(((const uint8_t*)translation) + translation->offset_to_texts);
            currentLanguageIndices = (const uint32_t*)(((const uint8_t*)translation) + translation->offset_to_indices);
            currentLanguageTypedText = (const touchgfx::TypedText::TypedTextData*)(((const uint8_t*)translation) + translation->offset_to_typedtext);
        }
        else
        {
            //compiled and linked in languages
            currentLanguagePtr = texts_all_languages;
            currentLanguageIndices = staticLanguageIndices[id];
            currentLanguageTypedText = typedTextDatabaseArray[id];
        }
    }

    if (currentLanguageTypedText)
    {
        currentLanguage = id;
        touchgfx::TypedText::registerTypedTextDatabase(currentLanguageTypedText,
                                                       TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
    }
}

void touchgfx::Texts::setTranslation(touchgfx::LanguageId id, const void* translation)
{
    languagesArray[id] = (const TranslationHeader*)translation;
}

const touchgfx::Unicode::UnicodeChar* touchgfx::Texts::getText(TypedTextId id) const
{
    return &currentLanguagePtr[currentLanguageIndices[id]];
}

