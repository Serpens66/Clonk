/*-- Zeitanzeige --*/

#strict 3

local iTimer,farbe,zinsen, maxsekunden,kontrollobjekt;

func Initialize() {
  SetVisibility(VIS_All);
  SetPosition(50, 100, this());
  Local(nil) = nil;
  Local(1) = nil;
  return(1);
}

func DieSettings(imaxsekunden,ifarbe,izinsen, ikontrollobjekt)
{
farbe= ifarbe;
zinsen = izinsen;
kontrollobjekt = ikontrollobjekt;
}

protected func Tick() // Zeit wird jetzt über spielregel variabel bestimmt
{
if ((HashGet(hashSpielRegeln, "Barriere") ?? 0) >= 0)
    {HashPut(hashSpielRegeln, "Barriere", (HashGet(hashSpielRegeln, "Barriere") ?? 0)-1); Tack(HashGet(hashSpielRegeln, "Barriere"),farbe,zinsen);}
else kontrollobjekt ->~ Zeitvorbei();
}

protected func Tack(int iSeconds, int dwColor, int iZinsen) {
  iSeconds ??= 0;
  dwColor ??= 0;
  iZinsen ??= 0;
  Message("@<c %x>%.2d:%.2d", this(), dwColor, (iSeconds ?? 0)/60, (iSeconds ?? 0)%60);
}
