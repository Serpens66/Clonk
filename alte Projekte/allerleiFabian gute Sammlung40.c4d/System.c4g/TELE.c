#strict
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
    return();
  }
  //Ziel nicht aktiv?
  if(GetID(pCur_Target) == TELE)
    if(pCur_Target->GetAction() ne "Activated")
  {
    Message("$TargetInactive$", this());
    Sound("Error");
    return();
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
    AddEffect("TeleTarget", pCur_Target, 101, 0, 0, TELE, this());
  }
  
  pCur_Target = pTargetTeleporter; 
  ClearArray();

  return(pCur_Target);    
}

global func CheckTeleTarget(object A)
{
  if(GetTeam() == GetTeam(A))
    return(true);
    
  if((GetController() != NO_OWNER) && (GetController(A) != NO_OWNER))
    if(!Hostile(GetController(),GetController(A)))
      return(true);
}

func SearchTargets(object pClonk)
{
  if(!pClonk) return(0);
  CreateMenu(HSGN, pClonk, this());

  var pTele, i = -1;
  for(pTele in FindObjects(Find_Func("IsTeleporterTarget"),Find_Exclude(this()),Find_Func("CheckTeleTarget",this())))
  {
    //Teleporter lokal speichern zum anzeigen
    ar_Tele[++i] = pTele;
    AddMenuItem("$ChooseTarget$", "SetTarget", GetID(pTele), pClonk, 0, pTele);
  }
  // Effekt für tolles Feature
  AddEffect("TeleMenu", pClonk, 101, 5, this(), 0, ar_Tele);
}




func FxTeleTargetStart(object target, int no, int temp, pFrom)
{
  if(temp) return();
  EffectVar(0, target, no) = pFrom;
}

func FxTeleTargetStop(object target, int no, int reason)
{
  if(EffectVar(0, target, no) && (reason > 2))
    EffectVar(0, target, no)->SetTarget();
}





public func FxBeamTimer(pTarget, iEffectNumber, iTime)
{
  for(var i = 0; i < 2; ++i)
  {
    var cur_X = pTarget->GetX() - GetX();
    var cur_Y = pTarget->GetY() - GetY();
    var x = RandomX( cur_X - pTarget->GetObjWidth() / 2, cur_X + pTarget->GetObjWidth() / 2);
    var y = RandomX( cur_Y - pTarget->GetObjHeight() / 2, cur_Y + pTarget->GetObjHeight() / 2);
    
    CreateParticle("LsrSprk",x,y,0,-2,170,RGBa(188, 127, 220, 50));
  }
  var new_Visibility = 0;

    var TargetRGB = pTarget->GetClrModulation();
    new_Visibility = GetRGBaValue(TargetRGB, 0) + 5;
    pTarget -> SetClrModulation(RGBa(255, 255, 255, new_Visibility));

  if(new_Visibility > 230)
  {
    var obj = EffectVar(2, pTarget, iEffectNumber);
    if(!obj->~OnTeleportIn(pTarget))
    {
      if( obj->GetCategory() & C4D_Structure)
      {
        Enter(obj, pTarget);
        var r = obj->GetObjHeight()/2-pTarget->GetObjHeight()/2;
        var a = obj->GetR();
        EffectVar(2, pTarget, iEffectNumber)->Exit(pTarget, 0, pTarget->GetObjHeight()/2, a);
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