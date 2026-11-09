/*-- Anti Lag --*/

#strict


func Initialize()
{
SetPosition(1,1);
Erstellen();
}

func Erstellen()
{
if(!(FindObject(FI_R))) CreateObject(FI_R, 1,1,-1); //kein leuchtendes feuer -> in allen LGFL TurnOff() ausführen
if(!(FindObject(SES_))) CreateObject(SES_, 1,1,-1); //keine Jahreszeiten
if(!(FindObject(UMG_))) CreateObject(UMG_, 1,1,-1); //keine Umweltschädengrafiken
if(!(FindObject(WOL_))) CreateObject(WOL_, 1,1,-1); //keine Wolken
if(!(FindObject(KOHE))) CreateObject(KOHE, 1,1,-1); //konstante Helligkeit 
if(!(FindObject(NOLI))) CreateObject(NOLI, 1,1,-1); //keine Lichteffekte(außer feuer) -> in allen IsLight() außer LGFL TurnOff() ausführen
if(!(FindObject(NOSM))) CreateObject(NOSM, 1,1,-1); //no smoke (evtl mit änderung der global smoke fkt? so auch weißen rauch?)
}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}
