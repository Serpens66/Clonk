/*-- Spielzielappend --*/

#strict
#appendto GOAL

private func CheckCooperative()
{
  if(regelkontrolle)
   if(regelkontrolle->~IsActive())
    return true;
  return _inherited();
}

private func CheckRivalry()
{
  if(regelkontrolle)
   if(regelkontrolle->~IsActive())
    return true;
  return _inherited();
}

func IsChooseableGoal() {return(1);}
