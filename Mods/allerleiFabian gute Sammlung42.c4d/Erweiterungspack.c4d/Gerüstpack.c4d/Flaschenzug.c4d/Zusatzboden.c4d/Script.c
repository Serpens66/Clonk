/*-- Zusatzboden --*/

#strict 3

protected func Initialize()
{
  Disappear();
}

private func CreateSolidMask()
{
  if(Equal(GetVisibility(), VIS_None)) SetVisibility(VIS_All);
  var iY= nil;
  if(!!SEqual(GetAction(), "AusfahrenLinks")) iY= 4;
  SetSolidMask(51*(GetPhase() ?? 0),iY,51,4);
  if(Equal(GetPhase(), 11)) return(1);
  ScheduleCall(this(),"CreateSolidMask",10);
}

private func RemoveSolidMask()
{
  if(Equal(GetVisibility(), VIS_None)) SetVisibility(VIS_All);
  var iY= nil;
  if(!!SEqual(GetAction(), "EinziehenLinks")) iY= 4;
  SetSolidMask(51*(11-(GetPhase() ?? 0)),iY,51,4);
  if(Equal(GetPhase(), 11)) return(S42MigrationFirst(1,Disappear()));
  ScheduleCall(this(),"RemoveSolidMask",10);
}

private func Disappear()
{
  SetVisibility(VIS_None);
  SetSolidMask();
}