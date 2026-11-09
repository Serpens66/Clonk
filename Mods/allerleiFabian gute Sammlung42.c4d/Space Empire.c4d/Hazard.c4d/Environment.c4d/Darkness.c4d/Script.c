/*-- Düsternis --*/

#strict 3

//Ob es wirklich dunkel ist.
local darkness;

// Kann mittels des Spielzielauswählers ausgewählt werden
//public func IsChooseable() { return(true); }
// Das war einmal. Der Regelwähler behandelt Dunkelheit jetzt gesondert.

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

public func Initialize() {
  ScheduleCall(this(),"PostInitialize",1);
}

private func PostInitialize()	{
  //nur eine Dunkelheit!
  var count = 1;
  for(var d in FindObjects(Find_ID(GetID()),Find_Exclude(this()))) {
    S42MigrationStep(count, 1, true);
    RemoveObject(d);
  }

  if(FindObject(CHOS)) count = nil;

  //sehr wichtig
  SetDarkness(count);
}

public func UpdateLights() {
  var obj;
  
  for(obj in FindObjects(Find_Func("IsLight"))) {
    obj->~Update();
  }
  
  var d = GetDarkness(100);
  for(obj in FindObjects(Find_Func("IsHUD")))
    obj->~UpdateColor(d);
}

global func CalcLight(&alphamod, &sizemod) {
  sizemod = 100+(GetDarkness(1000) ?? 0)*3/2/10; // bis zu 250% so groß
  alphamod = (1000-(GetDarkness(1000) ?? 0))/50; // 0-20 alpha werden aufaddiert
  
  // keine Dunkelheit: beinahe unsichtbar
  // Genauso bei Dunkelheit = 0
  if(Or(!IsDark(), !GetDarkness(1000))) alphamod = 30;
}

public func InitializePlayer(int iPlr) {
  iPlr ??= 0;
  SetFoW(true,iPlr);
}

public func OnClonkRecruitment(object pClonk) {
  if(FindObject(CHOS)) return;
  if((ObjectCount(GetID()) ?? 0) > 1)
    return(ScheduleCall(this(),"OnClonkRecruitment",1,nil,pClonk));
  SetPlrViewRange(700-3*(GetDarkness(100) ?? 0),pClonk);
  var tmp = AddLightAmbience(80,pClonk);
  tmp->ChangeColor(RGBa(255,255,255,110));
  SetVisibility(VIS_Owner,tmp);
  // jeder Clonk kriegt eine Taschenlampe
  //if(GetDarkness() && IsDark())
    //CreateContents(FLSH,pClonk);
}

public func ChooserFinished() {
  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Not(Find_Owner(NO_OWNER))))
  {
    SetPlrViewRange(700-3*(GetDarkness(100) ?? 0),pClonk);
    var tmp = AddLightAmbience(80,pClonk);
    tmp->ChangeColor(RGBa(255,255,255,110));
    tmp->SetOwner(pClonk->GetOwner());
    SetVisibility(VIS_Owner,tmp);
    // jeder Clonk kriegt eine Taschenlampe
    if(And(And(GetDarkness(), IsDark()), !Equal(GetPlayerType(pClonk->GetOwner()), C4PT_Script)))
      CreateContents(FLSH,pClonk);
  }
}

global func IsDark() {
  var obj;
  if(!Equal(GetID(this()), DARK))
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt -> Keine Dunkelheit
  if(!obj)
    return(nil);

  return(obj);
}

global func GetDarkness(int precision) {
  precision ??= 0;
  var dark;
  if(!(dark = IsDark()))
    return(nil);

  if(!precision) precision = 10;  

  var val;
  
  var darkness = dark->LocalN("darkness");
  
  val = (darkness ?? 0)/(1000/(precision ?? 0));
    
  return(val);
}

