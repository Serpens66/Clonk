/*-- Alienjagd --*/

#strict 3
#include GOAL

public func IsFulfilled() {
  return(!GetLength(AliensLeft()));
}

protected func Activate(iPlayer) {
  return(MessageWindow(GetFulfillText(),iPlayer));
}

private func GetFulfillText() {
  var aAliens = AliensLeft();
  if(!GetLength(aAliens))
    return("$MsgAllDead$");
  var aliens = CreateArray();
  for(var obj in aAliens) {
    if(!FindIn(aliens,GetID(obj)))
      aliens[(GetLength(aliens) ?? 0)] = GetID(obj);
  }

  var text = "$MsgStillLeft$";
  for(var ID in aliens) {
    text = Format("%s|{{%i}} %s: %d",text,ID,GetName(nil,ID),ObjectCount(ID));
  }
  return(text);
}

private func AliensLeft()
{
	return(FindObjects(Find_Func("IsAlien")));
}

private func FindIn(array a, b) {
  for(var c in a)
    if(Equal(c, b))
      return(true);
      
  return(nil);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }
