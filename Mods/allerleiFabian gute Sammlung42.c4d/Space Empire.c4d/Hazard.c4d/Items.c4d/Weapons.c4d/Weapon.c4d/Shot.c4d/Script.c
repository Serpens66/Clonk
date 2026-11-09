#strict 3

/* Schuss 1 */

local iTime, lx, ly, pTrail, iDamage, iPrec, iRefl, iTrailLength, iTrailSize, iXDir, iYDir;
local shooter; // Dingens/Clonk das den Schuss abgefeuert hat.
local bGlow;
local mlight;


func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
  // Team?
  if(shooter->GetTeam())
  	SetTeam(shooter->GetTeam());
}

public func Initialize() {
  SetObjectBlitMode(1);
}

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
{
  // Standardwerte setzen
  iAngle ??= 0;
  iSpeed ??= 0;
  iDist ??= 0;
  iSize ??= 0;
  iTrail ??= 0;
  iDmg ??= 0;
  iDmgPrec ??= 0;
  iGlowSize ??= 0;
  iAPrec ??= 0;
  iReflections ??= 0;
  if(!iSize)    iSize = 8;
  //if(!iGlowSize)
  //  iGlowSize = iSize;
  if(!iTrail)   iTrail = 300;
// auch null schaden ist erlaubt!
  //if(!iDmg)     iDamage = 3;
 // else 
 iDamage = iDmg;

  // und zuweisen
  iSize = Min((iSize ?? 0)+2,GetDefWidth());
  iPrec = iDmgPrec;

  // Positionieren
  SetPosition(GetX(),(GetY() ?? 0)+(GetDefWidth() ?? 0)/2);

  DoCon(100*(iSize ?? 0)/(GetDefWidth() ?? 0)-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*(iDist ?? 0)/(iSpeed ?? 0);
  
  if(!iTime)
    return(RemoveObject());

  var self = this();
  SetAction("Travel");
  if(!self) return;   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+(Sin(iAngle,iSpeed, iAPrec) ?? 0));
  SetYDir(-(Cos(iAngle,iSpeed, iAPrec) ?? 0));
  SetR(+(iAngle ?? 0));

  // Trail erzeugen
  CreateTrail(iSize, iTrail);
  
  bGlow = nil;
  // Tolles Leuchten erzeugen
  if(iGlowSize) {
    bGlow = true;
    SetGraphics(nil,this(),LIGH,1,GFXOV_MODE_Base, nil, 1);
    SetObjDrawTransform(100+35*(iGlowSize ?? 0),nil,nil, nil,100+35*(iGlowSize ?? 0),nil, this(),1);
    SetClrModulation(GlowColor(1),this(),1);
  }
  
  AddEffect("HitCheck", this(), 1,1, nil,GetID(),shooter);

  // Werte für Reflektionen speichern
  iRefl = iReflections;
  if(iRefl)
  {
    iTrailSize = iSize;
    iTrailLength = iTrail;
    iXDir = GetXDir(nil, 100);
    iYDir = GetYDir(nil, 100);
  }

  // Werte für Reflektionen speichern
  iRefl = iReflections;
  if(iRefl)
  {
    iTrailSize = iSize;
    iTrailLength = iTrail;
    iXDir = GetXDir(nil, 100);
    iYDir = GetYDir(nil, 100);
  }
    if(!mlight) // verzögert aufrufen, da sonst unschöne lichtflecken über clonk
    ScheduleCall(nil,"TurnLightOn",2);
    Schedule("SetPlrViewRange(100,this(),1)",2,nil,nil);
}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  iSize ??= 0;
  iTrail ??= 0;
  pTrail = CreateObject(TRAI,nil,nil,GetOwner());
  if(pTrail) {
    pTrail->Set((iSize ?? 0)-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Timer */

private func Traveling()
{
  SetPosition(GetX(), GetY(), pTrail);
  var iATime = GetActTime();

  // ausfaden
  SetClrModulation(Color(iATime));
  if(bGlow) {
    SetClrModulation(GlowColor(iATime),nil,1);
  }
  // löschen
  if((iATime ?? 0) >= (iTime ?? 0)) return(Remove());

  // außerhalb der Landschaft: löschen
  if((GetY() ?? 0)<0) return(Remove());
}
  
/* Treffer */

private func Hit()
{
  // Reflektion an Wänden (Eintrittswinkel = Austrittwinkel)
  if(iRefl)
  {
    if(!Equal(iXDir, nil))
    {
      // Positive XDir -> Rechts auf Material prüfen
      if((iXDir ?? 0) > 0)
      {
        if(GBackSolid(1,nil))
          S42MigrationAssign(iXDir, "*=", -1);
      // Und andersrum das gleiche Spiel
      }
      else
        if(GBackSolid(-1,nil))
          S42MigrationAssign(iXDir, "*=", -1);
    }
    if(!Equal(iYDir, nil))
    {
      // Positive YDir -> Unten auf Material prüfen
      if((iYDir ?? 0) > 0)
      {
        if(GBackSolid(nil,1))
          S42MigrationAssign(iYDir, "*=", -1);
      }
      // Und andersrum das gleiche Spiel
      else
        if(GBackSolid(nil,-1))
          S42MigrationAssign(iYDir, "*=", -1);
    }
    // Geschwindigkeit gehört neu gesetzt
    SetXDir(iXDir, nil, 100);
    SetYDir(iYDir, nil, 100);
    SetR(Angle(nil, nil, iXDir, iYDir));
    // Trail wird entfernt!
    pTrail->SetPosition(GetX(), GetY());
    pTrail->SetSpeed(nil,nil);
    pTrail->Remove();
    // Und neu erzeugt
    CreateTrail(iTrailSize, iTrailLength);

    // Eine Reflektion weniger
    S42MigrationStep(iRefl, -1, true);
    // Und erstmal nicht auftreffen
    return;
  }

  // umliegende Objekte beschädigen.
  var objs = FindObjects(	Find_AtPoint(),
							Find_NoContainer(),
							Find_Or(
								Find_Func("IsBulletTarget",GetID(),this(),shooter),
								Find_OCF(OCF_Alive)
							),
							Find_Func("CheckEnemy",this())
							/*Find_Not(Find_Func("HitExclude"))*/);  
 
  for(var pTarget in objs) {
    BulletStrike(pTarget);
  }

  HitObject();
}

private func HitObject(object pObject) {
  BulletStrike(pObject);
  Sparks(70*(GetCon() ?? 0)/100,Color(GetActTime()));
  Remove();
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255,255-(iPrg ?? 0)*2,255-(iPrg ?? 0)*2,(iPrg ?? 0)*2));
}

