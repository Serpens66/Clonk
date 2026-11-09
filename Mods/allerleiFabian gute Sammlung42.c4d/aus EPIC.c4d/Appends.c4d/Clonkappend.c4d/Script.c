/*-- Clonk trägt Dinge in der Hand --*/

#strict 3
#appendto CLNK
#include CLNK



/* Initialisierung */

protected func Initialize()
{
  if (Equal(GetID(), CLNK)){ AddEffect("InventarCheck", this, 1, 1, this);}
  else {return(_inherited());}

  return(_inherited());
}

private func FxInventarCheckTimer()
{
  if(And(Contents(), (Or(Or(Or(!!SEqual(GetAction(), "Walk"), !!SEqual(GetAction(), "HoldWalk")), !!SEqual(GetAction(), "Jump")), !!SEqual(GetAction(), "HoldJump")))))
    {
    //Richtungen
    var iDir         = GetDir(); if(Equal(iDir, nil)) iDir         = -1; else iDir         =  1;
    var iDirReversed = GetDir(); if(Equal(iDirReversed, nil)) iDirReversed =  1; else iDirReversed = -1;
    // Maße und Anpassungen
    var iYOffset =                -1000;
    var iXOffset =                 4000;
    var iXWidth  =  (iDirReversed ?? 0) * 1000;
    var iYHeight =                 1000;
    // Zusätzliches
    var iOverlayMode = GFXOV_MODE_Object;
    var szAction     = nil;
    
    // Sonderbehandlungen:
    if(Equal(GetID(Contents()), AXE1)) { iXWidth  =  (iDir ?? 0) * 1000;                   }
    if(Equal(GetID(Contents()), FLAG)) { iXWidth  =  (iDir ?? 0) * 1000;                   }
    if(Equal(GetID(Contents()), BOW1)) { iXOffset =  3000; iYOffset = -1500;        }
    if(Equal(GetID(Contents()), LANC)) { iYOffset = -15000;                         }
    if(Equal(GetID(Contents()), ARCH)) { iYOffset = -8000; }
    if(Equal(GetID(Contents()), CABO)) { iXOffset = -5000; }
    // Sonderbehandlungen mit Aktionen:
    if(Equal(GetID(Contents()), CNDL)) { iXOffset =   5500;  iYOffset = -6500; }
    if(Equal(GetID(Contents()), BANR)) { iYOffset = -15000; }



    
    var iR = GetR(Contents());
    SetR(nil, Contents());
    
    // Legt die Objektgrafik auf den Clonk
    SetGraphics(nil, nil, GetID(Contents()), 1, iOverlayMode, szAction, GetObjectBlitMode(Contents()), Contents());
    SetObjDrawTransform(iXWidth, nil, ((GetDir() ?? 0)*2-1) * (iXOffset ?? 0), nil, iYHeight, iYOffset, nil, 1);
    
    
    // Hand über das Objekt legen
    SetGraphics(nil, nil, CHD_, 2, GFXOV_MODE_Base);
    SetObjDrawTransform(((GetDir() ?? 0)*2-1) * 1000, nil, ((GetDir() ?? 0)*2-1) * 1000, nil, 1000, 00, nil, 2);

    if(this->~IsHidden()) {
      SetClrModulation(RGBa(205,205,205,170),this,1);
      SetClrModulation(RGBa(205,205,205,170),this,2);
    }
    if(!!SEqual(GetAction(), "Walk")) SetAction("HoldWalk");
    if(!!SEqual(GetAction(), "Jump")) SetAction("HoldJump");
    
    SetR(iR, Contents());
    }
  else
    {
    // Overlay Grafiken entfernen
    SetGraphics(nil, nil, nil, 1);
    SetGraphics(nil, nil, nil, 2);
    if(!!SEqual(GetAction(), "HoldWalk")) SetAction("Walk");
    if(!!SEqual(GetAction(), "HoldJump")) SetAction("Jump");
    }
}

private func FxInventarCheckStop()
{
    // Overlay Grafiken entfernen
    SetGraphics(nil, nil, nil, 1);
    SetGraphics(nil, nil, nil, 2);
    if(!!SEqual(GetAction(), "HoldWalk")) SetAction("Walk");
    if(!!SEqual(GetAction(), "HoldJump")) SetAction("Jump");
}
