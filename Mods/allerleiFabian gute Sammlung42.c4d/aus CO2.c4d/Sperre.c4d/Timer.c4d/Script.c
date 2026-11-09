/*-- Zeitanzeige --*/

#strict

local iTimer,farbe,zinsen, maxsekunden,kontrollobjekt;

func Initialize() {
  SetVisibility(VIS_All());
  SetPosition(50, 100, this());
  Local(0) = 0;
  Local(1) = 0;
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
if (HashGet(hashSpielRegeln, "Barriere") >= 0) 
    {HashPut(hashSpielRegeln, "Barriere", HashGet(hashSpielRegeln, "Barriere")-1); Tack(HashGet(hashSpielRegeln, "Barriere"),farbe,zinsen);}
else kontrollobjekt ->~ Zeitvorbei();
}

protected func Tack(int iSeconds, int dwColor, int iZinsen) {
  Message("@<c %x>%.2d:%.2d", this(), dwColor, iSeconds/60, iSeconds%60);
}
