/*-- Kräheattacken --*/

#strict
local maufeinmal,maktuell;

func Activate()
{
  MessageWindow(Format("%v Feinde aufeinmal. Aktuell leben %v",maufeinmal,maktuell),Par());
  return(1);
}

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  DynMessage("Clonks knabbern *Kräh*",0,48,RGBa(0,0,0,0));
  return(1);
}

/* Eventuell Vogel erstellen */
private func DoVogel()
{
  maufeinmal = ObjectCount(K_AN)+1 ;
  maktuell= ObjectCount2(Find_ID(KRAE), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pVogel = CreateObject(KRAE, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pVogel);
  return(1);
}
