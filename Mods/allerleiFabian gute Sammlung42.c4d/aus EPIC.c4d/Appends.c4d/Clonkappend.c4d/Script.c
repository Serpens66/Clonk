/*-- Clonk trägt Dinge in der Hand --*/

#strict
#appendto CLNK
#include CLNK



/* Initialisierung */

protected func Initialize()
{
  if (GetID() == CLNK){ AddEffect("InventarCheck", this, 1, 1, this);}
  else {return(_inherited());}

  return(_inherited());
}

private func FxInventarCheckTimer()
{
  if(Contents() && (GetAction() eq "Walk" || GetAction() eq "HoldWalk" || GetAction() eq "Jump" || GetAction() eq "HoldJump")) 
    {
    //Richtungen
    var iDir         = GetDir(); if(iDir         == 0) iDir         = -1; else iDir         =  1;
    var iDirReversed = GetDir(); if(iDirReversed == 0) iDirReversed =  1; else iDirReversed = -1;
    // Maße und Anpassungen
    var iYOffset =                -1000;
    var iXOffset =                 4000;
    var iXWidth  =  iDirReversed * 1000;
    var iYHeight =                 1000;
    // Zusätzliches
    var iOverlayMode = GFXOV_MODE_Object;
    var szAction     = 0;
    
    // Sonderbehandlungen:
    if(GetID(Contents()) == AXE1) { iXWidth  =  iDir * 1000;                   }
    if(GetID(Contents()) == FLAG) { iXWidth  =  iDir * 1000;                   }
    if(GetID(Contents()) == BOW1) { iXOffset =  3000; iYOffset = -1500;        }
    if(GetID(Contents()) == LANC) { iYOffset = -15000;                         }
    if(GetID(Contents()) == ARCH) { iYOffset = -8000; }
    if(GetID(Contents()) == CABO) { iXOffset = -5000; }
    // Sonderbehandlungen mit Aktionen:
    if(GetID(Contents()) == CNDL) { iXOffset =   5500;  iYOffset = -6500; }
    if(GetID(Contents()) == BANR) { iYOffset = -15000; }



    
    var iR = GetR(Contents());
    SetR(0, Contents());
    
    // Legt die Objektgrafik auf den Clonk
    SetGraphics(0, 0, GetID(Contents()), 1, iOverlayMode, szAction, GetObjectBlitMode(Contents()), Contents());
    SetObjDrawTransform(iXWidth, 0, (GetDir()*2-1) * iXOffset, 0, iYHeight, iYOffset, 0, 1);
    
    
    // Hand über das Objekt legen
    SetGraphics(0, 0, CHD_, 2, GFXOV_MODE_Base);
    SetObjDrawTransform((GetDir()*2-1) * 1000, 0, (GetDir()*2-1) * 1000, 0, 1000, 00, 0, 2);

    if(this->~IsHidden()) {
      SetClrModulation(RGBa(205,205,205,170),this,1);
      SetClrModulation(RGBa(205,205,205,170),this,2);
    }
    if(GetAction() eq "Walk") SetAction("HoldWalk");
    if(GetAction() eq "Jump") SetAction("HoldJump");
    
    SetR(iR, Contents());
    }
  else
    {
    // Overlay Grafiken entfernen
    SetGraphics(0, 0, 0, 1);
    SetGraphics(0, 0, 0, 2);
    if(GetAction() eq "HoldWalk") SetAction("Walk");
    if(GetAction() eq "HoldJump") SetAction("Jump");
    }
}

private func FxInventarCheckStop()
{
    // Overlay Grafiken entfernen
    SetGraphics(0, 0, 0, 1);
    SetGraphics(0, 0, 0, 2);
    if(GetAction() eq "HoldWalk") SetAction("Walk");
    if(GetAction() eq "HoldJump") SetAction("Jump");
}
