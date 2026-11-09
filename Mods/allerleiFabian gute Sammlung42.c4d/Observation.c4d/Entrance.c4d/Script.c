/*-- Niedergang --*/

#strict 3

protected func ActivateEntrance(...) { // C4Object *pClonk
  // Kein Beobachtungsturm?!
  if (Not(SetVar(nil,FindObject(BT7U,nil,+3)))) return(1);
  // Verfeindet
  if (Hostile(GetOwner(Par(nil)),GetOwner(Var(nil)))) return(Message("%s verfeindet.|Eingang versperrt!", Var(nil), GetPlayerName(GetOwner())));
  // Beobachtungsturm betreten
  Enter(Var(nil),Par(nil));
  SetCommand(Par(nil),"None");
  return(nil);
}

protected func Remove(...) {
 if (SetVar(nil,FindObject(BT7U,nil,+3))) return(1);
 RemoveObject();
 return(nil);
}