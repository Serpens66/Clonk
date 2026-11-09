/*-- Zusatzboden --*/

#strict

protected func Initialize()
{
  Disappear();
}

private func CreateSolidMask()
{
  if(GetVisibility()== VIS_None()) SetVisibility(VIS_All());
  var iY= 0;
  if(GetAction() eq "AusfahrenLinks") iY= 4;
  SetSolidMask(51*GetPhase(),iY,51,4);
  if(GetPhase()== 11) return(1);
  ScheduleCall(this(),"CreateSolidMask",10);
}

private func RemoveSolidMask()
{
  if(GetVisibility()== VIS_None()) SetVisibility(VIS_All());
  var iY= 0;
  if(GetAction() eq "EinziehenLinks") iY= 4;
  SetSolidMask(51*(11-GetPhase()),iY,51,4);
  if(GetPhase()== 11) return(1,Disappear());
  ScheduleCall(this(),"RemoveSolidMask",10);
}

private func Disappear()
{
  SetVisibility(VIS_None());
  SetSolidMask();
}