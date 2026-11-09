#strict 3

func Completion()
{
  SetR(Random(360));
}

func Wind2Turn()
{
  DoEnergy(Abs((GetRDir() ?? 0) * 5), GetActionTarget() );
  SetRDir((GetWind() ?? 0) / 4);
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy(GetActionTarget())),this());
}

