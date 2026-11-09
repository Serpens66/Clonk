/*-- Alte Handelsstation --*/

#strict 3

//Den Script der Original Handelsstation einfügen:
#include HSSN

//Die Zwei Eingangstöne (Etwas älter):
func SoundOpen(...) { return(Sound("DoorOpen"));
}
func SoundClose(...) { return(Sound("DoorClose"));
}

//Das Menü (Damit das Icon richtig ist):
func ContainedUp(...) {
  if(!Equal(GetOwner(Par(nil)), own))
{
    Sound("Error");
    Message("Das ist kein Clonk von %s!",this(),GetPlayerName(own));
    return(nil);
}

  Sound("Click");
  caller=Par(nil);
  CreateMenu(AEHN,caller,nil,nil,nil,nil,nil,nil);
  AddMenuItem("Verkaufsliste erstellen","SellList",LIST,caller);
  AddMenuItem("Kaufen","Buy",_BUY,caller);
  AddMenuItem("Geld geben","Money",MONY,caller);
  AddMenuItem("Meldung an einen Spieler geben","Meldung",MELD,caller);
  AddMenuItem("Spam - Menü","Spams",SPAM,caller);
  AddMenuItem("Clonk transferieren","Transfer",TRAN,caller);
  return(1);
}