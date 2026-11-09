/*-- Kräheattacken --*/

#strict 3
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
  DynMessage("Clonks knabbern *Kräh*",nil,48,RGBa(nil,nil,nil,nil));
  return(1);
}

/* Eventuell Vogel erstellen */
private func DoVogel()
{
  maufeinmal = (ObjectCount(K_AN) ?? 0)+1 ;
  maktuell= ObjectCount2(Find_ID(KRAE), Find_OCF(OCF_Alive));
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pVogel = CreateObject(KRAE, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pVogel);
  return(1);
}
