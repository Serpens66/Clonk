/*-- Alte Handelsstation --*/

#strict

//Den Script der Original Handelsstation einfügen:
#include HSSN

//Die Zwei Eingangstöne (Etwas älter):
SoundOpen: return(Sound("DoorOpen"));
SoundClose: return(Sound("DoorClose"));

//Das Menü (Damit das Icon richtig ist):
ContainedUp:
  if(GetOwner(Par(0))!=own)
{
    Sound("Error");
    Message("Das ist kein Clonk von %s!",this(),GetPlayerName(own));
    return(0);
}

  Sound("Click");
  caller=Par(0);
  CreateMenu(AEHN,caller,0,0,0,0,0,0);
  AddMenuItem("Verkaufsliste erstellen","SellList",LIST,caller);
  AddMenuItem("Kaufen","Buy",_BUY,caller);
  AddMenuItem("Geld geben","Money",MONY,caller);
  AddMenuItem("Meldung an einen Spieler geben","Meldung",MELD,caller);
  AddMenuItem("Spam - Menü","Spams",SPAM,caller);
  AddMenuItem("Clonk transferieren","Transfer",TRAN,caller);
  return(1);