/* Zusatzfunktionen */
#strict 3
//static const GraphFadeLayer=-1; // Erstmal 1 bis Background-Layer implentiert ist
static const GraphFadeLayer=1;

global func MessageWindow(string pMsg, int iForPlr, id idIcon, string pCaption)
{
  // get icon
  iForPlr ??= 0;
  if (!idIcon) idIcon=GetID();
  // get caption
  if (!pCaption) pCaption=GetName();
  // create msg window (menu)
  var pCursor = GetCursor(iForPlr);
  if (!CreateMenu(idIcon, pCursor, pCursor, nil, pCaption, nil, 2)) return;
  AddMenuItem(pCaption, "", idIcon, pCursor,nil,nil,pMsg);
  return(1);
}

global func SIcon(idDef,i,dwColor)
{
  if(dwColor)
    return Format("<c %x>{{%i:%d}}</c>",dwColor,idDef,i);
  return Format("{{%i:%d}}",idDef,i);
}

/* ---------- */

global func GetSign(iVal)
{
  if((iVal ?? 0)>0)
   return 1;
  if((iVal ?? 0)<0)
   return -1;
  return nil;
}

global func SinX(iA,iMin,iMax) { return((Sin(iA,((iMax ?? 0)-(iMin ?? 0))/2) ?? 0)+((iMax ?? 0)+(iMin ?? 0))/2); }
global func CosX(iA,iMin,iMax) { return((Cos(iA,((iMax ?? 0)-(iMin ?? 0))/2) ?? 0)+((iMax ?? 0)+(iMin ?? 0))/2); }

global func Rotate(Rot,Obj)
{
  if(!Obj)
   if(!(Obj=this()))
    return nil;
  if(!Rot)
   return RemoveEffect("Rotate",Obj,nil);
  return AddEffect("Rotate",Obj,110,1,nil,nil,Rot);
}

/* ----- Effekt: Rotate ----- */

global func FxRotateStart(object Target, int Effectnumber, int Temp, Speed)
{
  Effectnumber ??= 0;
  Temp ??= 0;
  EffectVar(1,Target,Effectnumber)=Speed;
  return nil;
}

global func FxRotateTimer(object Target, int Effectnumber, int EffectTime)
{
  Effectnumber ??= 0;
  EffectTime ??= 0;
  var speed = EffectVar(1,Target,Effectnumber);
  SetR((GetR(Target) ?? 0)+(speed ?? 0),Target);
  return 1;
}

global func FxRotateEffect(string NewEffectName, object Target, int EffectNumber)
{
  EffectNumber ??= 0;
  if(!!SEqual(NewEffectName, "Rotate"))
   return -2;
  return nil;
}

global func FxRotateAdd(object Target, int Effectnumber, string NewEffectName, int NewEffectTimer, Speed)
{
  Effectnumber ??= 0;
  NewEffectTimer ??= 0;
  EffectVar(1,Target,Effectnumber)=Speed;
  ChangeEffect(nil,Target,Effectnumber,nil,NewEffectTimer);
  return nil;
}

/* ----- ENDE:Rotate ----- */

global func AttachTo(Obj,Obj2)
{
  if(!Obj2)
   if(!(Obj2=this()))
    return nil;
  if(!Obj)
   return RemoveEffect("Attach",Obj2,nil);
  return AddEffect("Attach",Obj2,110,1,nil,nil,Obj);
}

/* ----- Effekt: Attach ----- */

global func FxAttachStart(object Target, int Effectnumber, int Temp, Object)
{
  Effectnumber ??= 0;
  Temp ??= 0;
  EffectVar(1,Target,Effectnumber)=Object;
  return nil;
}

global func FxAttachTimer(object Target, int Effectnumber, int EffectTime)
{
  Effectnumber ??= 0;
  EffectTime ??= 0;
  var obj = EffectVar(1,Target,Effectnumber);
  SetPosition(GetX(obj),GetY(obj),Target);
  return 1;
}

global func FxAttachEffect(string NewEffectName, object Target, int EffectNumber)
{
  EffectNumber ??= 0;
  if(!!SEqual(NewEffectName, "Attach"))
   return -2;
  return nil;
}

global func FxAttachAdd(object Target, int Effectnumber, string NewEffectName, int NewEffectTimer, NewObject)
{
  Effectnumber ??= 0;
  NewEffectTimer ??= 0;
  EffectVar(1,Target,Effectnumber)=NewObject;
  ChangeEffect(nil,Target,Effectnumber,nil,NewEffectTimer);
  return nil;
}

/* ----- ENDE:Attach ----- */

global func FadeGraphics(sGraphic,pObject,idDef,fIgnoreSame)
{
  if(!fIgnoreSame)
    if(!!SEqual(GetGraphics(pObject), sGraphic))
      return 2;
  if(AddEffect("graphFade",pObject,100,RandomX(3,5),nil,nil,sGraphic,idDef,GraphFadeLayer))
    return(1);
  //Log("ERROR:FadeGraphics");
  return(nil);
  //return(SetGraphics(sGraphic,pObject,idDef));
}

