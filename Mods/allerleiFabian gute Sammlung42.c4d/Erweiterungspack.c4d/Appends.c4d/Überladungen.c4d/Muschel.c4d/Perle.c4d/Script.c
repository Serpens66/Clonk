/*-- Perle --*/

#strict

protected func Hit() { Sound("MetalHit*"); }

private func Activate(pClonk)
{
  if(!DoMagicEnergy(+10,pClonk)) return(0);
  Sound("Electric");
  RemoveObject();
}

protected func Entrance(pContainer)
{
  if(GetID(Contained())!= WTWR) return(0);
  var tower= Contained();
  if(GetAction(tower) ne "Ready") return(0,ScheduleCall(this(),"Entrance",35));
  if(!DoMagicEnergy(+10,tower)) return(0,ScheduleCall(this(),"Entrance",35));
  ObjectSetAction(tower,"Energize");
  Sound("Electric");
  RemoveObject();
}