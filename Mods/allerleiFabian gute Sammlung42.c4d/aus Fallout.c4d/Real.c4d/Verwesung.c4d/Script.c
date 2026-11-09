/*-- Verwesende Clonks --*/

#strict

private func Check()
{
  var Obj;
  while(Obj= FindObject(0,0,0,0,0,OCF_Living(),0,0,0,Obj))
   if(!GetAlive(Obj))
    {
	if(GetID(Obj)==ALN5 || GetID(Obj)==MONS) CastPXS("Acid", 1, Random(8), GetX(Obj), GetY(Obj)); // bei monstern und aliens säure ausstoßen
    if(GetID(Obj)==FMNS) CastPXS("Lava", 1, Random(8), GetX(Obj), GetY(Obj)); // bei feuermonstern lava
	// Die "natürliche" Verwesung nicht beschleunigen, aber sonst schon
    if(!GetActMapVal("Name","Decay",GetID(Obj))) DoCon(-1,Obj);
    }
}