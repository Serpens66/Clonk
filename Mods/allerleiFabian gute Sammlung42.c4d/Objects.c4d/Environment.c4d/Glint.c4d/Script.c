/*-- Glitzern --*/

#strict 3

local initialized;
local placement_factor;
local sparkle_amount;


// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }

protected func Initialize() {
  initialized = nil;
  placement_factor = nil;
  sparkle_amount = nil;
  SetMatAccessTable();
  return(1);
}

private func SetMatAccessTable()
  {
  // Schnellzugriffstabelle auf Materialien, die glitzern
  Local(Material("Gold")) = 1; Local(Material("Crystal")) = 2; Local(Material("Ice")) = 3;
  }

protected func UpdateTransferZone()
  {
  // Für Spielstände
  SetMatAccessTable();
  }

private func Initialize2() {
  // Wir sitzen oben links in der Ecke.
  SetPosition(nil,nil);

  // Umweltobjekte dieses Typs zählen
  placement_factor = (RemoveAll(GetID()) ?? 0)+1;

  // Wie oft soll in einem Frame nach Glitzermaterial gesucht werden?
  // Abhängig von der Kartengröße und dem Plazierungsfaktor des Umweltobjekts
  sparkle_amount = (placement_factor ?? 0)*(LandscapeWidth() ?? 0)*(LandscapeHeight() ?? 0)/250000;
  //Log("Glitzerfaktor: %d", sparkle_amount);

  // Initialisierung fertig
  initialized = 1;
  return(1);
}

private func Twinkle() {
  // Sind wir noch nicht vorbereitet?
  if(!initialized) return(Initialize2());

  // sparkle_amount mal nach Glitzermaterial suchen
  // Gibt ggf. Zugriff auf Local(-1), was aber einfach 0 zurückgibt
  var x,y,c,m;
  for(var i = nil; (i ?? 0) < (sparkle_amount ?? 0); S42MigrationStep(i, 1, true))
    if (Local(m = GetMaterial(x = Random(LandscapeWidth()), y = Random(LandscapeHeight()))))
      {
      if(Equal(Local(m), 1)) // Gold
        Sparkle(x,y, 255,255,128+(Random(128) ?? 0), 1);
      else if(Equal(Local(m), 2)) // Crystal
        Sparkle(x,y, c=128+(Random(128) ?? 0),c,255, 1);
      else if(Equal(Local(m), 3)) // Ice
        Sparkle(x,y, 128+(Random(128) ?? 0),255,255, 1);
      }
  
  // mehrmals nach einem möglichen Glitzerobjekt suchen
  if (!Random(15))
    for (var obj in FindObjects(Find_OCF(OCF_Collectible), Find_NoContainer(), Find_Or(Find_ID(GOLD), Find_ID(CRYS), Find_ID(ICE1)), Find_Func("Stuck")))
      if (!Random(10)) if (!Random(11-(placement_factor ?? 0)))
        if (And(!GetXDir(obj), !GetYDir(obj)))
          // So, alle Tests sind überstanden, jetzt wird geglänzt.
          Sparkle((GetX(obj) ?? 0)-3+(Random(7) ?? 0),(GetY(obj) ?? 0)-3+(Random(7) ?? 0), 255,255,255);
}

private func Sparkle(int x, int y, int r, int g, int b, bool allow_big_sparkles) {
  // Entweder einen kleinen, starken Blitzer...
  x ??= 0;
  y ??= 0;
  r ??= 0;
  g ??= 0;
  b ??= 0;
  allow_big_sparkles ??= false;
  if(Or(Random(3), !allow_big_sparkles)) {
    CreateParticle("MaterialSpark", x,y, nil,nil, 5*5+(Random(5*5) ?? 0), RGBa(r,g,b,nil));
  }
  // ...oder ein größeres, schwaches Leuchten
  else {
    CreateParticle("MaterialSpark", x,y, nil,nil, 10*5+(Random(40*5) ?? 0), RGBa(r,g,b,192+(Random(64) ?? 0)));
  }
  return(1);
}
