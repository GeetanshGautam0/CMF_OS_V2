#pragma once
#include "typedefs.cpp"

bool LeftShiftPressed       = false;
bool RightShiftPressed      = false;
bool CapsLock               = false;
bool CTRLPressed            = false;
bool ALTPressed             = false;

uint_8 lastKBScanCode       = 0;
bool KBToggle               = false;
bool PRINT_KB_INPUT         = true;
bool DEBUG_PRINT_SCAN_CODE  = false;

bool KB_SpecialKeys [3]     = {};
const int KB_SK_CTRL        = 0;
const int KB_SK_SHIFT       = 1;
const int KB_SK_ALT         = 2;

bool DIS_INPUT              = false;
bool SYS_ERR                = false;
