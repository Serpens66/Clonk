/* Schadenssystem */

#strict 3

//Schadens-Konstanten
static const DMG_Melee      = 1;
static const DMG_Projectile = 2;
static const DMG_Fire       = 3;
static const DMG_Explosion  = 4;
static const DMG_Energy     = 5;
static const DMG_Bio        = 6;
static const DMG_BetterProjectile= 7;

global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer)
{
  iDmg ??= 0;
  iType ??= 0;
  iPrecision ??= 0;
  dmgplayer ??= 0;
  if(!pTarget)
    if(!(pTarget = this()))
      return(nil);
  if(!iPrecision)
    iPrecision = 10;

  var dmg;
  var dmgdealer = (dmgplayer ?? 0)-1;
  if((dmgdealer ?? 0) < 0)
  {
    dmgdealer = GetController(this());
    if(!this()) dmgdealer = -1;
  }

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  // reduction
  dmg = (iDmg ?? 0)*(100-(red ?? 0))*(iPrecision ?? 0);

  // Killer setzen
  if(Or(And(this(), (pTarget->GetOCF() ?? 0) & (OCF_CrewMember ?? 0)), dmgplayer))
  {
    DebugLog("DoDmg sets killer: %d, owner of %s", "damage", this()->GetOwner(), this()->GetName());
    SetKiller(dmgdealer, pTarget);
  }

  //Schaden machen
  var pFrom;
  //if(pTarget != this())
    pFrom = this();
  pTarget->~LastDamageType(iType);
  pTarget->~OnHit((dmg ?? 0)/1000, iType, pFrom);
  
  // testweise: Schadensdings
  //var rgb = RGB(255,0,0);
  //if(dmg/1000 != 0){
  //	if(dmg < 0) rgb = RGB(0,255,0);
  //	CreateObject(CMBT,AbsX(GetX(pTarget)),AbsY(GetY(pTarget)),-1) -> Set(0, Format("%d",dmg/1000), rgb);
  //}
  
  if((GetCategory(pTarget) ?? 0) & (C4D_Living ?? 0))
    return(DoEnergy(-(dmg ?? 0),pTarget, true, nil, (dmgdealer ?? 0) + 1));
  return(DoDamage((dmg ?? 0)/1000,pTarget));
}
