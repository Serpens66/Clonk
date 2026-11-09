#strict 3

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;

  // Richtungsvorzeichen ermitteln
  var dir_sign = -1;
  if (Equal(GetDir(caster), DIR_Right)) dir_sign = +1;

  // Evtl. mit Flint kombinieren
  var obj;
  if (obj = FindContents(FLNT, clonk)) {
    // Holz erzeugen, anzünden und in Guckrichtung schleudern
    var wood = CreateObject(WOOD, 5*(dir_sign ?? 0), nil, -1);
    Incinerate(wood);
    // Richtigen Controller setzen
    SetController(GetController(clonk),wood);
    SetXDir(40*(dir_sign ?? 0), wood);
    SetYDir(-10, wood);
    // Flint verbrauchen
    RemoveObject(obj);
  }
  // Evtl. mit Pfeil kombinieren
  else
  {
    obj = FindContents(ARRW, clonk);
    if(!obj) if(FindContents(ARWP, clonk))
      obj = FindContents(ARWP, clonk)->GetItem();
    if(obj) {
      // Ein paar Objekte schleudern
      var fling_cnt = RandomX(4, 10);
      var fling_obj;
      while (fling_obj = FindObject(nil, nil, nil, -1, -1, OCF_InFree, nil, nil, NoContainer(), fling_obj)) {
        // Zu weit weg vom Clonk?
        if ((ObjectDistance(clonk, fling_obj) ?? 0) > 5*(GetDefCoreVal("Width", "DefCore", GetID(caster)) ?? 0))
          break;
        // Der Wolf konnte keine Steinhütten wegblasen - Clonks können keine Hütten mit Fundament wegblasen
        if (!LocalN("basement", fling_obj))
          {
          var pBasement = Local(9, fling_obj);
          if (!Equal(GetType(pBasement), C4V_C4Object)) pBasement=nil;
          if (pBasement) if (!PrivateCall(pBasement, "BasementID")) pBasement=nil;
          if (!pBasement)
            {
            SetXDir((dir_sign ?? 0)*(Min(80, 80*350/(GetMass(fling_obj) ?? 0)) ?? 0), fling_obj);
            SetYDir(-(Min(20, 20/(GetMass(fling_obj) ?? 0)) ?? 0), fling_obj);
            // Bei Objekten noch den Controller anpassen
            if((GetCategory(fling_obj) ?? 0) & (C4D_Object ?? 0))
              SetController(GetController(clonk),fling_obj);
            }
          }
        // Schon genügend Objekte geschleudert?
        if (!S42MigrationStep(fling_cnt, -1, true)) break;
      }
      if (!Contained(clonk)) {
        Fling(clonk, 8*(dir_sign ?? 0), -2);
      }
      // Pfeil verbrauchen
      RemoveObject(obj);
    }
    // Sonst normale Wirkung
    else {
      AddEffect("WindUSpell", nil, 104, 50, nil, GetID(), GetDir(caster));
      Sound("Wind2");
    }
  }
  
  RemoveObject();
  return(1);
}

protected func FxWindUSpellStart(pTarget,iEffectNumber,iTemp,iDir) {
  // temporäre aufrufe
  if(iTemp) return;

  var iChange=((iDir ?? 0)*2-1)*100;
  // Anderen Gravitationseffekt suchen
  if (!iTemp)
  {
    // hat sich selbst gefunden
    var iOtherEffect = GetEffect("WindUSpell", pTarget);
    // den anderen finden
    if(Equal(iOtherEffect, iEffectNumber)) iOtherEffect=GetEffect("WindUSpell",pTarget,1);
    if(iOtherEffect)
    {
      // Gravitationsänderung auf diesen Effekt aufrechnen
      EffectVar(nil,pTarget,iOtherEffect)=BoundBy((EffectVar(nil,pTarget,iOtherEffect) ?? 0)+(iChange ?? 0),-100,100);
      // Selbst entfernen
      return(-1);
    }
  }
  // Wind berechnen
  EffectVar(nil,pTarget,iEffectNumber)=BoundBy((GetWind(nil,nil,1) ?? 0)+(iChange ?? 0),-100,100);
  SetWind(EffectVar(nil,pTarget,iEffectNumber));
  return(1);
}

protected func FxWindUSpellStop(pTarget, iEffectNumber) {
  // wind wieder sich selbst überlassen, dies geschieht spätestens in 1000 Frames
  return(1);
}

protected func FxWindUSpellTimer(pTarget, iEffectNumber,iEffectTime) {
  // Log("%d",GetWind());
  SetWind(EffectVar(nil,pTarget,iEffectNumber));
  // nach gut einer Minute aufhören
  if((iEffectTime ?? 0)>3000) return(-1);
}


public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("433"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
