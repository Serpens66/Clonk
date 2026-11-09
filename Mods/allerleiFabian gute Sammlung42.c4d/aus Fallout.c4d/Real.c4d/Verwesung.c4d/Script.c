/*-- Verwesende Clonks --*/

#strict 3

private func Check()
{
  var Obj;
  while(Obj= FindObject(nil,nil,nil,nil,nil,OCF_Living,nil,nil,nil,Obj))
   if(!GetAlive(Obj))
    {
	if(Or(Equal(GetID(Obj), ALN5), Equal(GetID(Obj), MONS))) CastPXS("Acid", 1, Random(8), GetX(Obj), GetY(Obj)); // bei monstern und aliens säure ausstoßen
    if(Equal(GetID(Obj), FMNS)) CastPXS("Lava", 1, Random(8), GetX(Obj), GetY(Obj)); // bei feuermonstern lava
	// Die "natürliche" Verwesung nicht beschleunigen, aber sonst schon
    if(!GetActMapVal("Name","Decay",GetID(Obj))) DoCon(-1,Obj);
    }
}