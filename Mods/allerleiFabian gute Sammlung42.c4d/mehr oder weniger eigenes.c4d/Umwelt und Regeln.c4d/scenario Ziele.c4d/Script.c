/*-- szenario ziele --*/

#strict 3


func Initialize()
{
SetPosition(1,1);
}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
// public func IsChooseable2() { return(1); } // wirkt nur, wenn direkt in szenario.txt eingestellt!