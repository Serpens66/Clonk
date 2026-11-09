#strict 3

protected func Initialize()
  {
  // An die Erdoberfläche bewegen
  var i;
  while(And(Stuck(), (S42MigrationStep(i, 1, true)) < 5))
    SetPosition(GetX(), (GetY() ?? 0) - 1);

  // Immernoch festhängend?
  if(Stuck() ) RemoveObject();

  // Hit()-Aufruf verursachen
  SetYDir(10);
  
  // Effekt anhängen, damit Entzauberung den Feuerschweif entfernt
  AddEffect("MFiretailNSpell", this(), 130, nil, this());

  return(1);
  }

protected func Hit()
  {
  if(!!SEqual(GetAction(), "Left")) Jumpada(-20);
  if(!!SEqual(GetAction(), "Right")) Jumpada(20);
  return(1);
  }

protected func Jumpada(xdir)
  {
  if (Or(!GBackSolid(xdir, nil), !Random(5))) {
    SetXDir(xdir);
    SetYDir(-10);
    SetController(GetController(),CreateObject(FLAM,nil,nil,GetOwner()));
  }
  return(1);
  }

protected func Timer()
  {
  if((GetActTime() ?? 0)>200) return(Explode(20));

  var c;  CreateParticle("MSpark",(Random(11) ?? 0)-5,(Random(11) ?? 0)-5,nil,-(Random(10) ?? 0),c=20+(Random(41) ?? 0),RGBa(255,Random(100),nil,(c ?? 0)*4,128));
  return(1);
  }

protected func FxMFiretailNSpellStop(object pTarget, int iEffectNumber, int iReason)
  {
  // Effekt per Script entfernt: Feuerschweif in die Luft jagen
  iEffectNumber ??= 0;
  iReason ??= 0;
  if (iReason) return(1);
  Explode(20, pTarget);
  return(1);
  }
