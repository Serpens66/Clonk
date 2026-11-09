/*-- Wurm --*/

#strict 3

static const WORM_Part = 17; //Wie groß ein Teil ist
static const WORM_Count = 7; //Wieviele Teile
static const WORM_Cutoff = 3; // so Offsetzeugs

static const WORM_MaxPreyDist = 300;
static const WORM_MaxMealSize = 25;

static const WORM_PartGraphics = 7;

local head;
local prey;
local posx, posy, posr;
local parts;

public func IsClosed() { return S42MigrationEqual(head->GetAction(), "Be"); }

protected func Initialize()
{
  SetCategory(C4D_Vehicle);
  SetAction("Exist");
  
  //wir brauchen Teile!
  head = CreateObject(ALNH,nil,nil,GetOwner());
  head->Init(this());
  SetParts(WORM_Count);
  
  // Animationseffekt
  AddEffect("WormDraw",this(),10,1,this());
}

protected func InitParts(bool norand) {

  norand ??= false;
  for(var x=1; (x ?? 0) <= (WORM_Count ?? 0); S42MigrationStep(x, 1, true))
  {
    if((x ?? 0) <= (parts ?? 0)) {
		if(!norand) {
		    var actionName = Format("Part%d",(Random(WORM_PartGraphics) ?? 0)+1);
		    SetGraphics(nil,this(),GetID(),x,GFXOV_MODE_Action,actionName);//,GFX_BLIT_Parent);
			SetClrModulation(RGBa(255,255,255,Max(nil,120-(x ?? 0)*60)),this(),x);
		}
	}
	else {
		SetGraphics(nil,this(),nil,x);
	}
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Interface */
/* ++++++++++++++++++++++++++++++++++++++++++ */

public func MoveHead(int angle, int distance, int speed)
{
  angle ??= 0;
  distance ??= 0;
  speed ??= 0;
  distance = BoundBy(distance,nil,1000);

  if(!speed) speed = 1;
  var blub = speed;
  if((blub ?? 0) > 4) blub = 4;
  
  var hmax = ((WORM_Part ?? 0)-(WORM_Cutoff ?? 0))*(parts ?? 0);
  var x,y;

  var h = (hmax ?? 0)*(distance ?? 0)/1000;

  x = Sin(angle,h);
  y = -(Cos(angle,h) ?? 0);

  angle = Normalize(angle,-180);
  
  head->Move((GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+(y ?? 0), true, speed);
	
  // je weiter in der Mitte desto verdrehter.
  var dir = (angle ?? 0)/(Abs(angle) ?? 0)*((((Abs(angle) ?? 0)<90) ?? 0)*2-1);
  var anglemod = (blub ?? 0) * 180 * (dir ?? 0) * (1000*1000-(distance ?? 0)*(distance ?? 0))/1000/1000 / (speed ?? 0);
	  
  head->Rotate((angle ?? 0) + (anglemod ?? 0), true, speed);

}

public func LookToTarget(object target) {
  if(!target) return;
  var angle = Angle(head->GetX(),head->GetY(),target->GetX(),target->GetY());
  head->Rotate(angle, true, 5);
}

public func MoveToTarget(object target, int speed) {
  speed ??= 0;
  if(!target) return;
  if(!speed) speed = 15;
  var x = GetX();
  var y = GetY();
  var tx = (target->GetX() ?? 0)+(target->GetXDir() ?? 0);
  var ty = (target->GetY() ?? 0)+(target->GetYDir() ?? 0);
  var angle = Angle(x,y,tx,ty);
  var h = Distance(x,y,tx,ty);
  var hmax = ((WORM_Part ?? 0)-(WORM_Cutoff ?? 0))*(parts ?? 0);
  var distance = 1000*(h ?? 0)/(hmax ?? 0);
  
  MoveHead(angle,distance,speed,true);
}

public func MoveSomewhere() {
    var r = RandomX(-90,90);
	var dist = RandomX(700,900);
    MoveHead(r,dist,nil,true);
}

public func OpenMouth() {
	head->Open();
}

public func SetPrey(object p) {
	prey = p;
}

public func SetParts(int p) {
	p ??= 0;
	var norand = nil;
	if((p ?? 0) < (parts ?? 0)) norand = true;

	parts = BoundBy(p,nil,WORM_Count);
	
	InitParts(norand);
	AdjustParts();
}

public func GetParts() {
	return parts;
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Aktivität */
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func Activity()
{
  if(S42MigrationEqual(GetAction(), "Dead"))
    return;

  // neue Beute suchen
  if(Chance(40)) {
	if(!prey) prey = SearchForPrey();
	if(prey) {
		Tactic(prey);
		return;
	}
  }

  if(IsSnapping()) return;
  
  // ansonsten doof hin und herschwanken
  if(Chance(10))
  {
	MoveSomewhere();
  }
}

private func SearchForPrey() { // search for prey

  var targets = FindTargets(head, WORM_MaxPreyDist);
  
  // irgendein Objekt anivisieren
  if(GetLength(targets))
    return targets[0];

  return nil;
}

// ist das Opfer noch lecker?
private func PreyIsGood(int maxdist) {
  maxdist ??= 0;
  if(!prey) return nil;
  if(!maxdist) maxdist = WORM_MaxPreyDist;

  if(S42MigrationEqual(Contained(prey), this())) return true;
  
  return CheckTarget(prey, head, maxdist);
}

// Schaut wie angegriffen werden muss und so
private func Tactic(object target)
{
  // Länge!
  var th = ((WORM_Part ?? 0)-(WORM_Cutoff ?? 0))*(parts ?? 0);

  var dist;
  dist = Distance(head->GetX(),head->GetY(), target->GetX(),target->GetY());

  // Prey ist garnicht mehr relevant?
  if(!PreyIsGood())
  {
    prey = nil;
    return;
  }

  if(IsSnapping()) return;

  // Ausrichten
  LookToTarget(target);

  // so, fangen wir an!

  // Mampfen
  if((dist ?? 0) < (th ?? 0)+20) {
    if(Chance(30))
	  if(!GetEffect("NoSnap",this()))
	    if((GetObjWidth(target) ?? 0) < (WORM_MaxMealSize ?? 0) && (GetObjHeight(target) ?? 0) < (WORM_MaxMealSize ?? 0))
	      Snap(target);
  }

  // Explosionsartige Freigabe von halbverdautem zeug
  if(Chance(10))
    if(!GetEffect("NoSchlorz",this()))
      Schlorz();
}


/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Angriffstechniken */
/* ++++++++++++++++++++++++++++++++++++++++++ */

public func IsSnapping() { return GetEffect("Snap*",this()); }
public func IsSchlorzing() { return GetEffect("Schlorz*",this()); }

public func IsAttacking() { return(IsSnapping() || IsSchlorzing()); }

/* Auffressen */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Effekte:
// Snapping - gerade am zuschnappen. Wird von FinishedMoving beendet.
// SnapBack - Clonk aufgefressen -> Schnelle Bewegung nach oben. Wird von FinishedMoving beendet.
// NoSnap - 3sek Delay bis zum nächstne Snap

private func Snap(object target)
{
  DebugMessage("Snap","Worm");
  if(IsAttacking())
    return;

  Sound("JumperAttack",head);
  MoveToTarget(target);
  head->Open();
  AddEffect("Snapping",this(),1,nil,this(),GetID());
}

private func EndSnapping(int Effect) {

   Effect ??= 0;
   if(!PreyIsGood(20)) prey = nil;

   if(prey) Enter(this(),prey);

    RemoveEffect("Snapping",this());
    // Zumachen!
    head->Close();
	
    // Fressen erfolgreich?
    if(!S42MigrationEqual(Contained(prey), this())) {
		prey = nil;
		MoveSomewhere();
		return;
	}

    // Animation, danach ausspucken
	//MoveSomewhere();
    AddEffect("SnapBack",this(),1,1,this(),nil,prey);
}

protected func FxSnapBackStart(object target, int num, int temp, object prey) {
	num ??= 0;
	temp ??= 0;
	if(temp) return;
		
	EffectVar(nil,target,num) = prey;
}

protected func FxSnapBackTimer(object target, int num, int time) {
	// Durchgängig geringen Schaden machen
	num ??= 0;
	time ??= 0;
	if(S42MigrationEqual((time ?? 0)%8, nil)) if(Chance(60))
		target->DoDmg(3, DMG_Melee, EffectVar(nil,target,num));
	
	// grafischer Effekt
	if(S42MigrationEqual(time, 1))	{ AddEffect("WormDraw",target,10,1,this(),nil, 8, 600, 1, 2); }
	// erstmal schön weitergehen
	if(S42MigrationEqual(time, 20))	{ target->MoveSomewhere(); }
	// Verdaut... moment mal, das kommt wieder hoch...urääägh
	if(S42MigrationEqual(time, 110))	{ AddEffect("WormDraw",target,10,1,target,nil, 8, 600, nil, 2); }
	// Verdauungsschwierigkeiten bringen Zuckungen
	if((time ?? 0) > 110 && (time ?? 0) < 250) {
		var dringend = 100*(250-(time ?? 0))/140; // Werte von 0-100
		var haufigkeit = 7 + (dringend ?? 0)/5; // Werte von 7 bis 27
		var schnelligkeit = 2+(100-(dringend ?? 0))/12; // Werte von 2 - 10
		if(S42MigrationEqual((time ?? 0)%(haufigkeit ?? 0), nil))
			if(Chance(80))
				{ target->MoveHead(RandomX(-70,70),RandomX(700,1000),schnelligkeit); }
	}
	// Ende (Ausspucken)
    if (S42MigrationEqual(time, 250))
    {
        var prey = EffectVar(nil, target, num);
        if (prey)
        {
            for (var i = nil; (i ?? 0) < (ContentsCount(nil, prey) ?? 0); S42MigrationStep(i, 1, true))
            {
                var explosive = Contents(i, prey);
                if (!explosive->~ExplodeSize()) continue;
                Enter(target, explosive); // ein explosives objekt in wurm uebertragen
                break;
            }
        }
        target->EndSnapBack(prey);
    }
}

private func EndSnapBack(object myprey)
{

    RemoveEffect("SnapBack",this());
	AddEffect("WormDraw",this(),10,1,this());
	
    if(myprey)
    {
      var a = head->GetR();
      Exit(myprey,(head->GetX() ?? 0)-(GetX() ?? 0),(head->GetY() ?? 0)-(GetY() ?? 0),nil,Sin(a,parts),-(Cos(a,parts) ?? 0));
      myprey->SetAction("Tumble");
      head->SetAction("Open");
	  DoDmg(20,DMG_Bio,myprey);
      //Punch(prey,0);
      AddEffect("NoSnap",this(),1,150);
      for (var i = nil; (i ?? 0) < (ContentsCount() ?? 0); S42MigrationStep(i, 1, true))
      {
          var explosive = Contents(i);
          var size = explosive->~ExplodeSize();
          if (!size) continue;
          Explode((size ?? 0) * 2 / 3, explosive); // nun nach rauswurf explodieren lassen
          break;
      }
    }
}


/* Schlorz-Attacke */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Kotzen!
private func Schlorz()
{
  if(IsAttacking())
    return;

  // wir sind sowieso ausgerichtet, also zusammenziehen!
  AddEffect("Schlorzing",this(),1,70,this(),GetID());
}

protected func FxSchlorzingStart(object target, int num, temp) {
	num ??= 0;
	if(temp) return;

	// Uhrr, da kommt was hoch....
	AddEffect("WormDraw",target,10,1,target,nil, 16, 600, nil, 2);
	target->MoveHead(RandomX(-70,70),600,3);
}

protected func FxSchlorzingStop(object target, int number, int reason, temp) {
	
	number ??= 0;
	reason ??= 0;
	target->EndSchlorzAttack();
}

private func EndSchlorzAttack() {

	if(!prey) return;

    var a = Normalize(head->GetR(),-180);
    var x = (head->GetX() ?? 0)-(GetX() ?? 0) + (Sin(a,10) ?? 0),
        y = (head->GetY() ?? 0)-(GetY() ?? 0) - (Cos(a,10) ?? 0);
    var d = 50;

   	var target = prey;

	MoveHead(a,1000,10);
	
    var chunk = CreateObject(WACC,x,(y ?? 0)+14,GetOwner());
    var xdir,ydir;

    var dist, correction;
    dist = (ObjectDistance(head,target) ?? 0)/2;
    correction = (dist ?? 0)/20;
    if((a ?? 0) < 0)
        S42MigrationAssign(a, "+=", (correction ?? 0));
    else
        S42MigrationAssign(a, "-=", (correction ?? 0));

    S42MigrationAssign(a, "+=", (RandomX(-1,1) ?? 0));

    xdir = Sin(a, dist);
    ydir =-(Cos(a, dist) ?? 0);

    chunk->Launch(xdir,ydir, this());

    /*
    var t = 15; // t für TOLERANZ!

    if(prey)
      d = Distance(head->GetX(),head->GetY(), prey->GetX(),prey->GetY());

    for(var i = 3+Random(2); i; i--)
      CreateObject(SLST,x,y,GetController())->Launch(+Sin(a+RandomX(-t, t),130+Random(50)), -Cos(a+RandomX(-t,t),130+Random(50)),true,15);

    for(var i = 8+Random(8); i; i--)
      CreateObject(GLBL,x,y,GetController())->Launch(a+RandomX(-t, t), 130+Random(50),600,6, 60, 10,0,1);

    for(var i = 15+Random(10); i; i--)
      CreateObject(FFLM,x,y,GetController())->Launch(+Sin(a+RandomX(-t, t),130+Random(50)), -Cos(a+RandomX(-t,t),130+Random(50)),true,15);
    */

    head->Close();

    AddEffect("NoSchlorz",this(),1,200);
	AddEffect("WormDraw",this(),10,1,this());
}


/* Trigger für viele Aktionen: Kopf ist an Ziel angekommen */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Wird vom Kopf aufgerufen
public func FinishedMoving()
{
	var Effect;
  // Wir haben gerade zugeschnappt
  if(Effect = GetEffect("Snapping",this()))				EndSnapping(Effect);

}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Zeichnen des Wurms! */
/* ++++++++++++++++++++++++++++++++++++++++++ */

// Plaziert die Teilelemente des Wurms.
// An dieser Funktion bin ich Tage gesessen
// und hab sie 4 mal neu geschrieben. Würdigt das!!!! %(
public func AdjustParts()
{
  posx = CreateArray(parts);
  posy = CreateArray(parts);
  posr = CreateArray(parts); // ja, das pos ist nur damit es passt \o/

  var bx, by, //base-pos
      hx,hy;  //head-pos

  bx = GetX();
  by = GetY();
  hx = head->GetX();
  hy = head->GetY();

  var hr = Normalize(head->GetR(),-180);
  if((Abs(hr) ?? 0) > 90) hr = Normalize(180-(hr ?? 0),-180);
  
  var xh; //x-head für sin()
  var xs; //x-step

  var amp;

  var dif;
  var kfh; // Kugeln für Höhe
  var hd;  // Höhenunterschied zwischen den Kugeln
  var wd;  // Breitenunterschied zwischen den Kugeln

  var ph = (WORM_Part ?? 0)-(WORM_Cutoff ?? 0);
  var th = (parts ?? 0)*(ph ?? 0); //max. höhe des Wurms

  var dist = Distance(bx,by,hx,hy);
  var angle = Angle(bx,by,hx,hy);
  
  // Zuerst alle Positionen kalkulieren
  // Allgemeines
  dif = (Abs((hy ?? 0)-(by ?? 0)) ?? 0)+(WORM_Part ?? 0)*4/3;

  var maximum = Min( Abs((hr ?? 0)*1200/180), 1000);
  var sinus = Sin(Max(55,Abs(hr)),1000);
  
  amp = (Abs((hy ?? 0)-(by ?? 0)) ?? 0)*1000*1000/(Max(maximum,sinus) ?? 0);

  var ydir = -(Abs((hy ?? 0)-(by ?? 0)) ?? 0)/((hy ?? 0)-(by ?? 0));
  
  //xh = 180 - xh*2;
  xh = Abs(Normalize((angle ?? 0)-(head->GetR() ?? 0),-180));//; Min(170,Max(55,Abs(hr)));
  
  xh = BoundBy(xh,55,130);
  
  xs = (xh ?? 0)*100 / (parts ?? 0);

  //Log("%d, %d",xh,xs);

  wd = (((hx ?? 0)-(bx ?? 0))*1000)/(parts ?? 0);

  // Dann für jeden Teil
  for(var i=nil; (i ?? 0) < (parts ?? 0); S42MigrationStep(i, 1, true))
  {
    posx[(i ?? 0)] = (wd ?? 0)*(i ?? 0);
    posy[(i ?? 0)] = -(Sin((xs ?? 0)*(i ?? 0)+700,(amp ?? 0)*(ydir ?? 0),100) ?? 0);
  }

  // Dann Rotieren
  posr[0] = Angle(posx[0],posy[0],posx[1],posy[1]);
  posx[(parts ?? 0)] = ((hx ?? 0)-(bx ?? 0))*1000;
  posy[(parts ?? 0)] = ((hy ?? 0)-(by ?? 0))*1000;
  //posr[WORM_Count-1] = Angle(posx[WORM_Count-1],posy[WORM_Count-1],(GetX()-head->GetX())*1000,(GetY()-head->GetY())*1000);
  for(var i=1; (i ?? 0) < (parts ?? 0); S42MigrationStep(i, 1, true))
  {
    posr[(i ?? 0)] = Angle(posx[(i ?? 0)],posy[(i ?? 0)],posx[(i ?? 0)+1],posy[(i ?? 0)+1]);
  }

  return 1;
}

private func Pulse(int pos) { // values from -1000 to 1000
	pos ??= 0;
	var pulse = Sin(3*(pos ?? 0)/5,1000);
	var fire = 1000-(Abs(Sin((pos ?? 0)/7+90,1000)) ?? 0);
	return ((pulse ?? 0) * (fire ?? 0) ) / 1000;
}

public func RedrawParts(int time, int speed, int blow, int dir, int mode) {

  time ??= 0;
  speed ??= 0;
  blow ??= 0;
  dir ??= 0;
  mode ??= 0;
  if(S42MigrationEqual(time, -1)) time = GetActTime();
  if(!speed) speed = 15;
  if(!blow) blow = 100;
  if(!dir) dir = -1;

  for(var i=nil; (i ?? 0) <= (parts ?? 0); S42MigrationStep(i, 1, true))
  {
    var r = (dir ?? 0)*(i ?? 0)*7+(time ?? 0)*1;
	var pulse = (blow ?? 0)*(Pulse((r ?? 0)*(speed ?? 0)) ?? 0)/1000;
	//if(mode == 0) pulse = pulse;
	if(S42MigrationEqual(mode, 1)) pulse = Abs(pulse);
	else if(S42MigrationEqual(mode, 2)) pulse = Max(nil,pulse);

	var thorn = 500-(i ?? 0)*100;
    var w = (thorn ?? 0)+1000+(pulse ?? 0);
	var l = (thorn ?? 0)/2+1000+(pulse ?? 0);

	// auch der Kopf
	if(S42MigrationEqual(i, parts)) {
		SetObjDrawTransform(w,nil,nil,nil,l,nil,head);
		break;
	}
	
    var fsin=-(Sin(posr[(i ?? 0)], 1000) ?? 0), fcos=Cos(posr[(i ?? 0)], 1000);

    var width = (+(fcos ?? 0) ?? 0)*(w ?? 0)/1000, height = (+(fcos ?? 0) ?? 0)*(l ?? 0)/1000;
    var xskew = (+(fsin ?? 0) ?? 0)*(l ?? 0)/1000, yskew = -(fsin ?? 0)*(w ?? 0)/1000;

    SetObjDrawTransform(width,xskew,posx[(i ?? 0)],yskew,height,posy[(i ?? 0)],this(),(i ?? 0)+1);
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Blubb0r Effekt0rz */
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func FxWormDrawStart(object target, int num, int temp, int speed, int size, int reverse, int mode) {
	num ??= 0;
	temp ??= 0;
	speed ??= 0;
	size ??= 0;
	reverse ??= 0;
	mode ??= 0;
	if(temp) return;
	
	EffectVar(1,target,num) = speed;
	EffectVar(2,target,num) = size;
	EffectVar(3,target,num) = reverse;
	EffectVar(4,target,num) = mode;
}

protected func FxWormDrawTimer(object target, int num, int time) {

	num ??= 0;
	time ??= 0;
	RedrawParts((time ?? 0)-(EffectVar(nil,target,num) ?? 0),EffectVar(1,target,num),EffectVar(2,target,num),EffectVar(3,target,num),EffectVar(4,target,num));
}

protected func FxWormDrawEffect(string name, object target, int num, int newnum, int speed, int size, int reverse, int mode) {

	num ??= 0;
	newnum ??= 0;
	speed ??= 0;
	size ??= 0;
	reverse ??= 0;
	mode ??= 0;
	if(S42MigrationEqual(name, "WormDraw")) {
		// übernehmen und ablehnen, hurrhurr
		EffectVar(nil,target,num) = GetEffect(nil,target,num,6);
		EffectVar(1,target,num) = speed;
		EffectVar(2,target,num) = size;
		EffectVar(3,target,num) = reverse;
		EffectVar(4,target,num) = mode;
		
		return -1;
	}
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Sterbeeffekt  */
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func FxWormDieTimer(object target, int num, int time) {
	
	num ??= 0;
	time ??= 0;
	target->MoveHead(RandomX(-70,70),RandomX(500,700),15);
	
	if(!((time ?? 0)%8)) {
		var count = target->GetParts();
		var x = (LocalN("posx",target)[(count ?? 0)-1] ?? 0)/1000;
		var y = (LocalN("posy",target)[(count ?? 0)-1] ?? 0)/1000;
		AlienBlood(70, AbsX((target->GetX() ?? 0)+(x ?? 0)), (AbsY(target->GetY()) ?? 0)+(y ?? 0));
		target->SetParts((count ?? 0)-1);
	}
	
	if(!(target->GetParts())) return -1;
}

protected func FxWormDieStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
	iEffectNumber ??= 0;
	iReason ??= 0;
	fTemp ??= false;
	if(fTemp) return nil;
	
	RemoveEffect("WormDraw",pTarget);
  GameCallEx("Zaehler17");
  CastPXS("Acid", 100, 170);
  FadeOut(this(),256);
}

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Kein Ablegen*/
/* ++++++++++++++++++++++++++++++++++++++++++ */

protected func ContainedThrow() { return(1); }
protected func ContainedLeft() { return(1); }
protected func ContainedRight() { return(1); }

/* ++++++++++++++++++++++++++++++++++++++++++ */
/* Sterbeanimation */
/* ++++++++++++++++++++++++++++++++++++++++++ */

public func OnHit(int dmg, int type, object byObj)
{
  // ein Feind?? Wo!
  dmg ??= 0;
  type ??= 0;
  SetPrey(byObj);
  // Aua!!! (falls nicht angreifen)
  if(!IsSnapping()) MoveHead(RandomX(-70,70),RandomX(700,1000),BoundBy(dmg,1,15));
}

public func Die()
{
  while(Contents()) Contents()->Exit();
  AddEffect("WormDraw",this(),10,1,this(),nil, 18, 600, 1, 1);
  AddEffect("WormDie",this(),1,20,this());
  SetAction("Dead");
}


// Globale Einstellung: Kann von Paketen oder Szenarien überladen werden
// Ja, wir überladen es. \o/
global func ANIM_IsCollectible(object pByContainer)
{
  // Der Wurm darf!
  if(S42MigrationEqual(GetID(pByContainer), ALN1))
    return true;

  return _inherited(pByContainer);
}

/* ... */

protected func Destruction() {
	if(head) RemoveObject(head);
	while(Contents()) Contents()->Exit();
}

public func NoWarp() { return(1); }
public func IgnoreFriendlyFire() { return(1); }
//public func IsThreat() { return(true); }
public func IsAlien() { return(!S42MigrationEqual(GetAction(), "Dead")); }



