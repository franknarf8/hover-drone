#ifndef CLAVIERHANDLER_H
#define CLAVIERHANDLER_H
#include <Windows.h>
//#include <conio.h> Pour kbhit()
#include <vector>

enum eClavierNumerique
{
   NUMPAD0 = VK_NUMPAD0, NUMPAD1 = VK_NUMPAD1, NUMPAD2 = VK_NUMPAD2, NUMPAD3 = VK_NUMPAD3, NUMPAD4 = VK_NUMPAD4,
   NUMPAD5 = VK_NUMPAD5, NUMPAD6 = VK_NUMPAD6, NUMPAD7 = VK_NUMPAD7, NUMPAD8 = VK_NUMPAD8, NUMPAD9 = VK_NUMPAD9,
   DECIMALPoint = VK_DECIMAL, ENTREE  = VK_RETURN , PLUS    = VK_ADD    , MOINS  = VK_SUBTRACT, MULT    = VK_MULTIPLY,
   NUMPADNull = 0
};

class ClavierHandler
{
   std::vector<bool> TouchesActivesClavierNumerique_;
   bool ClavierNumerique_,
        Lettres_,
        F_, //F1, F2...
        Controles_, // CTRL, ALT, TAB...
        Fleches_; // Fleches et Suppr, insert, Home, FIN...
public:
   ClavierHandler(bool ClavierNumerique = false,bool Lettres = false, bool F = false, bool Controles = false, bool Fleches = false)
      : ClavierNumerique_(ClavierNumerique), Lettres_(Lettres), F_(F), Controles_(Controles), Fleches_(Fleches){}

   int ObtenirDerniereToucheAppuyee();
   eClavierNumerique TesterClavierNumerique();

};
/*
Symbolic    constant	Hexadecimal value	Mouse or keyboard equivalent
VK_LBUTTON	   01	Left mouse button
VK_RBUTTON	   02	Right mouse button
VK_CANCEL	   03	Control-break processing
VK_MBUTTON	   04	Middle mouse button on a three-button mouse 0507	Undefined
VK_BACK	      08	BACKSPACE key
VK_TAB	      09	TAB key 
               0A-0B	Undefined
VK_CLEAR	      0C	CLEAR key
VK_RETURN	   0D	ENTER key 
               0E-0F	Undefined
VK_SHIFT	      10	SHIFT key
VK_CONTROL	   11	CTRL key
VK_MENU	      12	ALT key
VK_PAUSE	      13	PAUSE key
VK_CAPITAL	   14	CAPS LOCK key 
               15-19	Reserved for Kanji systems 1A	Undefined
VK_ESCAPE	   1B	ESC key
               1C-1F	Reserved for Kanji systems
VK_SPACE	      20	SPACEBAR
VK_PRIOR	      21	PAGE UP key
VK_NEXT	      22	PAGE DOWN key
VK_END	      23	END key
VK_HOME	      24	HOME key
VK_LEFT	      25	LEFT ARROW key
VK_UP	26	      UP ARROW key
VK_RIGHT	      27	RIGHT ARROW key
VK_DOWN	      28	DOWN ARROW key
VK_SELECT	   29	SELECT key
 	            2A	Specific to original equipment manufacturer
VK_EXECUTE	   2B	EXECUTE key
VK_SNAPSHOT	   2C	PRINT SCREEN key
VK_INSERT	   2D	INS key
VK_DELETE	   2E	DEL key
VK_HELP	      2F	HELP key
 	            3A-40	Undefined
VK_LWIN	      5B	Left Windows key on a Microsoft Natural Keyboard
VK_RWIN	      5C	Right Windows key on a Microsoft Natural Keyboard
VK_APPS	      5D	Applications key on a Microsoft Natural Keyboard
 	            5E-5F	Undefined
VK_NUMPAD0	   60	Numeric keypad 0 key
VK_NUMPAD1	   61	Numeric keypad 1 key
VK_NUMPAD2	   62	Numeric keypad 2 key
VK_NUMPAD3	   63	Numeric keypad 3 key
VK_NUMPAD4	   64	Numeric keypad 4 key
VK_NUMPAD5	   65	Numeric keypad 5 key
VK_NUMPAD6	   66	Numeric keypad 6 key
VK_NUMPAD7	   67	Numeric keypad 7 key
VK_NUMPAD8	   68	Numeric keypad 8 key
VK_NUMPAD9	   69	Numeric keypad 9 key
VK_MULTIPLY	   6A	Multiply key
VK_ADD	      6B	Add key
VK_SEPARATOR	6C	Separator key
VK_SUBTRACT	   6D	Subtract key
VK_DECIMAL	   6E	Decimal key
VK_DIVIDE	   6F	Divide key
VK_F1	         70	F1 key
VK_F2	         71	F2 key
VK_F3	         72	F3 key
VK_F4	         73	F4 key
VK_F5	         74	F5 key
VK_F6	         75	F6 key
VK_F7	         76	F7 key
VK_F8	         77	F8 key
VK_F9	         78	F9 key
VK_F10	      79	F10 key
VK_F11	      7A	F11 key
VK_F12	      7B	F12 key
VK_F13	      7C	F13 key
VK_F14	      7D	F14 key
VK_F15	      7E	F15 key
VK_F16	      7F	F16 key
VK_F17	      80H	F17 key
VK_F18	      81H	F18 key
VK_F19	      82H	F19 key
VK_F20	      83H	F20 key
VK_F21	      84H	F21 key
VK_F22	      85H	F22 key

(PPC only) Key used to lock device.
VK_F23	      86H	F23 key
VK_F24	      87H	F24 key
 	            88-8F	Unassigned
VK_NUMLOCK	   90	NUM LOCK key
VK_SCROLL	   91	SCROLL LOCK key
VK_LSHIFT	   0xA0	Left SHIFT
VK_RSHIFT	   0xA1	Right SHIFT
VK_LCONTROL	   0xA2	Left CTRL
VK_RCONTROL	   0xA3	Right CTRL
VK_LMENU	      0xA4	Left ALT
VK_RMENU	      0xA5	Right ALT
 	            BA-C0	Specific to original equipment manufacturer; reserved. See following tables.
 	            C1-DA	Unassigned
 	            DB-E2	Specific to original equipment manufacturer; reserved. See following tables.
 	            E3 � E4	Specific to original equipment manufacturer
 	            E5	Unassigned
 	            E6	Specific to original equipment manufacturer
VK_PACKET	   E7	Used to pass Unicode characters as if they were keystrokes. If VK_PACKET is used with SendInput, then the Unicode character to be delivered should be placed into the lower 16 bits of the scan code. If a keyboard message is removed from the message queue and the virtual key is VK_PACKET, then the Unicode character will be the upper 16 bits of the lparam.
 	            E8	Unassigned
 	            E9-F5	Specific to original equipment manufacturer
VK_ATTN	      F6	ATTN key
VK_CRSEL	      F7	CRSEL key
VK_EXSEL	      F8	EXSEL key
VK_EREOF	      F9	Erase EOF key
VK_PLAY	      FA	PLAY key
VK_ZOOM	      FB	ZOOM key
VK_NONAME	   FC	Reserved for future use
VK_PA1	      FD	PA1 key
VK_OEM_CLEAR	FE	CLEAR key
VK_KEYLOCK	   F22	Key used to lock device
*/
#endif
