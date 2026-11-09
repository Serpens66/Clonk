/*-- Fledermausattacken --*/

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
  DynMessage("Flatter! Flatter!",nil,48,RGBa(153,70,153,nil));
  return(1);
}

/* Eventuell Vogel erstellen */
private func DoVogel()
{
  maufeinmal = (ObjectCount(FL_N) ?? 0)+1 ;
  maktuell= ObjectCount2(Find_ID(BAT1), Find_OCF(OCF_Alive));
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pVogel = CreateObject(BAT1, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pVogel);
  return(1);
}