public func TrailColor(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255,255-(iPrg ?? 0)*2,255-(iPrg ?? 0)*2,(iPrg ?? 0)*2));
}

public func GlowColor(int iATime) {
  iATime ??= 0;
  return(RGBa(255,190,nil,50));
}

// Dummy-func
func HitExclude()
{

}

public func Remove() {
  if(pTrail) {
    pTrail->SetPosition(GetX(),GetY());
    pTrail->Remove();
  }

  RemoveObject();
}

/* Effekt für Trefferüberprüfung */

// EffectVars:
// 0 - alte X-Position
// 1 - alte Y-Position
// 2 - Schütze (Objekt, das die Waffe abgefeuert hat, üblicherweise ein Clonk)
// 3 - ID des Schützen
// 4 - Scharf? Wenn true wird der Schütze vom Projektil getroffen 
// 5 - niemals den Schützen treffen

public func FxHitCheckStart(object target, int effect, int temp, object byObj, bool neverShooter)
{
  effect ??= 0;
  temp ??= 0;
  neverShooter ??= false;
  if(temp) return;
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  if(!byObj)
    byObj = target;
  if(byObj->Contained())
    byObj = (byObj->Contained());
  EffectVar(2, target, effect) = byObj;
  EffectVar(3, target, effect) = GetID(byObj);
  EffectVar(4, target, effect) = nil;
  EffectVar(5, target, effect) = neverShooter;
}

private func FxHitCheckTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  var obj;
  // Oh man. :O
  var oldx = EffectVar(nil, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  // Schuss schon Scharf?
  var exclude = EffectVar(2, target, effect);
  // Ja, wir treffen nur uns selbst nicht (ja, Workaround .)
  if(EffectVar(4, target, effect)) exclude = target;
  
  //DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
  // Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
  // bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)
  for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
                         Find_NoContainer(),
                         Sort_Distance(oldx, oldy)))
  {
		// Excludes
		if(Equal(obj, target)) continue;
		if(Equal(obj, exclude)) continue;
		
		// CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(Or(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect)), (GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))) {
			DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
			return(target-> ~HitObject(obj));
	  }
  }

  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);

  //verdammt, kommentier doch mal... Also:
  // Der Schuss wird erst "scharf gemacht", d.h. kann den Schützen selbst treffen, wenn
  // der Schuss einmal die Shape des Schützen verlassen hat.
  
  // OKOK, Ich hab sogar das Restzeug kommentiert. :P
  if(!EffectVar(5,target,effect)) {
	  if(!EffectVar(4, target, effect)) {
	    // ready gibt an, ob wir schon "scharf" sind. True = Scharf
	    var ready = true;
	    // Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
	    for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
	    	// und schauen, ob es der Schütze ist.
	      if(Equal(foo, EffectVar(2, target, effect)))
	        // wir haben den Schützen gefunden -> Er ist noch an unserer Position
	        ready = nil;
	    // wir haben den Schützen nicht gefunden
	    if(ready)
	      // -> Wir treffen ihn ab jetzt.
	      EffectVar(4, target, effect) = true;
	  }
  }
}

// Ist ein Schuss! Nicht warpen!
func NoWarp() { return(true); }
// Ist ein Schuss!
func IsBullet() { return(true); }

func TurnLightOn()
{return;}
