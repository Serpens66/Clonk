/*-- Diese Objektdefinition wird ausgenutzt, um dem Clonk einen Kontextmenüeintrag anzuhängen...  --*/

#strict 3
#appendto CLNK

public func ContextDachklappeOeffnen(pCaller)
{
  [$OpenFlap$|Image=DCKL|Condition=DachklappeGeschlossen]
  SetComDir(COMD_Stop,this());
  var geruest;
  if(!(geruest= FindObject(GRS1,-25,20,50,50))) return(nil);
  geruest->~ Open();
  return(1);
}

public func ContextGeruestDachklappe(pCaller)
{
  [$CloseFlap$|Image=DCKL|Condition=DachklappeOffen]
  SetComDir(COMD_Stop,this());
  var geruest;
  if(!(geruest= FindObject(GRS1,-25,20,50,50))) return(nil);
  geruest->~ Close();
  return(1);
}

private func DachklappeGeschlossen()
{
  var geruest;
  if(!(geruest= FindObject(GRS1,-25,20,50,50))) return(nil);
  if(And((!Equal(GetOwner(), GetOwner(geruest))), (!Equal(GetOwner(geruest), NO_OWNER)))) return(nil);
  return(LocalN("Verschluss",geruest));
}

private func DachklappeOffen()
{
  var geruest;
  if(!(geruest= FindObject(GRS1,-25,20,50,50))) return(nil);
  if(And((!Equal(GetOwner(), GetOwner(geruest))), (!Equal(GetOwner(geruest), NO_OWNER)))) return(nil);
  return(!LocalN("Verschluss",geruest));
}