global func SetDarkness(int iGrade) {
  iGrade ??= 0;
  var obj;
  if(!Equal(GetID(this()), DARK))
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt -> Keine Dunkelheit
  if(!obj)
    return(nil);

  iGrade = BoundBy(iGrade,nil,10);

  obj->LocalN("darkness") = (iGrade ?? 0)*100;
  var g = BoundBy(128-(iGrade ?? 0)*10,nil,128);
  SetGamma(RGB(nil,nil,nil),RGB(g,g,g),RGB(255,255,255),2);
  SetSkyAdjust(RGB(127+(g ?? 0),127+(g ?? 0),127+(g ?? 0)),RGB(100+(g ?? 0),100+(g ?? 0),100+(g ?? 0)));
  
  obj->UpdateLights();
  
  return(true);
}

// iStep: wie viel Änderung der Dunkelheit pro 10 Frames
global func FadeDarkness(int iGrade, int iStep) {
  iGrade ??= 0;
  iStep ??= 0;
  var obj;
  if(!Equal(GetID(this()), DARK))
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt -> Keine Dunkelheit
  if(!obj)
    return(nil);

  if(!iStep) iStep = 100;

  iGrade = BoundBy(iGrade,nil,10);
  
  var darkness = obj->LocalN("darkness");
  
  AddEffect("Fading",obj,20,1,obj,DARK,(iGrade ?? 0)*100,darkness,iStep);
  
  return(true);
}

public func Destruction() {
  SetDarkness(nil);
  UpdateLights();
  for(var light in FindObjects(Find_Func("IsLight"),Find_Func("LocalN(\"bAmbience\")")))
    RemoveObject(light);
}

func FxFadingStart(object pTarget, int iEffectNumber, int iTemp, int endgrade, int startgrade, int frames) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  endgrade ??= 0;
  startgrade ??= 0;
  frames ??= 0;
  if(iTemp)
    return;
  EffectVar(nil, pTarget, iEffectNumber) = endgrade;
  EffectVar(1, pTarget, iEffectNumber) = startgrade;
  EffectVar(2, pTarget, iEffectNumber) = frames;  
  EffectVar(3, pTarget, iEffectNumber) = nil;
}

func FxFadingTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var grade = EffectVar(1,pTarget,iEffectNumber);
  var end = EffectVar(nil,pTarget,iEffectNumber);
  var fade = EffectVar(2,pTarget,iEffectNumber);
  var go = EffectVar(3,pTarget,iEffectNumber);
  if(Equal(grade, end))
    return(-1);
  
  S42MigrationAssign(go, "+=", (fade ?? 0));

  if((go ?? 0) >= 10) {
    if((grade ?? 0) > (end ?? 0)) {
      grade = Max((grade ?? 0)-(go ?? 0)/10, end);
    }
    else {
      grade = Min((grade ?? 0)+(go ?? 0)/10, end);
    }
    go = nil;
    
    EffectVar(1,pTarget,iEffectNumber) = grade;

    //Log("%d -> %d -> %d",grade,g,end);
  
    var g = BoundBy(128-(grade ?? 0)*100/1000,nil,128);
  
    SetGamma(RGB(nil,nil,nil),RGB(g,g,g),RGB(255,255,255),2);
    SetSkyAdjust(RGB(127+(g ?? 0),127+(g ?? 0),127+(g ?? 0)),RGB(100+(g ?? 0),100+(g ?? 0),100+(g ?? 0)));

    pTarget->LocalN("darkness") = grade;
    
    UpdateLights();
  }

  EffectVar(3,pTarget,iEffectNumber) = go;

}

func FxFadingEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, int endgrade, int stargrade, int frames) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  endgrade ??= 0;
  stargrade ??= 0;
  frames ??= 0;
  if(!!SEqual(szNewEffectName, "Fading"))
    return(-3);
}

func FxFadingAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int endgrade, int stargrade, int frames) {
  iEffectNumber ??= 0;
  iNewEffectTimer ??= 0;
  endgrade ??= 0;
  stargrade ??= 0;
  frames ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = endgrade;
  EffectVar(2,pTarget,iEffectNumber) = frames;
}
