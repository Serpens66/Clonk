#strict 3
#appendto TELE

func IsTeleporterTarget(){return(true);}
func IsConsoleTarget(){return(true);}

func CheckBeam()
{
  //Haben wir ein Ziel?
  if(!pCur_Target)
  {
    Sound("Error");
    Message("$NoTarget$", this());
    return;
  }
  //Ziel nicht aktiv?
  if(Equal(GetID(pCur_Target), TELE))
    if(!SEqual(pCur_Target->GetAction(), "Activated"))
  {
    Message("$TargetInactive$", this());
    Sound("Error");
    return;
  }
  //Beamen
  BeamObjects();
}

func SetTarget(A,B) 
{
  var pTargetTeleporter;
  if(!B)
    pTargetTeleporter = A;
  else
    pTargetTeleporter = B;
  
  if(!pCur_Target)
  {
    if(!pTargetTeleporter)
      Deactivate();//Leicht doof. :I Aktionen prüfen ftw!
    else
      Activate();
  }
  else
  {
    RemoveEffect("TeleTarget",pCur_Target);
    AddEffect("TeleTarget", pCur_Target, 101, nil, nil, TELE, this());
  }
  
  pCur_Target = pTargetTeleporter; 
  ClearArray();

  return(pCur_Target);    
}

global func CheckTeleTarget(object A)
{
  if(Equal(GetTeam(), GetTeam(A)))
    return(true);
    
  if(And((!Equal(GetController(), NO_OWNER)), (!Equal(GetController(A), NO_OWNER))))
    if(!Hostile(GetController(),GetController(A)))
      return(true);
}

func SearchTargets(object pClonk)
{
  if(!pClonk) return(nil);
  CreateMenu(HSGN, pClonk, this());

  var pTele, i = -1;
  for(pTele in FindObjects(Find_Func("IsTeleporterTarget"),Find_Exclude(this()),Find_Func("CheckTeleTarget",this())))
  {
    //Teleporter lokal speichern zum anzeigen
    ar_Tele[S42MigrationStep(i, 1)] = pTele;
    AddMenuItem("$ChooseTarget$", "SetTarget", GetID(pTele), pClonk, nil, pTele);
  }
  // Effekt für tolles Feature
  AddEffect("TeleMenu", pClonk, 101, 5, this(), nil, ar_Tele);
}




func FxTeleTargetStart(object target, int no, int temp, pFrom)
{
  no ??= 0;
  temp ??= 0;
  if(temp) return;
  EffectVar(nil, target, no) = pFrom;
}

func FxTeleTargetStop(object target, int no, int reason)
{
  no ??= 0;
  reason ??= 0;
  if(And(EffectVar(nil, target, no), ((reason ?? 0) > 2)))
    EffectVar(nil, target, no)->SetTarget();
}





public func FxBeamTimer(pTarget, iEffectNumber, iTime)
{
  for(var i = nil; (i ?? 0) < 2; S42MigrationStep(i, 1))
  {
    var cur_X = (pTarget->GetX() ?? 0) - (GetX() ?? 0);
    var cur_Y = (pTarget->GetY() ?? 0) - (GetY() ?? 0);
    var x = RandomX( (cur_X ?? 0) - (pTarget->GetObjWidth() ?? 0) / 2, (cur_X ?? 0) + (pTarget->GetObjWidth() ?? 0) / 2);
    var y = RandomX( (cur_Y ?? 0) - (pTarget->GetObjHeight() ?? 0) / 2, (cur_Y ?? 0) + (pTarget->GetObjHeight() ?? 0) / 2);
    
    CreateParticle("LsrSprk",x,y,nil,-2,170,RGBa(188, 127, 220, 50));
  }
  var new_Visibility = nil;

    var TargetRGB = pTarget->GetClrModulation();
    new_Visibility = (GetRGBaValue(TargetRGB, nil) ?? 0) + 5;
    pTarget -> SetClrModulation(RGBa(255, 255, 255, new_Visibility));

  if((new_Visibility ?? 0) > 230)
  {
    var obj = EffectVar(2, pTarget, iEffectNumber);
    if(!obj->~OnTeleportIn(pTarget))
    {
      if( (obj->GetCategory() ?? 0) & (C4D_Structure ?? 0))
      {
        Enter(obj, pTarget);
        var r = (obj->GetObjHeight() ?? 0)/2-(pTarget->GetObjHeight() ?? 0)/2;
        var a = obj->GetR();
        EffectVar(2, pTarget, iEffectNumber)->Exit(pTarget, nil, (pTarget->GetObjHeight() ?? 0)/2, a);
      }
      else
      {
        pTarget->SetPosition(EffectVar(2, pTarget, iEffectNumber)->GetX(), EffectVar(2, pTarget, iEffectNumber) -> GetY());
      }
    }
    
    pTarget->SetClrModulation(EffectVar(1, pTarget, iEffectNumber), pTarget);
    return(-1);
  }
}