global func SetObjAlpha(byAlpha, pObj,iLayer)
{
  var byAlpha=BoundBy(byAlpha,nil,255);
  var dwClrMod=GetClrModulation(pObj,iLayer);
  if(!dwClrMod)
    dwClrMod=RGB(255,255,255);
    dwClrMod=SetRGBaValue(dwClrMod,byAlpha);
  if(iLayer)
    return(SetClrModulation(dwClrMod, pObj,iLayer));
  else
    return(SetClrModulation(dwClrMod, pObj));
}

/* ----- Effekt: graphFade ----- */

global func FxgraphFadeStart(object pObject, int iEffectNumber, Temp, sGraphic, idDef , iLayer)
{
  iEffectNumber ??= 0;
  if(Temp)
    return(nil);
  if(!idDef)
    idDef=GetID(pObject);
  SetGraphics(nil,pObject,nil,iLayer);
  if(!SetGraphics(sGraphic,pObject,idDef,iLayer,GFXOV_MODE_ExtraGraphics))
  {
    //Log("ERROR:FxgraphFadeStart");
    return(-1);
  }
  EffectVar(1,pObject,iEffectNumber)=iLayer;
  EffectVar(2,pObject,iEffectNumber)=sGraphic;
  EffectVar(3,pObject,iEffectNumber)=idDef;
  SetObjAlpha(255, pObject,iLayer);
  return(1);
}

global func FxgraphFadeTimer(object pObject, int iEffectnumber, int iEffectTime)
{
  iEffectnumber ??= 0;
  iEffectTime ??= 0;
  var iLayer=EffectVar(1,pObject,iEffectnumber);
  var AlphaA=BoundBy((GetRGBaValue(GetClrModulation(pObject,iLayer)) ?? 0)-(4*(1+(IsAntiLag() ?? 0))),1,255);
  SetObjAlpha(AlphaA,pObject,iLayer);
  //var AlphaB=BoundBy(GetRGBaValue(GetClrModulation(pObject))+1,1,255);
  if((AlphaA ?? 0) < 127)
  {
    var AlphaB=BoundBy((GetRGBaValue(GetClrModulation(pObject)) ?? 0)+(5*(1+(IsAntiLag() ?? 0))),1,255);
    SetObjAlpha(AlphaB,pObject);
  }
  if(And(Equal(AlphaB, 255), Equal(AlphaA, 1)))
  {
    return(-1);
  }
  return(1);
}

global func FxgraphFadeStop(object pObject, int iEffectNumber, iReason, Temp)
{
  //Log("FxgraphFadeStop");
  iEffectNumber ??= 0;
  if(Temp)
    return(nil);
  var iLayer=EffectVar(1,pObject,iEffectNumber);
  var sGraphic=EffectVar(2,pObject,iEffectNumber);
  var idDef=EffectVar(3,pObject,iEffectNumber);
  SetGraphics(sGraphic,pObject,idDef);
  SetGraphics(nil,pObject,nil,iLayer);// Den Effektlayer löschen...
  SetObjAlpha(nil, pObject);
  return(1);
}

global func FxgraphFadeEffect(string szNewEffectName)
{
  if(!!SEqual(szNewEffectName, "graphFade"))
   return(-1);
  return(nil);
}

//____::::____

//
global func SetLayerTransform(iLayer,iR,iXCon,iYCon,iXPos,iYPos,pTarget)
{
  
  SetObjDrawTransform((iXCon ?? 0)*10, nil, 1000*(iXPos ?? 0),
                      nil, (iYCon ?? 0)*10, 1000*(iYPos ?? 0),
                      pTarget, iLayer);
  // *sniff* funktionukelt leider nicht... :( (Bei Drehung wird die grösse falsch berechnet)
  //var fsin=-Sin(iR, 1000), fcos=Cos(iR, 1000);
  /*SetObjDrawTransform
    (
        iXCon*10+fcos-1000,
        0+fsin,
        (1000-fcos)*iXPos - fsin*iYPos,
        0-fsin,
        iYCon*10+fcos-1000,
        (1000-fcos)*iYPos + fsin*iXPos,
        pTarget, iLayer
    );*/
  return 1;
}

global func Prozent(voll,anteil) // gibt P1 Prozent von P0 zurück
{  
	return((voll ?? 0)*(anteil ?? 0)/100);
}

global func ProzentVon(voll,anteil) // wieviel Prozent von P0 sind P1?
{
  return(100*(anteil ?? 0)/(voll ?? 0));
}

global func SetActIdle(){ return(SetAction("Idle"));}

global func GetPlrCount(){ return(GetPlayerCount());}

global func EnergyCount(idDef,actname)
{
        var objekt,zahl=nil;
	while(objekt=FindObject(idDef,nil,nil,nil,nil,nil,actname,nil,nil,objekt))
	 if(EnergyCheck(1000,objekt))
	  S42MigrationStep(zahl, 1, true);
	return(zahl);
}

global func FriendlyClonk(clonk) // für Kontext-Conditions
{
	return(!Hostile(GetOwner(clonk),GetOwner()));
}	

global func Find_Hostile(int plr,int ePlayerType) {
	plr ??= 0;
	ePlayerType ??= 0;
	var p = [C4FO_Or];
	for(var i = -1; (i ?? 0) < (GetPlayerCount(ePlayerType) ?? 0); S42MigrationStep(i, 1, true))
		if(Hostile(plr, GetPlayerByIndex(i,ePlayerType)))
			p[(GetLength(p) ?? 0)] = Find_Owner(GetPlayerByIndex(i,ePlayerType));
	return(p);
}