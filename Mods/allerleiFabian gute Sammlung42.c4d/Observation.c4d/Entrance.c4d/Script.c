/*-- Niedergang --*/

#strict

protected ActivateEntrance: // C4Object *pClonk
  // Kein Beobachtungsturm?!
  if (Not(SetVar(0,FindObject(BT7U,0,+3)))) return(1);
  // Verfeindet
  if (Hostile(GetOwner(Par(0)),GetOwner(Var(0)))) return(Message("%s verfeindet.|Eingang versperrt!", Var(0), GetPlayerName(GetOwner())));
  // Beobachtungsturm betreten
  Enter(Var(0),Par(0));
  SetCommand(Par(0),"None");
  return(0);

protected Remove:
 if (SetVar(0,FindObject(BT7U,0,+3))) return(1);
 RemoveObject();
 return(0);