#include "ClavierHandler.h"

int ClavierHandler::ObtenirDerniereToucheAppuyee()
{
   return 0;
}

eClavierNumerique ClavierHandler::TesterClavierNumerique()
{
   if (ClavierNumerique_)
   {
      if(GetAsyncKeyState(VK_NUMPAD0))
      {
         return NUMPAD0;
      }
      if(GetAsyncKeyState(VK_NUMPAD1))
      {
         return NUMPAD1;
      }
      if(GetAsyncKeyState(VK_NUMPAD2))
      {
         return NUMPAD2;
      }
      if(GetAsyncKeyState(VK_NUMPAD3))
      {
         return NUMPAD3;
      }
      if(GetAsyncKeyState(VK_NUMPAD4))
      {
         return NUMPAD4;
      }
      if(GetAsyncKeyState(VK_NUMPAD5))
      {
         return NUMPAD5;
      }
      if(GetAsyncKeyState(VK_NUMPAD6))
      {
         return NUMPAD6;
      }
      if(GetAsyncKeyState(VK_NUMPAD7))
      {
         return NUMPAD7;
      }
      if(GetAsyncKeyState(VK_NUMPAD8))
      {
         return NUMPAD8;
      }
      if(GetAsyncKeyState(VK_NUMPAD9))
      {
         return NUMPAD9;
      }
      if(GetAsyncKeyState(VK_DECIMAL))
      {
         return DECIMALPoint;
      }
      if(GetAsyncKeyState(VK_RETURN))
      {
         return ENTREE;
      }
      if(GetAsyncKeyState(VK_ADD))
      {
         return PLUS;
      }
      if(GetAsyncKeyState(VK_SUBTRACT))
      {
         return MOINS;
      }
      if(GetAsyncKeyState(VK_MULTIPLY))
      {
         return MULT;
      }
      return NUMPADNull;
   }
}