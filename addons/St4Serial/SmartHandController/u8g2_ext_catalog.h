#pragma once
#include <U8g2lib.h>
#include "Catalog.h"
#include "u8g2_ext_event.h"
unsigned short ext_UserInterfaceCatalog(u8g2_t *u8g2, Pad *extPad, const char *title, unsigned short start_pos, Catalog cat);