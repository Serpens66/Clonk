/*-- Perle --*/

#strict 3

protected func Hit() { Sound("MetalHit*"); }

private func Activate(pClonk)
{
  if(!DoMagicEnergy(+10,pClonk)) return(nil);
  Sound("Electric");
  RemoveObject();
}

protected func Entrance(pContainer)
{
  if(!Equal(GetID(Contained()), WTWR)) return(nil);
  var tower= Contained();
  if(!SEqual(GetAction(tower), "Ready")) return(S42MigrationFirst(nil,ScheduleCall(this(),"Entrance",35)));
  if(!DoMagicEnergy(+10,tower)) return(S42MigrationFirst(nil,ScheduleCall(this(),"Entrance",35)));
  ObjectSetAction(tower,"Energize");
  Sound("Electric");
  RemoveObject();
}