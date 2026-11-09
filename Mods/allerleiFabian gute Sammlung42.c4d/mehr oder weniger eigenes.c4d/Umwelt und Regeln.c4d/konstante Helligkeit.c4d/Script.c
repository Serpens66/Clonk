/*-- konstante Helligkeit --*/

#strict


func Initialize()
{
SetPosition(1,1);
Initialized();
}

func Initialized()
{
// es wird die Tageszeit einfach auf Mittags gesetzt und die Helligkeit dazu hergestellt. Danach wird die Helligkeit nicht mehr verändert (außer zurzeit himmel), solange das KOHE objekt da ist.
var timeobject;
timeobject=FindObject(TIME);
if(timeobject) timeobject->~SetTimeTIME(1,1);
}

func Destruction()
{
// es wird die Tageszeit einfach auf Mittags gesetzt und die Helligkeit dazu hergestellt
var timeobject;
timeobject=FindObject(TIME);
if(timeobject) timeobject->~SetTimeTIME(1,1);
}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsAntiLagChooseable() { return(1); }